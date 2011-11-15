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

#include "HookMechanism.hpp"

#include <tchar.h>
#include <stdio.h>

HINSTANCE gHookLoader = nullptr;
HHOOK gGlobalHook = nullptr;
unsigned int gLoadOnce = 1;
HANDLE gSharedMemory;

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    DisableThreadLibraryCalls(hinstDLL);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        gHookLoader = hinstDLL;
    }

    return TRUE;
}

LRESULT CALLBACK GlobalHookCb(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    } else {
        if (nCode == HCBT_CREATEWND)
        {
            if (InterlockedCompareExchange(&gLoadOnce, 0, 1) == 1)
            {
                gLoadOnce = false;

                TCHAR moduleName[MAX_PATH];
                GetModuleFileName(GetModuleHandle(NULL), moduleName, MAX_PATH);

                TCHAR filePath[MAX_PATH];
                ExpandEnvironmentStrings(TEXT("%HOMEDRIVE%\\%HOMEPATH%\\processes.log"), filePath, MAX_PATH);

                bool doHook = SearchHooks(moduleName);

                FILE * pf = nullptr;
                if (_tfopen_s(&pf, filePath, TEXT("a")) == 0)
                {
                    _ftprintf_s(pf, TEXT("%s = %s\n"), moduleName, ((doHook)?TEXT("true"):TEXT("false")));
                    fclose(pf);
                }
            }
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}

bool WINAPI InstallGlobalHook()
{
    if (!gGlobalHook)
    {
        gGlobalHook = SetWindowsHookEx(WH_CBT, &GlobalHookCb, gHookLoader, 0);
        if (!gGlobalHook)
        {
            DWORD error = GetLastError();

            TCHAR buffer[1024];
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, error, LANG_USER_DEFAULT, buffer, 1024, NULL) > 0)
            {
                MessageBox(NULL, buffer, TEXT("Voodoo Stub Error (IGH)"), MB_OK);
            }
            return false;
        }
    }

    return true;
}

bool WINAPI RemoveGlobalHook()
{
    if (gGlobalHook)
    {
        BOOL success = UnhookWindowsHookEx(gGlobalHook);
        if (success == 0)
        {
            DWORD error = GetLastError();

            TCHAR buffer[1024];
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, error, LANG_USER_DEFAULT, buffer, 1024, NULL) > 0)
            {
                MessageBox(NULL, buffer, TEXT("Voodoo Stub Error (RGH)"), MB_OK);
            }

            return false;
        }
    }

    return true;
}

#define MAX_KEY_LENGTH 255

bool WINAPI SearchHooks(_In_z_ TCHAR * moduleName)
{
    HKEY hookRoot = nullptr;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\VoodooShader\\Hooks"), 0, KEY_READ, &hookRoot) == ERROR_SUCCESS && hookRoot != nullptr)
    {
        SearchHooksInKey(moduleName, hookRoot);
        RegCloseKey(hookRoot);
    }

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\VoodooShader\\Hooks"), 0, KEY_READ, &hookRoot) == ERROR_SUCCESS && hookRoot != nullptr)
    {
        SearchHooksInKey(moduleName, hookRoot);
        RegCloseKey(hookRoot);
    }

    return false;
}

bool WINAPI SearchHooksInKey(_In_z_ TCHAR * moduleName, _In_ HKEY key)
{
    DWORD index = 0, nameSize = MAX_KEY_LENGTH;
    TCHAR hookID[MAX_KEY_LENGTH];

    while (RegEnumKeyEx(key, index++, hookID, &nameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
    {
        HKEY hookKey = nullptr;

        if (RegOpenKeyEx(key, hookID, 0, KEY_READ, &hookKey) == ERROR_SUCCESS)
        {
            DWORD valueType = 0, valueSize = 1024;
            TCHAR valueBuffer[1024];

            if (RegQueryValueEx(hookKey, TEXT("target"), NULL, &valueType, (BYTE*)valueBuffer, &valueSize) == ERROR_SUCCESS)
            {
                OutputDebugString(moduleName);
                if (_tcscmp(moduleName, valueBuffer) == 0)
                {
                    OutputDebugString(TEXT(" == "));
                } else {
                    OutputDebugString(TEXT(" != "));
                }
                OutputDebugString(valueBuffer);
                OutputDebugString(TEXT("\n"));
            }

            RegCloseKey(hookKey);
        }
    }

    return false;
}