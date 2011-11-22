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

#include "SysAPIHandler.hpp"

#include "Loader_Version.hpp"

#include <string>

VoodooShader::ICore * gVoodooCore = nullptr;
VoodooShader::IAdapter * gVoodooAdapter = nullptr;
VoodooShader::InitParams gInitParams;
HINSTANCE gLoaderHandle = nullptr;

const VoodooShader::Version moduleVersion = VOODOO_META_VERSION_STRUCT(LOADER);

const VoodooShader::Version * VOODOO_CALL API_ModuleVersion(void)
{
    return &moduleVersion;
}

const uint32_t VOODOO_CALL API_ClassCount(void)
{
    return 0;
}

const wchar_t * VOODOO_CALL API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * pUuid)
{
    UNREFERENCED_PARAMETER(index);
    UNREFERENCED_PARAMETER(pUuid);

    return nullptr;
}

VoodooShader::IObject * VOODOO_CALL API_ClassCreate(_In_ const uint32_t number, _In_ VoodooShader::ICore *pCore)
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
    else if (fdwReason == DLL_PROCESS_DETACH)
    {
        UnloadVoodoo();
        //return (BOOL)UnloadVoodoo();
    }

    return TRUE;
}

/**
 * Locate and load the Voodoo core, verify the functions and initialize the framework.
 */
bool WINAPI LoadVoodoo()
{
    wchar_t pathConfig[MAX_PATH];
    wchar_t pathGlobalRoot[MAX_PATH];
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
        ErrorMessage(L"Voodoo Loader: Unable to retrieve loader path.");
        return false;
    }

    std::wstring loaderPath(buffer);
    std::wstring::size_type pos = loaderPath.rfind(L'\\');
    if (pos == std::wstring::npos)
    {
        ErrorMessage(L"Voodoo Loader: Unable to parse loader path.");
        return false;
    }

    loaderPath = loaderPath.substr(0, pos);
    wcscpy_s(pathLoader, loaderPath.c_str());

    // Get the local root and target
    HMODULE hModule = GetModuleHandle(nullptr);

    if (hModule == nullptr)
    {
        ErrorMessage(L"Unable to retrieve target module.");
        return false;
    }

    if (GetModuleFileName(hModule, buffer, MAX_PATH) == 0)
    {
        ErrorMessage(L"Unable to retrieve target path.");
        return false;
    }

    std::wstring targetPath(buffer);
    pos = targetPath.rfind(L'\\');
    if (pos == std::wstring::npos)
    {
        ErrorMessage(L"Voodoo Loader: Unable to parse target path.");
        return false;
    }

    std::wstring targetModule = targetPath.substr(pos+1);
    targetPath = targetPath.substr(0, pos);
    wcscpy_s(pathTarget, targetModule.c_str());
    wcscpy_s(pathLocalRoot, targetPath.c_str());

    // Get the global root
    HKEY keyVoodoo = nullptr;
    LONG keyOpen = RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &keyVoodoo);

    if (keyOpen != ERROR_SUCCESS)
    {
        keyOpen = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &keyVoodoo);

        if (keyOpen != ERROR_SUCCESS)
        {
            ErrorMessage(L"Voodoo Loader: Unable to find Voodoo registry key.");
            return false;
        }
    }

    DWORD valueType = REG_NONE, valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueEx(keyVoodoo, L"path", nullptr, &valueType, (BYTE *)pathGlobalRoot, &valueSize);

    if (valueQuery != ERROR_SUCCESS || valueType == REG_NONE)
    {
        ErrorMessage
        (
            L"Voodoo Loader: Unable to retrieve Voodoo root from the registry (error %d).", valueQuery
        );
        return false;
    }

    HMODULE coreLibrary = nullptr;
        
    wcscpy_s(pathCoreLib, pathGlobalRoot);
    wcscat_s(pathCoreLib, L"\\bin\\Voodoo_Core.dll");

    coreLibrary = LoadLibraryEx(pathCoreLib, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!coreLibrary)
    {
        ErrorMessage(L"Voodoo Loader: Unable to load core DLL.");
        return false;
    }

    VoodooCreateFunc createFunc = (VoodooCreateFunc)GetProcAddress(coreLibrary, "CreateCore");

    if (createFunc == nullptr)
    {
        ErrorMessage(L"Voodoo Loader: Unable to find core create function.");
        return false;
    }

    gInitParams.Config = pathConfig;
    gInitParams.GlobalRoot = pathGlobalRoot;
    gInitParams.Loader = pathLoader;
    gInitParams.LocalRoot = pathLocalRoot;
    gInitParams.RunRoot = pathRunRoot;
    gInitParams.Target = pathTarget;

    try
    {
        gVoodooCore = createFunc(&gInitParams, false);
    }
    catch(const std::exception & exc)
    {
        ErrorMessage(L"Error details: %S", exc.what());

        gVoodooCore = nullptr;
    }

    return (gVoodooCore != nullptr);
}

bool WINAPI UnloadVoodoo(void)
{
    if (gVoodooCore)
    {
        return (gVoodooCore->Release() == 0);
    } else {
        return true;
    }
}
