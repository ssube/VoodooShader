/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */

/*
 * This file is part of the Voodoo Shader unit test suite.
 * 
 * These tests focus on core functionality.
 */

#include <cfixcc.h>

#define VOODOO_NO_PUGIXML
#define VOODOO_STATIC_IMPORT
#include "VoodooFramework.hpp"
using namespace VoodooShader;

class CoreInnerTests : public cfixcc::TestFixture
{
public:
    ICore * pCore;

    virtual void Before()
    {
        pCore = CreateCore(VOODOO_SDK_VERSION);
        pCore->AddRef();
        pCore->Init(VSTR("D:\\Code\\VoodooShader\\Tests\\Resources\\default.xmlconfig"));
        CFIXCC_ASSERT_NOT_EQUALS_MESSAGE(pCore, nullptr, L"Core is null before test.");
    }

    virtual void After()
    {
        CFIXCC_ASSERT_NOT_EQUALS_MESSAGE(pCore, nullptr, L"Core is null after test.")
        pCore->Reset();
        pCore->Release();
    }

    void GetParser()
    {
        IParser * pParser = pCore->GetParser();
        CFIXCC_ASSERT_NOT_EQUALS(pParser, nullptr);
    }

    void GetHookManager()
    {
        IHookManager * pHookManager = pCore->GetHookManager();
        CFIXCC_ASSERT_NOT_EQUALS(pHookManager, nullptr);
    }

    void GetFileSystem()
    {
        IFileSystem * pFileSystem = pCore->GetFileSystem();
        CFIXCC_ASSERT_NOT_EQUALS(pFileSystem, nullptr);
    }

    void GetLogger()
    {
        ILogger * pLogger = pCore->GetLogger();
        CFIXCC_ASSERT_NOT_EQUALS(pLogger, nullptr);
    }

    void GetConfig()
    {
        XmlDocument pConfig = pCore->GetConfig();
        CFIXCC_ASSERT_NOT_EQUALS(pConfig, nullptr);
    }
}

CFIXCC_BEGIN_CLASS(CoreInnerTests)
    CFIXCC_METHOD(GetParser)
    CFIXCC_METHOD(GetHookManager)
    CFIXCC_METHOD(GetFileSystem)
    CFIXCC_METHOD(GetLogger)
    CFIXCC_METHOD(GetConfig)
CFIXCC_END_CLASS()

