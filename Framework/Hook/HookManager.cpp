/**
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

#include <algorithm>

#include "HookManager.hpp"
#include "Hook_Version.hpp"

namespace VoodooShader
{
    namespace EasyHook
    {
        static const Version moduleVersion = VOODOO_META_VERSION_STRUCT(HOOK);
        static const wchar_t * name_VSEHHookManager = L"VSEHHookManager";
        static const Uuid clsid_VSEHHookManager = CLSID_VSEHHookManager;

        const Version * VOODOO_CALL API_ModuleVersion(void)
        {

            return &moduleVersion;
        }

        const uint32_t VOODOO_CALL API_ClassCount(void)
        {
            return 1;
        }

        const wchar_t * VOODOO_CALL API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid)
        {
            if (!pUuid)
            {
                return nullptr;
            }

            if (index == 0)
            {
                *pUuid = clsid_VSEHHookManager;
                return name_VSEHHookManager;
            }

            return nullptr;
        }

        IObject * VOODOO_CALL API_ClassCreate(_In_ const uint32_t number, _In_ ICore *core)
        {
            if (number == 0)
            {
                return new VSEHHookManager(core);
            }

            return nullptr;
        }

        VSEHHookManager::VSEHHookManager(ICore * pCore) :
            m_Core(pCore)
        {
            m_ThreadCount = 1;

            m_ThreadIDs = new ULONG[m_ThreadCount];
            m_ThreadIDs[0] = 0;

            LhSetGlobalInclusiveACL(m_ThreadIDs, m_ThreadCount);

            m_Core->GetLogger()->Log(LL_Info, VOODOO_HOOK_NAME, "Created hook manager.", m_ThreadCount);
        }

        VSEHHookManager::~VSEHHookManager(void)
        {
            this->RemoveAll();

            m_Core->GetLogger()->Log(LL_Info, VOODOO_HOOK_NAME, "Destroying hook manager.");

            delete[] m_ThreadIDs;
        }

        uint32_t VSEHHookManager::AddRef(void) const
        {
            return ++m_Refs;
        }


        uint32_t VSEHHookManager::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSEHHookManager::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_VSEHHookManager;
                    return true;
                } else {
                    return false;
                }
            } else {
                if (clsid == IID_IObject || clsid == IID_IHookManager || clsid == CLSID_VSEHHookManager) {
                    *ppOut = this;
                    return true;
                } else {
                    *ppOut = nullptr;
                    return false;
                }
            }
        }

        String VSEHHookManager::ToString(void) const
        {
            return L"VSEHHookManager";
        }

        ICore * VSEHHookManager::GetCore(void) const
        {
            return m_Core;
        }

        bool VSEHHookManager::Add(const String & name, void * pSrc, void * pDest)
        {
            HookMap::iterator hook = m_Hooks.find(name);

            if (hook != m_Hooks.end())
            {
                Throw(VOODOO_HOOK_NAME, "Attempted to create a hook with a duplicate name.", m_Core);
            }

            m_Core->GetLogger()->Log
            (
                LL_Debug, VOODOO_HOOK_NAME,
                "Creating hook %s. Redirecting function %p to %p.",
                name.GetData(), pSrc, pDest
            );

            TRACED_HOOK_HANDLE hookHandle = new HOOK_TRACE_INFO();
            DWORD result = LhInstallHook(pSrc, pDest, nullptr, hookHandle);

            if (result && hookHandle)
            {
                m_Core->GetLogger()->Log
                (
                    LL_Error, VOODOO_HOOK_NAME,
                    "Error %u creating hook %s (%p, %p).",
                    result, name.GetData(), pSrc, pDest
                );

                return false;
            }
            else
            {
                LhSetInclusiveACL(m_ThreadIDs, m_ThreadCount, hookHandle);

                m_Hooks[name] = hookHandle;

                return true;
            }
        }

        bool VSEHHookManager::Remove(const String & name)
        {
            HookMap::iterator hook = m_Hooks.find(name);

            m_Core->GetLogger()->Log(LL_Debug, VOODOO_HOOK_NAME, L"Removing hook %s.", name.GetData());

            if (hook != m_Hooks.end())
            {
                TRACED_HOOK_HANDLE tracedHandle = (TRACED_HOOK_HANDLE)hook->second;
                DWORD result = LhUninstallHook(tracedHandle);

                delete tracedHandle;

                if (result != 0)
                {
                    m_Core->GetLogger()->Log
                    (
                        LL_Error, VOODOO_HOOK_NAME,
                        "Error %u removing hook %s.",
                        result, name.GetData()
                    );

                    return true;
                }
                else
                {
                    m_Hooks.erase(hook);

                    return false;
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Debug, VOODOO_HOOK_NAME,
                    L"Trying to remove hook %s (does not exist).", name.GetData()
                );
                return false;
            }
        }

        void VSEHHookManager::RemoveAll(void)
        {
            LhUninstallAllHooks();
            LhWaitForPendingRemovals();

            std::for_each
            (
                m_Hooks.begin(), m_Hooks.end(),
                [](std::pair<String, TRACED_HOOK_HANDLE> chook){delete chook.second;}
            );

            m_Hooks.clear();
        }
    }
}
