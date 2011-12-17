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

#include <vector>
#include <tchar.h>
#include <strsafe.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")

struct HookDef
{
    TCHAR Name[MAX_PATH];
    TCHAR Target[MAX_PATH];
    TCHAR Config[MAX_PATH];
};

typedef HookDef * HHOOKDEF;

/**
 * Display a formatted error message.
 * 
 * @param msg Format string for the message.
 * @param ... Format arguments.
 */
inline static void WINAPI ErrorMessage(_In_ DWORD errorCode, _In_ _Printf_format_string_ LPTSTR msg, ...)
{
    va_list args;
    va_start(args, msg);

    int bufsize = _vsctprintf(msg, args) + 1;
    std::vector<TCHAR> buffer(bufsize);

    _vsntprintf_s(&buffer[0], bufsize, bufsize-1, msg, args);

    va_end(args);

    TCHAR title[32];
    _tprintf_s(TEXT("Voodoo Error 0X%04X"), errorCode);

    MessageBox(nullptr, &buffer[0], title, MB_OK | MB_ICONWARNING);
}

/**
 * Load a module from the system directory (uses absolute path to guarantee proper file).
 *
 * @param libname The filename (and optionally extension) of the file to load. This may contain a path, but it will be
 *     interpreted relative to the system directory.
 * @return A handle to the module if loaded or a nullptr handle otherwise.
 */
inline static HMODULE WINAPI LoadSystemLibrary(const LPTSTR libname)
{
    TCHAR path[MAX_PATH];

    GetSystemDirectory(path, MAX_PATH);
    StringCchCat(path, MAX_PATH, TEXT("\\"));
    StringCchCat(path, MAX_PATH, libname);

    return LoadLibrary(path);
}

inline static void * WINAPI FindFunction(const LPTSTR libname, const LPCSTR funcname, HMODULE * pModule)
{
    if (*pModule == nullptr)
    {
        *pModule = LoadSystemLibrary(libname);
    }

    return GetProcAddress(*pModule, funcname);
}

inline static bool WINAPI GetVoodooPath(_In_count_c_(MAX_PATH) TCHAR * pBuffer)
{
    if (!pBuffer) return false;

    HKEY rootPath = nullptr;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\VoodooShader"), 0, KEY_READ, &rootPath) == ERROR_SUCCESS && rootPath != nullptr)
    {
        DWORD valueType = 0, valueSize = MAX_PATH;

        if (RegQueryValueEx(rootPath, TEXT("Path"), NULL, &valueType, (BYTE*)pBuffer, &valueSize) == ERROR_SUCCESS)
        {
            OutputDebugString(pBuffer);
            OutputDebugString(TEXT("\n"));

            PathAddBackslash(pBuffer);

            RegCloseKey(rootPath);
            return true;
        } else {
            RegCloseKey(rootPath);
            return false;
        }
    } else {
        return false;
    }
}

inline static bool WINAPI GetVoodooBinPrefix(_In_count_c_(MAX_PATH) TCHAR * pBuffer)
{
    if (!pBuffer) return false;

    HKEY rootPath = nullptr;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\VoodooShader"), 0, KEY_READ, &rootPath) == ERROR_SUCCESS && rootPath != nullptr)
    {
        DWORD valueType = 0, valueSize = MAX_PATH;

        if (RegQueryValueEx(rootPath, TEXT("BinPrefix"), NULL, &valueType, (BYTE*)pBuffer, &valueSize) == ERROR_SUCCESS)
        {
            OutputDebugString(pBuffer);
            OutputDebugString(TEXT("\n"));

            PathAddBackslash(pBuffer);

            RegCloseKey(rootPath);
            return true;
        } else {
            RegCloseKey(rootPath);
            return false;
        }
    } else {
        return false;
    }
}

inline static bool WINAPI GetVoodooBinPath(_In_count_c_(MAX_PATH) TCHAR * pBuffer)
{
    if (!pBuffer) return false;

    TCHAR path[MAX_PATH];
    TCHAR prefix[MAX_PATH];

    GetVoodooPath(path);
    GetVoodooBinPrefix(prefix);

    PathCombine(pBuffer, path, prefix);
    PathAddBackslash(pBuffer);

    return true;
}

inline static bool WINAPI MatchHook(_In_z_ TCHAR * module, _In_z_ TCHAR * target)
{
    if (!module || !target) return false;

    return (PathMatchSpec(module, target) == TRUE);
}

#define MAX_KEY_LENGTH 255

inline static HHOOKDEF WINAPI SearchHooks(_In_z_ TCHAR * moduleName)
{
    HKEY hookRoot = nullptr;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\VoodooShader\\Hooks"), 0, KEY_READ, &hookRoot) == ERROR_SUCCESS && hookRoot != nullptr)
    {
        DWORD index = 0, nameSize = MAX_KEY_LENGTH;
        TCHAR hookID[MAX_KEY_LENGTH];

        while (RegEnumKeyEx(hookRoot, index++, hookID, &nameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS && nameSize > 0)
        {
            HKEY hookKey = nullptr;

            if (RegOpenKeyEx(hookRoot, hookID, 0, KEY_READ, &hookKey) == ERROR_SUCCESS)
            {
                DWORD valueType = 0, valueSize = MAX_PATH;
                TCHAR valueBuffer[MAX_PATH];

                if (RegQueryValueEx(hookKey, TEXT("Active"), NULL, &valueType, (BYTE*)valueBuffer, &valueSize) == ERROR_SUCCESS)
                {
                    if (valueBuffer[0] == TEXT('1') || _tcsicmp(valueBuffer, TEXT("true")) == 0)
                    {
                        valueType = 0, valueSize = MAX_PATH;

                        if (RegQueryValueEx(hookKey, TEXT("Target"), NULL, &valueType, (BYTE*)valueBuffer, &valueSize) == ERROR_SUCCESS)
                        {
                            if (MatchHook(moduleName, valueBuffer))
                            {
                                HookDef * hook = new HookDef();

                                StringCchCopy(hook->Target, MAX_PATH, valueBuffer);

                                valueType = 0, valueSize = MAX_PATH;
                                RegQueryValueEx(hookKey, TEXT("Name"), NULL, &valueType, (BYTE*)hook->Name, &valueSize);

                                valueType = 0, valueSize = MAX_PATH;
                                RegQueryValueEx(hookKey, TEXT("Config"), NULL, &valueType, (BYTE*)hook->Config, &valueSize);

                                RegCloseKey(hookKey);
                                RegCloseKey(hookRoot);

                                return hook;
                            }
                        }
                    }
                }

                RegCloseKey(hookKey);
            }

            nameSize = MAX_KEY_LENGTH;
        }
    }

    RegCloseKey(hookRoot);

    return false;
}
