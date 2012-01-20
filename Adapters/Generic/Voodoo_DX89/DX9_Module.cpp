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

#include "DX9_Module.hpp"

#include "DX9_Version.hpp"

VoodooShader::ICoreRef gpVoodooCore = nullptr;
VoodooShader::ILoggerRef gpVoodooLogger = nullptr;

VoodooShader::IEffect * testEffect = nullptr;

IDirect3DVertexBuffer9 * gpFSQuadVerts = nullptr;
IDirect3DSurface9 *backbufferSurf = nullptr;

IDirect3DSurface9 *surface_Frame0 = nullptr;
VoodooShader::ITexture* texture_Frame0 = nullptr;
IDirect3DSurface9 *surface_Pass0 = nullptr;
VoodooShader::ITexture* texture_Pass0 = nullptr;
IDirect3DSurface9 * surface_Scratch = nullptr;
VoodooShader::ITexture* texture_Scratch = nullptr;

volatile LONG gObjectLock = 0;
ModuleHook hookList[] =
{
    { false, L"d3d9.dll",      "Direct3DCreate9",      &VSDirect3DCreate9 },
    //{ false, L"d3d9.dll",      "Direct3DCreate9Ex",    &VSDirect3DCreate9Ex },
    //{ false, L"dinput8.dll",   "DirectInput8Create",   &VSDirectInput8Create },
    //{ false, L"dinput.dll",    "DirectInputCreateA",   &VSDirectInputCreateA },
    //{ false, L"dinput.dll",    "DirectInputCreateW",   &VSDirectInputCreateW },
    //{ false, L"dsound8.dll",   "DirectSoundCreate8",   &VSDirectSoundCreate8 },
    //{ false, nullptr,          nullptr,                nullptr }
};
const VoodooShader::Version * VOODOO_CALLTYPE API_PluginInit(VoodooShader::ICore * pCore)
{
    if (!pCore) return nullptr;
    gpVoodooCore = pCore;

    InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryA", &VSLoadLibraryA);
    InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryW", &VSLoadLibraryW);
    InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryExA", &VSLoadLibraryExA);
    InstallDllHook(TEXT("kernel32.dll"), "LoadLibraryExW", &VSLoadLibraryExW);

    static const VoodooShader::Version dx9version = VOODOO_VERSION_STRUCT(DX89);
    return &dx9version;
}

const uint32_t VOODOO_CALLTYPE API_ClassCount()
{
    return 0;
}

const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * refid)
{
    return nullptr;
}

VoodooShader::IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore)
{
    return nullptr;
}

// Intercept library loading to trigger hook installation
bool WINAPI InstallDllHook(LPTSTR name, LPCSTR symbol, LPVOID pDest)
{
    if (!gpVoodooCore || !name || !symbol) return false;

    VoodooShader::IHookManagerRef mgr = gpVoodooCore->GetHookManager();
    if (!mgr) return false;

    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    VoodooShader::String fullname = name;
    fullname += VSTR(".");
    fullname += symbol;
    
    return (SUCCEEDED(mgr->Add(fullname, offset, pDest)));
}

int WINAPI InstallKnownHooks()
{    int success = 0;
    int hookCount = sizeof(hookList) / sizeof(hookList[0]);

    for (int i = 0; i < hookCount; ++i)
    {
        if (!hookList[i].Installed && InstallDllHook(hookList[i].Name, hookList[i].Symbol, hookList[i].Func))
        {
            hookList[i].Installed = true;
            ++success;
        }
    }

    return success;
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

LPVOID WINAPI VSDirect3DCreate9(UINT sdkVersion)
{
    VOODOO_DEBUGBREAK;

    if (!gFunc_Direct3DCreate9)
    {
        gFunc_Direct3DCreate9 = (Type_Direct3DCreate9)FindFunction(TEXT("d3d9.dll"), true, "Direct3DCreate9", &gModule_D3D9);
    }

    LPVOID pD3D9 = gFunc_Direct3DCreate9(sdkVersion);

    if (!pD3D9)
    {
        return pD3D9;
    }
        
    if (InterlockedCompareExchange(&gSingleExport, 1, 0) == 0)
    {
        GlobalLog(VSTR("Loading Voodoo Shader from %s."), VSTR(__FUNCTION__));

        gSingleExport = false;
    }

    return pD3D9;
}
