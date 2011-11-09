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

#include <set>
#include <string>

HINSTANCE gHookLoader = nullptr;
HHOOK gGlobalHook = nullptr;
bool logOnce = true;
HANDLE gSharedMemory;

typedef std::set<std::wstring> HookSet;
HookSet gHooks;

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
        if (logOnce)
        {
            TCHAR moduleName[MAX_PATH];
            GetModuleFileName(GetModuleHandle(NULL), moduleName, MAX_PATH);

            TCHAR filePath[MAX_PATH];
            ExpandEnvironmentStrings(TEXT("%HOMEDRIVE%\\%HOMEPATH%\\processes.log"), filePath, MAX_PATH);

            FILE * pf = _tfopen(filePath, TEXT("a"));
            if (pf)
            {
                _ftprintf(pf, TEXT("%s\n"), moduleName);
                fclose(pf);
            }

            logOnce = false;
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}

bool WINAPI InstallGlobalHook()
{
    gGlobalHook = SetWindowsHookEx(WH_CBT, &GlobalHookCb, gHookLoader, 0);
    if (!gGlobalHook)
    {
        DWORD error = GetLastError();
        return false;
    }

    /*
    gSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(void*), TEXT("Global\\VoodooHookObject"));
    if (gSharedMemory == NULL)
    {
        return false;
    }

    LPVOID pBuffer = MapViewOfFile(gSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
    */

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
            return false;
        }
    }

    return true;
}

bool WINAPI AddModule(_In_z_ LPTSTR name)
{
    std::pair<HookSet::iterator, bool> insertPoint = gHooks.insert(name);
    return (insertPoint.second);
}

bool WINAPI RemoveModule(_In_z_ LPTSTR name)
{
    HookSet::iterator iter = gHooks.find(name);
    if (iter != gHooks.end())
    {
        gHooks.erase(iter);
        return true;
    } else {
        return false;
    }
}

bool WINAPI RemoveAllModules()
{
    gHooks.clear();
    return true;
}

bool WINAPI IsDllLoaded(_In_z_ LPTSTR name)
{
    HMODULE module = GetModuleHandle(name);
    return (module != NULL);
}

bool WINAPI IsAnyDllLoaded(_In_z_ LPTSTR name[])
{
    int i = 0;
    while (name[i])
    {
        if (IsDllLoaded(name[i++])) return true;
    }
    return false;
}
