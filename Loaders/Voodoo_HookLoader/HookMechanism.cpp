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

HINSTANCE gHookLoader = nullptr;
HHOOK gGlobalHook = nullptr;

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        gHookLoader = hinstDLL;
    }

    return TRUE;
}

LRESULT CALLBACK GlobalHookCb(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    MessageBox(NULL, L"Voodoo Hook triggered.");

    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    } else {
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
    } else {
        return true;
    }
}

bool WINAPI RemoveGlobalHook()
{
    if (gGlobalHook)
    {
        BOOL success = UnhookWindowsHookEx(gGlobalHook);
        if (success != 0)
        {
            return true;
        } else {
            DWORD error = GetLastError();
            return false;
        }
    }
}

