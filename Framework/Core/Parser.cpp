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

#include "ICore.hpp"
#include "ILogger.hpp"
#include "IParser.hpp"

#include <sstream>
#include <iostream>

namespace VoodooShader
{
    IParser::IParser(_In_ ICore *core) 
        : m_Core(core)
    { }

    IParser::~IParser(void)
    {
        m_Variables.clear();
    }

    String IParser::ToLower(_In_ String input)
    {

        String ret = input;

        std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
        return ret;
    }

    String IParser::ToUpper(_In_ String input)
    {

        String ret = input;

        std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
        return ret;
    }

    void IParser::Add(_In_ String name, _In_ String value, _In_ VariableType Type)
    {

        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            logger->Log
                (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Adding variable \"%s\" with value \"%s\".",
                name.c_str(),
                value.c_str()
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
                        finalname.c_str()
                        );
                }
            }
        }
        else
        {
            m_Variables[finalname] = value;
        }
    }

    void IParser::Remove(_In_ String name)
    {

        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Removing variable \"%s\".", name.c_str());
        }

        String finalname = this->Parse(name, PF_VarName);
        Dictionary::iterator varIter = m_Variables.find(finalname);

        if (varIter != m_Variables.end())
        {
            m_Variables.erase(varIter);
        }
    }

    String IParser::Parse(_In_ String input, _In_ ParseFlags flags)
    {

        Dictionary parseState;

        return this->ParseStringRaw(input, flags, 0, parseState);
    }

    String IParser::ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ int depth, _In_ Dictionary &state)
    {
        using namespace std;

        ILoggerRef logger(m_Core->GetLogger());

        if (logger)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Parsing string \"%s\" (%X).", input.c_str(), flags);
        }

        if (depth > IParser::VarMaxDepth || input.length() < 3)
        {
            return input;
        }


        // Parse out any variables in the filename, first
        String iteration = input;
        // Variable parsing loop
        bool loop = true;

        while (loop)
        {

            const size_t itlen = iteration.length();

            if (itlen < 3)
            {

                // Stop parsing if the string is too small to contain variables (this should
                // never be hit the first loop)
                break;
            }

            size_t endpos = iteration.find_first_of(IParser::VarDelimEnd);

            if (endpos == string::npos)
            {

                // Stop parsing if no closing delimiter is found
                break;
            }

            String varname = iteration.substr(0, endpos);
            size_t startpos = varname.find_last_of(IParser::VarDelimStart);

            if (startpos == String::npos || startpos == 0 || iteration[startpos - 1] != IParser::VarDelimPre)
            {

                // Stop parsing if no opening sequence is found, or there is no room for one
                break;
            }

            varname = varname.substr(startpos + 1);

            if (varname.length() == 0)
            {


                // Erase the variable sequence if it is the empty variable and restart the loop
                stringstream output;

                output << iteration.substr(0, startpos - 1);
                if (endpos < itlen)
                {
                    output << iteration.substr(endpos + 1);
                }

                iteration = output.str();
                continue;
            }


            // Handle state variables
            size_t statepos = varname.find(':');

            if (statepos != String::npos)
            {

                // State set, handle
                String newvalue = varname.substr(statepos + 1);

                newvalue = this->ParseStringRaw(newvalue, flags, ++depth, state);

                varname = varname.substr(0, statepos);
                varname = this->ParseStringRaw(varname, PF_VarName, ++depth, state);

                state[varname] = newvalue;

                stringstream output;

                output << iteration.substr(0, startpos - 1) << iteration.substr(endpos + 1);
                iteration = output.str();

                continue;
            }


            // Handle variable flags
            bool supress = false;
            bool parse = true;


            // The length of varname is > 0, guaranteed in line 134
            if (varname[0] == '?')
            {
                supress = true;
                varname = varname.substr(1);
            }
            else if (varname[0] == '!')
            {
                parse = false;
                varname = varname.substr(1);
            }

            // Properly format the variable name
            varname = this->ParseStringRaw(varname, PF_VarName, ++depth, state);


            // Lookup and replace the variable
            bool foundvar = true;
            String varvalue;
            Dictionary::iterator variter = m_SysVariables.find(varname);

            if (variter != m_SysVariables.end())
            {
                varvalue = variter->second;
            }
            else
            {
                variter = state.find(varname);
                if (variter != state.end())
                {
                    varvalue = variter->second;
                }
                else
                {
                    variter = m_Variables.find(varname);
                    if (variter != m_Variables.end())
                    {
                        varvalue = variter->second;
                    }
                    else
                    {


                        // Unrecognized variable, try env
                        size_t reqSize = 0;

                        getenv_s(&reqSize, nullptr, 0, varname.c_str());
                        if (reqSize != 0)
                        {

                            char *buffer = new char[reqSize];

                            getenv_s(&reqSize, buffer, reqSize, varname.c_str());
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

            output << iteration.substr(0, startpos - 1);
            if (parse && varvalue.length() > 0)
            {
                output << this->ParseStringRaw(varvalue, flags, ++depth, state);
            }
            else if (!foundvar && !supress)
            {
                output << "--badvar:" << varname << "--";
            }

            output << iteration.substr(endpos + 1);

            iteration = output.str();
        }

        // Handle slash replacement
        if (flags == PF_None)
        {
            return iteration;
        }
        else if (flags == PF_VarName)
        {
            return this->ToLower(iteration);
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
            size_t total = iteration.length();
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
            iteration = this->ToLower(iteration);
        }
        else if (flags & PF_Uppercase)
        {
            iteration = this->ToUpper(iteration);
        }

        if (logger)
        {
            logger->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.c_str());
        }

        return iteration;
    }
}
