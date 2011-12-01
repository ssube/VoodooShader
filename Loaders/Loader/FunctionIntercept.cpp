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

ULONG threadList[] = {0};

ModuleHook hookList[] =
{
    { L"d3d8.dll",      "Direct3DCreate8",      &VSDirect3DCreate8 },
    { L"d3d9.dll",      "Direct3DCreate9",      &VSDirect3DCreate9 },
    { L"d3d9.dll",      "Direct3DCreate9Ex",    &VSDirect3DCreate9Ex },
    { L"d3d10.dll",     "D3D10CreateDevice",    &VSD3D10CreateDevice },
    { L"dinput8.dll",   "DirectInput8Create",   &VSDirectInput8Create },
    { L"dinput.dll",    "DirectInputCreateA",   &VSDirectInputCreateA },
    { L"dinput.dll",    "DirectInputCreateW",   &VSDirectInputCreateW },
    { L"dsound8.dll",   "DirectSoundCreate8",   &VSDirectSoundCreate8 },
    { nullptr,          nullptr,                nullptr }
};

TCHAR moduleName[MAX_PATH];

bool WINAPI InstallDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol, LPVOID pFunc)
{
    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    if (gHooks.find(offset) != gHooks.end()) return false;

    TRACED_HOOK_HANDLE tracer = new HOOK_TRACE_INFO();
    NTSTATUS result = LhInstallHook(offset, pFunc, nullptr, tracer);

    if (result != STATUS_NO_MEMORY && result != 0xC00000BB && result != 0xC000009A)
    {
        LhSetInclusiveACL(threadList, 1, tracer);
        gHooks[offset] = tracer;
        return true;
    } else {
        PWCHAR estr = RtlGetLastErrorString();
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

    NTSTATUS result = LhUninstallHook(hook->second);

    if (result)
    {
        delete hook->second;
        gHooks.erase(hook);
        return true;
    } else {
        return false;
    }
}

int WINAPI InstallHookList(_In_ int hookCount, _In_count_(hookCount) ModuleHook * hooks)
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
    return InstallHookList(sizeof(hookList) / sizeof(ModuleHook), hookList);
}

bool WINAPI LoadEasyHook()
{
    LhSetGlobalInclusiveACL(threadList, 1);

    return true;
}

bool WINAPI UnloadEasyHook()
{
    LhUninstallAllHooks();

    return true;
}