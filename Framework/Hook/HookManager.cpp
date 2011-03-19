/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#include <easyhook.h>

#include "HookManager.hpp"
#include "Hook_Version.hpp"

namespace VoodooShader
{
    HookManager * CreateHookManager(_In_ Core * core)
    {
        return new EasyHook::HookManager(core);
    }

    void DestroyHookManager( _In_ HookManager * manager )
    {
        delete manager;
    }

    namespace EasyHook
    {
        HookManager::HookManager(Core * core)
            : mCore(core)
        {
            mHooks.clear();

            mThreadCount = 1;

            mThreadIDs = new ULONG[mThreadCount];
            //memcpy(mThreadIDs, threadsList, sizeof(unsigned int) * threadsCount);
            mThreadIDs[0] = 0;

            LhSetGlobalInclusiveACL(mThreadIDs, mThreadCount);

            Version hookVersion = VOODOO_META_VERSION_STRUCT(HOOK);
            mCore->LogModule(hookVersion);
            mCore->Log(LL_Info, VOODOO_HOOK_NAME, "Created hook manager attached to %u threads.", mThreadCount);
        }

        HookManager::~HookManager()
        {
            this->RemoveAllHooks();

            mCore->Log(LL_Info, VOODOO_HOOK_NAME, "Destroying hook manager.");

            delete mThreadIDs;
        }

        bool HookManager::CreateHook(std::string name, void * src, void * dest)
        {
            HookMap::iterator hook = mHooks.find(name);

            if ( hook != mHooks.end() )
            {
                Throw
                (
                    VOODOO_HOOK_NAME, 
                    "Attempted to create a hook with a duplicate name.", 
                    mCore
                );
            }

            mCore->Log
            (
                LL_Debug, 
                VOODOO_HOOK_NAME,
                "Creating hook %s. Redirecting function %p to %p.",
                name.c_str(), src, dest
            );

            TRACED_HOOK_HANDLE hookHandle = NULL;

            DWORD result = LhInstallHook(src, dest, NULL, hookHandle);

            if ( ( result != 0 ) || ( hookHandle == NULL ) )
            {
                mCore->Log
                (
                    LL_Error,
                    VOODOO_HOOK_NAME,
                    "Error %u creating hook %s (%p, %p).",
                    result, name, src, dest
                );

                return false;
            } else {
                LhSetInclusiveACL(mThreadIDs, mThreadCount, hookHandle);

                mHooks[name] = hookHandle;

                return true;
            }
        }

        bool HookManager::RemoveHook(std::string name)
        {
            HookMap::iterator hook = mHooks.find(name);

            mCore->Log
            (
                LL_Debug,
                VOODOO_HOOK_NAME,
                "Removing hook %s.",
                name.c_str()
            );

            if ( hook != mHooks.end() )
            {
                DWORD result = LhUninstallHook((TRACED_HOOK_HANDLE)hook->second);

                if ( result != 0 )
                {
                    mCore->Log
                    (
                        LL_Error,
                        VOODOO_HOOK_NAME,
                        "Error %u removing hook %s.",
                        result, name.c_str()
                    );

                    return true;
                } else {
                    mHooks.erase(hook);

                    return false;
                }
            } else {
                Throw
                (
                    VOODOO_HOOK_NAME,
                    "Trying to remove hook that does not exist.", 
                    mCore
                );
            }
        }

        void HookManager::RemoveAllHooks()
        {
            LhUninstallAllHooks();
            LhWaitForPendingRemovals();

            mHooks.clear();
        }
    }
}
