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

#include "VSLogger.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSLogger
    DeclareDebugCache();

    VSLogger::VSLogger(ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { 
        AddThisToDebugCache();
    }

    VSLogger::~VSLogger()
    { 
        RemoveThisFromDebugCache();
    }

    uint32_t VOODOO_METHODTYPE VSLogger::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSLogger::Release() CONST
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

    bool VOODOO_METHODTYPE VSLogger::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSLogger;
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
            else if (clsid == IID_ILogger)
            {
                *ppOut = static_cast<const ILogger*>(this);
            }
            else if (clsid == CLSID_VSLogger)
            {
                *ppOut = static_cast<const VSLogger*>(this);
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

    String VOODOO_METHODTYPE VSLogger::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return VSTR("VSLogger()");
    }

    ICore * VOODOO_METHODTYPE VSLogger::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    bool VOODOO_METHODTYPE VSLogger::Open(_In_ const String & filename, _In_ const bool append)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(filename);
        UNREFERENCED_PARAMETER(append);

        return true;
    }

    bool VOODOO_METHODTYPE VSLogger::Open(_In_ IFile * const pFile, _In_ const bool append)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pFile);
        UNREFERENCED_PARAMETER(append);

        return true;
    }

    bool VOODOO_METHODTYPE VSLogger::IsOpen() const
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return true;
    }

    bool VOODOO_METHODTYPE VSLogger::Close()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return true;
    }

    bool VOODOO_METHODTYPE VSLogger::Flush()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return true;
    }

    void VOODOO_METHODTYPE VSLogger::SetFilter(_In_ const uint32_t level)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(level);
    }

    LogLevel VOODOO_METHODTYPE VSLogger::GetFilter() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return LL_Unknown;
    }

    void VOODOO_METHODTYPE VSLogger::SetFlags(_In_ const LogFlags flush)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(flush);
    }

    LogFlags VOODOO_METHODTYPE VSLogger::GetFlags() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return LF_Unknown;
    }

    bool VOODOO_METHODTYPE VSLogger::LogMessage
    (
        _In_ const uint32_t level,
        _In_ const String & source,
        _In_ const String & msg
    )
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(level);
        UNREFERENCED_PARAMETER(source);
        UNREFERENCED_PARAMETER(msg);

#if defined(_DEBUG)
        if ((level & (LL_Critical | LL_Warning | LL_Error)) > 0)
        {
            OutputDebugString(msg.GetData());
        }
#endif

        return true;
    }
}
