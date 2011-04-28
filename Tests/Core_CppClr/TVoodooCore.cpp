#include "stdafx.h"

#include "VoodooFramework.hpp"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Core_CppClr
{
	[TestClass]
	public ref class TVoodooCore
	{
	private:
		TestContext^ testContextInstance;
        IVoodooCore * pCore;

	public: 
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		[ClassInitialize()]
		static void MyClassInitialize(TestContext^ testContext) 
        {
            CoInitialize(NULL);

            HRESULT hr = CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, (void**)&pCore);

            WIN_ASSERT_TRUE(SUCCEEDED(hr) && pCore != NULL, _T("IVoodooCore not created."));

            InitParams iParams;
            ZeroMemory(&iParams, sizeof(iParams));
            iParams.Config = SysAllocString(L"M:\\VoodooShader\\Tests\\Resources\\Init_Working.xmlconfig");
            iParams.Target = SysAllocString(L"M:\\VoodooShader\\TargetApp.exe");
            iParams.Loader = SysAllocString(L"M:\\VoodooShader\\LoaderApp.exe");
            iParams.GlobalRoot = iParams.RunRoot = iParams.LocalRoot = SysAllocString(L"M:\\VoodooShader\\");

            hr = pCore->Initialize(iParams);
            Microsoft::VisualStudio::TestTools::UnitTesting::Assert::IsTrue(SUCCEEDED(hr), L"Core init returned {0}.", hr);

            SysFreeString(iParams.Config);
        };
		//
		//Use ClassCleanup to run code after all tests in a class have run
		[ClassCleanup()]
		static void MyClassCleanup() 
        {
            pCore->Release();

            CoUninitialize();
        };
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 

		[TestMethod]
		void TestMethod1()
		{
			IVoodooParser * pParser;
            HRESULT hr = pCore->get_Parser(&pParser);

            Assert::IsTrue(SUCCEEDED(hr));
            Assert::IsNotNull(pParser);
		};
	};
}
