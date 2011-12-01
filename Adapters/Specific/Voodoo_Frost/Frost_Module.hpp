/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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
#pragma once

#define VOODOO_IMPORT
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

// GLEW OpenGL extensions
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GL/gl.h"
#include "Cg/cgGL.h"

#include "Frost_Version.hpp"

namespace VoodooShader
{
    namespace Frost
    {
        class FrostAdapter;
        class FrostTexture;

        const Version * API_ModuleVersion(void);
        uint32_t API_ClassCount(void);
        const wchar_t * API_ClassInfo(_In_ uint32_t index, Uuid & clsid);
        IObject * API_ClassCreate(_In_ uint32_t index, _In_ ICore * pCore);
    }
}

extern VoodooShader::ICore * gpVoodooCore;
extern VoodooShader::IShader * gpTestShader;

extern GLint gDepthTexture;
extern GLint gThisFrame;
extern GLint gLastFrame;
extern GLint gLastShader;
extern GLint gLastPass;
extern GLsizei gViewWidth;
extern GLsizei gViewHeight;
