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

#include "easyhook.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct ModuleHook
{
    bool Installed;
    TCHAR * Name;
    const char * Symbol;
    void * Func;
};

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved);

bool WINAPI InstallDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol, LPVOID pFunc);
bool WINAPI RemoveDllHook(_In_z_ LPTSTR name, _In_z_ LPCSTR symbol);

int WINAPI InstallHookList(_In_ int hookCount, _In_count_(hookCount) ModuleHook * hooks);
int WINAPI InstallKnownHooks();

bool WINAPI LoadEasyHook();
bool WINAPI UnloadEasyHook();

bool WINAPI InstallSystemHooks();
HMODULE WINAPI VSLoadLibraryA(_In_ LPCSTR lpFileName);
HMODULE WINAPI VSLoadLibraryW(_In_ LPCWSTR lpFileName);
HMODULE WINAPI VSLoadLibraryExA(_In_ LPCSTR lpFileName, HANDLE hFile, _In_ DWORD dwFlags);
HMODULE WINAPI VSLoadLibraryExW(_In_ LPCWSTR lpFileName, HANDLE hFile, _In_ DWORD dwFlags);