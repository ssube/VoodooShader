#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Core
{		
	TEST_CLASS(UnitTest1)
	{
	public:
	    TEST_METHOD(TestMethod1)
	    {
                Assert::IsNull((void*)nullptr);
	    }

            TEST_METHOD(TestMethod2)
            {
                Assert::IsTrue(true);
            }
	};
}
