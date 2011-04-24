#include "Common.hpp"

#include <strsafe.h>
#include "Core_i.c"

/**
 * @addtogroup VoodooLoader Voodoo/Loader
 * @{
 */

IVoodooCore * gVoodooCore = NULL;
IVoodooAdapter * gVoodooAdapter = NULL;
InitParams gInitParams;

/**
 * Load a module from the system directory (uses absolute path to guarantee proper file).
 */
VARIANT * PtrVariant(void * ptr)
{
    VARIANT * v = new VARIANT();;
    VariantInit(v);
    V_VT(v) = VT_BYREF;
    V_BYREF(v) = ptr;
    return v;
}

HMODULE LoadSystemLibrary(const PTCHAR libname)
{
    TCHAR Path[MAX_PATH];

    GetSystemDirectory(Path, MAX_PATH);
    StringCchCat(Path, MAX_PATH, _T("\\"));
    StringCchCat(Path, MAX_PATH, libname);

    return LoadLibrary(Path);
}

bool WINAPI VoodooStartup()
{
    HRESULT hr = CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&gVoodooCore);

    if ( FAILED(hr) )
    {
        CString error;
        error.Format(_T("Unable to create Voodoo Core. Error %X."), hr);
        MessageBox(NULL, error, _T("Voodoo Error"), MB_ICONWARNING|MB_OK);
        return false;
    } else {
        HRESULT hr = gVoodooCore->Initialize(gInitParams);
        return ( SUCCEEDED(hr) );
    }
}

bool WINAPI VoodooShutdown()
{
    if ( gVoodooCore )
    {
        gVoodooCore->Release();
    }

    return true;
}

bool WINAPI GetGlobalRoot()
{
    HKEY pKey = NULL;
    TCHAR pPath[MAX_PATH];

    LONG keyOpen = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\VoodooShader"), 0, KEY_QUERY_VALUE, &pKey);

    if ( keyOpen != ERROR_SUCCESS )
    {
        keyOpen = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\VoodooShader"), 0, KEY_QUERY_VALUE, &pKey);

        if ( keyOpen != ERROR_SUCCESS )
        {
            MessageBox(NULL, _T("Voodoo Loader: Unable to find Voodoo registry key."), _T("Loader Error"), MB_ICONERROR|MB_OK);
            return false;
        }
    }

    // Key is open
    DWORD valueType = REG_NONE;
    DWORD valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueEx(pKey, _T("Path"), NULL, &valueType, (BYTE*)pPath, &valueSize);

    if ( valueQuery != ERROR_SUCCESS || valueType == REG_NONE )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to retrieve path from registry."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        return false;
    }

    gInitParams.GlobalRoot = SysAllocString(pPath);
}

bool WINAPI GetLocalRoot()
{
    HMODULE hModule = GetModuleHandle(NULL);
    if ( hModule == NULL )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to retrieve target module."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        return false;
    }

    TCHAR pPath[MAX_PATH];
    if ( GetModuleFileName(hModule, pPath, MAX_PATH) == 0 )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to retrieve target path."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        return false;
    }

    CString Path = pPath;
    int lastSlash = Path.ReverseFind('\\');

    if ( lastSlash == -1 )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to parse target path."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        return false;
    }

    gInitParams.LocalRoot = SysAllocString(Path.Left(lastSlash));
    gInitParams.Target = SysAllocString(Path.Mid(lastSlash));
}

bool WINAPI GetRunRoot()
{
    TCHAR pPath[MAX_PATH];

    if ( GetCurrentDirectory(MAX_PATH, pPath) == 0 )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to retrieve current path."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        return false;
    }

    gInitParams.RunRoot = SysAllocString(pPath);
}

bool WINAPI GetLoader(HINSTANCE hLoader)
{
    TCHAR pPath[MAX_PATH];
    if ( GetModuleFileName(hLoader, pPath, MAX_PATH) == 0 )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to retrieve loader path."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        return false;
    }

    CString Path = pPath;
    int lastSlash = Path.ReverseFind('\\');

    if ( lastSlash == -1 )
    {
        MessageBox(NULL, _T("Voodoo Loader: Unable to parse loader path."), _T("Loader Error"), MB_ICONERROR|MB_OK);
        return false;
    }

    gInitParams.Loader = SysAllocString(Path.Mid(lastSlash));
}
/**
 * @}
 */