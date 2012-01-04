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

/**
 * Direct3D 9
 */
typedef int (WINAPI * Type_D3DPERF_BeginEvent)(DWORD, LPCWSTR);
typedef int (WINAPI * Type_D3DPERF_EndEvent)();
typedef int (WINAPI * Type_D3DPERF_GetStatus)();
typedef BOOL (WINAPI * Type_D3DPERF_QueryRepeatFrame)();
typedef void (WINAPI * Type_D3DPERF_SetMarker)(DWORD, LPCWSTR);
typedef void (WINAPI * Type_D3DPERF_SetOptions)(DWORD);
typedef void (WINAPI * Type_D3DPERF_SetRegion)(DWORD, LPCWSTR);
typedef LPVOID (WINAPI * Type_Direct3DCreate9)(UINT);
typedef DWORD (WINAPI * Type_Direct3DCreate9Ex)(UINT, LPVOID *);

HMODULE gModule_D3D9;
Type_D3DPERF_BeginEvent gFunc_D3DPERF_BeginEvent;
Type_D3DPERF_EndEvent gFunc_D3DPERF_EndEvent;
Type_D3DPERF_GetStatus gFunc_D3DPERF_GetStatus;
Type_D3DPERF_QueryRepeatFrame gFunc_D3DPERF_QueryRepeatFrame;
Type_D3DPERF_SetMarker gFunc_D3DPERF_SetMarker;
Type_D3DPERF_SetOptions gFunc_D3DPERF_SetOptions;
Type_D3DPERF_SetRegion gFunc_D3DPERF_SetRegion;
Type_Direct3DCreate9 gFunc_Direct3DCreate9;
Type_Direct3DCreate9Ex gFunc_Direct3DCreate9Ex;

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
        if (LoadVoodoo())
        {
            VoodooShader::Variant D3DObj;
            ZeroMemory(&D3DObj, sizeof(VoodooShader::Variant));
            D3DObj.Type = VoodooShader::UT_UInt32;
            D3DObj.Components = 1;
            D3DObj.VUInt32.X = sdkVersion;

            gVoodooCore->GetAdapter()->SetProperty(VSTR("SdkVersion"), &D3DObj);

            ZeroMemory(&D3DObj, sizeof(VoodooShader::Variant));
            D3DObj.Type = VoodooShader::UT_PVoid;
            D3DObj.Components = 1;
            D3DObj.VPVoid = pD3D9;
            
            if (gVoodooCore->GetAdapter()->SetProperty(L"IDirect3D9", &D3DObj))
            {
                pD3D9 = D3DObj.VPVoid;
            }
        }

        gSingleExport = false;
    }

    return pD3D9;
}

HRESULT WINAPI VSDirect3DCreate9Ex(UINT sdkVersion, LPVOID * dx)
{
    VOODOO_DEBUGBREAK;

    if (!gFunc_Direct3DCreate9Ex)
    {
        gFunc_Direct3DCreate9Ex = (Type_Direct3DCreate9Ex)FindFunction(TEXT("d3d9.dll"), true, "Direct3DCreate9Ex", &gModule_D3D9);
    }

    DWORD hr = gFunc_Direct3DCreate9Ex(sdkVersion, dx);

    if (FAILED(hr))
    {
        return hr;
    }
    
    if (InterlockedCompareExchange(&gSingleExport, 1, 0) == 0)
    {
        if (LoadVoodoo())
        {
            VoodooShader::Variant D3DObj;
            ZeroMemory(&D3DObj, sizeof(VoodooShader::Variant));
            D3DObj.Type = VoodooShader::UT_UInt32;
            D3DObj.Components = 1;
            D3DObj.VUInt32.X = sdkVersion;

            gVoodooCore->GetAdapter()->SetProperty(VSTR("SdkVersion"), &D3DObj);

            ZeroMemory(&D3DObj, sizeof(VoodooShader::Variant));
            D3DObj.Type = VoodooShader::UT_PVoid;
            D3DObj.Components = 1;
            D3DObj.VPVoid = *dx;

            if (gVoodooCore->GetAdapter()->SetProperty(L"IDirect3D9Ex", &D3DObj))
            {
                *dx = D3DObj.VPVoid;
            }
        }
        gSingleExport = false;
    }

    return hr;
}

int WINAPI VSD3DPERF_BeginEvent(DWORD dw, LPCWSTR str)
{
    if (!gFunc_D3DPERF_BeginEvent)
    {
        gFunc_D3DPERF_BeginEvent = (Type_D3DPERF_BeginEvent)FindFunction(TEXT("d3d9.dll"), true, "D3DPERF_BeginEvent", &gModule_D3D9);
    }

    return gFunc_D3DPERF_BeginEvent(dw, str);
}

int WINAPI VSD3DPERF_EndEvent()
{
    if (!gFunc_D3DPERF_EndEvent)
    {
        gFunc_D3DPERF_EndEvent = (Type_D3DPERF_EndEvent)FindFunction(TEXT("d3d9.dll"), true, "D3DPERF_EndEvent", &gModule_D3D9);
    }

    return gFunc_D3DPERF_EndEvent();
}

DWORD WINAPI VSD3DPERF_GetStatus()
{
    if (!gFunc_D3DPERF_GetStatus)
    {
        gFunc_D3DPERF_GetStatus = (Type_D3DPERF_GetStatus)FindFunction(TEXT("d3d9.dll"), true, "D3DPERF_GetStatus", &gModule_D3D9);
    }

    return gFunc_D3DPERF_GetStatus();
}

BOOL WINAPI VSD3DPERF_QueryRepeatFrame()
{
    if (!gFunc_D3DPERF_QueryRepeatFrame)
    {
        gFunc_D3DPERF_QueryRepeatFrame = (Type_D3DPERF_QueryRepeatFrame)FindFunction(TEXT("d3d9.dll"), true, "D3DPERF_QueryRepeatFrame", &gModule_D3D9);
    }

    return gFunc_D3DPERF_QueryRepeatFrame();
}

void WINAPI VSD3DPERF_SetMarker(DWORD dw, LPCWSTR str)
{
    if (!gFunc_D3DPERF_SetMarker)
    {
        gFunc_D3DPERF_SetMarker = (Type_D3DPERF_SetMarker)FindFunction(TEXT("d3d9.dll"), true, "D3DPERF_SetMarker", &gModule_D3D9);
    }

    return gFunc_D3DPERF_SetMarker(dw, str);
}

void WINAPI VSD3DPERF_SetOptions(DWORD dwOptions)
{
    if (!gFunc_D3DPERF_SetOptions)
    {
        gFunc_D3DPERF_SetOptions = (Type_D3DPERF_SetOptions)FindFunction(TEXT("d3d9.dll"), true, "D3DPERF_SetOptions", &gModule_D3D9);
    }

    return gFunc_D3DPERF_SetOptions(dwOptions);
}

void WINAPI VSD3DPERF_SetRegion(DWORD dw, LPCWSTR str)
{
    if (!gFunc_D3DPERF_SetRegion)
    {
        gFunc_D3DPERF_SetRegion = (Type_D3DPERF_SetRegion)FindFunction(TEXT("d3d9.dll"), true, "D3DPERF_SetRegion", &gModule_D3D9);
    }

    return gFunc_D3DPERF_SetRegion(dw, str);
}
