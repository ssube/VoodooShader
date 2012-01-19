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

#include "FunctionIntercept.hpp"

#include "ExportLoadpoints.hpp"
#include "VoodooTie.hpp"

#include "Support.inl"

#include <strsafe.h>
#include <map>

volatile LONG gSingleExport;

std::map<FARPROC, TRACED_HOOK_HANDLE> gHooks;

ULONG threadList[] = {0};

ModuleHook hookList[] =
{
    { false, L"d3d8.dll",      "Direct3DCreate8",      &VSDirect3DCreate8 },
    { false, L"d3d9.dll",      "Direct3DCreate9",      &VSDirect3DCreate9 },
    { false, L"d3d9.dll",      "Direct3DCreate9Ex",    &VSDirect3DCreate9Ex },
    { false, L"d3d10.dll",     "D3D10CreateDevice",    &VSD3D10CreateDevice },
    { false, L"dinput8.dll",   "DirectInput8Create",   &VSDirectInput8Create },
    { false, L"dinput.dll",    "DirectInputCreateA",   &VSDirectInputCreateA },
    { false, L"dinput.dll",    "DirectInputCreateW",   &VSDirectInputCreateW },
    { false, L"dsound8.dll",   "DirectSoundCreate8",   &VSDirectSoundCreate8 },
    { false, nullptr,          nullptr,                nullptr }
};

TCHAR moduleName[MAX_PATH];

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        InstallSystemHooks();

        gLoaderHandle = hinstDLL;
    }

    return TRUE;
}

bool WINAPI InstallDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol, LPVOID pFunc)
{
    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    if (gHooks.find(offset) != gHooks.end()) return false;

#ifdef _DEBUG
    FILE * pf = GetVoodooGlobalLog();
    if (pf)
    {
        _ftprintf(pf, TEXT("InstallDllHook(%s, %S, %p) == "), name, symbol, pFunc);
    }
#endif

    TRACED_HOOK_HANDLE tracer = new HOOK_TRACE_INFO();
    NTSTATUS result = LhInstallHook(offset, pFunc, nullptr, tracer);

    if (result != STATUS_NO_MEMORY && result != 0xC00000BB && result != 0xC000009A)
    {
        LhSetInclusiveACL(threadList, 1, tracer);
        gHooks[offset] = tracer;

#ifdef _DEBUG
        if (pf)
        {
            _ftprintf(pf, TEXT("true\n"));
            fclose(pf);
        }
#endif

        return true;
    }
    else
    {
        PWCHAR estr = RtlGetLastErrorString();
        ErrorMessage(0x200A, TEXT("EasyHook Error: %s"), estr);

#ifdef _DEBUG
        if (pf)
        {
            _ftprintf(pf, TEXT("false\n"));
            fclose(pf);
        }
#endif

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
    }
    else
    {
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

    for (int i = 0; i < hookCount; ++i)
    {
        if (!hooks[i].Installed && InstallDllHook(hooks[i].Name, hooks[i].Symbol, hooks[i].Func))
        {
            hooks[i].Installed = true;
            ++success;
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

// Intercept library loading to trigger hook installation
bool WINAPI InstallSystemHooks()
{
    if (!InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryA", &VSLoadLibraryA)) return false;
    if (!InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryW", &VSLoadLibraryW)) return false;
    if (!InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryExA", &VSLoadLibraryExA)) return false;
    if (!InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryExW", &VSLoadLibraryExW)) return false;
    return true;
}

HMODULE WINAPI VSLoadLibraryA(_In_ LPCSTR lpFileName)
{
    HMODULE retval = LoadLibraryA(lpFileName);

#ifdef _DEBUG
    FILE * pf = GetVoodooGlobalLog();
    if (pf)
    {
        _ftprintf(pf, TEXT("DETOUR: VSLoadLibraryA(%S)\n"), lpFileName);
        fclose(pf);
    }
#endif

    if (retval)
    {
        InstallKnownHooks();
    }

    return retval;
}

HMODULE WINAPI VSLoadLibraryW(_In_ LPCWSTR lpFileName)
{
    HMODULE retval = LoadLibraryW(lpFileName);

#ifdef _DEBUG
    FILE * pf = GetVoodooGlobalLog();
    if (pf)
    {
        _ftprintf(pf, TEXT("DETOUR: VSLoadLibraryW(%s)\n"), lpFileName);
        fclose(pf);
    }
#endif

    if (retval)
    {
        InstallKnownHooks();
    }

    return retval;
}

HMODULE WINAPI VSLoadLibraryExA(_In_ LPCSTR lpFileName, HANDLE hFile, _In_ DWORD dwFlags)
{
    HMODULE retval = LoadLibraryExA(lpFileName, hFile, dwFlags);

#ifdef _DEBUG
    FILE * pf = GetVoodooGlobalLog();
    if (pf)
    {
        _ftprintf(pf, TEXT("DETOUR: VSLoadLibraryExA(%S, %p, %d)\n"), lpFileName, hFile, dwFlags);
        fclose(pf);
    }
#endif

    if (retval)
    {
        InstallKnownHooks();
    }

    return retval;
}

HMODULE WINAPI VSLoadLibraryExW(_In_ LPCWSTR lpFileName, HANDLE hFile, _In_ DWORD dwFlags)
{
    HMODULE retval = LoadLibraryExW(lpFileName, hFile, dwFlags);

#ifdef _DEBUG
    FILE * pf = GetVoodooGlobalLog();
    if (pf)
    {
        _ftprintf(pf, TEXT("DETOUR: VSLoadLibraryExW(%s, %p, %d)\n"), lpFileName, hFile, dwFlags);
        fclose(pf);
    }
#endif

    if (retval)
    {
        InstallKnownHooks();
    }

    return retval;
}
