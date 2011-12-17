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

#include "VoodooTie.hpp"

#include "Loader_Version.hpp"

#include "Support.inl"

#include <string>

VoodooShader::ICore * gVoodooCore = nullptr;
VoodooShader::IAdapter * gVoodooAdapter = nullptr;
VoodooShader::InitParams gInitParams;
HINSTANCE gLoaderHandle = nullptr;

const VoodooShader::Version moduleVersion = VOODOO_META_VERSION_STRUCT(LOADER);

const VoodooShader::Version * VOODOO_CALLTYPE API_ModuleVersion()
{
    return &moduleVersion;
}

const uint32_t VOODOO_CALLTYPE API_ClassCount()
{
    return 0;
}

const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * pUuid)
{
    UNREFERENCED_PARAMETER(index);
    UNREFERENCED_PARAMETER(pUuid);

    return nullptr;
}

VoodooShader::IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t number, _In_ VoodooShader::ICore *pCore)
{
    UNREFERENCED_PARAMETER(number);
    UNREFERENCED_PARAMETER(pCore);

    return nullptr;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        gLoaderHandle = hinstDLL;
    }

    return TRUE;
}

/**
 * Locate and load the Voodoo core, verify the functions and initialize the framework.
 */
bool WINAPI LoadVoodoo()
{
    ZeroMemory(&gInitParams, sizeof(gInitParams));

    // Get the target
    HMODULE targetModule = GetModuleHandle(nullptr);
    if (targetModule == nullptr)
    {
        ErrorMessage(0x2003, L"Unable to retrieve target module.");
        return false;
    }
    gInitParams.Target = new wchar_t[MAX_PATH];
    GetModuleFileName(targetModule, gInitParams.Target, MAX_PATH);

    // Get the hook
    HHOOKDEF hook = SearchHooks(gInitParams.Target);
    if (!hook)
    {
        ErrorMessage(0xFFFF, TEXT("Unable to locate hook for target: %s"), gInitParams.Target);
    }

    // Set the config
    gInitParams.Config = new wchar_t[MAX_PATH];
    StringCchCopy(gInitParams.Config, MAX_PATH, hook->Config);

    // Set the run root
    gInitParams.RunRoot = new wchar_t[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, gInitParams.RunRoot);

    // Set the loader
    gInitParams.Loader = new wchar_t[MAX_PATH];
    GetModuleFileName(gLoaderHandle, gInitParams.Loader, MAX_PATH);

    // Get the local root and target
    gInitParams.LocalRoot = new wchar_t[MAX_PATH];
    StringCchCopy(gInitParams.LocalRoot, MAX_PATH, gInitParams.Target);
    PathRemoveFileSpec(gInitParams.LocalRoot);
    PathAddBackslash(gInitParams.LocalRoot);

    // Get the global root
    gInitParams.GlobalRoot = new wchar_t[MAX_PATH];
    GetVoodooBinPath(gInitParams.GlobalRoot);

    TCHAR corePath[MAX_PATH];
    PathCombine(corePath, gInitParams.GlobalRoot, TEXT("Voodoo_Core.dll"));

    HMODULE coreLibrary = LoadLibraryEx(corePath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!coreLibrary)
    {
        ErrorMessage(0x2006, L"Voodoo Loader: Unable to load core DLL.");
        return false;
    }

    VoodooShader::Functions::CoreCreateFunc createFunc = (VoodooShader::Functions::CoreCreateFunc)GetProcAddress(coreLibrary, "CreateCore");

    if (createFunc == nullptr)
    {
        ErrorMessage(0x2007, L"Voodoo Loader: Unable to find core create function.");
        return false;
    }

    gVoodooCore = createFunc();
    
    if (!gVoodooCore)
    {
        ErrorMessage(0x2008, L"Unable to create Voodoo core.");
    } else {
        gVoodooCore->AddRef();

        if (!gVoodooCore->Initialize(&gInitParams))
        {
            ErrorMessage(0x2009, L"Unable to initialize Voodoo core.");
            gVoodooCore->Release();
            gVoodooCore = nullptr;
        }
    }

    return (gVoodooCore != nullptr);
}

bool WINAPI UnloadVoodoo()
{
    if (gVoodooCore)
    {
        return (gVoodooCore->Release() == 0);
    } else {
        return true;
    }
}
