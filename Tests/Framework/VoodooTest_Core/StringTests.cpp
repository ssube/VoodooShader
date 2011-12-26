// Voodoo Shader Framework Unit Tests: Voodoo/ICore:VoodooShader::ICore
#include "WinUnit.h"

#include "VoodooFramework.hpp"
#pragma comment(lib, "Voodoo_Core.lib")

using namespace VoodooShader;

BEGIN_TEST(String_Ctor_Empty)
{
    String str;
    WIN_ASSERT_ZERO(str.GetLength(), L"Empty ctor string length is %d, not 0.", str.GetLength());
}
END_TEST;

BEGIN_TEST(String_Ctor_Char)
{
    String str('a');
    WIN_ASSERT_EQUAL(L'a', str[0], L"Char ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_Ctor_CountChar)
{
    String str(10, 'a');
    WIN_ASSERT_EQUAL(10, str.GetLength(), L"CountChar ctor did not init to proper length (%d of 10).", str.GetLength());
}
END_TEST;

BEGIN_TEST(String_Ctor_WChar)
{
    String str(L'a');
    WIN_ASSERT_EQUAL(L'a', str[0], L"WChar ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_Ctor_CountWChar)
{
    String str(10, L'a');
    WIN_ASSERT_EQUAL(10, str.GetLength(), L"CountWChar ctor did not init to proper length (%d of 10).", str.GetLength());
}
END_TEST;

BEGIN_TEST(String_Ctor_Str)
{
    String str("test");
    WIN_ASSERT_EQUAL(str, L"test", L"Str ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_Ctor_WStr)
{
    String str(L"test");
    WIN_ASSERT_EQUAL(str, L"test", L"WStr ctor did not copy properly.");
}
END_TEST;

BEGIN_TEST(String_Ctor_SizeStr)
{
    String str(4, "testing");
    WIN_ASSERT_EQUAL(str, L"test", L"SizeStr ctor did not convert properly.");
}
END_TEST;

BEGIN_TEST(String_Ctor_SizeWStr)
{
    String str(4, L"testing");
    WIN_ASSERT_EQUAL(str, L"test", L"SizeWStr ctor did not copy properly.");
}
END_TEST;

BEGIN_TEST(String_Ctor_Copy)
{
    String str(L"test");
    String other(str);
    WIN_ASSERT_EQUAL(other, str, L"Copy ctor did not copy properly.");
}
END_TEST;

BEGIN_TEST(String_ToUuid)
{
    Uuid uuid, ouuid;
    ZeroMemory(&uuid, sizeof(Uuid));
    String str(uuid);
    WIN_ASSERT_TRUE(str.ToUuid(&ouuid), L"ToUuid failed.");
    WIN_ASSERT_EQUAL(uuid, ouuid, L"Uuids are not equal.");
}
END_TEST;

BEGIN_TEST(String_ToCharStr)
{
    String str(L"testing");

    int32_t size = str.GetLength();
    std::vector<char> buffer(size);

    WIN_ASSERT_EQUAL(str.GetLength(), str.ToCharStr(size, &buffer[0]), L"ToCharStr returnd bad length.");
    WIN_ASSERT_ZERO(strcmp("testing", &buffer[0]), L"Strings do not match.");
}
END_TEST;

BEGIN_TEST(String_ToString)
{
    String str(L"testing");
    std::wstring stdstr = str.ToString();

    WIN_ASSERT_ZERO(wcscmp(stdstr.c_str(), str.GetData()), L"ToString strings do not match.");
}
END_TEST;

BEGIN_TEST(String_Append_WChar)
{
    String str(L"tes");
    str.Append(L't');

    WIN_ASSERT_TRUE(str == L"test", L"WChar Append does not match.");
}
END_TEST;

BEGIN_TEST(String_Append_SizeWChar)
{
    String str(L"tes");
    str.Append(3, L't');

    WIN_ASSERT_TRUE(str == L"testtt", L"SizeWChar Append does not match.");
}
END_TEST;

BEGIN_TEST(String_Append_WStr)
{
    String str(L"tes");
    str.Append(L"ting");

    WIN_ASSERT_TRUE(str == L"testing", L"WStr Append does not match.");
}
END_TEST;

BEGIN_TEST(String_Append_SizeWStr)
{
    String str(L"tes");
    str.Append(4, L"tingting");

    WIN_ASSERT_TRUE(str == L"testing", L"SizeWStr Append does not match.");
}
END_TEST;

BEGIN_TEST(String_Append_String)
{
    String str(L"tes");
    String end(L"ting");
    str.Append(end);

    WIN_ASSERT_TRUE(str == L"testing", L"SizeWStr Append does not match.");
}
END_TEST;

