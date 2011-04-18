#include "Common.hpp"

#include <strsafe.h>

/**
 * @addtogroup VoodooLoader Voodoo/Loader
 * @{
 */

LPUNKNOWN VoodooCore = NULL;

/**
 * Load a module from the system directory (uses absolute path to guarantee proper file).
 */
HMODULE LoadSystemLibrary(const PTCHAR libname)
{
    TCHAR Path[MAX_PATH];

    GetSystemDirectory(Path, MAX_PATH);
    StringCchCat(Path, MAX_PATH, "\\");
    StringCchCat(Path, MAX_PATH, libname);

    return LoadLibrary(Path);
}

void LoadVoodoo()
{
    HRESULT hr = CoCreateInstance(CLSID_Core, NULL, CLSCTX_INPROC_SERVER, IID_VoodooCore, &VoodooCore);

    if ( FAILED(hr) )
    {
        MessageBox(NULL, T"Unable to load the Voodoo core.", T"Voodoo Error", MB_ICONWARNING|MB_OK);
    } else {
        HRESULT hr = VoodooCore->Initialize("VoodooConfig.xml");
    }
}

void UnloadVoodoo()
{
    if ( VoodooCore )
    {
        VoodooCore->Release();
    }
}

/**
 * @}
 */