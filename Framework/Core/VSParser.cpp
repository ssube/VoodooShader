/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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
// System
#pragma warning(push,3)
#   include <shlwapi.h>
#   include <string>
#   include <sstream>
#   include <iostream>
#pragma warning(pop)

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSParser
    DeclareDebugCache();

    VariableMap g_GlobalVariables;

    _Check_return_ IParser * VOODOO_CALLTYPE CreateParser()
    {
        static VSParser * pParser = nullptr;

        if (!pParser)
        {
            try
            {
                pParser = new VSParser();
            }
            catch (const std::exception & exc)
            {
                UNREFERENCED_PARAMETER(exc);
                pParser = nullptr;
            }
        }

        return pParser;
    }

    VOODOO_METHODTYPE VSParser::VSParser() :
        m_Refs(0)
    {
        m_Logger = CreateLogger();

        AddThisToDebugCache();
    }

    VOODOO_METHODTYPE VSParser::~VSParser()
    {
        RemoveThisFromDebugCache();

        m_Variables.clear();
        m_Logger = nullptr;
    }

    uint32_t VOODOO_METHODTYPE VSParser::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSParser::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

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

    VoodooResult VOODOO_METHODTYPE VSParser::QueryInterface(_In_ Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!ppOut)
        {
            return VSFERR_INVALIDPARAMS;
        }
        else
        {
            if (refid == IID_IObject)
            {
                *ppOut = static_cast<IObject*>(this);
            }
            else if (refid == IID_IParser)
            {
                *ppOut = static_cast<IParser*>(this);
            }
            else if (refid == CLSID_VSParser)
            {
                *ppOut = static_cast<VSParser*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return VSFERR_INVALIDUUID;
            }

            (*ppOut)->AddRef();
            return VSF_OK;
        }
    }

    String VOODOO_METHODTYPE VSParser::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return VSTR("VSParser()");
    }

    ICore * VOODOO_METHODTYPE VSParser::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return nullptr;
    }

    VoodooResult VOODOO_METHODTYPE VSParser::Add(_In_ CONST String & name, _In_ CONST String & value, _In_ CONST VariableType type)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (m_Logger)
        { 
            StringFormat msg(VSTR("Adding variable '%1%' with value '%2%'."));
            msg << name << value;
            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, msg);
        }

        String finalname = this->Parse(name);        
        VariableMap::iterator varIter = g_GlobalVariables.find(finalname);
        
        if (varIter != g_GlobalVariables.end() && (varIter->second.second & VSVar_System) == VSVar_System)
        {
            StringFormat msg(VSTR("Unable to add duplicate variable '%1%' (global system variable already exists)."));
            msg << finalname;
            m_Logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, msg);

            return VSF_FAIL;
        }
        else if ((type & VSVar_Global) == VSVar_Global)
        {
            g_GlobalVariables[finalname] = Variable(value, type);
            return VSF_OK;
        }

        varIter = m_Variables.find(finalname);

        if (varIter != m_Variables.end() && (varIter->second.second & VSVar_System) == VSVar_System)
        {
            if (m_Logger)
            {
                StringFormat msg(VSTR("Unable to add duplicate variable '%1%' (system variable already exists)."));
                msg << finalname;
                m_Logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, msg);
            }
            return VSF_FAIL;
        }
        else
        {
            m_Variables[finalname] = Variable(value, type);
            return VSF_OK;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSParser::Remove(_In_ CONST String & name)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (m_Logger)
        {
            StringFormat msg(VSTR("Removing variable '%1%'."));
            msg << name;
            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, msg);
        }

        String finalname = this->Parse(name, VSParse_None);
        VariableMap::iterator varIter = m_Variables.find(finalname);

        if (varIter != m_Variables.end() && varIter->second.second != VSVar_System)
        {
            m_Variables.erase(varIter);
            return VSF_OK;
        }
        else
        {
            return VSF_FAIL;
        }
    }

    String VOODOO_METHODTYPE VSParser::Parse(_In_ CONST String & input, _In_ CONST ParseFlags flags) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        StringMap parseState;
        return this->ParseStringRaw(input, flags, 0, parseState);
    }

    String VSParser::ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ uint32_t depth, _In_ StringMap & state) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (m_Logger)
        {
            m_Logger->LogMessage
            (
                VSLog_CoreDebug, VOODOO_CORE_NAME, 
                StringFormat(VSTR("Parsing string '%1%' (%2%).")) << input << flags
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

                        if (m_Logger)
                        {
                            m_Logger->LogMessage(VSLog_CoreInfo, VOODOO_CORE_NAME, 
                                StringFormat("Variable local flags of %1% found, merged %2% with original %3%, flags set to %4%.") << newFlags << merge << flags << localFlags);
                        }
                    } 
                    catch (const std::exception & exc)
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
                varname = this->ParseStringRaw(varname, VSParse_None, ++depth, state);

                state[varname] = newvalue;

                std::wstringstream output;

                output << iteration.Substr(0, startpos - 1).GetData() << iteration.Substr(endpos + 1).GetData();
                iteration = output.str();

                continue;
            }

            // Properly format the variable name
            varname = this->ParseStringRaw(varname, VSParse_None, ++depth, state).ToLower();

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
        if (flags == VSParse_None)
        {
            return iteration;
        }

        if (flags & VSParse_SlashFlags)
        {
            bool singleslash = (flags & VSParse_SlashSingle) == VSParse_SlashSingle;
            bool doubleslash = (flags & VSParse_SlashDouble) == VSParse_SlashDouble;
            bool prevslash = false;
            bool slashrewrite = false;
            wchar_t slashchar = VSTR(' ');

            if (flags & VSParse_SlashTrail)
            {
                iteration += VSTR("\\");
            }

            if (flags & VSParse_SlashOnly)
            {
                slashrewrite = true;
                slashchar = VSTR('/');
            }
            else if (flags & VSParse_SlashBack)
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

        if (flags & VSParse_PathFlags)
        {
            if (flags & VSParse_PathCanon)
            {
                LPWSTR buffer = new TCHAR[MAX_PATH];
                if (PathCanonicalize(buffer, iteration.GetData()) == TRUE)
                {
                    iteration = buffer;
                }
                delete[] buffer;
            }

            if (flags & VSParse_PathRoot)
            {
                LPWSTR buffer = new TCHAR[MAX_PATH];
                CopyMemory(buffer, iteration.GetData(), iteration.GetLength());

                if (PathStripToRoot(buffer) == TRUE)
                {
                    iteration = buffer;
                }

                delete[] buffer;
            }
            else if (flags & VSParse_PathOnly)
            {
                LPWSTR buffer = new TCHAR[iteration.GetLength()];
                CopyMemory(buffer, iteration.GetData(), iteration.GetLength());

                if (PathRemoveFileSpec(buffer) != 0)
                {
                    iteration = buffer;
                }

                delete[] buffer;
            }
            else if (flags & VSParse_PathFile)
            {
                iteration = PathFindFileName(iteration.GetData());
            }
            else if (flags & VSParse_PathExt)
            {
                iteration = PathFindExtension(iteration.GetData());
            }
        }

        if (m_Logger)
        {
            m_Logger->LogMessage
            (
                VSLog_CoreDebug, VOODOO_CORE_NAME,
                StringFormat(VSTR("Returning string '%1%' from parser.")) << iteration
            );
        }

        return iteration;
    }
}
