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

#include "VoodooGL.hpp"

#include "Frost_Adapter.hpp"

HWND gNwnWindow = nullptr;
bool gSecondContext = false;
bool gFrostEnabled = false;
bool gDrawnGeometry = false;
float gFogMult = 99.0f;

using namespace VoodooShader;

/**
 * Begins a draw operation and sets the expected vertex order. 
 * 
 * @param mode Vertex order (GL_QUADS, etc).
 */
void GLAPIENTRY vglBegin(GLenum mode)
{
    gDrawnGeometry = true;

    /**
     * gThisFrame.DrawnGeometry = true if ( mode == GL_QUADS &&
     * !glIsEnabled(GL_LIGHTING) ) gThisFrame.DrawnShadows = true materials.unbind()
     */
    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glBegin(%u)", mode);

    return glBegin(mode);
}

/**
 * Binds a texture to the given target for use in draw ops. 
 * 
 * @param target The target to bind to (often GL_TEXTURE_2D).
 * @param texture The texture ID to be bound (must be a valid ID). 
 * 
 * @note This function also binds material shaders as appropriate. If texture is linked to a Shader, the default technique 
 *       will be retrieved and set as the active effect.
 */
void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glBindTexture(%u, %u)", target, texture);

    /**
     * Pseudocode for the material system:
     * 
     * if ( texture != 0 && texture != lastTexture )
     * { 
     *      lastTexture = texture;
     *      find Technique from Materials[texture] 
     *      
     *      if ( Technique is valid ) 
     *      { 
     *          setup variables on Technique 
     *          get first Pass from Technique 
     *          bind Pass 
     *          gBoundPass = Pass 
     *      } 
     *  } else if ( gBoundPass ) { 
     *      unbind gBoundPass 
     *      gBoundPass = nullptr 
     *  }
     */
    return glBindTexture(target, texture);
}

#define MASK_ALL (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT)
#define MASK_HUD (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)

void GLAPIENTRY vglClear(GLbitfield mask)
{
    if (mask == MASK_HUD && glIsEnabled(GL_LIGHT0))
    {
        GLint prevTexture = 0;

        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture);

        // If depth is enabled, cache that now
        glBindTexture(GL_TEXTURE_2D, gDepthTexture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, gViewWidth, gViewHeight, 0);

        // Fill the color buffers
        glBindTexture(GL_TEXTURE_2D, gThisFrame);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, gViewWidth, gViewHeight, 0);
        glBindTexture(GL_TEXTURE_2D, gLastShader);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, gViewWidth, gViewHeight, 0);
        glBindTexture(GL_TEXTURE_2D, gLastPass);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, gViewWidth, gViewHeight, 0);

        // Do shaders
        reinterpret_cast<VoodooShader::VoodooFrost::FrostAdapter*>(gpVoodooCore->GetAdapter())->DrawShader(gpTestShader);
    }

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glClear(%u)", mask);

    return glClear(mask);
}

void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{
    /**
     * for ( GLsizei i = 0; i < n; ++i ) gMaterials.remove(i);
     */
    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glDeleteTextures(%i, %p)", n, textures);

    return glDeleteTextures(n, textures);
}

void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    gDrawnGeometry = true;

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glDrawElements(%u, %i, %u, %p)", mode, count, type, indices);

    return glDrawElements(mode, count, type, indices);
}

void GLAPIENTRY vglEnable(GLenum cap)
{
    if (cap == GL_FOG && gFogMult > 0) return;

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glEnable(%u)", cap);

    return glEnable(cap);
}

void GLAPIENTRY vglEnd(void)
{
    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glEnd()");

    return glEnd();
}

void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params)
{
    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glFogfv(%u, %p)", pname, params);

    return glFogfv(pname, params);
}

void GLAPIENTRY vglFogf(GLenum pname, GLfloat param)
{
    if (pname == GL_FOG_START) param = param * gFogMult;
    if (pname == GL_FOG_END) param = param * gFogMult;

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glFogf(%u, %f)", pname, param);

    return glFogf(pname, param);
}

const GLubyte * GLAPIENTRY vglGetString (GLenum name)
{
    const GLubyte *result = glGetString(name);

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glGetString(%u) == %S", name, result);

    return result;
}

void GLAPIENTRY vglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    // VoodooFrost->SetViewport(x, y, width, height)
    gViewWidth = width;
    gViewHeight = height;

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"glViewport(%i, %i, %i, %i)", x, y, width, height);

    return glViewport(x, y, width, height);
}

HGLRC WINAPI vwglCreateContext(HDC hdc)
{
    HGLRC result = wglCreateContext(hdc);

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"wglCreateContext(%p) == %p", hdc, result);

    return result;
}

BOOL WINAPI vwglDeleteContext(HGLRC hglrc)
{
    if (gSecondContext && gNwnWindow)
    {
        //VoodooFrost->SetGLRC(nullptr);
        gSecondContext = false;
    }

    BOOL result = wglDeleteContext(hglrc);

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"wglDeleteContext(%p) == %i", hglrc, result);

    return result;
}

PROC WINAPI vwglGetProcAddress(LPCSTR name)
{
    PROC result = wglGetProcAddress(name);

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"wglGetProcAddress(%S) == %p", name, result);

    return result;
}

BOOL WINAPI vwglMakeCurrent(HDC hdc, HGLRC hglrc)
{
    BOOL result = wglMakeCurrent(hdc, hglrc);

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"wglMakeCurrent(%p, %p) == %i", hdc, hglrc, result);

    if (!gSecondContext)
    {
        gSecondContext = true;
    }
    else if (!gFrostEnabled)
    {
        GLenum glewStatus = glewInit();

        if (glewStatus != GLEW_OK)
        {
            gpVoodooCore->GetLogger()->Log(LL_ModError, VOODOO_FROST_NAME, L"Unable to initialize GLEW (code %u).", glewStatus);

            return result;
        }

        gNwnWindow = WindowFromDC(hdc);

        char title[64];

        if (GetWindowTextA(gNwnWindow, title, 64) > 0)
        {
            strcat_s(title, " [ Voodoo Frost ]");
            SetWindowTextA(gNwnWindow, title);
        }
        
        VoodooFrost::FrostAdapter * adapter = reinterpret_cast<VoodooShader::VoodooFrost::FrostAdapter*>(gpVoodooCore->GetAdapter());
        adapter->SetDC(hdc);
        adapter->SetGLRC(hglrc);

        gpVoodooCore->GetLogger()->Log(LL_ModInfo, VOODOO_FROST_NAME,
            L"OpenGL driver information:&lt;br /&gt;\nVendor: %S&lt;br /&gt;\nRenderer: %S&lt;br /&gt;\nVersion: %S",
            glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));

        GLint viewportInfo[4];

        glGetIntegerv(GL_VIEWPORT, viewportInfo);
        gViewWidth = viewportInfo[2];
        gViewHeight = viewportInfo[3];

        gFrostEnabled = true;
    }

    return result;
}

BOOL WINAPI vwglSwapLayerBuffers(HDC hdc, UINT uint)
{
    BOOL result = wglSwapLayerBuffers(hdc, uint);

    gpVoodooCore->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"wglSwapLayerBuffers(%p, %u) == %i", hdc, uint, result);

    // updateShaderVars()
    gDrawnGeometry = false;

    return result;
}
