
#include <iostream>
#include <windows.h>
#include "objbase.h"
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#include "Core_i.h"
#include "Core_i.c"

using namespace std;

int wmain(int argc, wchar_t * argv[])
{
    wcout << L"Beginning Voodoo/Core basic tests." << endl;

    CoInitialize(NULL);

    wcout << L"COM system initialized." << endl;

    IVoodooCore * pCore = NULL;
    HRESULT hr = CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&pCore);
    if ( FAILED(hr) || pCore == NULL )
    {
        wcout << L"Unable to create core instance." << endl;
        return 1;
    }

    BSTR pFileName = SysAllocString(L"M:\\VoodooShader\\Tests\\Voodoo_Basic.xmlconfig");
    VARIANT vc;
    VariantInit(&vc);
    V_VT(&vc) = VT_BSTR;
    V_BSTR(&vc) = pFileName;
    hr = pCore->Initialize(vc);
    SysFreeString(pFileName);

    if ( FAILED(hr) )
    {
        wcout << L"Unable to init core: " << hr << endl;
    }

    CoUninitialize();
}