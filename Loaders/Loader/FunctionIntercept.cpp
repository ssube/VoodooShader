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

#include "FunctionIntercept.hpp"

#include "ExportLoadpoints.hpp"
#include "SysAPIHandler.hpp"
#include "VoodooTie.hpp"

#include <strsafe.h>
#include <map>

std::map<FARPROC, TRACED_HOOK_HANDLE> gHooks;

HMODULE gEH_Module;
func_EH_Install gEH_Install;
func_EH_Uninstall gEH_Uninstall;
func_EH_UninstallAll gEH_UninstallAll;
func_EH_SetACL gEH_SetACL;
func_EH_SetGlobalACL gEH_SetGlobalACL;
func_EH_GetLastErrorString gEH_GetLastErrorString;

ULONG threadList[] = {0};

ModuleHooks hookList[] =
{
    { L"d3d8.dll",  "Direct3DCreate8",  &Voodoo3DCreate8 },
    { L"d3d9.dll",  "Direct3DCreate9",  &Voodoo3DCreate9 },
    { nullptr,      nullptr,            nullptr }
};

TCHAR moduleName[MAX_PATH];

bool WINAPI InstallDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol, LPVOID pFunc)
{
    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    if (gHooks.find(offset) != gHooks.end()) return false;

    if (!gEH_Install && !LoadEasyHook()) return false;

    TRACED_HOOK_HANDLE tracer = new HOOK_TRACE_INFO();
    DWORD result = gEH_Install(offset, pFunc, nullptr, tracer);

    if (result)
    {
        gEH_SetACL(threadList, 1, tracer);
        gHooks[offset] = tracer;
        return true;
    } else {
        PWCHAR estr = gEH_GetLastErrorString();
        ErrorMessage(TEXT("EasyHook Error: %s"), estr);
        return false;
    }
}

bool WINAPI RemoveDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol)
{
    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    std::map<FARPROC, TRACED_HOOK_HANDLE>::iterator hook = gHooks.find(offset);
    if (hook == gHooks.end()) return false;

    DWORD result = gEH_Uninstall(hook->second);

    if (result)
    {
        delete hook->second;
        gHooks.erase(hook);
        return true;
    } else {
        return false;
    }
}

int WINAPI InstallHookList(_In_ int hookCount, _In_count_(hookCount) ModuleHooks * hooks)
{
    if (!hooks)
    {
        return 0;
    }

    int success = 0;
    TCHAR * module = nullptr;

    for (int i = 0; i < hookCount; ++i)
    {
        if (hooks[i].name) module = hooks[i].name;
        if (module)
        {
            if (InstallDllHook(hooks[i].name, hooks[i].symbol, hooks[i].func))
            {
                ++success;
            }
        }
    }

    return success;
}

int WINAPI InstallKnownHooks()
{
    return InstallHookList(2, hookList);
}

bool WINAPI LoadEasyHook()
{
    if (!GetVoodooPath()) return false;
    if (gEH_Module) return true;

    TCHAR ehPath[MAX_PATH];
    StringCchCopy(ehPath, MAX_PATH, gVoodooPath);
    StringCchCat(ehPath, MAX_PATH, TEXT("\\bin\\EasyHook32.dll"));
    gEH_Module = LoadLibrary(ehPath);

    if (gEH_Module == NULL) return false;

    gEH_Install = (func_EH_Install)GetProcAddress(gEH_Module, "_LhInstallHook@16");
    gEH_Uninstall = (func_EH_Uninstall)GetProcAddress(gEH_Module, "_LhUninstallHook@4");
    gEH_UninstallAll = (func_EH_UninstallAll)GetProcAddress(gEH_Module, "_LhUninstallAllHooks@0");
    gEH_SetACL = (func_EH_SetACL)GetProcAddress(gEH_Module, "_LhSetInclusiveACL@12");
    gEH_SetGlobalACL = (func_EH_SetGlobalACL)GetProcAddress(gEH_Module, "_LhSetGlobalInclusiveACL@8");
    gEH_GetLastErrorString = (func_EH_GetLastErrorString)GetProcAddress(gEH_Module, "_RtlGetLastErrorString@0");

    if (!gEH_SetACL || !gEH_SetGlobalACL || !gEH_Install || !gEH_Uninstall || !gEH_UninstallAll || !gEH_GetLastErrorString)
    {
        FreeLibrary(gEH_Module);
        return false;
    }

    gEH_SetGlobalACL(threadList, 1);

    return true;
}

bool WINAPI UnloadEasyHook()
{
    if (gEH_Module)
    {
        gEH_UninstallAll();

        FreeLibrary(gEH_Module);

        return true;
    } else {
        return false;
    }

}