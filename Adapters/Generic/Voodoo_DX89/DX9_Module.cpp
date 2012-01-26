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

#include "DX9_Module.hpp"
// Voodoo DX89
#include "DX9_Exports.hpp"
#include "DX9_Version.hpp"
// Voodoo Utility
#include "Support.inl"
// 

struct ModuleHook
{
    bool Installed;
    TCHAR * Name;
    const char * Symbol;
    void * Func;
};

ModuleHook hookList[] =
{
    { false, L"kernel32.dll",   "LoadLibraryA",         &VSLoadLibraryA },
    { false, L"kernel32.dll",   "LoadLibraryW",         &VSLoadLibraryW },
    { false, L"kernel32.dll",   "LoadLibraryExA",       &VSLoadLibraryExA },
    { false, L"kernel32.dll",   "LoadLibraryExW",       &VSLoadLibraryExW },
    { false, L"d3d9.dll",       "Direct3DCreate9",      &VSDirect3DCreate9 },
    //{ false, L"d3d9.dll",      "Direct3DCreate9Ex",    &VSDirect3DCreate9Ex },
    //{ false, L"dinput8.dll",   "DirectInput8Create",   &VSDirectInput8Create },
    //{ false, L"dinput.dll",    "DirectInputCreateA",   &VSDirectInputCreateA },
    //{ false, L"dinput.dll",    "DirectInputCreateW",   &VSDirectInputCreateW },
    //{ false, L"dsound8.dll",   "DirectSoundCreate8",   &VSDirectSoundCreate8 },
    //{ false, nullptr,          nullptr,                nullptr }
};

VoodooShader::CoreRef gpVoodooCore = nullptr;
VoodooShader::LoggerRef gpVoodooLogger = nullptr;

VoodooShader::VoodooResult VOODOO_CALLTYPE FinalizeEvent(VoodooShader::ICore * pCore, uint32_t count, VoodooShader::Variant * pArgs)
{
    UNREFERENCED_PARAMETER(pCore);
    UNREFERENCED_PARAMETER(count);
    UNREFERENCED_PARAMETER(pArgs);

    InstallKnownHooks();
    return VSF_OK;
}

const VoodooShader::Version * VOODOO_CALLTYPE API_PluginInit(VoodooShader::ICore * pCore)
{
    if (!pCore) return nullptr;
    gpVoodooCore = pCore;

    pCore->OnEvent(VoodooShader::EventIds::Finalize, &FinalizeEvent);

    static const VoodooShader::Version dx9version = VOODOO_VERSION_STRUCT(DX89);
    return &dx9version;
}

const uint32_t VOODOO_CALLTYPE API_ClassCount()
{
    return 0;
}

const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * refid)
{
    UNREFERENCED_PARAMETER(index);
    UNREFERENCED_PARAMETER(refid);

    return nullptr;
}

VoodooShader::IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore)
{
    UNREFERENCED_PARAMETER(index);
    UNREFERENCED_PARAMETER(pCore);

    return nullptr;
}

// Intercept library loading to trigger hook installation
bool WINAPI InstallDllHook(LPTSTR name, LPCSTR symbol, LPVOID pDest)
{
    if (!gpVoodooCore || !name || !symbol) return false;

    VoodooShader::HookManagerRef mgr = gpVoodooCore->GetHookManager();
    if (!mgr) return false;

    HMODULE module = GetModuleHandle(name);
    if (!module) return false;

    FARPROC offset = GetProcAddress(module, symbol);
    if (!offset) return false;

    VoodooShader::String fullname = name;
    fullname += VSTR(".");
    fullname += symbol;
    
    return (SUCCEEDED(mgr->Add(fullname, offset, pDest)));
}

int WINAPI InstallKnownHooks()
{
    int success = 0;
    int hookCount = sizeof(hookList) / sizeof(hookList[0]);

    for (int i = 0; i < hookCount; ++i)
    {
        if (!hookList[i].Installed && InstallDllHook(hookList[i].Name, hookList[i].Symbol, hookList[i].Func))
        {
            hookList[i].Installed = true;
            ++success;
        }
    }

    return success;
}
