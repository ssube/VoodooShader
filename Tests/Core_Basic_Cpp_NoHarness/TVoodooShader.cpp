#include "Includes.hpp"

namespace
{
    InitParams Params;
    IVoodooCore * pCore;
}

FIXTURE(ShaderFixture);

SETUP(ShaderFixture)
{
    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&pCore);

    WIN_ASSERT_TRUE(SUCCEEDED(hr) && pCore != NULL, _T("IVoodooCore not created."));

    CComBSTR config = L"M:\\VoodooShader\\Tests\\Resources\\Init_Working.xmlconfig";
    CComBSTR target = L"M:\\VoodooShader\\TargetApp.exe";
    CComBSTR loader = L"M:\\VoodooShader\\LoaderMod.dll";
    CComBSTR root   = L"M:\\VoodooShader\\";

    ZeroMemory(&Params, sizeof(Params));
    Params.Config = config;
    Params.Target = target;
    Params.Loader = loader;
    Params.GlobalRoot = Params.RunRoot = Params.LocalRoot = root;

    hr = pCore->Initialize(Params);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("Config: %s\nHRESULT: %X\n"), Params.Config, hr);
}

TEARDOWN(ShaderFixture)
{
    WIN_ASSERT_EQUAL(0, pCore->Release(), _T("IVoodooCore not properly released."));

    pCore = NULL;
    CoUninitialize();
}

BEGIN_TESTF(ShaderNull, ShaderFixture)
{
    IVoodooShader * pShader = NULL;
    HRESULT hr = pCore->CreateShader(NULL, NULL, &pShader);
    WIN_ASSERT_TRUE(FAILED(hr));
    WIN_ASSERT_NULL(pShader);

    //pShader
}
END_TESTF;