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

#include "HookManager.hpp"
#include "Hook_Version.hpp"

#include "Format.hpp"

#pragma warning(push)
#pragma warning(disable: 4005)
#include <ntstatus.h>
#pragma warning(pop)

#include <algorithm>

namespace VoodooShader
{
    namespace EasyHook
    {
        static const Version moduleVersion = VOODOO_META_VERSION_STRUCT(HOOK);
        static const wchar_t * name_VSEHHookManager = L"VSEHHookManager";
        static const Uuid clsid_VSEHHookManager = CLSID_VSEHHookManager;

        const Version * VOODOO_CALLTYPE API_ModuleVersion()
        {

            return &moduleVersion;
        }

        const uint32_t VOODOO_CALLTYPE API_ClassCount()
        {
            return 1;
        }

        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid)
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

        IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t number, _In_ ICore *core)
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

            m_Core->GetLogger()->LogMessage(LL_ModInfo, VOODOO_HOOK_NAME, VSTR("Created hook manager."));
        }

        VSEHHookManager::~VSEHHookManager()
        {
            this->RemoveAll();

            m_Core->GetLogger()->LogMessage(LL_ModInfo, VOODOO_HOOK_NAME, VSTR("Destroying hook manager."));

            delete[] m_ThreadIDs;
        }

        uint32_t VSEHHookManager::AddRef() const
        {
            return SAFE_INCREMENT(m_Refs);
        }


        uint32_t VSEHHookManager::Release() const
        {
            if (SAFE_DECREMENT(m_Refs) == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSEHHookManager::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
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
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                } else if (clsid == IID_IHookManager) {
                    *ppOut = static_cast<const IHookManager*>(this);
                } else if (clsid == CLSID_VSEHHookManager) {
                    *ppOut = static_cast<const VSEHHookManager*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String VSEHHookManager::ToString() const
        {
            return VSTR("VSEHHookManager");
        }

        ICore * VSEHHookManager::GetCore() const
        {
            return m_Core;
        }

        bool VSEHHookManager::Add(const String & name, void * pSrc, void * pDest)
        {
            HookMap::iterator hook = m_Hooks.find(name);

            if (hook != m_Hooks.end())
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_ModError, VOODOO_HOOK_NAME,
                    Format(VSTR("Attempted to create a hook with a duplicate name (%1%).")) << name
                );

                return false;
            }

            m_Core->GetLogger()->LogMessage
            (
                LL_ModDebug, VOODOO_HOOK_NAME,
                Format(VSTR("Creating hook %1%. Redirecting function %2% to %3%.")) << name << pSrc << pDest
            );

            TRACED_HOOK_HANDLE hookHandle = new HOOK_TRACE_INFO();
            DWORD result = LhInstallHook(pSrc, pDest, nullptr, hookHandle);

            if (result == STATUS_NOT_SUPPORTED || result == STATUS_NO_MEMORY || result == STATUS_INSUFFICIENT_RESOURCES)
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_ModError, VOODOO_HOOK_NAME,
                    Format(VSTR("Error %1 creating hook %s.")) << (uint32_t)result << name
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

            m_Core->GetLogger()->LogMessage(LL_ModDebug, VOODOO_HOOK_NAME, Format(VSTR("Removing hook %1%.")) << name);

            if (hook != m_Hooks.end())
            {
                TRACED_HOOK_HANDLE tracedHandle = (TRACED_HOOK_HANDLE)hook->second;
                DWORD result = LhUninstallHook(tracedHandle);

                delete tracedHandle;

                if (result != 0)
                {
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_ModError, VOODOO_HOOK_NAME,
                        Format(VSTR("Error %1% removing hook %2%.")) << result << name
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
                m_Core->GetLogger()->LogMessage
                (
                    LL_ModDebug, VOODOO_HOOK_NAME,
                    Format(VSTR("Trying to remove hook %1% (does not exist).")) << name.GetData()
                );
                return false;
            }
        }

        bool VSEHHookManager::RemoveAll()
        {
            LhUninstallAllHooks();
            LhWaitForPendingRemovals();

            std::for_each
            (
                m_Hooks.begin(), m_Hooks.end(),
                [](std::pair<String, TRACED_HOOK_HANDLE> chook){delete chook.second;}
            );

            m_Hooks.clear();

            return true;
        }
    }
}
