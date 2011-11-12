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
#include <windows.h>

LRESULT CALLBACK GlobalHookCb(_In_  int nCode, _In_  WPARAM wParam, _In_  LPARAM lParam);

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved);

/* Public functions */
bool WINAPI InstallGlobalHook();
bool WINAPI RemoveGlobalHook();

bool WINAPI AddModule(_In_z_ LPTSTR name);
bool WINAPI RemoveModule(_In_z_ LPTSTR name);
bool WINAPI RemoveAllModules();

/* Internal functions */
bool WINAPI IsDllLoaded(_In_z_ LPTSTR name);
bool WINAPI IsAnyDllLoaded(_In_z_ LPTSTR name[]);

bool WINAPI SearchHooks(_In_z_ TCHAR * moduleName);
bool WINAPI SearchHooksInKey(_In_z_ TCHAR * moduleName, _In_ HKEY key);
