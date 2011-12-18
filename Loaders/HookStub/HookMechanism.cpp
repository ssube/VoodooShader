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

#include "Support.inl"

HINSTANCE gHookLoader = nullptr;
unsigned int gLoadOnce = 1;
HMODULE gFullLoader = nullptr;
HHOOK gSystemHook = nullptr;

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
        if (nCode == HCBT_CREATEWND || nCode == HCBT_ACTIVATE)
        {
            if (InterlockedCompareExchange(&gLoadOnce, 0, 1) == 1)
            {
                gLoadOnce = false;

                TCHAR moduleName[MAX_PATH];
                GetModuleFileName(GetModuleHandle(NULL), moduleName, MAX_PATH);

                HHOOKDEF hook = SearchHooks(moduleName);

                FILE * pf = GetVoodooGlobalLog();
                if (pf)
                {
                    _ftprintf_s(pf, TEXT("%s = %s\n"), moduleName, (hook ? hook->Target : TEXT("false")));
                    fclose(pf);
                }

                if (hook)
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
    if (gSystemHook) return gSystemHook;
    
    gSystemHook = SetWindowsHookEx(WH_CBT, &GlobalHookCb, gHookLoader, 0);

    if (!gSystemHook)
    {
        DWORD error = GetLastError();

        TCHAR buffer[1024];
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, error, LANG_USER_DEFAULT, buffer, 1024, NULL) > 0)
        {
            ErrorMessage(0x1001, buffer);
        } else {
            ErrorMessage(0x1002, TEXT("Error setting global hook."));
        }
        return false;
    }

    return gSystemHook;
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
                ErrorMessage(0x1003, buffer);
            } else {
                ErrorMessage(0x1004, TEXT("Error removing global hook."));
            }
        }
    }
}

bool WINAPI LoadFullLoader()
{
    int result = 0;
    TCHAR binPath[MAX_PATH];

    GetVoodooBinPath(binPath);
    PathCombine(binPath, binPath, TEXT("Voodoo_Loader.dll"));

    gFullLoader = LoadLibraryEx(binPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (gFullLoader)
    {
        FARPROC hookFunc = GetProcAddress(gFullLoader, "InstallKnownHooks");
        if (hookFunc)
        {
            result = hookFunc();
        } else {
            ErrorMessage(0x1006, TEXT("Unable to launch Voodoo loader."));
        }
    } else {
        ErrorMessage(0x1005, TEXT("Unable to load Voodoo loader."));
    }

    return result > 0;
}