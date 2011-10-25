/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 */

#include <sstream>
#include <iostream>

#include "Version.hpp"

#include "ICore.hpp"
#include "ILogger.hpp"
#include "IParser.hpp"

namespace VoodooShader
{
    IParser::IParser(_In_ ICore * pCore) : 
        m_Core(pCore)
    { }

    IParser::~IParser(void)
    {
        m_Variables.clear();
        m_SysVariables.clear();
    }

    int32_t IParser::AddRef() const
    {
        return ++m_Refs;
    }

    int32_t IParser::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    String IParser::ToString(void) const
    {
        return "Parser";
    }

    ICore * IParser::GetCore(void) const
    {
        return m_Core;
    }

    void IParser::Add(_In_ const String & name, _In_ const String & value, _In_ VariableType Type)
    {

        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            logger->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Adding variable \"%s\" with value \"%s\".",
                name.GetData(),
                value.GetData()
            );
        }

        String finalname = this->Parse(name, PF_VarName);

        if (Type == VT_System)
        {
            Dictionary::iterator varIter = m_SysVariables.find(finalname);

            if (varIter == m_SysVariables.end())
            {
                m_SysVariables[finalname] = value;
            }
            else
            {
                if (logger)
                {
                    logger->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Unable to add duplicate system variable \"%s\".",
                        finalname.GetData()
                    );
                }
            }
        }
        else
        {
            m_Variables[finalname] = value;
        }
    }

    void IParser::Remove(_In_ const String & name)
    {
        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Removing variable \"%s\".", name.GetData());
        }

        String finalname = this->Parse(name, PF_VarName);
        Dictionary::iterator varIter = m_Variables.find(finalname);

        if (varIter != m_Variables.end())
        {
            m_Variables.erase(varIter);
        }
    }

    String IParser::Parse(_In_ String input, _In_ ParseFlags flags) const
    {
        Dictionary parseState;

        return this->ParseStringRaw(input, flags, 0, parseState);
    }

    String IParser::ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ int depth, _In_ Dictionary & state) const
    {
        using namespace std;

        ILoggerRef logger(m_Core->GetLogger());

        if (logger)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Parsing string \"%s\" (%X).", input.GetData(), flags);
        }

        if (depth > IParser::VarMaxDepth || input.GetLength() < 3)
        {
            return input;
        }

        // Parse out any variables in the filename, first
        String iteration = input;
        // Variable parsing loop
        bool loop = true;

        while (loop)
        {
            const size_t itlen = iteration.GetLength();

            if (itlen < 3)
            {
                // Stop parsing if the string is too small to contain variables (this should
                // never be hit the first loop)
                break;
            }

            size_t endpos = iteration.Find(IParser::VarDelimEnd);

            if (endpos == string::npos)
            {
                // Stop parsing if no closing delimiter is found
                break;
            }

            String varname = iteration.Substr(0, endpos);
            size_t startpos = varname.ReverseFind(IParser::VarDelimStart);

            if (startpos < 1 || iteration[startpos - 1] != IParser::VarDelimPre)
            {
                // Stop parsing if no opening sequence is found, or there is no room for one
                break;
            }

            varname = varname.Substr(startpos + 1);

            if (varname.GetLength() == 0)
            {
                // Erase the variable sequence if it is the empty variable and restart the loop
                stringstream output;

                output << iteration.Substr(0, startpos - 1);
                if (endpos < itlen)
                {
                    output << iteration.Substr(endpos + 1);
                }

                iteration = output.str();
                continue;
            }


            // Handle state variables
            size_t statepos = varname.Find(IParser::VarMarkerState);

            if (statepos >= 0)
            {
                // State set, handle
                String newvalue = varname.Substr(statepos + 1);

                newvalue = this->ParseStringRaw(newvalue, flags, ++depth, state);

                varname = varname.Substr(0, statepos);
                varname = this->ParseStringRaw(varname, PF_VarName, ++depth, state);

                state[varname] = newvalue;

                stringstream output;

                output << iteration.Substr(0, startpos - 1) << iteration.Substr(endpos + 1);
                iteration = output.str();

                continue;
            }

            // Handle variable flags
            bool supress = false;
            bool parse = true;

            if (varname[0] == IParser::VarMarkerSupp)
            {
                supress = true;
                varname = varname.Substr(1);
            }
            else if (varname[0] == IParser::VarMarkerRaw)
            {
                parse = false;
                varname = varname.Substr(1);
            }

            // Properly format the variable name
            varname = this->ParseStringRaw(varname, PF_VarName, ++depth, state);

            // Lookup and replace the variable
            bool foundvar = true;
            String varvalue;
            Dictionary::const_iterator variter = m_SysVariables.find(varname);

            if (variter != m_SysVariables.end())
            {
                varvalue = variter->second;
            } else {
                variter = state.find(varname);
                if (variter != state.end())
                {
                    varvalue = variter->second;
                } else {
                    variter = m_Variables.find(varname);
                    if (variter != m_Variables.end())
                    {
                        varvalue = variter->second;
                    } else {
                        // Unrecognized variable, try env
                        size_t reqSize = 0;

                        getenv_s(&reqSize, nullptr, 0, varname.GetData());
                        if (reqSize != 0)
                        {

                            char *buffer = new char[reqSize];

                            getenv_s(&reqSize, buffer, reqSize, varname.GetData());
                            varvalue = buffer;
                            delete[] buffer;
                        }
                        else
                        {
                            foundvar = false;
                        }
                    }
                }
            }

            stringstream output;

            output << iteration.Substr(0, startpos - 1);
            if (parse && varvalue.GetLength() > 0)
            {
                output << this->ParseStringRaw(varvalue, flags, ++depth, state);
            }
            else if (!foundvar && !supress)
            {
                output << "--badvar:" << varname << "--";
            }

            output << iteration.Substr(endpos + 1);

            iteration = output.str();
        }

        // Handle slash replacement
        if (flags == PF_None)
        {
            return iteration;
        }
        else if (flags == PF_VarName)
        {
            iteration.ToLower();
            return iteration;
        }

        if (flags & (PF_SingleSlash | PF_SlashOnly | PF_BackslashOnly))
        {

            bool singleslash = (flags & PF_SingleSlash);
            bool prevslash = false;
            bool slashrewrite = false;
            char slashchar = ' ';

            if (flags & PF_SlashOnly)
            {
                slashrewrite = true;
                slashchar = '/';
            }
            else if (flags & PF_BackslashOnly)
            {
                slashrewrite = true;
                slashchar = '\\';
            }

            stringstream output;
            size_t total = iteration.GetLength();
            size_t cur = 0;

            while (cur < total)
            {
                char inchar = iteration[cur];

                ++cur;

                if (inchar == '/' || inchar == '\\')
                {
                    if (slashrewrite)
                    {
                        inchar = slashchar;
                    }

                    if (singleslash && prevslash)
                    {
                        continue;
                    }

                    prevslash = true;
                    output << inchar;
                }
                else
                {
                    prevslash = false;
                    output << inchar;
                }
            }

            iteration = output.str();
        }

        if (flags & PF_Lowercase)
        {
            iteration.ToLower();
        } else if (flags & PF_Uppercase) {
            iteration.ToUpper();
        }

        if (logger)
        {
            logger->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.GetData());
        }

        return iteration;
    }
}
