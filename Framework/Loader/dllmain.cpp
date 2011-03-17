
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

/**
 * Reads the Adapter's name from the Voodoo config file and loads it, if present.
 */
void LoadAdapter()
{
    FILE * config = NULL;
    HKEY VoodooPathKey = NULL;
    char VoodooPath[MAX_PATH];
    char AdapterModule[MAX_PATH];

    errno_t error = fopen_s(&config, "VoodooConfig.xml", "r");

    if ( error != 0 || config == NULL )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to open config file.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    if ( !fgets(AdapterModule, MAX_PATH, config) )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to read adapter from config file.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    LONG keyOpen = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

    if ( keyOpen != ERROR_SUCCESS )
    {
        keyOpen = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

        if ( keyOpen != ERROR_SUCCESS )
        {
            MessageBoxA(NULL, "Voodoo Loader: Unable to find Voodoo registry key.", "Loader Error", MB_ICONERROR|MB_OK);
            exit(1);
        }
    }

    // Key is open
    DWORD valueType = REG_NONE;
    DWORD valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueExA(VoodooPathKey, "Path", NULL, &valueType, (BYTE*)VoodooPath, &valueSize);

    if ( valueQuery != ERROR_SUCCESS || valueType == REG_NONE )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to retrieve path from registry.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    StringCchCatA(VoodooPath, MAX_PATH, "\\bin\\");
    StringCchCatA(VoodooPath, MAX_PATH, AdapterModule);

    HMODULE adapter = LoadLibraryExA(VoodooPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if ( !adapter )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to load adapter DLL.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    typedef bool (__stdcall * InitFunc)(HMODULE);
    InitFunc initFunc = (InitFunc)GetProcAddress(adapter, "LoadAdapter");

    if ( !initFunc )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to find adapter EP.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    bool success = (*initFunc)(NULL); //! @todo Provide the process handle

    if ( !success )
    {
        MessageBoxA(NULL, "Voodoo Loader: Adapter failed to load.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    return;
}

// Support functions

// Load a module from the system directory (uses absolute path to guarantee proper file).
HMODULE LoadSystemLibrary(const char * libname)
{
    char Path[MAX_PATH];

    GetSystemDirectoryA (Path, MAX_PATH);
    strcat_s(Path, MAX_PATH, "\\");
    strcat_s(Path, MAX_PATH, libname);

    return LoadLibraryA(Path);
}

// Most DirectX libraries use an identical loading function, with only the name varying.
// This function takes an SDK version, library name and function name, loads and calls the 
// proper init function.
void * WINAPI VoodooDXCreateGeneric(UINT sdkVersion, const char * lib, const char * func)
{
    typedef void * (__stdcall * DXInitFunc)(UINT);

    LoadAdapter();

    HMODULE baselib = LoadSystemLibrary(lib);

    if ( !baselib )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    DXInitFunc initFunc = (DXInitFunc)GetProcAddress(baselib, func);

    if ( !initFunc )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    return (*initFunc)(sdkVersion);
}

// Various adapter-loading entry points
// Direct3D 8
void * WINAPI Voodoo3DCreate8(UINT sdkVersion)
{
    return VoodooDXCreateGeneric(sdkVersion, "d3d8.dll", "Direct3DCreate8");
}

// Direct3D 9
void * WINAPI Voodoo3DCreate9(UINT sdkVersion)
{
    return VoodooDXCreateGeneric(sdkVersion, "d3d9.dll", "Direct3DCreate9");
}

// DirectInput 8
HRESULT WINAPI VoodooInput8Create
(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID * ppvOut,
    LPVOID punkOuter
)
{
    typedef HRESULT (__stdcall * DIInitFunc)(HINSTANCE, DWORD, REFIID, LPVOID*, LPVOID);

    LoadAdapter();

    HMODULE baselib = LoadSystemLibrary("dinput8.dll");

    if ( !baselib )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    DIInitFunc initFunc = (DIInitFunc)GetProcAddress(baselib, "DirectInput8Create");

    if ( !initFunc )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    return (*initFunc)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

// DirectSound 8
HRESULT VoodooSoundCreate8
(
    LPCGUID lpcGuidDevice,
    LPVOID * ppDS8,
    LPVOID pUnkOuter
)
{
    typedef HRESULT (__stdcall * DSInitFunc)(LPCGUID, LPVOID*, LPVOID);

    LoadAdapter();

    HMODULE baselib = LoadSystemLibrary("dsound8.dll");

    if ( !baselib )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to load system DLL.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    DSInitFunc initFunc = (DSInitFunc)GetProcAddress(baselib, "DirectSoundCreate8");

    if ( !initFunc )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to find system EP.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    return (*initFunc)(lpcGuidDevice, ppDS8, pUnkOuter);
}