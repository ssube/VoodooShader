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

#include "VSHookManager.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSHookManager
    DeclareDebugCache();

    VSHookManager::VSHookManager(_In_ ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { 
        m_Core->GetLogger()->LogMessage(VSLog_PlugInfo, VOODOO_CORE_NAME, VSTR("Created hook manager."));

        AddThisToDebugCache();
    }

    VSHookManager::~VSHookManager()
    { 
        RemoveThisFromDebugCache();

        m_Core->GetLogger()->LogMessage(VSLog_PlugInfo, VOODOO_CORE_NAME, VSTR("Destroying hook manager."));

        m_Core = nullptr;
    }

    uint32_t VOODOO_METHODTYPE VSHookManager::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSHookManager::Release() CONST
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

    VoodooResult VOODOO_METHODTYPE VSHookManager::QueryInterface(_In_ Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut)
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
            else if (refid == IID_IHookManager)
            {
                *ppOut = static_cast<IHookManager*>(this);
            }
            else if (refid == CLSID_VSHookManager)
            {
                *ppOut = static_cast<VSHookManager*>(this);
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

    String VOODOO_METHODTYPE VSHookManager::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSTR("VSHookManager()");
    }

    ICore * VOODOO_METHODTYPE VSHookManager::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Core;
    }

    VoodooResult VOODOO_METHODTYPE VSHookManager::Add(_In_ CONST String & name, _In_ void * pSrc, _In_ void * pDest)
    {
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(pSrc);
        UNREFERENCED_PARAMETER(pDest);

        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSF_OK;
    }

    VoodooResult VOODOO_METHODTYPE VSHookManager::Remove(_In_ CONST String & name)
    {
        UNREFERENCED_PARAMETER(name);

        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSF_OK;
    }

    VoodooResult VOODOO_METHODTYPE VSHookManager::RemoveAll()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSF_OK;
    }
}
