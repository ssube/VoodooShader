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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef _DEBUG
#   define VOODOO_DEBUGBREAK //DebugBreak()
#else
#   define VOODOO_DEBUGBREAK
#endif

// Single-export flag
extern volatile LONG gSingleExport;

// Direct3D8 Exports
void * WINAPI VSDirect3DCreate8(UINT sdkVersion);

// Direct3D9 Exports
int WINAPI VSD3DPERF_BeginEvent(DWORD col, LPCWSTR wszName);
int WINAPI VSD3DPERF_EndEvent();
void WINAPI VSD3DPERF_SetMarker(DWORD col, LPCWSTR wszName);
void WINAPI VSD3DPERF_SetRegion(DWORD col, LPCWSTR wszName);
BOOL WINAPI VSD3DPERF_QueryRepeatFrame();
void WINAPI VSD3DPERF_SetOptions(DWORD dwOptions);
DWORD WINAPI VSD3DPERF_GetStatus();
LPVOID WINAPI VSDirect3DCreate9(UINT SDKVersion);
HRESULT WINAPI VSDirect3DCreate9Ex(UINT SDKVersion, LPVOID * dx);

// Direct3D10 Exports
HRESULT WINAPI VSD3D10CreateDevice(LPVOID pAdapter, DWORD DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, LPVOID *ppDevice);
HRESULT WINAPI VSD3D10CreateDeviceAndSwapChain(LPVOID pAdapter, DWORD DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, LPVOID pSwapChainDesc, LPVOID *ppSwapChain, LPVOID *ppDevice);
HRESULT WINAPI VSD3D10CreateBlob(SIZE_T NumBytes, LPVOID *ppBuffer);

// DirectInput Exports
HRESULT WINAPI VSDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPVOID punkOuter);
HRESULT WINAPI VSDirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID * lplpDirectInput, LPVOID punkOuter);
HRESULT WINAPI VSDirectInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPVOID * lplpDirectInput, LPVOID punkOuter);

// DirectSound Exports
HRESULT WINAPI VSDirectSoundCreate8(LPCGUID lpcGuidDevice, LPVOID * ppDS8, LPVOID pUnkOuter);
