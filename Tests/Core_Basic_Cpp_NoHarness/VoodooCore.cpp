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

    BSTR pFileName = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_Working.xmlconfig");
    WIN_ASSERT_NOT_EQUAL(nullptr, pFileName);

    WIN_TRACE(_T("Using config file: %s\n"), pFileName);

    hr = pCore->Initialize(pFileName);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    SysFreeString(pFileName);
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
    BSTR pFileName = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_Working.xmlconfig");
    WIN_ASSERT_NOT_EQUAL(nullptr, pFileName);

    WIN_TRACE(_T("Using config file: %s\n"), pFileName);

    HRESULT hr = pCore->Initialize(pFileName);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    SysFreeString(pFileName);
}
END_TESTF;

// Test Core init with a bad XML
BEGIN_TESTF(CoreInit_BadXml, CoreFixture)
{
    BSTR pFileName = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_BadXml.xmlconfig");
    WIN_ASSERT_NOT_EQUAL(nullptr, pFileName);

    WIN_TRACE(_T("Using config file: %s\n"), pFileName);

    HRESULT hr = pCore->Initialize(pFileName);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    //WIN_ASSERT_EQUAL(E_INVALIDCFG, hr);
    WIN_ASSERT_TRUE(FAILED(hr));

    SysFreeString(pFileName);
}
END_TESTF;

// Test Core init with a bad XML
BEGIN_TESTF(CoreInit_BadCLSID, CoreFixture)
{
    BSTR pFileName = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_BadCLSID.xmlconfig");
    WIN_ASSERT_NOT_EQUAL(nullptr, pFileName);

    WIN_TRACE(_T("Using config file: %s\n"), pFileName);

    HRESULT hr = pCore->Initialize(pFileName);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    //WIN_ASSERT_EQUAL(E_BADCLSID, hr);
    WIN_ASSERT_TRUE(FAILED(hr));

    SysFreeString(pFileName);
}
END_TESTF;

// Test uninit-ed core with get_Parser
BEGIN_TESTF(CoreNoInit_GetParser, CoreFixture)
{
    IVoodooParser * pParser = NULL;

    HRESULT hr = pCore->get_Parser(&pParser);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    WIN_ASSERT_TRUE(FAILED(hr));
    WIN_ASSERT_EQUAL(nullptr, pParser);
}
END_TESTF;

// Test getting Parser from init-ed core
BEGIN_TESTF(CoreInit_GetParser, CoreInitFixture)
{
    IVoodooParser * pParser = NULL;

    HRESULT hr = pCore->get_Parser(&pParser);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    WIN_ASSERT_TRUE(SUCCEEDED(hr));
    WIN_ASSERT_NOT_EQUAL(nullptr, pParser);
}
END_TESTF;

// Test putting and getting a (fake) context
BEGIN_TESTF(CoreInit_FakeContext, CoreInitFixture)
{
    void * context = (void*)0x0a0d09FF;

    VARIANT vc;
    VariantInit(&vc);
    V_VT(&vc) = VT_BYREF;
    V_BYREF(&vc) = context;

    HRESULT hr = pCore->put_CgContext(vc);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    VARIANT ovc;
    hr = pCore->get_CgContext(&ovc);
    WIN_TRACE(_T("HRESULT: %X\n"), hr);

    WIN_ASSERT_EQUAL(VT_BYREF, V_VT(&ovc));

    void * outcontext = V_BYREF(&ovc);

    WIN_ASSERT_EQUAL(context, outcontext);
}
END_TESTF;