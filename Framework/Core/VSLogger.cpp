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

#include "VSLogger.hpp"

namespace VoodooShader
{
    VSLogger::VSLogger(ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { }

    VSLogger::~VSLogger()
    { }

    uint32_t VOODOO_METHODTYPE VSLogger::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSLogger::Release() CONST
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VOODOO_METHODTYPE VSLogger::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSLogger;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_ILogger) {
                *ppOut = static_cast<const ILogger*>(this);
            } else if (clsid == CLSID_VSLogger) {
                *ppOut = static_cast<const VSLogger*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSLogger::ToString() CONST
    {
        return L"VSLogger()";
    }

    ICore * VOODOO_METHODTYPE VSLogger::GetCore() CONST
    {
        return m_Core;
    }

    bool VOODOO_METHODTYPE VSLogger::Open(_In_ const String & filename, _In_ const bool append)
    {
        UNREFERENCED_PARAMETER(filename);
        UNREFERENCED_PARAMETER(append);

        return true;
    }

    bool VOODOO_METHODTYPE VSLogger::Open(_In_ IFile * const pFile, _In_ const bool append)
    {
        UNREFERENCED_PARAMETER(pFile);
        UNREFERENCED_PARAMETER(append);

        return true;
    }

    void VOODOO_METHODTYPE VSLogger::Close()
    { }

    void VOODOO_METHODTYPE VSLogger::Flush()
    { }

    void VOODOO_METHODTYPE VSLogger::SetLogLevel(_In_ const LogLevel level)
    {
        UNREFERENCED_PARAMETER(level);
    }

    const LogLevel VOODOO_METHODTYPE VSLogger::GetLogLevel() CONST
    {
        return LL_Unknown;
    }

    void VOODOO_METHODTYPE VSLogger::LogModule(_In_ const Version * const pModule)
    {
        UNREFERENCED_PARAMETER(pModule);
    }

    void VOODOO_METHODTYPE VSLogger::Log
    (
        _In_ const LogLevel level,
        _In_ const wchar_t * source,
        _In_ _Printf_format_string_ const wchar_t * format, 
        ...
    )
    {
        UNREFERENCED_PARAMETER(level);
        UNREFERENCED_PARAMETER(source);
        UNREFERENCED_PARAMETER(format);
    }

    void VOODOO_METHODTYPE VSLogger::SetFlags(_In_ const LogFlags flush)
    {
        UNREFERENCED_PARAMETER(flush);
    }

    const LogFlags VOODOO_METHODTYPE VSLogger::GetFlags() CONST
    {
        return LF_Unknown;
    }
}
