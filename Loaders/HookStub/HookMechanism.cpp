/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

// Globals
VoodooShader::ICore * gVoodooCore = nullptr;
HINSTANCE gHookLoader = nullptr;
unsigned int gLoadOnce = 1;
HHOOK gSystemHook = nullptr;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    DisableThreadLibraryCalls(hinstDLL);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        gHookLoader = hinstDLL;
    }

    return TRUE;
}

LRESULT CALLBACK GlobalHookCb(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    else
    {
        if (nCode == HCBT_CREATEWND || nCode == HCBT_ACTIVATE)
        {
            if (InterlockedCompareExchange(&gLoadOnce, 0, 1) == 1)
            {
                gLoadOnce = false;

                TCHAR moduleName[MAX_PATH];
                GetModuleFileName(GetModuleHandle(NULL), moduleName, MAX_PATH);

                HHOOKDEF hook = SearchHooks(moduleName);

                if (hook)
                {
                    GlobalLog(TEXT("Hook target %s matched for module %s.\n"), hook->Target, moduleName);

                    LoadVoodoo(hook);

                    delete hook;
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
        }
        else
        {
            ErrorMessage(0x1002, TEXT("Error setting global hook."));
        }
        return false;
    }

    GlobalLog(TEXT("Installed global hook.\n"));

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
            }
            else
            {
                ErrorMessage(0x1004, TEXT("Error removing global hook."));
            }
        } else {
            GlobalLog(TEXT("Removed global hook.\n"));
        }
    }
}

bool WINAPI LoadVoodoo(HHOOKDEF pHook)
{
    if (gVoodooCore) return true;

    GlobalLog(TEXT("Loading Voodoo Shader framework.\n"));

    // Get the target
    HMODULE targetModule = GetModuleHandle(nullptr);
    if (targetModule == nullptr)
    {
        ErrorMessage(0x2003, TEXT("Unable to retrieve target module."));
        return false;
    }

    TCHAR targetName[MAX_PATH];
    GetModuleFileName(targetModule, targetName, MAX_PATH);

    TCHAR corePath[MAX_PATH];
    GetVoodooBinPath(corePath);
    PathCombine(corePath, corePath, TEXT("Voodoo_Core.dll"));

    HMODULE coreLibrary = LoadLibraryEx(corePath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!coreLibrary)
    {
        ErrorMessage(0x2006, L"Voodoo Loader: Unable to load core DLL.");
        return false;
    } else {
        GlobalLog(TEXT("Loaded core from %s.\n"), corePath);
    }

    VoodooShader::Functions::CoreCreateFunc createFunc = 
        (VoodooShader::Functions::CoreCreateFunc)GetProcAddress(coreLibrary, "CreateCore");

    if (createFunc == nullptr)
    {
        ErrorMessage(0x2007, L"Voodoo Loader: Unable to find core create function.");
        return false;
    }

    gVoodooCore = createFunc(0);
    
    if (!gVoodooCore)
    {
        ErrorMessage(0x2008, L"Unable to create Voodoo Shader core.");
    }
    else
    {
        gVoodooCore->AddRef();

        if (FAILED(gVoodooCore->Init(pHook->Config)))
        {
            ErrorMessage(0x2009, L"Unable to initialize Voodoo Shader core.");
            gVoodooCore->Release();
            gVoodooCore = nullptr;
        } else {
            GlobalLog(TEXT("Initialized Voodoo Shader, passing logging to core.\n"), corePath);
        }
    }

    return (gVoodooCore != nullptr);
}
