#include "Common.hpp"

#include <strsafe.h>

/**
 * @addtogroup VoodooLoader Voodoo/Loader
 * @{
 */

IVoodooCore * gVoodooCore = NULL;
IVoodooAdapter * gVoodooAdapter = NULL;
InitParams gInitParams;

void WINAPI ErrorMessage(LPTSTR pMsg, ...)
{
    va_list args;
    va_start(args, pMsg);

    CString Msg;
    Msg.FormatV(pMsg, args);

    va_end(args);

    MessageBox(NULL, Msg, VSTR("Voodoo Loader Error"), MB_OK|MB_ICONWARNING);
}

VARIANT * ToVariant(IUnknown * ptr)
{
    VARIANT * v = new VARIANT();;
    VariantInit(v);
    V_VT(v) = VT_UNKNOWN;
    V_UNKNOWN(v) = ptr;
    return v;
}

/**
 * Load a module from the system directory (uses absolute path to guarantee proper file).
 *
 * @param libname The filename (and optionally extension) of the file to load. This may contain a
 *     path, but it will be interepreted relative to the system directory.
 * @return A handle to the module if loaded or a null handle otherwise.
 */
HMODULE LoadSystemLibrary(const LPTSTR libname)
{
    TCHAR Path[MAX_PATH];

    GetSystemDirectory(Path, MAX_PATH);
    StringCchCat(Path, MAX_PATH, VSTR("\\"));
    StringCchCat(Path, MAX_PATH, libname);

    return LoadLibrary(Path);
}

/**
 * Creates and initializes the Voodoo core object, if one does not already exist. This triggers
 * most of the loading process and must be called after CoInitialize.
 * 
 * @return True if the core was able to be created and initialized, false on errors.
 **/
bool WINAPI VoodooStartup()
{
    if ( gVoodooCore != NULL )
    {
        return false;
    }

    HRESULT hr = CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&gVoodooCore);

    if ( FAILED(hr) )
    {
        ErrorMessage(VSTR("Unable to create Voodoo Core. CCI Error %X."), hr);
        return false;
    } else {
        hr = gVoodooCore->Initialize(gInitParams);

        if ( FAILED(hr))
        {
            ErrorMessage(VSTR("Unable to create Voodoo Core. Init Error %X."), hr);
            return false;
        } else {
            hr  = gVoodooCore->get_Adapter(&gVoodooAdapter);

            if ( FAILED(hr))
            {
                ErrorMessage(VSTR("Unable to retrieve Voodoo Adapter. Error %X."), hr);
                return false;
            } else {
                return true;
            }
        }
    }
}

/**
 * Release the global Voodoo core, if it exists, and deallocate all global init parameter strings.
 * 
 * @return Always true.
 **/
bool WINAPI VoodooShutdown()
{
    if ( gVoodooCore )
    {
        gVoodooCore->Release();
    }

    SysFreeString(gInitParams.GlobalRoot);
    SysFreeString(gInitParams.LocalRoot);
    SysFreeString(gInitParams.RunRoot);
    SysFreeString(gInitParams.Target);
    SysFreeString(gInitParams.Loader);
    SysFreeString(gInitParams.Config);

    return true;
}

/**
 * Retrieve the global root path for Voodoo from the registry and set the global init params to use
 * it.
 * 
 * @return Success of the operation.
 **/
bool WINAPI GetGlobalRoot()
{
    HKEY pKey = NULL;
    TCHAR pPath[MAX_PATH];

    LONG keyOpen = RegOpenKeyEx(HKEY_CURRENT_USER, VSTR("SOFTWARE\\VoodooShader"), 0, KEY_QUERY_VALUE, &pKey);

    if ( keyOpen != ERROR_SUCCESS )
    {
        keyOpen = RegOpenKeyEx(HKEY_LOCAL_MACHINE, VSTR("SOFTWARE\\VoodooShader"), 0, KEY_QUERY_VALUE, &pKey);

        if ( keyOpen != ERROR_SUCCESS )
        {
            ErrorMessage(VSTR("Unable to find Voodoo registry key. Error %u."), keyOpen);
            return false;
        }
    }

    // Key is open
    DWORD valueType = REG_NONE;
    DWORD valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueEx(pKey, VSTR("Path"), NULL, &valueType, (BYTE*)pPath, &valueSize);

    if ( valueQuery != ERROR_SUCCESS || valueType == REG_NONE )
    {
        ErrorMessage(VSTR("Unable to retrieve path from registry. Error %u."), valueQuery);
        return false;
    }

    gInitParams.GlobalRoot = SysAllocString(pPath);

    return true;
}

bool WINAPI GetLocalRoot()
{
    HMODULE hModule = GetModuleHandle(NULL);
    if ( hModule == NULL )
    {
        ErrorMessage(VSTR("Unable to retrieve target module."));
        return false;
    }

    TCHAR pPath[MAX_PATH];
    if ( GetModuleFileName(hModule, pPath, MAX_PATH) == 0 )
    {
        ErrorMessage(VSTR("Unable to retrieve target path."));
        return false;
    }

    CString Path = pPath;
    int lastSlash = Path.ReverseFind('\\');

    if ( lastSlash == -1 )
    {
        ErrorMessage(VSTR("Voodoo Loader: Unable to parse target path."));
        return false;
    }

    gInitParams.LocalRoot = SysAllocString(Path.Left(lastSlash));
    gInitParams.Target = SysAllocString(Path.Mid(lastSlash));

    return true;
}

bool WINAPI GetRunRoot()
{
    TCHAR pPath[MAX_PATH];

    if ( GetCurrentDirectory(MAX_PATH, pPath) == 0 )
    {
        ErrorMessage(VSTR("Voodoo Loader: Unable to retrieve current path."));
        return false;
    }

    gInitParams.RunRoot = SysAllocString(pPath);

    return true;
}

bool WINAPI GetLoader(HINSTANCE hLoader)
{
    TCHAR pPath[MAX_PATH];
    if ( GetModuleFileName(hLoader, pPath, MAX_PATH) == 0 )
    {
        ErrorMessage(VSTR("Voodoo Loader: Unable to retrieve loader path."));
        return false;
    }

    CString Path = pPath;
    int lastSlash = Path.ReverseFind('\\');

    if ( lastSlash == -1 )
    {
        ErrorMessage(VSTR("Voodoo Loader: Unable to parse loader path."));
        return false;
    }

    gInitParams.Loader = SysAllocString(Path.Mid(lastSlash));

    return true;
}
/**
 * @}
 */