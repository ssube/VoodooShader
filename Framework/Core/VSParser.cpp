/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

#include "shlwapi.h"

#include <string>
#include <sstream>
#include <iostream>

namespace VoodooShader
{
    DebugCache(VSParser);

    VOODOO_METHODTYPE VSParser::VSParser(_In_ ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    {
        AddThisToDebugCache(VSParser);
    }

    VOODOO_METHODTYPE VSParser::~VSParser()
    {
        RemoveThisFromDebugCache(VSParser);

        m_Variables.clear();
    }

    uint32_t VOODOO_METHODTYPE VSParser::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSParser::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return m_Refs;
        }
    }

    bool VOODOO_METHODTYPE VSParser::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSParser;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            }
            else if (clsid == IID_IParser)
            {
                *ppOut = static_cast<const IParser*>(this);
            }
            else if (clsid == CLSID_VSParser)
            {
                *ppOut = static_cast<const VSParser*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSParser::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return VSTR("VSParser()");
    }

    ICore * VOODOO_METHODTYPE VSParser::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    void VOODOO_METHODTYPE VSParser::Add(_In_ const String & name, _In_ const String & value, _In_ const VariableType type)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            Format msg(VSTR("Adding variable '%1%' with value '%2%'."));
            msg << name << value;
            logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, msg);
        }

        String finalname = this->Parse(name);        
        VariableMap::iterator varIter = m_Variables.find(finalname);

        if (varIter == m_Variables.end())
        {
            m_Variables[finalname] = Variable(value, type);
        }
        else
        {
            if (varIter->second.second == VT_System)
            {
                if (logger)
                {
                    Format msg(VSTR("Unable to add duplicate variable '%1%' (system variable already exists)."));
                    msg << finalname;
                    logger->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, msg);
                }
            }
            else
            {
                m_Variables[finalname] = Variable(value, type);
            }
        }
    }

    void VOODOO_METHODTYPE VSParser::Remove(_In_ const String & name)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        ILoggerRef logger = m_Core->GetLogger();

        if (logger)
        {
            Format msg(VSTR("Removing variable '%1%'."));
            msg << name;
            m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, msg);
        }

        String finalname = this->Parse(name, PF_None);
        VariableMap::iterator varIter = m_Variables.find(finalname);

        if (varIter != m_Variables.end() && varIter->second.second != VT_System)
        {
            m_Variables.erase(varIter);
        }
    }

    String VOODOO_METHODTYPE VSParser::Parse(_In_ const String & input, _In_ const ParseFlags flags) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        StringMap parseState;

        return this->ParseStringRaw(input, flags, 0, parseState);
    }

    String VSParser::ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ uint32_t depth, _In_ StringMap & state) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        ILoggerRef logger(m_Core->GetLogger());

        if (logger)
        {
            logger->LogMessage
            (
                LL_CoreDebug, VOODOO_CORE_NAME, 
                Format(VSTR("Parsing string '%1%' (%2%).")) << input << flags
            );
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

            // Handle flags
            bool require = false;
            bool parse = true;

            if (varname[0] == VSParser::VarMarkerReq)
            {
                require = true;
                varname = varname.Substr(1);
            }
            
            if (varname[0] == VSParser::VarMarkerDelay)
            {
                parse = false;
                varname = varname.Substr(1);
            }

            // Handle modes
            ParseFlags localFlags = flags;
            if (varname[0] == VSParser::VarMarkerMode)
            {
                int8_t merge = 0;
                varname = varname.Substr(1);

                if (varname[0] == VSParser::VarMarkerModeM)
                {
                    merge = 1;
                }
                else if (varname[0] == VSParser::VarMarkerModeR)
                {
                    merge = -1;
                }

                uint32_t modesEnd = varname.Find(VSParser::VarMarkerModeD);
                if (modesEnd != String::Npos)
                {
                    try
                    {
                        String modesString = varname.Substr(0, modesEnd - 1);
                        int newFlags = stoi(modesString.ToString());

                        if (merge == 0)
                        {
                            localFlags = (ParseFlags)newFlags;
                        }
                        else if (merge == -1)
                        {
                            localFlags = (ParseFlags)(flags ^ newFlags);
                        }
                        else if (merge == 1)
                        {
                            localFlags = (ParseFlags)(flags | newFlags);
                        }

                        if (logger)
                        {
                            logger->LogMessage(LL_CoreInfo, VOODOO_CORE_NAME, 
                                Format("Variable local flags of %1% found, merged %2% with original %3%, flags set to %4%.") << newFlags << merge << flags << localFlags);
                        }
                    } catch (std::exception & exc)
                    {
                        UNREFERENCED_PARAMETER(exc);
                    }

                    varname = varname.Substr(modesEnd + 1);
                }
            }

            // Handle state variables
            uint32_t statepos = varname.Find(VSParser::VarMarkerState);

            if (statepos != String::Npos)
            {
                // State set, handle
                String newvalue = varname.Substr(statepos + 1);

                newvalue = this->ParseStringRaw(newvalue, localFlags, ++depth, state);

                varname = varname.Substr(0, statepos);
                varname = this->ParseStringRaw(varname, PF_None, ++depth, state);

                state[varname] = newvalue;

                std::wstringstream output;

                output << iteration.Substr(0, startpos - 1).GetData() << iteration.Substr(endpos + 1).GetData();
                iteration = output.str();

                continue;
            }

            // Properly format the variable name
            varname = this->ParseStringRaw(varname, PF_None, ++depth, state).ToLower();

            // Lookup and replace the variable
            bool foundvar = true;
            String varvalue;
            StringMap::const_iterator stateiter = state.find(varname);
            if (stateiter != state.end())
            {
                varvalue = stateiter->second;
            }
            else
            {
                VariableMap::const_iterator variter = m_Variables.find(varname);
                if (variter != m_Variables.end())
                {
                    varvalue = variter->second.first;
                }
                else
                {
                    // Unrecognized variable, try env
                    size_t reqSize = 0;

                    _wgetenv_s(&reqSize, nullptr, 0, varname.GetData());
                    if (reqSize != 0)
                    {
                        std::vector<wchar_t> buffer(reqSize);
                        _wgetenv_s(&reqSize, &buffer[0], reqSize, varname.GetData());
                        varvalue = String(buffer);
                    }
                    else
                    {
                        foundvar = false;
                    }
                }
            }

            std::basic_stringstream<wchar_t> output;

            output << iteration.Substr(0, startpos - 1).GetData();
            if (parse && varvalue.GetLength() > 0)
            {
                output << this->ParseStringRaw(varvalue, localFlags, ++depth, state).GetData();
            }
            else if (!foundvar && require)
            {
                output << VSTR("badvar:") << varname.GetData();
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
            char slashchar = VSTR(' ');

            if (flags & PF_SlashTrail)
            {
                iteration += VSTR("\\");
            }

            if (flags & PF_SlashOnly)
            {
                slashrewrite = true;
                slashchar = VSTR('/');
            }
            else if (flags & PF_SlashBack)
            {
                slashrewrite = true;
                slashchar = VSTR('\\');
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
            }
            else if (flags & PF_PathOnly)
            {
                LPWSTR buffer = new TCHAR[iteration.GetLength()];
                CopyMemory(buffer, iteration.GetData(), iteration.GetLength());

                if (PathRemoveFileSpec(buffer) != 0)
                {
                    iteration = buffer;
                }

                delete[] buffer;
            }
            else if (flags & PF_PathFile)
            {
                iteration = PathFindFileName(iteration.GetData());
            }
            else if (flags & PF_PathExt)
            {
                iteration = PathFindExtension(iteration.GetData());
            }
        }

        if (logger)
        {
            logger->LogMessage
            (
                LL_CoreDebug, VOODOO_CORE_NAME,
                Format(VSTR("Returning string '%1%' from parser.")) << iteration
            );
        }

        return iteration;
    }
}
