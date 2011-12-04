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
unsigned int gLoadOnce = 1;
HMODULE gFullLoader;
TCHAR gFilePath[MAX_PATH];

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

                ExpandEnvironmentStrings(TEXT("%HOMEDRIVE%\\%HOMEPATH%\\processes.log"), gFilePath, MAX_PATH);

                bool doHook = SearchHooks(moduleName);

                FILE * pf = nullptr;
                if (_tfopen_s(&pf, gFilePath, TEXT("a")) == 0)
                {
                    _ftprintf_s(pf, TEXT("%s = %s\n"), moduleName, ((doHook)?TEXT("true"):TEXT("false")));
                    fclose(pf);
                }

                if (doHook)
                {
                    LoadFullLoader();
                }
            }
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}

HHOOK WINAPI InstallGlobalHook()
{
    HHOOK hook = SetWindowsHookEx(WH_CBT, &GlobalHookCb, gHookLoader, 0);

    if (!hook)
    {
        DWORD error = GetLastError();

        TCHAR buffer[1024];
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, error, LANG_USER_DEFAULT, buffer, 1024, NULL) > 0)
        {
            MessageBox(NULL, buffer, TEXT("Voodoo Stub Error (IGH)"), MB_OK);
        }
        return false;
    }

    return hook;
}

void WINAPI RemoveGlobalHook(HHOOK hook)
{
    if (hook)
    {
        BOOL success = UnhookWindowsHookEx(hook);
        if (success == 0)
        {
            DWORD error = GetLastError();

            TCHAR buffer[1024];
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, error, LANG_USER_DEFAULT, buffer, 1024, NULL) > 0)
            {
                MessageBox(NULL, buffer, TEXT("Voodoo Stub Error (RGH)"), MB_OK);
            }
        }
    }
}

#define MAX_KEY_LENGTH 255

bool WINAPI SearchHooks(_In_z_ TCHAR * moduleName)
{
    HKEY hookRoot = nullptr;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\VoodooShader\\Hooks"), 0, KEY_READ, &hookRoot) == ERROR_SUCCESS && hookRoot != nullptr)
    {
        bool found = SearchHooksInKey(moduleName, hookRoot);
        RegCloseKey(hookRoot);
        if (found) return found;
    }

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\VoodooShader\\Hooks"), 0, KEY_READ, &hookRoot) == ERROR_SUCCESS && hookRoot != nullptr)
    {
        bool found = SearchHooksInKey(moduleName, hookRoot);
        RegCloseKey(hookRoot);
        if (found) return found;
    }

    return false;
}

bool WINAPI SearchHooksInKey(_In_z_ TCHAR * moduleName, _In_ HKEY key)
{
    DWORD index = 0, nameSize = MAX_KEY_LENGTH;
    TCHAR hookID[MAX_KEY_LENGTH];

    while (RegEnumKeyEx(key, index++, hookID, &nameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS && nameSize > 0)
    {
        HKEY hookKey = nullptr;

        if (RegOpenKeyEx(key, hookID, 0, KEY_READ, &hookKey) == ERROR_SUCCESS)
        {
            bool found = false;
            DWORD valueType = 0, valueSize = 1024;
            TCHAR valueBuffer[1024];

            if (RegQueryValueEx(hookKey, TEXT("active"), NULL, &valueType, (BYTE*)valueBuffer, &valueSize) == ERROR_SUCCESS)
            {
                if (valueBuffer[0] == TEXT('1') || _tcsicmp(valueBuffer, TEXT("true")) == 0)
                {
                    if (RegQueryValueEx(hookKey, TEXT("target"), NULL, &valueType, (BYTE*)valueBuffer, &valueSize) == ERROR_SUCCESS)
                    {
                        found = _tcscmp(moduleName, valueBuffer) == 0;

                        FILE * pf = nullptr;
                        if (_tfopen_s(&pf, gFilePath, TEXT("a")) == 0)
                        {
                            const TCHAR * match = TEXT("!=");
                            if (found)
                            {
                                match = TEXT(" == ");
                            }
                            _ftprintf_s(pf, TEXT("%s %s %s\n"), moduleName, match, valueBuffer);
                            fclose(pf);
                        }
                    }
                }
            }

            RegCloseKey(hookKey);

            if (found) return found;
        }

        nameSize = MAX_KEY_LENGTH;
    }

    return false;
}

bool WINAPI LoadFullLoader()
{
    int result = 0;
    HKEY rootPath = nullptr;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\VoodooShader"), 0, KEY_READ, &rootPath) == ERROR_SUCCESS && rootPath != nullptr)
    {
        DWORD valueType = 0, valueSize = 1024;
        TCHAR valueBuffer[1024];

        if (RegQueryValueEx(rootPath, TEXT("path"), NULL, &valueType, (BYTE*)valueBuffer, &valueSize) == ERROR_SUCCESS)
        {
            OutputDebugString(valueBuffer);
            OutputDebugString(TEXT("\n"));

            _tcscat(valueBuffer, TEXT("\\bin\\Voodoo_Loader.dll"));

            gFullLoader = LoadLibraryEx(valueBuffer, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

            if (gFullLoader)
            {
                FARPROC hookFunc = GetProcAddress(gFullLoader, "InstallKnownHooks");
                result = hookFunc();
            }
        }

        RegCloseKey(rootPath);
    }

    return result > 0;
}