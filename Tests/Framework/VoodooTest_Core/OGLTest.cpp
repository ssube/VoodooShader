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
 * These tests focus on OpenGL functionality.
 */

#include "WinUnit.h"

#include <windows.h>

#include <gl/gl.h>
#include <gl/glut.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

#pragma comment(lib, "cg.lib")

#define VOODOO_STATIC_LINK
#include "VoodooFramework.hpp"
using namespace VoodooShader;

namespace
{
    ICore *gpCore;
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

    gpCore = CreateCore(0);
    WIN_ASSERT_NOT_NULL(gpCore, L"OGLCore::Setup: gpCore is nullptr.");
    gpCore->AddRef();
    gpCore->Initialize(L"D:\\Code\\VoodooShader\\Tests\\Resources\\OpenGL.xmlconfig");

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
    IFile * file = gpCore->GetFileSystem()->GetFile(L"test.cgfx");
    IShader* shader = gpCore->CreateShader(file, nullptr);
    WIN_ASSERT_NULL(shader, L"Core_CreateShader: Unable to create shader.");
}
END_TESTF;
