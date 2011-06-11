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

    MessageBox(NULL, Msg, _T("Voodoo Loader Error"), MB_OK|MB_ICONWARNING);
}

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
        ErrorMessage(_T("Unable to create Voodoo Core. CCI Error %X."), hr);
        return false;
    } else {
        HRESULT hr = gVoodooCore->Initialize(gInitParams);

        if ( FAILED(hr))
        {
            ErrorMessage(_T("Unable to create Voodoo Core. Init Error %X."), hr);
            return false;
        } else {
            return true;
        }
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
            ErrorMessage(_T("Unable to find Voodoo registry key. Error %u."), keyOpen);
            return false;
        }
    }

    // Key is open
    DWORD valueType = REG_NONE;
    DWORD valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueEx(pKey, _T("Path"), NULL, &valueType, (BYTE*)pPath, &valueSize);

    if ( valueQuery != ERROR_SUCCESS || valueType == REG_NONE )
    {
        ErrorMessage(_T("Unable to retrieve path from registry. Error %u."), valueQuery);
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
        ErrorMessage(_T("Unable to retrieve target module."));
        return false;
    }

    TCHAR pPath[MAX_PATH];
    if ( GetModuleFileName(hModule, pPath, MAX_PATH) == 0 )
    {
        ErrorMessage(_T("Unable to retrieve target path."));
        return false;
    }

    CString Path = pPath;
    int lastSlash = Path.ReverseFind('\\');

    if ( lastSlash == -1 )
    {
        ErrorMessage(_T("Voodoo Loader: Unable to parse target path."));
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
        ErrorMessage(_T("Voodoo Loader: Unable to retrieve current path."));
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
        ErrorMessage(_T("Voodoo Loader: Unable to retrieve loader path."));
        return false;
    }

    CString Path = pPath;
    int lastSlash = Path.ReverseFind('\\');

    if ( lastSlash == -1 )
    {
        ErrorMessage(_T("Voodoo Loader: Unable to parse loader path."));
        return false;
    }

    gInitParams.Loader = SysAllocString(Path.Mid(lastSlash));

    return true;
}
/**
 * @}
 */