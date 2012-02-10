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

#include "VSFilesystem.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSFileSystem
    DeclareDebugCache();

    VSFileSystem::VSFileSystem(_In_ ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    {
        AddThisToDebugCache();
    }

    VSFileSystem::~VSFileSystem()
    {
        RemoveThisFromDebugCache();

        m_Core = nullptr;
    }

    uint32_t VOODOO_METHODTYPE VSFileSystem::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSFileSystem::Release() CONST
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

    VoodooResult VOODOO_METHODTYPE VSFileSystem::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ IObject ** ppOut)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

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
            else if (refid == IID_IFileSystem)
            {
                *ppOut = static_cast<IFileSystem*>(this);
            }
            else if (refid == CLSID_VSFileSystem)
            {
                *ppOut = static_cast<VSFileSystem*>(this);
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

    String VOODOO_METHODTYPE VSFileSystem::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSTR("VSFileSystem()");
    }

    ICore * VOODOO_METHODTYPE VSFileSystem::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Core;
    }

    VoodooResult VOODOO_METHODTYPE VSFileSystem::AddPath(_In_ const String & dir, PathType type)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(dir);
        UNREFERENCED_PARAMETER(type);

        return VSFERR_INVALIDCALL;
    }

    VoodooResult VOODOO_METHODTYPE VSFileSystem::RemovePath(_In_ const String & dir)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(dir);

        return VSFERR_INVALIDCALL;
    }

    IFile * VOODOO_METHODTYPE VSFileSystem::GetFile(_In_ const String & name, const GetFileMode mode) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(mode);

        return nullptr;
    }
}
