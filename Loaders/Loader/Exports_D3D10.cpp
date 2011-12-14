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

#include "VoodooTie.hpp"

#include "Support.inl"

#include <stdlib.h>

#ifdef _DEBUG
#   define VOODOO_DEBUGBREAK DebugBreak()
#else
#   define VOODOO_DEBUGBREAK
#endif

typedef DWORD (WINAPI * Type_D3D10CreateDevice)(LPVOID pAdapter, DWORD DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, LPVOID *ppDevice);
typedef DWORD (WINAPI * Type_D3D10CreateDeviceAndSwapChain)(LPVOID pAdapter, DWORD DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, LPVOID pSwapChainDesc, LPVOID *ppSwapChain, LPVOID *ppDevice);
typedef DWORD (WINAPI * Type_D3D10CreateBlob)(SIZE_T NumBytes, LPVOID *ppBuffer);

HMODULE gModule_D3D10;
Type_D3D10CreateDevice gFunc_D3D10CreateDevice;
Type_D3D10CreateDeviceAndSwapChain gFunc_D3D10CreateDeviceAndSwapChain;
Type_D3D10CreateBlob gFunc_D3D10CreateBlob;

HRESULT WINAPI VSD3D10CreateDevice(LPVOID pAdapter, DWORD DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, LPVOID *ppDevice)
{
    if (!gFunc_D3D10CreateDevice)
    {
        gFunc_D3D10CreateDevice = (Type_D3D10CreateDevice)FindFunction(TEXT("d3d10.dll"), "D3D10CreateDevice", &gModule_D3D10);
    }

    return gFunc_D3D10CreateDevice(pAdapter, DriverType, Software, Flags, SDKVersion, ppDevice);
}

HRESULT WINAPI VSD3D10CreateDeviceAndSwapChain(LPVOID pAdapter, DWORD DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, LPVOID pSwapChainDesc, LPVOID *ppSwapChain, LPVOID *ppDevice)
{
    if (!gFunc_D3D10CreateDeviceAndSwapChain)
    {
        gFunc_D3D10CreateDeviceAndSwapChain = (Type_D3D10CreateDeviceAndSwapChain)FindFunction(TEXT("d3d10.dll"), "D3D10CreateDeviceAndSwapChain", &gModule_D3D10);
    }

    return gFunc_D3D10CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice);
}

HRESULT WINAPI VSD3D10CreateBlob(SIZE_T NumBytes, LPVOID *ppBuffer)
{
    if (!gFunc_D3D10CreateBlob)
    {
        gFunc_D3D10CreateBlob = (Type_D3D10CreateBlob)FindFunction(TEXT("d3d10.dll"), "D3D10CreateBlob", &gModule_D3D10);
    }

    return gFunc_D3D10CreateBlob(NumBytes, ppBuffer);
}
