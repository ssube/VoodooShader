/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */

#include "VSParser.hpp"

#include "ICore.hpp"
#include "ILogger.hpp"

#include "Version.hpp"

#include "shlwapi.h"

#include <string>
#include <sstream>
#include <iostream>

namespace VoodooShader
{
    VSParser::VSParser(_In_ ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { }

    VSParser::~VSParser(void)
    {
        m_Variables.clear();
        m_SysVariables.clear();
    }

    uint32_t VSParser::AddRef() const
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VSParser::Release() const
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VSParser::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSParser;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IParser) {
                *ppOut = static_cast<const IParser*>(this);
            } else if (clsid == CLSID_VSParser) {
                *ppOut = static_cast<const VSParser*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VSParser::ToString(void) const
    {
        return L"VSParser";
    }

    ICore * VSParser::GetCore(void) const
    {
        return m_Core;
    }

    void VSParser::Add(_In_ const String & name, _In_ const String & value, _In_ const VariableType Type)
    {

        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            logger->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                L"Adding variable \"%s\" with value \"%s\".",
                name.GetData(),
                value.GetData()
            );
        }

        String finalname = this->Parse(name);

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
                        L"Unable to add duplicate system variable \"%s\".",
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

    void VSParser::Remove(_In_ const String & name)
    {
        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"Removing variable \"%s\".", name.GetData());
        }

        String finalname = this->Parse(name, PF_None);
        Dictionary::iterator varIter = m_Variables.find(finalname);

        if (varIter != m_Variables.end())
        {
            m_Variables.erase(varIter);
        }
    }

    String VSParser::Parse(_In_ const String & input, _In_ const ParseFlags flags) const
    {
        Dictionary parseState;

        return this->ParseStringRaw(input, flags, 0, parseState);
    }

    String VSParser::ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ uint32_t depth, _In_ Dictionary & state) const
    {
        ILoggerRef logger(m_Core->GetLogger());

        if (logger)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"Parsing string \"%s\" (%X).", input.GetData(), flags);
        }

        if (depth > VSParser::VarMaxDepth || input.GetLength() < 3)
        {
            return input;
        }

        // Parse out any variables in the filename, first
        String iteration = input;

        // Variable parsing loop
        bool loop = true;

        while (loop)
        {
            const uint32_t itlen = iteration.GetLength();

            if (itlen < 3)
            {
                // Stop parsing if the string is too small to contain variables (this should
                // never be hit the first loop)
                break;
            }

            uint32_t endpos = iteration.Find(VSParser::VarDelimEnd);

            if (endpos == String::Npos)
            {
                // Stop parsing if no closing delimiter is found
                break;
            }

            String varname = iteration.Substr(0, endpos);
            uint32_t startpos = varname.ReverseFind(VSParser::VarDelimStart);

            if (startpos < 1 || iteration[startpos - 1] != VSParser::VarDelimPre)
            {
                // Stop parsing if no opening sequence is found, or there is no room for one
                break;
            }

            varname = varname.Substr(startpos + 1);

            if (varname.GetLength() == 0)
            {
                // Erase the variable sequence if it is the empty variable and restart the loop
                std::wstringstream output;

                output << iteration.Substr(0, startpos - 1).GetData();
                if (endpos < itlen)
                {
                    output << iteration.Substr(endpos + 1).GetData();
                }

                iteration = output.str();
                continue;
            }

            // Handle state variables
            uint32_t statepos = varname.Find(VSParser::VarMarkerState);

            if (statepos != String::Npos)
            {
                // State set, handle
                String newvalue = varname.Substr(statepos + 1);

                newvalue = this->ParseStringRaw(newvalue, flags, ++depth, state);

                varname = varname.Substr(0, statepos);
                varname = this->ParseStringRaw(varname, PF_None, ++depth, state);

                state[varname] = newvalue;

                std::wstringstream output;

                output << iteration.Substr(0, startpos - 1).GetData() << iteration.Substr(endpos + 1).GetData();
                iteration = output.str();

                continue;
            }

            // Handle variable flags
            bool supress = false;
            bool parse = true;

            if (varname[0] == VSParser::VarMarkerSupp)
            {
                supress = true;
                varname = varname.Substr(1);
            }
            else if (varname[0] == VSParser::VarMarkerRaw)
            {
                parse = false;
                varname = varname.Substr(1);
            }

            // Properly format the variable name
            varname = this->ParseStringRaw(varname, PF_None, ++depth, state).ToLower();

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

                        _wgetenv_s(&reqSize, nullptr, 0, varname.GetData());
                        if (reqSize != 0)
                        {
                            std::vector<wchar_t> buffer(reqSize);
                            _wgetenv_s(&reqSize, &buffer[0], reqSize, varname.GetData());
                            varvalue = String(buffer);
                        } else {
                            foundvar = false;
                        }
                    }
                }
            }

            std::wstringstream output;

            output << iteration.Substr(0, startpos - 1).GetData();
            if (parse && varvalue.GetLength() > 0)
            {
                output << this->ParseStringRaw(varvalue, flags, ++depth, state).GetData();
            }
            else if (!foundvar && !supress)
            {
                output << L"--badvar:" << varname.GetData() << L"--";
            }

            output << iteration.Substr(endpos + 1).GetData();

            iteration = output.str();
        }

        // Handle slash replacement
        if (flags == PF_None)
        {
            return iteration;
        }

        if (flags & PF_SlashFlags)
        {
            bool singleslash = (flags & PF_SlashSingle) == PF_SlashSingle;
            bool doubleslash = (flags & PF_SlashDouble) == PF_SlashDouble;
            bool prevslash = false;
            bool slashrewrite = false;
            char slashchar = L' ';

            if (flags & PF_SlashTrail)
            {
                iteration += L"\\";
            }

            if (flags & PF_SlashOnly)
            {
                slashrewrite = true;
                slashchar = L'/';
            }
            else if (flags & PF_SlashBack)
            {
                slashrewrite = true;
                slashchar = L'\\';
            }

            std::wstringstream output;
            size_t total = iteration.GetLength();
            size_t cur = 0;

            while (cur < total)
            {
                wchar_t inchar = iteration[cur++];

                if (inchar == L'/' || inchar == L'\\')
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
                    if (doubleslash)
                    {
                        output << inchar;
                    }
                }
                else
                {
                    prevslash = false;
                    output << inchar;
                }
            }

            iteration = output.str();
        }

        if (flags & PF_PathFlags)
        {
            if (flags & PF_PathCanon)
            {
                LPWSTR buffer = new TCHAR[MAX_PATH];
                if (PathCanonicalize(buffer, iteration.GetData()) == TRUE)
                {
                    iteration = buffer;
                }
                delete[] buffer;
            }

            if (flags & PF_PathRoot)
            {
                LPWSTR buffer = new TCHAR[MAX_PATH];
                CopyMemory(buffer, iteration.GetData(), iteration.GetLength());

                if (PathStripToRoot(buffer) == TRUE)
                {
                    iteration = buffer;
                }

                delete[] buffer;
            } else if (flags & PF_PathOnly) {
                LPWSTR buffer = new TCHAR[iteration.GetLength()];
                CopyMemory(buffer, iteration.GetData(), iteration.GetLength());

                if (PathRemoveFileSpec(buffer) != 0)
                {
                    iteration = buffer;
                }

                delete[] buffer;
            } else if (flags & PF_PathFile) {
                iteration = PathFindFileName(iteration.GetData());
            } else if (flags & PF_PathExt) {
                iteration = PathFindExtension(iteration.GetData());
            }
        }

        if (logger)
        {
            logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Returning string %s from parser.", iteration.GetData());
        }

        return iteration;
    }
}
