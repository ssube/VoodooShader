#include "Includes.hpp"

namespace
{
    IVoodooParser * pParser;
}

FIXTURE(ParserFixture);

SETUP(ParserFixture)
{
    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(CLSID_VoodooParser, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooParser, (void**)&pParser);

    WIN_ASSERT_TRUE(SUCCEEDED(hr) && pParser != NULL, VSTR("IVoodooParser not created."));
}

TEARDOWN(ParserFixture)
{
    WIN_ASSERT_EQUAL(0, pParser->Release(), VSTR("IVoodooParser not properly released."));

    pParser = NULL;
    CoUninitialize();
}

// Test the Core init with a known-working config.
BEGIN_TESTF(Parser_SingleVar, ParserFixture)
{
    CComBSTR varname =  L"testvar";
    CComBSTR varvalue = L"parser test";
    CComBSTR msg =      L"this is a $(testvar) msg.";
    CComBSTR expected = L"this is a parser test msg.";

    HRESULT hr = pParser->Add(varname, varvalue, VT_Normal);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    hr = pParser->Parse(msg, PF_None, &msg);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));
    WIN_ASSERT_EQUAL(expected, msg, VSTR("String mismatch:\n\tParsed: %s\n"),msg );
}
END_TESTF;

BEGIN_TESTF(Parser_EmptyVar, ParserFixture)
{
    CComBSTR varname1 =  L"testvar";
    CComBSTR varvalue1 = L"parser test";
    CComBSTR varname2 =  L"evar";
    CComBSTR varvalue2 = L"";
    CComBSTR msg =       L"this is a $(testvar) msg, this is $(evar).";
    CComBSTR expected =  L"this is a parser test msg, this is .";

    HRESULT hr = pParser->Add(varname1, varvalue1, VT_Normal);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    hr = pParser->Add(varname2, varvalue2, VT_Normal);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    hr = pParser->Parse(msg, PF_None, &msg);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));
    WIN_ASSERT_EQUAL(expected, msg, VSTR("String mismatch:\n\tParsed: %s\n"),msg );

    hr = pParser->Remove(varname1);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    hr = pParser->Remove(varname2);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));
}
END_TESTF;

BEGIN_TESTF(Parser_SysOverwrite, ParserFixture)
{
    CComBSTR varname =  L"testvar";
    CComBSTR varvalue = L"sysvar test";
    CComBSTR msg =      L"this is a $(testvar) msg.";
    CComBSTR expected = L"this is a sysvar test msg.";

    HRESULT hr = pParser->Add(varname, varvalue, VT_System);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));

    hr = pParser->Add(varname, varvalue, VT_System);
    WIN_ASSERT_TRUE(FAILED(hr));

    hr = pParser->Parse(msg, PF_None, &msg);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));
    WIN_ASSERT_EQUAL(expected, msg, VSTR("String mismatch:\n\tParsed: %s\n"),msg );

    hr = pParser->Remove(varname);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));
}
END_TESTF;

BEGIN_TESTF(Parser_StateVar, ParserFixture)
{
    CComBSTR msg =      L"this is a $(svtest:state variable test)$(svtest) msg.";
    CComBSTR expected = L"this is a state variable test msg.";

    HRESULT hr = pParser->Parse(msg, PF_None, &msg);
    WIN_ASSERT_TRUE(SUCCEEDED(hr));
    WIN_ASSERT_EQUAL(expected, msg, VSTR("String mismatch:\n\tParsed: %s\n"),msg );
}
END_TESTF;
