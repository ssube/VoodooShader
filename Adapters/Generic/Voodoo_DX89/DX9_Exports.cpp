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
#pragma once

#include "DX9_Exports.hpp"
// Voodoo Support
#include "Support.inl"
// Global Types 
typedef IDirect3D8 * (WINAPI * Type_Direct3DCreate8)(UINT);
typedef IDirect3D9 * (WINAPI * Type_Direct3DCreate9)(UINT);
// Globals
HMODULE gModule_D3D8;
HMODULE gModule_D3D9;
Type_Direct3DCreate8 gFunc_Direct3DCreate8;
Type_Direct3DCreate9 gFunc_Direct3DCreate9;

// Exports
HMODULE WINAPI VSLoadLibraryA(_In_ LPCSTR lpFileName)
{
    HMODULE retval = LoadLibraryA(lpFileName);

#ifdef _DEBUG
    FILE * pf = GetVoodooGlobalLog();
    if (pf)
    {
        _ftprintf(pf, TEXT("Intercept: VSLoadLibraryA(%S)\n"), lpFileName);
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
        _ftprintf(pf, TEXT("Intercept: VSLoadLibraryW(%s)\n"), lpFileName);
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
        _ftprintf(pf, TEXT("Intercept: VSLoadLibraryExA(%S, %p, %d)\n"), lpFileName, hFile, dwFlags);
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
        _ftprintf(pf, TEXT("Intercept: VSLoadLibraryExW(%s, %p, %d)\n"), lpFileName, hFile, dwFlags);
        fclose(pf);
    }
#endif

    if (retval)
    {
        InstallKnownHooks();
    }

    return retval;
}


IDirect3D8 * WINAPI VSDirect3DCreate8(UINT sdkVersion)
{
    if (!gFunc_Direct3DCreate8)
    {
        gFunc_Direct3DCreate8 = (Type_Direct3DCreate8)FindFunction(TEXT("d3d8.dll"), true, "Direct3DCreate8", &gModule_D3D8);
    }

    IDirect3D8 * pD3D8 = gFunc_Direct3DCreate8(sdkVersion);
    
    if (!pD3D8)
    {
        return pD3D8;
    }

    volatile static UINT SingleD3D8 = 0;
    if (InterlockedCompareExchange(&SingleD3D8, 1, 0) == 0)
    {
        GlobalLog(VSTR("Loading Voodoo Shader from %s."), VSTR(__FUNCTION__));

        VoodooShader::VoodooDX8::CVoodoo3D8 * pV3D8 = new VoodooShader::VoodooDX8::CVoodoo3D8(sdkVersion, nullptr);
        pV3D8->VSCacheCaps(pD3D8);
        pD3D8->Release();

        IDirect3D9 * pD3D9 = VSDirect3DCreate9(D3D_SDK_VERSION);
        pV3D8->VSSetRealObject(pD3D9);
        pD3D8 = pV3D8;

        SingleD3D8 = 0;
    }

    return pD3D8;
}

IDirect3D9 * WINAPI VSDirect3DCreate9(UINT sdkVersion)
{
    if (!gFunc_Direct3DCreate9)
    {
        gFunc_Direct3DCreate9 = (Type_Direct3DCreate9)FindFunction(TEXT("d3d9.dll"), true, "Direct3DCreate9", &gModule_D3D9);
    }

    IDirect3D9 * pD3D9 = gFunc_Direct3DCreate9(sdkVersion);

    if (!pD3D9)
    {
        return pD3D9;
    }
        
    volatile static UINT SingleD3D9 = 0;
    if (InterlockedCompareExchange(&SingleD3D9, 1, 0) == 0)
    {
        GlobalLog(VSTR("Loading Voodoo Shader from %s."), VSTR(__FUNCTION__));

        VoodooShader::VoodooDX9::CVoodoo3D9 * pV3D9 = new VoodooShader::VoodooDX9::CVoodoo3D9(sdkVersion, pD3D9);
        pD3D9 = pV3D9;

        SingleD3D9 = 0;
    }

    return pD3D9;
}
