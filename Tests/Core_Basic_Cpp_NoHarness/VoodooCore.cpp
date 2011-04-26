#include "Includes.hpp"

namespace
{
    IVoodooCore * pCore;
}

FIXTURE(CoreFixture);

SETUP(CoreFixture)
{
    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&pCore);

    WIN_ASSERT_TRUE(SUCCEEDED(hr) && pCore != NULL, _T("IVoodooCore not created."));
}

TEARDOWN(CoreFixture)
{
    WIN_ASSERT_EQUAL(0, pCore->Release(), _T("IVoodooCore not properly released."));

    pCore = NULL;
    CoUninitialize();
}

FIXTURE(CoreInitFixture);

SETUP(CoreInitFixture)
{
    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&pCore);

    WIN_ASSERT_TRUE(SUCCEEDED(hr) && pCore != NULL, _T("IVoodooCore not created."));

    InitParams iParams;
    iParams.Config = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_Working.xmlconfig");
    WIN_ASSERT_NOT_NULL(iParams.Config);

    hr = pCore->Initialize(iParams);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("Config: %s\nHRESULT: %X\n"), iParams.Config, hr);

    SysFreeString(iParams.Config);
}

TEARDOWN(CoreInitFixture)
{
    WIN_ASSERT_EQUAL(0, pCore->Release(), _T("IVoodooCore not properly released."));

    pCore = NULL;
    CoUninitialize();
}

// Test the Core init with a known-working config.
BEGIN_TESTF(CoreInit_Working, CoreFixture)
{
    InitParams iParams;
    iParams.Config = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_Working.xmlconfig");
    WIN_ASSERT_NOT_NULL(iParams.Config);

    HRESULT hr = pCore->Initialize(iParams);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("Config: %s\nHRESULT: %X\n"), iParams.Config, hr);

    SysFreeString(iParams.Config);
}
END_TESTF;

// Test Core init with a bad XML
BEGIN_TESTF(CoreInit_BadXml, CoreFixture)
{
    InitParams iParams;
    iParams.Config = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_BadXml.xmlconfig");
    WIN_ASSERT_NOT_NULL(iParams.Config);

    HRESULT hr = pCore->Initialize(iParams);
    //WIN_ASSERT_EQUAL(E_INVALIDCFG, hr);
    WIN_ASSERT_TRUE(FAILED(hr), _T("Config: %s\nHRESULT: %X\n"), iParams.Config, hr);

    SysFreeString(iParams.Config);
}
END_TESTF;

// Test Core init with a bad XML
BEGIN_TESTF(CoreInit_BadCLSID, CoreFixture)
{
    InitParams iParams;
    iParams.Config = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_BadCLSID.xmlconfig");
    WIN_ASSERT_NOT_NULL(iParams.Config);

    HRESULT hr = pCore->Initialize(iParams);
    //WIN_ASSERT_EQUAL(E_BADCLSID, hr);
    WIN_ASSERT_TRUE(FAILED(hr), _T("Config: %s\nHRESULT: %X\n"), iParams.Config, hr);

    SysFreeString(iParams.Config);
}
END_TESTF;

// Test uninit-ed core with get_Parser
BEGIN_TESTF(CoreNoInit_GetParser, CoreFixture)
{
    IVoodooParser * pParser = NULL;

    HRESULT hr = pCore->get_Parser(&pParser);
    WIN_ASSERT_TRUE(FAILED(hr), _T("HRESULT: %X\n"), hr);
    WIN_ASSERT_NOT_NULL(pParser);
}
END_TESTF;

// Test getting Parser from init-ed core
BEGIN_TESTF(CoreInit_GetParser, CoreInitFixture)
{
    IVoodooParser * pParser = NULL;

    HRESULT hr = pCore->get_Parser(&pParser);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);
    WIN_ASSERT_NOT_NULL(pParser);

    IUnknown * pUnk = NULL;
    hr = pParser->QueryInterface(IID_IUnknown, (void**)&pUnk);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));
    WIN_ASSERT_NOT_NULL(pUnk);
}
END_TESTF;

// Test putting and getting a (fake) context
BEGIN_TESTF(CoreInit_FakeContext, CoreInitFixture)
{
    void * context = (void*)0x0A0D09FF;

    VARIANT vc;
    VariantInit(&vc);
    V_VT(&vc) = VT_BYREF;
    V_BYREF(&vc) = context;

    HRESULT hr = pCore->put_CgContext(vc);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);

    VARIANT ovc;
    hr = pCore->get_CgContext(&ovc);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);
    WIN_ASSERT_EQUAL(VT_BYREF, V_VT(&ovc), _T("Return variant type: %X"), V_VT(&ovc));

    void * outcontext = V_BYREF(&ovc);
    WIN_ASSERT_EQUAL(context, outcontext);
}
END_TESTF;

BEGIN_TESTF(CoreInit_TextureTest, CoreInitFixture)
{
    TextureDesc Desc = { 256, 256, 256, false, true, TF_RGBA8 };
    void * TexAddr1 = (void*)0xDD44BB99;
    void * TexAddr2 = (void*)0x11CC7700;
    IVoodooTexture * pPassTex = nullptr;
    IVoodooTexture * pTechTex = nullptr;
    BSTR TexName1 = SysAllocString(L"Texture1");
    BSTR TexName2 = SysAllocString(L"Texture2");
    VARIANT TexData;
    VariantInit(&TexData);
    V_VT(&TexData) = VT_BYREF;

    V_BYREF(&TexData) = TexAddr1;
    HRESULT hr = pCore->CreateTexture(TexName1, Desc, TexData, &pPassTex);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);

    Desc.RenderTarget = false;
    V_BYREF(&TexData) = TexAddr2;
    hr = pCore->CreateTexture(TexName2, Desc, TexData, &pTechTex);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);

    SysFreeString(TexName1);
    SysFreeString(TexName2);

    hr = pCore->SetStageTexture(TS_Pass, pPassTex);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);

    hr = pCore->SetStageTexture(TS_Shader, pTechTex);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);

    IVoodooTexture * pPassOTex = nullptr;
    IVoodooTexture * pTechOTex = nullptr;

    hr = pCore->GetStageTexture(TS_Pass, &pPassOTex);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);

    hr = pCore->GetStageTexture(TS_Shader, &pTechOTex);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);

    WIN_ASSERT_EQUAL(pPassTex, pPassOTex);
    WIN_ASSERT_EQUAL(pTechTex, pTechOTex);

    VARIANT OTexData;
    VariantInit(&OTexData);

    hr = pPassOTex->get_Data(&OTexData);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);
    WIN_ASSERT_EQUAL(VT_BYREF, V_VT(&OTexData));
    WIN_ASSERT_EQUAL(TexAddr1, V_BYREF(&OTexData));

    hr = pTechOTex->get_Data(&OTexData);
    WIN_ASSERT_TRUE(SUCCEEDED(hr), _T("HRESULT: %X\n"), hr);
    WIN_ASSERT_EQUAL(VT_BYREF, V_VT(&OTexData));
    WIN_ASSERT_EQUAL(TexAddr2, V_BYREF(&OTexData));
}
END_TESTF;