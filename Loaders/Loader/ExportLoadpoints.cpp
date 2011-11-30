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

#include "ExportLoadpoints.hpp"

#include "SysAPIHandler.hpp"
#include "VoodooTie.hpp"

#include <stdlib.h>

/**
 * Most DirectX libraries use an identical loading function, with only the name varying. This function takes an SDK
 * version, library name and function name, loads and calls the proper init function.
 */
void * WINAPI VoodooDXCreateGeneric(UINT sdkVersion, const LPTSTR lib, const LPCSTR func)
{
    typedef void * (WINAPI * DXInitFunc) (UINT);

    HMODULE baselib = LoadSystemLibrary(lib);

    if (!baselib)
    {
        MessageBox(nullptr, L"Voodoo Loader: Unable to load system DLL.", L"Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DXInitFunc initFunc = (DXInitFunc) GetProcAddress(baselib, func);

    if (!initFunc)
    {
        MessageBox(nullptr, L"Voodoo Loader: Unable to find system EP.", L"Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    return (*initFunc) (sdkVersion);
}

/**
 * Direct3D 8
 */
void * WINAPI Voodoo3DCreate8(UINT sdkVersion)
{
    void * pD3D8 = VoodooDXCreateGeneric(sdkVersion, L"d3d8.dll", "Direct3DCreate8");

    if (LoadVoodoo())
    {
        VoodooShader::Variant D3DObj;
        D3DObj.Type = VoodooShader::UT_PVoid;
        D3DObj.VPVoid = pD3D8;

        gVoodooCore->GetAdapter()->SetProperty(L"IDirect3D8", &D3DObj);

        if (D3DObj.VPVoid != pD3D8)
        {
            pD3D8 = D3DObj.VPVoid;
        }
    }

    return pD3D8;
}

/**
 * Direct3D 9
 */
void * WINAPI Voodoo3DCreate9(UINT sdkVersion)
{
    void * pD3D9 = VoodooDXCreateGeneric(sdkVersion, L"d3d9.dll", "Direct3DCreate9");

    if (LoadVoodoo())
    {
        VoodooShader::Variant D3DObj;
        D3DObj.Type = VoodooShader::UT_PVoid;
        D3DObj.VPVoid = pD3D9;

        gVoodooCore->GetAdapter()->SetProperty(L"IDirect3D9", &D3DObj);

        if (D3DObj.VPVoid != pD3D9)
        {
            pD3D9 = D3DObj.VPVoid;
        }
    }

    return pD3D9;
}

/**
 * DirectInput 8
 */
HRESULT WINAPI VoodooInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPVOID punkOuter)
{
    typedef HRESULT (WINAPI *DIInitFunc) (HINSTANCE, DWORD, REFIID, LPVOID *, LPVOID);

    HMODULE baselib = LoadSystemLibrary(L"dinput8.dll");

    if (!baselib)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DIInitFunc initFunc = (DIInitFunc) GetProcAddress(baselib, "DirectInput8Create");

    if (!initFunc)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    LPVOID i8obj = nullptr;
    HRESULT result = initFunc(hinst, dwVersion, riidltf, &i8obj, punkOuter);

    if (SUCCEEDED(result) && LoadVoodoo())
    {
        VoodooShader::Variant DIObj;
        DIObj.Type = VoodooShader::UT_PVoid;
        DIObj.VPVoid = i8obj;

        gVoodooCore->GetAdapter()->SetProperty(L"IDirectInput8", &DIObj);

        if (DIObj.VPVoid != i8obj)
        {
            i8obj = DIObj.VPVoid;
        }
    }

    *ppvOut = i8obj;
    return result;
}

/**
 * DirectInput (unknown version)
 */
HRESULT WINAPI VoodooInputCreateGeneric
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID *lplpDirectInput,
    LPVOID punkOuter,
    const char * func
)
{
    typedef HRESULT (WINAPI *DIInitFunc) (HINSTANCE, DWORD, LPVOID, LPVOID);

    HMODULE baselib = LoadSystemLibrary(L"dinput.dll");

    if (!baselib)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DIInitFunc initFunc = (DIInitFunc) GetProcAddress(baselib, func);

    if (!initFunc)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    return (*initFunc) (hinst, dwVersion, lplpDirectInput, punkOuter);
}

HRESULT WINAPI VoodooInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID *lplpDirectInput, LPVOID punkOuter)
{
    LPVOID iaobj = nullptr;
    HRESULT result = VoodooInputCreateGeneric(hinst, dwVersion, &iaobj, punkOuter, "DirectInputCreateA");

    if (SUCCEEDED(result) && LoadVoodoo())
    {
        VoodooShader::Variant DIObj;
        DIObj.Type = VoodooShader::UT_PVoid;
        DIObj.VPVoid = iaobj;

        gVoodooCore->GetAdapter()->SetProperty(L"IDirectInputA", &DIObj);

        if (DIObj.VPVoid != iaobj)
        {
            iaobj = DIObj.VPVoid;
        }
    }

    *lplpDirectInput = iaobj;
    return result;
}

HRESULT WINAPI VoodooInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPVOID *lplpDirectInput, LPVOID punkOuter)
{
    LPVOID iwobj = nullptr;
    HRESULT result = VoodooInputCreateGeneric(hinst, dwVersion, &iwobj, punkOuter, "DirectInputCreateW");

    if (SUCCEEDED(result) && LoadVoodoo())
    {
        VoodooShader::Variant DIObj;
        DIObj.Type = VoodooShader::UT_PVoid;
        DIObj.VPVoid = iwobj;

        gVoodooCore->GetAdapter()->SetProperty(L"IDirectInputW", &DIObj);

        if (DIObj.VPVoid != iwobj)
        {
            iwobj = DIObj.VPVoid;
        }
    }

    *lplpDirectInput = iwobj;
    return result;
}

/**
 * DirectSound 8
 */
HRESULT WINAPI VoodooSoundCreate8(LPCGUID lpcGuidDevice, LPVOID * ppDS8, LPVOID pUnkOuter)
{
    typedef HRESULT (__stdcall *DSInitFunc) (LPCGUID, LPVOID *, LPVOID);

    HMODULE baselib = LoadSystemLibrary(L"dsound8.dll");

    if (!baselib)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DSInitFunc initFunc = (DSInitFunc) GetProcAddress(baselib, "DirectSoundCreate8");

    if (!initFunc)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    LPVOID s8obj = nullptr;
    HRESULT result = initFunc(lpcGuidDevice, &s8obj, pUnkOuter);

    if (SUCCEEDED(result) && LoadVoodoo())
    {
        VoodooShader::Variant DSObj;
        DSObj.Type = VoodooShader::UT_PVoid;
        DSObj.VPVoid = s8obj;

        gVoodooCore->GetAdapter()->SetProperty(L"IDirectSound8", &DSObj);

        if (DSObj.VPVoid != s8obj)
        {
            s8obj = DSObj.VPVoid;
        }
    }

    *ppDS8 = s8obj;
    return result;
}
