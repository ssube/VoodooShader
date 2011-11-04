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

#include "Loader.hpp"

#include "Loader_Version.hpp"

#include <string>

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
        gLoaderHandle = hinstDLL;
    }
    else if (fdwReason == DLL_PROCESS_DETACH)
    {
        return (BOOL)UnloadVoodoo();
    }
}

/**
 * Locate and load the Voodoo core, verify the functions and initialize the framework.
 */
bool WINAPI LoadVoodoo()
{
    wchar_t pathGlobalRoot[MAX_PATH];
    wchar_t pathLocalRoot[MAX_PATH];
    wchar_t pathRunRoot[MAX_PATH];
    wchar_t pathTarget[MAX_PATH];
    wchar_t pathLoader[MAX_PATH];
    wchar_t pathConfig[MAX_PATH];
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

    std::wstring targetModule = targetPath.substr(pos);
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
            MessageBox
            (
                nullptr, L"Voodoo Loader: Unable to find Voodoo registry key.", L"Loader Error", MB_ICONERROR | MB_OK
            );
            return false;
        }
    }

    DWORD valueType = REG_NONE, valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueEx(keyVoodoo, L"Path", nullptr, &valueType, (BYTE *)pathGlobalRoot, &valueSize);

    if (valueQuery != ERROR_SUCCESS || valueType == REG_NONE)
    {
        MessageBox
        (
            nullptr, L"Voodoo Loader: Unable to retrieve path from registry.", L"Loader Error", MB_ICONERROR | MB_OK
        );
        return false;
    }

    wcscpy_s(pathCoreLib, pathGlobalRoot);
    wcscpy_s(pathCoreLib, L"\\bin\\Voodoo_Core.dll");

    HMODULE coreLibrary = LoadLibraryEx(pathCoreLib, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!coreLibrary)
    {
        MessageBox(nullptr, L"Voodoo Loader: Unable to load core DLL.", L"Loader Error", MB_ICONERROR | MB_OK);
        return false;
    }

    VoodooCreateFunc createFunc = (VoodooCreateFunc)GetProcAddress(coreLibrary, "CreateCore");

    if (createFunc == nullptr)
    {
        MessageBox(nullptr, L"Voodoo Loader: Unable to find core create function.", L"Loader Error", MB_ICONERROR | MB_OK);
        return false;
    }

    try
    {
        gVoodooCore = createFunc(&gInitParams, false);
    }
    catch(const std::exception & exc)
    {
        wchar_t message[1024];
        wsprintf(message, L"Error details: %S", exc.what());

        MessageBox(nullptr, message, L"Loader Error", MB_ICONERROR | MB_OK);

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
        return false;
    }
}

/**
 * Most DirectX libraries use an identical loading function, with only the name varying. This function takes an SDK
 * version, library name and function name, loads and calls the proper init function.
 */
void * WINAPI VoodooDXCreateGeneric(UINT sdkVersion, const wchar_t * lib, const char * func)
{
    typedef void * (WINAPI * DXInitFunc) (UINT);

    HMODULE baselib = LoadSystemLibrary(lib);

    if (!baselib)
    {
        MessageBox(nullptr, L"Voodoo Loader: Unable to load system DLL.", L"Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DXInitFunc initFunc = (DXInitFunc) GetProcAddress(baselib, func);

    if (!initFunc)
    {
        MessageBox(nullptr, L"Voodoo Loader: Unable to find system EP.", L"Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    return (*initFunc) (sdkVersion);
}

/**
 * Direct3D 8
 */
void * WINAPI Voodoo3DCreate8(UINT sdkVersion)
{
    void * pD3D8 = VoodooDXCreateGeneric(sdkVersion, L"d3d8.dll", "Direct3DCreate8");

    if (LoadVoodoo())
    {
        //gVoodooCore->GetAdapter()->SetProperty(L"D3D8_Object")
    }

    return pD3D8;
}

/**
 * Direct3D 9
 */
void * WINAPI Voodoo3DCreate9(UINT sdkVersion)
{
    void * pD3D9 = VoodooDXCreateGeneric(sdkVersion, L"d3d9.dll", "Direct3DCreate9");

    if (LoadVoodoo())
    {

    }

    return pD3D9;
}

/**
 * DirectInput 8
 */
HRESULT WINAPI VoodooInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPVOID punkOuter)
{
    typedef HRESULT (WINAPI *DIInitFunc) (HINSTANCE, DWORD, REFIID, LPVOID *, LPVOID);

    HMODULE baselib = LoadSystemLibrary(L"dinput8.dll");

    if (!baselib)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DIInitFunc initFunc = (DIInitFunc) GetProcAddress(baselib, "DirectInput8Create");

    if (!initFunc)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    return (*initFunc) (hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

/**
 * DirectInput (unknown version)
 */
HRESULT WINAPI VoodooInputCreateGeneric
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID *lplpDirectInput,
    LPVOID punkOuter,
    const char * func
)
{

    typedef HRESULT (WINAPI *DIInitFunc) (HINSTANCE, DWORD, LPVOID, LPVOID);

    HMODULE baselib = LoadSystemLibrary(L"dinput.dll");

    if (!baselib)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DIInitFunc initFunc = (DIInitFunc) GetProcAddress(baselib, func);

    if (!initFunc)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    return (*initFunc) (hinst, dwVersion, lplpDirectInput, punkOuter);
}

HRESULT WINAPI VoodooInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID *lplpDirectInput, LPVOID punkOuter)
{
    return VoodooInputCreateGeneric(hinst, dwVersion, lplpDirectInput, punkOuter, "DirectInputCreateA");
}

HRESULT WINAPI VoodooInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPVOID *lplpDirectInput, LPVOID punkOuter)
{
    return VoodooInputCreateGeneric(hinst, dwVersion, lplpDirectInput, punkOuter, "DirectInputCreateW");
}

/**
 * DirectSound 8
 */
HRESULT WINAPI VoodooSoundCreate8(LPCGUID lpcGuidDevice, LPVOID * ppDS8, LPVOID pUnkOuter)
{
    typedef HRESULT (__stdcall *DSInitFunc) (LPCGUID, LPVOID *, LPVOID);

    HMODULE baselib = LoadSystemLibrary(L"dsound8.dll");

    if (!baselib)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    DSInitFunc initFunc = (DSInitFunc) GetProcAddress(baselib, "DirectSoundCreate8");

    if (!initFunc)
    {
        MessageBoxA(nullptr, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR | MB_OK);
        exit(1);
    }

    return (*initFunc) (lpcGuidDevice, ppDS8, pUnkOuter);
}
