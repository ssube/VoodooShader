#include "Loader.hpp"

/**
 * @addtogroup VoodooLoader Voodoo/Loader
 * @{
 */

funcTypeLoad funcLoad;
funcTypeUnload funcUnload;

void * core = NULL;

/**
 * Reads the adapter's name from the Voodoo config file and loads it, if present.
 */
bool LoadVoodoo()
{
    HKEY VoodooPathKey = NULL;
    char VoodooPath[MAX_PATH];
    char VoodooCorePath[MAX_PATH];

    LONG keyOpen = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

    if ( keyOpen != ERROR_SUCCESS )
    {
        keyOpen = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

        if ( keyOpen != ERROR_SUCCESS )
        {
            MessageBoxA(NULL, "Voodoo Loader: Unable to find Voodoo registry key.", "Loader Error", MB_ICONERROR|MB_OK);
            return false;
        }
    }

    // Key is open
    DWORD valueType = REG_NONE;
    DWORD valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueExA(VoodooPathKey, "Path", NULL, &valueType, (BYTE*)VoodooPath, &valueSize);

    if ( valueQuery != ERROR_SUCCESS || valueType == REG_NONE )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to retrieve path from registry.", "Loader Error", MB_ICONERROR|MB_OK);
        return false;
    }

    StringCchCopyA(VoodooCorePath, MAX_PATH, VoodooPath);
    StringCchCatA(VoodooCorePath, MAX_PATH, "\\bin\\Voodoo_Core.dll");

    HMODULE adapter = LoadLibraryExA(VoodooCorePath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if ( !adapter )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to load core DLL.", "Loader Error", MB_ICONERROR|MB_OK);
        return false;
    }

    funcLoad   = (funcTypeLoad)  GetProcAddress(adapter, "CreateCore");
    funcUnload = (funcTypeUnload)GetProcAddress(adapter, "DestroyCore");

    if ( funcLoad == NULL )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to find core EP.", "Loader Error", MB_ICONERROR|MB_OK);
        return false;
    }

    try
    {
        core = (*funcLoad)(VoodooPath);
    } catch ( std::exception & exc ) {
        MessageBoxA(NULL, exc.what(), "Loader Error", MB_ICONERROR|MB_OK);
        core = NULL;
    }

    return ( core != NULL );
}

bool UnloadVoodoo()
{
    if ( funcUnload && core )
    {
        (*funcUnload)(core);
    }

    return true;
}

BOOL WINAPI DllMain
(
    _In_ void * _HDllHandle, 
    _In_ unsigned _Reason, 
    _In_opt_ void * _Reserved
)
{
    UNREFERENCED_PARAMETER(_HDllHandle);
    UNREFERENCED_PARAMETER(_Reserved);

    if ( _Reason == DLL_PROCESS_ATTACH )
    {
        return (BOOL)LoadVoodoo();
    } else if ( _Reason == DLL_PROCESS_DETACH ) {
        return (BOOL)UnloadVoodoo();
    }
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

/**
 * @}
 */