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

#include "HookManager.hpp"
#include "VoodooGL.hpp"

namespace VoodooShader
{
	namespace Frost
	{
		HookManager * VoodooHooker = NULL;

		/**
		 * Install a single hook at the specified point. This will only affect the
		 * process(es) the HookManager is bound to.
		 *
		 * @param name The name for the hook.
		 * @param src The point to install the hook at.
		 * @param dest The function to redirect execution into.
		 * @return The success of the hook installation.
		 * @throws Exception if a hook with the same name already exists.
		 *
		 * @note The name is often the name of the function in src (&func) for
		 *		simplicities sake. 
		 * @warning The calling convention of src and dest must be identical, or bad
		 *		things might happen. This is only a bother with member functions, but
		 *		can be worked around relatively easily.
		 */
		bool HookManager::InstallHook(std::string name, void * src, void * dest)
		{
			HookMap::iterator hook = mHooks.find(name);

			if ( hook != mHooks.end() )
			{
				Throw("Voodoo Frost: Attempted to install a hook with a duplicate name.", VoodooCore);
			}

			TRACED_HOOK_HANDLE hookHandle = NULL;

			DWORD result = LhInstallHook(src, dest, NULL, hookHandle);

			if ( ( result != 0 ) || ( hookHandle == NULL ) )
			{
				VoodooCore->GetLog()->Format("Voodoo Frost: Error %d installing hook %s (%d, %d).\n")
					.With(result).With(name).With(src).With(dest).Done();

				return false;
			} else {
				LhSetInclusiveACL(mThreadIDs, mThreadCount, hookHandle);

				mHooks[name] = hookHandle;

				return true;
			}
		}

		/**
		 * Uninstall a hook.
		 *
		 * @param name The name of the hook to remove.
		 * @return The success of the removal operation.
		 * @throws Exception of the hook is not found.
		 * 
		 * @warning <em>Do not</em>, under any circumstances, remove a hook while
		 *		execution is passing through the trampoline function. This can cause
		 *		the process to crash in rare cases. I'm not sure the reason, but it's
		 *		not good. Until I replace EasyHook, be careful!
		 */
		bool HookManager::UninstallHook(std::string name)
		{
			HookMap::iterator hook = mHooks.find(name);

			if ( hook != mHooks.end() )
			{
				DWORD result = LhUninstallHook(hook->second);

				if ( result != 0 )
				{
					VoodooCore->GetLog()->Format("Voodoo Frost: Error %d removing hook %s.\n")
						.With(result).With(name).Done();

					return true;
				} else {
					mHooks.erase(hook);

					return false;
				}
			} else {
				Throw("Voodoo Frost: Trying to uninstall hook that does not exist.", VoodooCore);
			}
		}

		/**
		 * Uninstalls all hooks.
		 */
		void HookManager::UninstallAllHooks()
		{
			LhUninstallAllHooks();
			LhWaitForPendingRemovals();

			mHooks.clear();
		}

		HookManager::HookManager()
		{
			mHooks.clear();

			mThreadIDs = new ULONG;
			mThreadCount = 1;

			mThreadIDs[0] = NULL;

			LhSetGlobalInclusiveACL(mThreadIDs, mThreadCount);
		}

		HookManager::~HookManager()
		{
			this->UninstallAllHooks();

			delete mThreadIDs;
		}

		#define HOOK_PARAMS(n) #n, &n, &v##n

		/**
		 * Install all significant OpenGL hooks. 
		 *
		 * @todo This function needs moved out into the DLL init process.
		 */
		void HookOpenGL(void)
		{
			VoodooCore->GetLog()->Log("Voodoo Frost: Beginning OpenGL hook procedure.\n");

			bool success = true;

			// System-related
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glGetString));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glViewport));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglCreateContext));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglDeleteContext));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglGetProcAddress));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglMakeCurrent));

			// Shader-related
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glClear));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglSwapLayerBuffers));

			// Material-related
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glBindTexture));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glDeleteTextures));

			// Shader/material shared
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glBegin));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glDrawElements));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glEnd));

			// Fog-related
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glEnable));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glFogf));
			success &= VoodooHooker->InstallHook(HOOK_PARAMS(glFogfv));

			// Check the results and handle
			if ( success )
			{
				VoodooCore->GetLog()->Log("Voodoo Frost: OpenGL hooked successfully.\n");
			} else {
				VoodooCore->GetLog()->Log("Voodoo Frost: OpenGL hook procedure failed.\n");
			}
		}
	}
}
