#include "stdafx.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Core_Basic_Cpp
{
	[TestClass]
	public ref class Parser
	{
	private:
		TestContext^ testContextInstance;

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
            HRESULT hr = CoInitialize(NULL);
            if ( FAILED(hr) )
            {
                DebugBreak();
            }
        };
		//
		//Use ClassCleanup to run code after all tests in a class have run
		[ClassCleanup()]
		static void MyClassCleanup() 
        {
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
            IVoodooCore * pCore = NULL;
            if ( FAILED(CoCreateInstance(CLSID_VoodooCore, NULL, CLSCTX_INPROC_SERVER, IID_IVoodooCore, &pCore)) || pCore == NULL )
            {
                return;
            }

            VARIANT vconfig;
            V_VT(&vconfig) = V_BSTR;
            V_BSTR(&vconfig) = L"m:\\voodoocinfig\\voodooconfig.xmlconfig";
            if ( FAILED(pCore->Initialize(cvonfig)) )
            {
                return;
            }

            IVoodooParser pParser = NULL;
            pCore->get_Parser(&pParser);

            pParser->AddVariable(L"language", L"C++");

            CComBSTR pInput = L"Today wE USE $(language).";
            CComBSTR pOutput;
            pParser->Parse(pInput, Lowercase, &pOutput);

//            pOutput.WriteToStream()
		};
	};
}
