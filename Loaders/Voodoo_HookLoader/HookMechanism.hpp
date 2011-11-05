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
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct SingleHook
{
    const char * symbol;
    void * func;
};

struct ModuleHooks
{
    TCHAR * name;
    SingleHook hooks[];
};

extern HINSTANCE gHookLoader;
extern HHOOK gGlobalHook;

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved);

LRESULT CALLBACK GlobalHookCb(__in  int nCode, __in  WPARAM wParam, __in  LPARAM lParam);

bool WINAPI InstallGlobalHook();
bool WINAPI RemoveGlobalHook();

bool WINAPI IsDllLoaded(_In_z_ LPTSTR name);

bool WINAPI InstallDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol, LPVOID pFunc);
bool WINAPI RemoveDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol);

int WINAPI HookList(_In_z_ ModuleHooks * hooks);

// Voodoo module functions
/*
const VoodooShader::Version * VOODOO_CALL API_ModuleVersion(void);
const uint32_t VOODOO_CALL API_ClassCount(void);
const wchar_t * VOODOO_CALL API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * pUuid);
VoodooShader::IObject * VOODOO_CALL API_ClassCreate(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore);*/