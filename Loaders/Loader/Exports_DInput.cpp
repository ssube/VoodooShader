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

#include "ExportLoadpoints.hpp"

#include "VoodooTie.hpp"

#include "Support.inl"

#include <stdlib.h>

typedef DWORD (WINAPI * Type_DirectInput8Create)(HINSTANCE, DWORD, REFIID, LPVOID *, LPVOID);
typedef DWORD (WINAPI * Type_DirectInputCreate)(HINSTANCE, DWORD, LPVOID, LPVOID);

HMODULE gModule_DInput;
HMODULE gModule_DInput8;

Type_DirectInput8Create gFunc_DirectInput8Create;
Type_DirectInputCreate gFunc_Type_DirectInputCreateA;
Type_DirectInputCreate gFunc_Type_DirectInputCreateW;

/**
 * DirectInput 8
 */
HRESULT WINAPI VSDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPVOID punkOuter)
{
    VOODOO_DEBUGBREAK;

    if (!gFunc_DirectInput8Create)
    {
        gFunc_DirectInput8Create = (Type_DirectInput8Create)FindFunction(TEXT("dinput8.dll"), true, "DirectInput8Create", &gModule_DInput8);
    }

    LPVOID i8obj = nullptr;
    DWORD result = gFunc_DirectInput8Create(hinst, dwVersion, riidltf, &i8obj, punkOuter);

    if (FAILED(result))
    {
        *ppvOut = i8obj;
        return result;
    }
    
    if (InterlockedCompareExchange(&gSingleExport, 1, 0) == 0)
    {
        if (LoadVoodoo())
        {
            VoodooShader::Variant DIObj;
            ZeroMemory(&DIObj, sizeof(VoodooShader::Variant));
            DIObj.Type = VoodooShader::UT_UInt32;
            DIObj.Components = 1;
            DIObj.VUInt32.X = dwVersion;

            gVoodooCore->GetAdapter()->SetProperty(VSTR("DIVersion"), &DIObj);

            ZeroMemory(&DIObj, sizeof(VoodooShader::Variant));
            DIObj.Type = VoodooShader::UT_PVoid;
            DIObj.Components = 1;
            DIObj.VPVoid = i8obj;

            if (gVoodooCore->GetAdapter()->SetProperty(L"IDirectInput8", &DIObj))
            {
                i8obj = DIObj.VPVoid;
            }
        }
        gSingleExport = false;
    }

    *ppvOut = i8obj;
    return result;
}

/**
 * DirectInput (unknown version)
 */
HRESULT WINAPI VSDirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID * lplpDirectInput, LPVOID punkOuter)
{
    VOODOO_DEBUGBREAK;

    if (!gFunc_Type_DirectInputCreateA)
    {
        gFunc_Type_DirectInputCreateA = (Type_DirectInputCreate)FindFunction(TEXT("dinput8.dll"), true, "DirectInputCreateA", &gModule_DInput8);
    }

    LPVOID iaobj = nullptr;
    DWORD result = gFunc_Type_DirectInputCreateA(hinst, dwVersion, &iaobj, punkOuter);

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

HRESULT WINAPI VSDirectInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPVOID * lplpDirectInput, LPVOID punkOuter)
{
    VOODOO_DEBUGBREAK;

    if (!gFunc_Type_DirectInputCreateW)
    {
        gFunc_Type_DirectInputCreateW = (Type_DirectInputCreate)FindFunction(TEXT("dinput8.dll"), true, "DirectInputCreateW", &gModule_DInput8);
    }

    LPVOID iwobj = nullptr;
    DWORD result = gFunc_Type_DirectInputCreateW(hinst, dwVersion, &iwobj, punkOuter);

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
