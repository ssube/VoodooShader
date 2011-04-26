// 
// #include <windows.h>
// #include "objbase.h"
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

#include "VoodooFramework.hpp"

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

    IVoodooParser * pParser;
    pCore->get_Parser(&pParser);

    BSTR Parsed;
    pParser->AddVariable(L"lang", L"c++");
    pParser->Parse(L"this is from $(lang).", PF_None, &Parsed);

    CoUninitialize();
}