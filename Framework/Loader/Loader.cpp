
#include "Loader.hpp"

#include "Loader_Version.hpp"

BOOL WINAPI DllMain
(
    _In_ HINSTANCE hInst, 
    _In_ unsigned Reason, 
    _In_opt_ void * Reserved
)
{
    UNREFERENCED_PARAMETER(Reserved);

    if ( Reason == DLL_PROCESS_ATTACH )
    {
        bool init = true;
        init &= GetRunRoot();
        init &= GetLoader(hInst);
        return (BOOL)init;
    } else if ( Reason == DLL_PROCESS_DETACH ) {
        return (BOOL)VoodooShutdown();
    }
}

// Support functions


/**
 * Most DirectX libraries use an identical loading function, with only the name varying.
 * This function takes an SDK version, library name and function name, loads and calls the 
 * proper init function.
 */
IUnknown * WINAPI VoodooDXCreateGeneric(UINT sdkVersion, const PTCHAR lib, const char * func)
{
    typedef IUnknown * (WINAPI * DXInitFunc)(UINT);

    HMODULE baselib = LoadSystemLibrary(lib);

    if ( !baselib )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to load system DLL."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    DXInitFunc initFunc = (DXInitFunc)GetProcAddress(baselib, func);

    if ( !initFunc )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to find system EP."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    IUnknown * dxObj = (*initFunc)(sdkVersion);

    return dxObj;
}

// Various adapter-loading entry points
// Direct3D 8
IUnknown * WINAPI Voodoo3DCreate8(UINT sdkVersion)
{
    IUnknown * D3D8Obj = VoodooDXCreateGeneric(sdkVersion, _T("d3d8.dll"), "Direct3DCreate8");

    VoodooStartup();
    VARIANT * v = PtrVariant(D3D8Obj);
    gVoodooAdapter->SetProperty(L"Direct3DCreate8", v);
    delete v;

    return D3D8Obj;
}

// Direct3D 9
IUnknown * WINAPI Voodoo3DCreate9(UINT sdkVersion)
{
    IUnknown * D3D9Obj = VoodooDXCreateGeneric(sdkVersion, _T("d3d9.dll"), "Direct3DCreate9");

    VoodooStartup();
    VARIANT * v = PtrVariant(D3D9Obj);
    gVoodooAdapter->SetProperty(L"Direct3DCreate9", v);
    delete v;

    return D3D9Obj;
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
    typedef HRESULT (WINAPI * DIInitFunc)(HINSTANCE, DWORD, REFIID, LPVOID*, LPVOID);

    HMODULE baselib = LoadSystemLibrary(_T("dinput8.dll"));
    if ( !baselib )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to load system DLL."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    DIInitFunc initFunc = (DIInitFunc)GetProcAddress(baselib, "DirectInput8Create");
    if ( !initFunc )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to find system EP."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    return (*initFunc)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

// DirectInput (unknown version)
HRESULT WINAPI VoodooInputCreateGeneric
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID * lplpDirectInput,
    LPVOID punkOuter,
    const char * func
)
{
    typedef HRESULT (WINAPI * DIInitFunc)(HINSTANCE, DWORD, LPVOID, LPVOID);

    HMODULE baselib = LoadSystemLibrary(_T("dinput.dll"));

    if ( !baselib )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to load system DLL."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    DIInitFunc initFunc = (DIInitFunc)GetProcAddress(baselib, func);

    if ( !initFunc )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to find system EP."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    return (*initFunc)(hinst, dwVersion, lplpDirectInput, punkOuter);
}

HRESULT WINAPI VoodooInputCreateA
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID * lplpDirectInput,
    LPVOID punkOuter
)
{
    return VoodooInputCreateGeneric(hinst, dwVersion, lplpDirectInput, punkOuter, "DirectInputCreateA");
}

HRESULT WINAPI VoodooInputCreateW
(
    HINSTANCE hinst,
    DWORD dwVersion,
    LPVOID * lplpDirectInput,
    LPVOID punkOuter
)
{
    return VoodooInputCreateGeneric(hinst, dwVersion, lplpDirectInput, punkOuter, "DirectInputCreateW");
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

    HMODULE baselib = LoadSystemLibrary(_T("dsound8.dll"));

    if ( !baselib )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to load system DLL."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    DSInitFunc initFunc = (DSInitFunc)GetProcAddress(baselib, "DirectSoundCreate8");

    if ( !initFunc )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to find system EP."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        exit(1);
    }

    return (*initFunc)(lpcGuidDevice, ppDS8, pUnkOuter);
}
