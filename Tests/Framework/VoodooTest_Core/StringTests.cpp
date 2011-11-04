// Voodoo Shader Framework Unit Tests: Voodoo/ICore:VoodooShader::ICore
#include "WinUnit.h"

#include "VoodooFramework.hpp"
#pragma comment(lib, "Voodoo_Core.lib")

using namespace VoodooShader;

BEGIN_TEST(String_EmptyCtor)
{
    String str;
    WIN_ASSERT_ZERO(str.GetLength(), L"Empty ctor string length is %d, not 0.", str.GetLength());
}
END_TEST;

BEGIN_TEST(String_CCharCtor)
{
    String str('a');
    WIN_ASSERT_EQUAL(L'a', str[0], L"CChar ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_CCharMCtor)
{
    String str(10, 'a');
    WIN_ASSERT_EQUAL(10, str.GetLength(), L"CCharM ctor did not init to proper length (%d of 10).", str.GetLength());
}
END_TEST;

BEGIN_TEST(String_CharCtor)
{
    String str(L'a');
    WIN_ASSERT_EQUAL(L'a', str[0], L"Char ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_CharMCtor)
{
    String str(10, L'a');
    WIN_ASSERT_EQUAL(10, str.GetLength(), L"CharM ctor did not init to proper length (%d of 10).", str.GetLength());
}
END_TEST;

BEGIN_TEST(String_CStrCtor)
{
    String str("test");
    WIN_ASSERT_EQUAL(str, L"test", L"CStr ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_StrCtor)
{
    String str(L"test");
    WIN_ASSERT_EQUAL(str, L"test", L"CStr ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_CStrPCtor)
{
    String str(4, "testing");
    WIN_ASSERT_EQUAL(str, L"test", L"CStrP ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_StrPCtor)
{
    String str(4, L"testing");
    WIN_ASSERT_EQUAL(str, L"test", L"CStrP ctor did not convert properly.");
}
END_TEST;
