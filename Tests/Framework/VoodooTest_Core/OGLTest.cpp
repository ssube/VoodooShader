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

#pragma warning(push,3)
#include <windows.h>
#pragma warning(pop)

#pragma warning(push,3)
#include <gl/gl.h>
#pragma warning(pop)
#pragma warning(push,3)
#include <gl/glut.h>
#pragma warning(pop)
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

#define VOODOO_STATIC_IMPORT
#include "VoodooFramework.hpp"
using namespace VoodooShader;

namespace
{
    ICore *gpCore;
    int gGlutWindow;
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
    gpCore->Init(L"D:\\Code\\VoodooShader\\Tests\\Resources\\OpenGL.xmlconfig");
}

TEARDOWN(OGLCore)
{
    gpCore->Release();

    glutDestroyWindow(gGlutWindow);
}

BEGIN_TESTF(Core_CreateShader, OGLCore)
{
    IFile * file = gpCore->GetFileSystem()->GetFile(L"test.fx");
    IEffect * effect = gpCore->CreateEffect(file);
    WIN_ASSERT_NULL(effect, L"Core_CreateEffect: Unable to create effect.");
}
END_TESTF;
