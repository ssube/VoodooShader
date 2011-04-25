// 
// #include <windows.h>
// #include "objbase.h"
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

#include "Core_i.h"
#include "Core_i.c"

#include "WinUnit.h"

int main(int argc, TCHAR * argv[])
{
    CoInitialize(NULL);

    IVoodooCore * pCore = NULL;
    if ( FAILED(CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&pCore)) )
    {
        return 1;
    }

    InitParams params;
    ZeroMemory(&params, sizeof(params));
    params.Config = SysAllocString(L"VoodooConfig.xmlconfig");
    params.GlobalRoot = SysAllocString(L"M:\\VoodooShader\\");
    if ( FAILED(pCore->Initialize(params)) )
    {
        return 1;
    }

    CoUninitialize();
}