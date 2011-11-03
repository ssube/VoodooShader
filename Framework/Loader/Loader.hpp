/**
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
#pragma once

#include "Loader_Common.hpp"

/**
 * @addtogroup voodoo_module_loader Voodoo/Loader @{ 
 */
HINSTANCE gLoaderHandle;

typedef VoodooShader::ICore * (VOODOO_CALL * VoodooCreateFunc)(const VoodooShader::InitParams * const pInitParams, _In_ bool catchErrors);

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved);

bool WINAPI LoadVoodoo();
bool WINAPI UnloadVoodoo(void);

HMODULE WINAPI LoadSystemLibrary(const wchar_t * libname);

// Direct3D create functions
void * WINAPI VoodooD3DCreateGeneric(UINT sdkVersion, const wchar_t * lib, const char * func);
void * WINAPI Voodoo3DCreate8(UINT sdkVersion);
void * WINAPI Voodoo3DCreate9(UINT sdkVersion);

// DirectInput create functions
HRESULT WINAPI VoodooInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPVOID punkOuter);
HRESULT WINAPI VoodooInputCreateGeneric(HINSTANCE hinst, DWORD dwVersion, LPVOID * lplpDirectInput, LPVOID punkOuter, const char * func);
HRESULT WINAPI VoodooInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID * lplpDirectInput, LPVOID punkOuter);
HRESULT WINAPI VoodooInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPVOID * lplpDirectInput, LPVOID punkOuter);

// DirectSound create functions
HRESULT WINAPI VoodooSoundCreate8(LPCGUID lpcGuidDevice, LPVOID * ppDS8, LPVOID pUnkOuter);

/* @} */
