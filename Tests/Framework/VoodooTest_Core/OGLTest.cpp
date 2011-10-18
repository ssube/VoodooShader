// Voodoo Shader Framework Unit Tests: Voodoo/ICore:VoodooShader::ICore
#include "WinUnit.h"

#include "VoodooFramework.hpp"
#pragma comment(lib, "Voodoo_Core.lib")
using namespace VoodooShader;

#include <windows.h>

#include <gl/gl.h>
#include <gl/glut.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

namespace
{
    ICore *gpCore;
    InitParams gParams;
    int gGlutWindow;
    CGcontext gCgContext;
} 

FIXTURE(OGLCore);

SETUP(OGLCore)
{
    char cwd[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, cwd);
    cwd[MAX_PATH] = 0;

    glutInit(nullptr, nullptr);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(320, 320);
    gGlutWindow = glutCreateWindow("VoodooTest_Core::OGLTest");

    gParams.Config = "D:\\Code\\VoodooShader\\Tests\\Resources\\ogl.xmlconfig";
    gParams.GlobalRoot = gParams.LocalRoot = gParams.RunRoot = cwd;
    gParams.Loader = "TestLoader.exe";
    gParams.Target = "TestCore.exe";

    gpCore = CreateCore(&gParams);
    WIN_ASSERT_NOT_nullptr(gpCore, "OGLCore::Setup: gpCore is nullptr.");

    gCgContext = cgCreateContext();
    WIN_ASSERT_TRUE(cgIsContext(gCgContext), "OGLCore::Setup: gCgContext is not valid.");

    WIN_ASSERT_TRUE(gpCore->SetCgContext(gCgContext), "OGLCore::Setup: Unable to set core.");
}

TEARDOWN(OGLCore)
{
    gpCore->SetCgContext(nullptr);

    delete gpCore;

    cgDestroyContext(gCgContext);

    glutDestroyWindow(gGlutWindow);
}

BEGIN_TESTF(Core_CreateShader, OGLCore)
{
    IShader* shader = gpCore->CreateShader("test.cgfx", nullptr);
    WIN_ASSERT_NOT_nullptr(shader.get(), "Core_CreateShader: Unable to create shader.");
}
END_TESTF;
