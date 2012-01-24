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

#include "VSHookManager.hpp"
// System
#pragma warning(push,3)
#pragma warning(disable: 4005)
#include <ntstatus.h>
#pragma warning(pop)

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSHookManager
    DeclareDebugCache();

    VSHookManager::VSHookManager(_In_ ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { 
        m_ThreadCount = 1;

        m_ThreadIDs = new ULONG[m_ThreadCount];
        m_ThreadIDs[0] = 0;

        LhSetGlobalInclusiveACL(m_ThreadIDs, m_ThreadCount);

        m_Core->GetLogger()->LogMessage(VSLog_ModInfo, VOODOO_CORE_NAME, VSTR("Created hook manager."));

        AddThisToDebugCache();
    }

    VSHookManager::~VSHookManager()
    { 
        RemoveThisFromDebugCache();

        this->RemoveAll();

        m_Core->GetLogger()->LogMessage(VSLog_ModInfo, VOODOO_CORE_NAME, VSTR("Destroying hook manager."));

        delete[] m_ThreadIDs;
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

    VoodooResult VOODOO_METHODTYPE VSHookManager::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ IObject ** ppOut)
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

    VoodooResult VOODOO_METHODTYPE VSHookManager::Add(_In_ const String & name, _In_ void * pSrc, _In_ void * pDest)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        HookMap::iterator hook = m_Hooks.find(name);

        if (hook != m_Hooks.end())
        {
            m_Core->GetLogger()->LogMessage
            (
                VSLog_ModError, VOODOO_CORE_NAME,
                StringFormat(VSTR("Attempted to create a hook with a duplicate name (%1%).")) << name
            );

            return VSFERR_INVALIDPARAMS;
        }

        m_Core->GetLogger()->LogMessage
        (
            VSLog_ModDebug, VOODOO_CORE_NAME,
            StringFormat(VSTR("Creating hook %1%. Redirecting function %2% to %3%.")) << name << pSrc << pDest
        );

        TRACED_HOOK_HANDLE hookHandle = new HOOK_TRACE_INFO();
        NTSTATUS result = LhInstallHook(pSrc, pDest, nullptr, hookHandle);

        if (result == STATUS_NOT_SUPPORTED || result == STATUS_NO_MEMORY || result == STATUS_INSUFFICIENT_RESOURCES)
        {
            m_Core->GetLogger()->LogMessage
            (
                VSLog_ModError, VOODOO_CORE_NAME,
                StringFormat(VSTR("Error %1 creating hook %s.")) << (uint32_t)result << name
            );

            return VSFERR_INVALIDCALL;
        }
        else
        {
            LhSetInclusiveACL(m_ThreadIDs, m_ThreadCount, hookHandle);

            m_Hooks[name] = hookHandle;

            return VSF_OK;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSHookManager::Remove(_In_ const String & name)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        HookMap::iterator hook = m_Hooks.find(name);

        m_Core->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Removing hook %1%.")) << name);

        if (hook != m_Hooks.end())
        {
            TRACED_HOOK_HANDLE tracedHandle = (TRACED_HOOK_HANDLE)hook->second;
            NTSTATUS result = LhUninstallHook(tracedHandle);

            delete tracedHandle;

            if (result != 0)
            {
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_ModError, VOODOO_CORE_NAME,
                    StringFormat("Error %1% removing hook %2%.") << (uint32_t)result << name
                );

                return VSFERR_INVALIDCALL;
            }
            else
            {
                delete hook->second;
                m_Hooks.erase(hook);

                return VSF_OK;
            }
        }
        else
        {
            m_Core->GetLogger()->LogMessage
            (
                VSLog_ModDebug, VOODOO_CORE_NAME,
                StringFormat(VSTR("Trying to remove hook %1% (does not exist).")) << name.GetData()
            );
            return VSFERR_INVALIDPARAMS;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSHookManager::RemoveAll()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        std::for_each
        (
            m_Hooks.begin(), m_Hooks.end(),
            [this](std::pair<String, TRACED_HOOK_HANDLE> chook)
            {
                this->Remove(chook.first);
            }
        );

        m_Hooks.clear();

        return VSF_OK;
    }
}
