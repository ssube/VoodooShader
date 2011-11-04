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

#pragma comment(lib, "cg.lib")

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
    wchar_t cwd[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, cwd);
    cwd[MAX_PATH] = 0;

    glutInit(nullptr, nullptr);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(320, 320);
    gGlutWindow = glutCreateWindow("VoodooTest_Core::OGLTest");

    gParams.Config = L"D:\\Code\\VoodooShader\\Tests\\Resources\\OpenGL.xmlconfig";
    gParams.GlobalRoot = gParams.LocalRoot = gParams.RunRoot = cwd;
    gParams.Loader = L"TestLoader.exe";
    gParams.Target = L"TestCore.exe";

    gpCore = CreateCore(&gParams);
    WIN_ASSERT_NOT_NULL(gpCore, L"OGLCore::Setup: gpCore is nullptr.");

    gCgContext = cgCreateContext();
    WIN_ASSERT_TRUE(cgIsContext(gCgContext), L"OGLCore::Setup: gCgContext is not valid.");

    WIN_ASSERT_TRUE(gpCore->SetCgContext(gCgContext), L"OGLCore::Setup: Unable to set core.");
}

TEARDOWN(OGLCore)
{
    gpCore->SetCgContext(nullptr);

    gpCore->Release();

    cgDestroyContext(gCgContext);

    glutDestroyWindow(gGlutWindow);
}

BEGIN_TESTF(Core_CreateShader, OGLCore)
{
    IFile * file = gpCore->GetFileSystem()->FindFile(L"test.cgfx");
    IShader* shader = gpCore->CreateShader(file, nullptr);
    WIN_ASSERT_NULL(shader, L"Core_CreateShader: Unable to create shader.");
}
END_TESTF;
