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

#include "HookMechanism.hpp"

#include <map>
#include <fstream>

HINSTANCE gHookLoader = nullptr;
HHOOK gGlobalHook = nullptr;

std::map<FARPROC, TRACED_HOOK_HANDLE> gHooks;

HMODULE gEH_Module;
func_EH_Install gEH_Install;
func_EH_Uninstall gEH_Uninstall;
func_EH_UninstallAll gEH_UninstallAll;
func_EH_SetACL gEH_SetACL;
func_EH_SetGlobalACL gEH_SetGlobalACL;

ULONG threadList[] = {0};

ModuleHooks hookList[] =
{
    { L"d3d8.dll", "Direct3DCreate8", nullptr },
    { L"d3d9.dll", "Direct3DCreate9", nullptr },
};

std::wfstream fout;
TCHAR moduleName[MAX_PATH];

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    DisableThreadLibraryCalls(hinstDLL);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        gHookLoader = hinstDLL;
    }

    return TRUE;
}

LRESULT CALLBACK GlobalHookCb(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    } else {
        if (!fout.is_open())
        {
            fout.open("D:\\Hook.log");
            GetModuleFileName(GetModuleHandle(NULL), moduleName, MAX_PATH);
        } else {
            fout << moduleName << L": GlobalHookCb(" << nCode << "," << wParam << ", " << lParam << ");";
        }

        if (nCode == HCBT_CREATEWND)
        {
            MessageBox(NULL, L"Voodoo Hook triggered: HCBT_CREATEWND", L"Voodoo Hook", MB_OK);
        } else if (nCode == HCBT_DESTROYWND) {
            MessageBox(NULL, L"Voodoo Hook triggered: HCBT_DESTROYWND", L"Voodoo Hook", MB_OK);
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}

bool WINAPI InstallGlobalHook()
{
    gGlobalHook = SetWindowsHookEx(WH_CBT, &GlobalHookCb, gHookLoader, 0);
    if (!gGlobalHook)
    {
        DWORD error = GetLastError();
        return false;
    }

    return true;
}

bool WINAPI RemoveGlobalHook()
{
    if (gGlobalHook)
    {
        BOOL success = UnhookWindowsHookEx(gGlobalHook);
        if (success == 0)
        {
            DWORD error = GetLastError();
            return false;
        }
    }

    return true;
}

bool WINAPI IsDllLoaded(_In_z_ LPTSTR name)
{
    HMODULE module = GetModuleHandle(name);
    return (module != NULL);
}

bool WINAPI InstallDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol, LPVOID pFunc)
{
    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    if (gHooks.find(offset) != gHooks.end()) return false;

    if (!gEH_Install && LoadEasyHook()) return false;

    TRACED_HOOK_HANDLE tracer = new HOOK_TRACE_INFO();
    DWORD result = gEH_Install(offset, pFunc, nullptr, tracer);

    if (result)
    {
        gEH_SetACL(threadList, 1, tracer);
        gHooks[offset] = tracer;
        return true;
    } else {
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

int WINAPI InstallHookList(_In_z_ ModuleHooks * hooks)
{
    if (!hooks)
    {
        return 0;
    }

    int success = 0;
    TCHAR * module = nullptr;

    for (int i = 0; hooks[i].name || hooks[i].symbol; ++i)
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

bool WINAPI LoadEasyHook()
{
    gEH_Module = LoadLibrary(L"EasyHook.dll");

    if (gEH_Module == NULL) return false;

    gEH_Install = (func_EH_Install)GetProcAddress(gEH_Module, "_LhInstallHook@16");
    gEH_Uninstall = (func_EH_Uninstall)GetProcAddress(gEH_Module, "_LhUninstallHook@4");
    gEH_UninstallAll = (func_EH_UninstallAll)GetProcAddress(gEH_Module, "_LhUninstallAllHooks@0");
    gEH_SetACL = (func_EH_SetACL)GetProcAddress(gEH_Module, "_LhSetInclusiveACL@12");
    gEH_SetGlobalACL = (func_EH_SetGlobalACL)GetProcAddress(gEH_Module, "_LhSetGlobalInclusiveACL@8");

    if (!gEH_SetACL || !gEH_SetGlobalACL || !gEH_Install || !gEH_Uninstall)
    {
        FreeLibrary(gEH_Module);
        return false;
    }

    gEH_SetGlobalACL(threadList, 1);

    return true;
}

bool WINAPI UnloadEasyHook()
{
    gEH_UninstallAll();

    FreeLibrary(gEH_Module);

    return true;
}