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
TCHAR gVoodooPath[MAX_PATH] = { 0 };
TCHAR gVoodooBinPrefix[MAX_PATH] = { 0 };
TCHAR gVoodooBinPath[MAX_PATH] = { 0 };

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
    wchar_t pathConfig[MAX_PATH];
    wchar_t pathLoader[MAX_PATH];
    wchar_t pathLocalRoot[MAX_PATH];
    wchar_t pathRunRoot[MAX_PATH];
    wchar_t pathTarget[MAX_PATH];

    wchar_t pathCoreLib[MAX_PATH];
    TCHAR buffer[MAX_PATH];

    //! @todo Get the config file
    wcscpy_s(pathConfig, L"voodoo_config.xml");

    // Get the run root
    GetCurrentDirectory(MAX_PATH, pathRunRoot);

    // Get the loader
    if (GetModuleFileName(gLoaderHandle, buffer, MAX_PATH) == 0)
    {
        ErrorMessage(0x2001, L"Voodoo Loader: Unable to retrieve loader path.");
        return false;
    }

    std::wstring loaderPath(buffer);
    std::wstring::size_type pos = loaderPath.rfind(L'\\');
    if (pos == std::wstring::npos)
    {
        ErrorMessage(0x2002, L"Voodoo Loader: Unable to parse loader path.");
        return false;
    }

    loaderPath = loaderPath.substr(0, pos);
    wcscpy_s(pathLoader, loaderPath.c_str());

    // Get the local root and target
    HMODULE hModule = GetModuleHandle(nullptr);

    if (hModule == nullptr)
    {
        ErrorMessage(0x2003, L"Unable to retrieve target module.");
        return false;
    }

    if (GetModuleFileName(hModule, buffer, MAX_PATH) == 0)
    {
        ErrorMessage(0x2004, L"Unable to retrieve target path.");
        return false;
    }

    std::wstring targetPath(buffer);
    pos = targetPath.rfind(L'\\');
    if (pos == std::wstring::npos)
    {
        ErrorMessage(0x2005, L"Voodoo Loader: Unable to parse target path.");
        return false;
    }

    std::wstring targetModule = targetPath.substr(pos+1);
    targetPath = targetPath.substr(0, pos);
    wcscpy_s(pathTarget, targetModule.c_str());
    wcscpy_s(pathLocalRoot, targetPath.c_str());

    // Get the global root
    GetVoodooPath(MAX_PATH, gVoodooPath);
    GetVoodooBinPrefix(MAX_PATH, gVoodooBinPrefix);
    StringCchCopy(gVoodooBinPath, MAX_PATH, gVoodooPath);
    StringCchCat(gVoodooBinPath, MAX_PATH, gVoodooBinPrefix);

    wcscpy_s(pathCoreLib, gVoodooBinPath);
    wcscat_s(pathCoreLib, L"\\Voodoo_Core.dll");

    HMODULE coreLibrary = LoadLibraryEx(pathCoreLib, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

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

    gInitParams.Config = pathConfig;
    gInitParams.GlobalRoot = gVoodooPath;
    gInitParams.Loader = pathLoader;
    gInitParams.LocalRoot = pathLocalRoot;
    gInitParams.RunRoot = pathRunRoot;
    gInitParams.Target = pathTarget;

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
