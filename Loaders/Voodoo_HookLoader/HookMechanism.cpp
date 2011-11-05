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

HINSTANCE gHookLoader = nullptr;
HHOOK gGlobalHook = nullptr;

std::map<FARPROC, TRACED_HOOK_HANDLE> gHooks;

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
    return (module);
}

bool WINAPI InstallDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol, LPVOID pFunc)
{
    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    if (gHooks.find(offset) != gHooks.end()) return false;

    TRACED_HOOK_HANDLE tracer = new HOOK_TRACE_INFO();
    DWORD result = LhInstallHook(offset, pFunc, nullptr, tracer);

    if (result)
    {
        LhSetInclusiveACL(m_ThreadIDs, m_ThreadCount, tracer);
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

    DWORD result = LhUninstallHook(hook->second);

    if (result)
    {
        delete hook->second;
        gHooks.erase(hook);
        return true;
    } else {
        return false;
    }
}

int WINAPI HookList(_In_z_ ModuleHooks * hooks)
{
    if (!hooks)
    {
        return 0;
    }

    int success = 0;
    for (int i = 0; hooks[i].name; ++i)
    {
        for (int j = 0; hooks[i].hooks[j].symbol; ++j)
        {
            if (HookDllFunction(hooks[i].name, hooks[i].hooks[j].symbol, hooks[i].hooks[j].func))
            {
                ++success;
            }
        }
    }

    return success;
}