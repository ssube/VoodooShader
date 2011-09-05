// Voodoo Shader Framework
// Unit Tests:
//    Voodoo/Core:VoodooShader::Core
//

#include "WinUnit.h"

#include "VoodooFramework.hpp"
#pragma comment(lib, "Voodoo_Core.lib")
using namespace VoodooShader;

namespace
{
    Core * gpCore;
    InitParams gParams;
}

FIXTURE(DefaultCore);

SETUP(DefaultCore)
{
    char cwd[MAX_PATH+1];
    GetCurrentDirectory(MAX_PATH, cwd);
    cwd[MAX_PATH] = 0;

    gParams.Config = "D:\\Code\\VoodooShader\\Tests\\Resources\\default.xmlconfig";
    gParams.GlobalRoot = gParams.LocalRoot = gParams.RunRoot = cwd;
    gParams.Loader = "TestLoader.exe";
    gParams.Target = "TestCore.exe";

    gpCore = CreateCore(&gParams);
    WIN_ASSERT_NOT_NULL(gpCore, "DefaultCore::Setup: gpCore is null.");
}

TEARDOWN(DefaultCore)
{
    DestroyCore(gpCore);
}

BEGIN_TESTF(Core_GetParser, DefaultCore)
{
    ParserRef parser = gpCore->GetParser();
    WIN_ASSERT_NOT_NULL(parser.get(), "Core_GetParser: parser is null.");
}
END_TESTF

BEGIN_TESTF(Core_GetHookManager, DefaultCore)
{
    IHookManagerRef hook = gpCore->GetHookManager();
    WIN_ASSERT_NOT_NULL(hook.get(), "Core_GetHookManager: hook manager is null.");
}
END_TESTF

BEGIN_TESTF(Core_GetFileSystem, DefaultCore)
{
    IFileSystemRef filesystem = gpCore->GetFileSystem();
    WIN_ASSERT_NOT_NULL(filesystem.get(), "Core_GetFileSystem: filesystem is null.");
}
END_TESTF

BEGIN_TESTF(Core_GetAdapter, DefaultCore)
{
    IAdapterRef adapter = gpCore->GetAdapter();
    WIN_ASSERT_NOT_NULL(adapter.get(), "Core_GetAdapter: adapter is null.");
}
END_TESTF

BEGIN_TESTF(Core_GetLogger, DefaultCore)
{
    ILoggerRef Logger = gpCore->GetLogger();
    WIN_ASSERT_NOT_NULL(Logger.get(), "Core_GetLogger: logger is null.");
}
END_TESTF

BEGIN_TESTF(Core_GetConfig, DefaultCore)
{
    void * pConfig = gpCore->GetConfig();
    WIN_ASSERT_NOT_NULL(pConfig, "Core_GetConfig: pConfig is null.");
}
END_TESTF

// Test covers:
//     Core::GetCgContext
//     Core::SetCgContext
BEGIN_TESTF(Core_CgContext, DefaultCore)
{
    CGcontext inCg = (CGcontext*)0xFFFFFFFF;
    WIN_ASSERT_TRUE(gpCore->SetCgContext(inCg), "Core_CgContext: SetCgContext(%p) returned false.", inCg);

    CGcontext pCg = gpCore->GetCgContext();
    WIN_ASSERT_EQUAL(inCg, pCg, "Core_CgContext: inCg != pCg.");

    inCg = (CGcontext*)0x88888888;
    WIN_ASSERT_FALSE(gpCore->SetCgContext(inCg), "Core_CgContext: SetCgContext(%p) allowed overwriting.", inCg);
    
    inCg = nullptr; 
    WIN_ASSERT_TRUE(gpCore->SetCgContext(inCg), "Core_CgContext: SetCgContext(%p) returned false.", inCg);

    pCg = gpCore->GetCgContext();
    WIN_ASSERT_EQUAL(inCg, pCg, "Core_CgContext: inCg != pCg.");
}
END_TESTF


