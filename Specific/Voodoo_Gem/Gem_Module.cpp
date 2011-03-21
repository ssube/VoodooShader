#include "Gem_Module.hpp"

#include "Gem_Adapter.hpp"
#include "IVoodoo3D8.hpp"

#include <regex>

HRESULT DefaultErrorCode = D3DERR_INVALIDCALL;

VoodooShader::Core * VoodooCore = NULL;
VoodooShader::IHookManager * VoodooHooker = NULL;
VoodooShader::Gem::Adapter * VoodooGem = NULL;

//! @todo Shift most of these globals, except the core and adapter, into the adapter (as members).
//!        This may work well with functions to set some of the matrices and such. Shifting them
//!        properly is a right pain and will take some rethinking of parts of the system.

D3DCAPS8 d3d8Caps;

IVoodoo3D8 * VoodooObject = NULL;
IVoodoo3DDevice8 * VoodooDevice = NULL;

TextureTuple gBackbuffer;
TextureTuple gScratch;
TextureTuple gThisFrame;
std::map<VoodooShader::String, TextureTuple> gTextures;

VoodooShader::ParameterRef gMatrixView, gMatrixProj, gMatrixWorld;

D3DPRESENT_PARAMETERS gParams;

VoodooShader::ShaderRef testShader;
VoodooShader::String gLastFilename;
bool gNextTexture = false;

namespace VoodooShader
{
    namespace Gem
    {
        int API_ClassCount()
        {
            return 1;
        }

        const char * API_ClassInfo
        (
            _In_ int number
        )
        {
            if ( number == 0 )
            {
                return "Gem_Adapter";
            } else {
                return NULL;
            }
        }

        IObject * API_ClassCreate
        (
            _In_ int number, 
            _In_ Core * core
        )
        {
            if ( number == 0 )
            {
                return new Gem::Adapter(core);
            } else {
                return NULL;
            }
        }
    }
}

void * WINAPI Gem_D3D8Create(UINT sdkVersion)
{
    //IDirect3D9 * realObject = Direct3DCreate9(sdkVersion);
    //IVoodoo3D8 * fakeObject = new IVoodoo3D8(realObject);

    //return fakeObject;

    VoodooCore->Log
    (
        LL_Info,
        VOODOO_GEM_NAME,
        "Direct3DCreate8 called, SDK version: %u.", 
        sdkVersion
    );

    //Load the real d3d8 dll and get device caps
    char Path[MAX_PATH];
    GetSystemDirectoryA (Path, MAX_PATH);
    strcat_s (Path, MAX_PATH, "\\d3d8.dll");

    HMODULE d3ddll = LoadLibraryA(Path);

    if ( d3ddll == NULL )
    {
        VoodooCore->Log
        (
            LL_Fatal,
            VOODOO_GEM_NAME,
            "Could not load D3D8 library."
        );

        return NULL;
    }

    typedef IDirect3D8 * (__stdcall * D3DFunc8)(UINT);
    D3DFunc8 d3d8func = (D3DFunc8)GetProcAddress(d3ddll, "Direct3DCreate8");

    if (d3d8func == NULL) 
    {
        VoodooCore->Log
        (
            LL_Fatal, 
            VOODOO_GEM_NAME, 
            "Could not find D3D8 create true func."
         );

        return 0;
    }

    IDirect3D8 * TempObject = (d3d8func)(sdkVersion);
    HRESULT hr = TempObject->GetDeviceCaps(0, D3DDEVTYPE_HAL, &d3d8Caps);
    if (hr != D3D_OK) 
    { 
        VoodooCore->Log
            (
            LL_Error,
            VOODOO_GEM_NAME,
            "Could not get D3D8 caps."
            );
    }
    TempObject->Release();

    if ( d3ddll )
    {
        FreeLibrary(d3ddll);
    }

    // Call DX9 to create a real device with the latest version
    IDirect3D9 * object = Direct3DCreate9(D3D_SDK_VERSION);
    // Turn it into a FakeObject and return it.
    IVoodoo3D8 * vObj = new IVoodoo3D8(object);
    VoodooObject = vObj;
    return vObj;
}

__out HANDLE WINAPI Gem_CreateFileA
(
    __in LPCSTR lpFileName, 
    __in DWORD dwDesiredAccess, 
    __in DWORD dwShareMode, 
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
    __in DWORD dwCreationDisposition, 
    __in DWORD dwFlagsAndAttributes, 
    __in_opt HANDLE hTemplateFile
)
{
    //StringCchCopyA(createFileName, MAX_PATH, lpFileName);
    HANDLE file = CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

    gLastFilename = lpFileName;

    try
    {
        std::tr1::regex imageformat(".*\\.(dds|tga|bmp|targa)");
        gNextTexture = std::tr1::regex_match(gLastFilename, imageformat);
    } catch ( std::tr1::regex_error & error ) {
        VoodooCore->Log
        (
            LL_Error,
            VOODOO_GEM_NAME,
            "Regex error: %s.",
            error.what()
        );
        gNextTexture = false;
    }

    if ( VoodooCore )
    {
        LogLevel ll = ( file == INVALID_HANDLE_VALUE ) ? LL_Warning_API : LL_Info_API;

        VoodooCore->Log
        (
            ll,
            VOODOO_GEM_NAME,
            "CreateFileA(%s, %u, %u, %p, %u, %u, %p) == %p",
            lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
            dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, file
        );
    }

    return file;
}