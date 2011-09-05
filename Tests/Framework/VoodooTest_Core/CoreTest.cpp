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

BEGIN_TESTF(Core_GetConfig, DefaultCore)
{
    void * pConfig = gpCore->GetConfig();
    WIN_ASSERT_NOT_NULL(pConfig, "Core_GetConfig: pConfig is null.");
}
END_TESTF
