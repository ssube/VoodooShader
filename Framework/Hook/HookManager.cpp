/**
 * \ This file is part of the Voodoo Shader Framework, a comprehensive shader
 * support library. Copyright (c) 2010-2011 by Sean Sube This program is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version. This
 * program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details. You
 * should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth
 * Floor, Boston, MA 02110-1301 US Support and more information may be found at
 * http://www.voodooshader.com, or by contacting the developer at
 * peachykeen@voodooshader.com \
 */
#include <easyhook.h>

#include "HookManager.hpp"
#include "Hook_Version.hpp"

namespace VoodooShader
{
 namespace EasyHook
{

 /**
  *
  */
 Version API_ModuleVersion(void)
 {

  Version moduleVersion = VOODOO_META_VERSION_STRUCT(HOOK);

  return moduleVersion;
 }

 /**
  *
  */
 int API_ClassCount(void)
 {
  return 1;
 }

 /**
  *
  */
 const char *API_ClassInfo(_In_ int number)
 {
  if (number == 0)
  {
   return "EHHookManager";
  }
  else
  {
   return nullptr;
  }
 }

 /**
  *
  */
 IObject *API_ClassCreate(_In_ int number, _In_ ICore *core)
 {
  if (number == 0)
  {
   return new EasyHook::HookManager(core);
  }
  else
  {
   return nullptr;
  }
 }

 /**
  *
  */
 HookManager::HookManager(ICore *core) :
  m_Core(core)
 {
  m_Hooks.clear();

  m_ThreadCount = 1;

  m_ThreadIDs = new ULONG[m_ThreadCount];
  m_ThreadIDs[0] = 0;

  LhSetGlobalInclusiveACL(m_ThreadIDs, m_ThreadCount);

  m_Core->GetLogger()->Log(LL_Info, VOODOO_HOOK_NAME, "Created hook manager.", m_ThreadCount);
 }

 /**
  *
  */
 HookManager::~HookManager(void)
 {
  this->RemoveAll();

  m_Core->GetLogger()->Log(LL_Info, VOODOO_HOOK_NAME, "Destroying hook manager.");

  delete m_ThreadIDs;
 }

 /**
  *
  */
 String HookManager::ToString(void)
 {
  return "EHHookManager";
 }

 /**
  *
  */
 ICore *HookManager::GetCore(void)
 {
  return m_Core;
 }

 /**
  *
  */
 bool HookManager::Add(String name, void *src, void *dest)
 {

  HookMap::iterator hook = m_Hooks.find(name);

  if (hook != m_Hooks.end())
  {
   Throw(VOODOO_HOOK_NAME, "Attempted to create a hook with a duplicate name.", m_Core);
  }

  m_Core->GetLogger()->Log
   (
    LL_Debug,
    VOODOO_HOOK_NAME,
    "Creating hook %s. Redirecting function %p to %p.",
    name.c_str(),
    src,
    dest
   );

  TRACED_HOOK_HANDLE hookHandle = new HOOK_TRACE_INFO();
  DWORD result = LhInstallHook(src, dest, nullptr, hookHandle);

  if ((result != 0) || (hookHandle == nullptr))
  {
   m_Core->GetLogger()->Log
    (
     LL_Error,
     VOODOO_HOOK_NAME,
     "Error %u creating hook %s (%p, %p).",
     result,
     name.c_str(),
     src,
     dest
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

 /**
  *
  */
 bool HookManager::Remove(std::string name)
 {

  HookMap::iterator hook = m_Hooks.find(name);

  m_Core->GetLogger()->Log(LL_Debug, VOODOO_HOOK_NAME, "Removing hook %s.", name.c_str());

  if (hook != m_Hooks.end())
  {

   TRACED_HOOK_HANDLE tracedHandle = (TRACED_HOOK_HANDLE) hook->second;
   DWORD result = LhUninstallHook(tracedHandle);

   delete tracedHandle;

   if (result != 0)
   {
    m_Core->GetLogger()->Log
     (
      LL_Error,
      VOODOO_HOOK_NAME,
      "Error %u removing hook %s.",
      result,
      name.c_str()
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
   Throw(VOODOO_HOOK_NAME, "Trying to remove hook that does not exist.", m_Core);
  }
 }

 /**
  *
  */
 void HookManager::RemoveAll(void)
 {
  LhUninstallAllHooks();
  LhWaitForPendingRemovals();

  m_Hooks.clear();
 }
}
}
