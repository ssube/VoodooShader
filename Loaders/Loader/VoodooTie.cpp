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

#include "VoodooTie.hpp"

#include "Loader_Version.hpp"

#include "Support.inl"

#include <string>

VoodooShader::ICore * gVoodooCore = nullptr;
HINSTANCE gLoaderHandle = nullptr;

const VoodooShader::Version moduleVersion = VOODOO_VERSION_STRUCT(LOADER);

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

/**
 * Locate and load the Voodoo core, verify the functions and initialize the framework.
 */
bool WINAPI LoadVoodoo()
{
    if (gVoodooCore) return true;

    GlobalLog(TEXT("Loading Voodoo Shader framework."));

    // Get the target
    HMODULE targetModule = GetModuleHandle(nullptr);
    if (targetModule == nullptr)
    {
        ErrorMessage(0x2003, TEXT("Unable to retrieve target module."));
        return false;
    }

    TCHAR targetName[MAX_PATH];
    GetModuleFileName(targetModule, targetName, MAX_PATH);

    // Get the hook
    HHOOKDEF hook = SearchHooks(targetName);
    if (!hook)
    {
        ErrorMessage(0xFFFF, TEXT("Unable to locate hook for target: %s"), targetName);
        return false;
    }

    TCHAR corePath[MAX_PATH];
    GetVoodooBinPath(corePath);
    PathCombine(corePath, corePath, TEXT("Voodoo_Core.dll"));

    HMODULE coreLibrary = LoadLibraryEx(corePath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!coreLibrary)
    {
        ErrorMessage(0x2006, L"Voodoo Loader: Unable to load core DLL.");
        return false;
    } else {
        GlobalLog(TEXT("Loaded core from %s."), corePath);
    }

    VoodooShader::Functions::CoreCreateFunc createFunc = (VoodooShader::Functions::CoreCreateFunc)GetProcAddress(coreLibrary, "CreateCore");

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

        if (!gVoodooCore->Init(hook->Config))
        {
            ErrorMessage(0x2009, L"Unable to initialize Voodoo Shader core.");
            gVoodooCore->Release();
            gVoodooCore = nullptr;
        } else {
            GlobalLog(TEXT("Initialized Voodoo Shader, passing logging to core."), corePath);
        }
    }

    if (hook) delete hook;

    return (gVoodooCore != nullptr);
}

bool WINAPI UnloadVoodoo()
{
    if (gVoodooCore)
    {
        return (gVoodooCore->Release() == 0);
    }
    else
    {
        return true;
    }
}
