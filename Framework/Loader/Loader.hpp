/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

/**
 * @addtogroup VoodooLoader Voodoo/Loader
 * @{
 */

typedef void * (* funcTypeLoad)(const char *, const char *);
typedef void (* funcTypeUnload)(void *);

bool LoadVoodoo();
bool UnloadVoodoo();

BOOL WINAPI DllMain
(
    _In_ void * _HDllHandle, 
    _In_ unsigned _Reason, 
    _In_opt_ void * _Reserved
);

HMODULE LoadSystemLibrary(const char * libname);
void * WINAPI VoodooDXCreateGeneric(UINT sdkVersion, const char * lib, const char * func);
void * WINAPI Voodoo3DCreate8(UINT sdkVersion);
void * WINAPI Voodoo3DCreate9(UINT sdkVersion);
HRESULT WINAPI VoodooInput8Create
(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID * ppvOut,
    LPVOID punkOuter
);
HRESULT WINAPI VoodooInputCreateGeneric
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID * lplpDirectInput,
    LPVOID punkOuter,
    const char * func
);
HRESULT WINAPI VoodooInputCreateA
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID * lplpDirectInput,
    LPVOID punkOuter
);
HRESULT WINAPI VoodooInputCreateW
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID * lplpDirectInput,
    LPVOID punkOuter
);
HRESULT VoodooSoundCreate8
(
    LPCGUID lpcGuidDevice,
    LPVOID * ppDS8,
    LPVOID pUnkOuter
);

/**
 * @}
 */
