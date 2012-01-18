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

#include "VoodooGL.hpp"
// Voodoo Frost
#include "Frost_Adapter.hpp"
// Voodoo Core
#include "Format.hpp"
// WinAPI
#include <strsafe.h>

// VoodooGL globals
HWND gWindow = nullptr;
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
    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glBegin(%u)") << mode);

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
    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glBindTexture(%u, %u)") << target << texture);

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
     *  } 
     *  else if ( gBoundPass )
     *  {
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
        reinterpret_cast<VoodooShader::VoodooFrost::FrostAdapter*>(gpVoodooCore->GetAdapter())->DrawEffect(gpTestEffect);
    }

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glClear(%u)") << mask);

    return glClear(mask);
}

void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{
    /**
     * for ( GLsizei i = 0; i < n; ++i ) gMaterials.remove(i);
     */
    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glDeleteTextures(%i, %p)") << n << textures);

    return glDeleteTextures(n, textures);
}

void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    gDrawnGeometry = true;

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glDrawElements(%u, %i, %u, %p)") << mode << count << type << indices);

    return glDrawElements(mode, count, type, indices);
}

void GLAPIENTRY vglEnable(GLenum cap)
{
    if (cap == GL_FOG && gFogMult > 0) return;

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glEnable(%u)") << cap);

    return glEnable(cap);
}

void GLAPIENTRY vglEnd(void)
{
    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, VSTR("glEnd()"));

    return glEnd();
}

void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params)
{
    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glFogfv(%u, %p)") << pname << params);

    return glFogfv(pname, params);
}

void GLAPIENTRY vglFogf(GLenum pname, GLfloat param)
{
    if (pname == GL_FOG_START) param = param * gFogMult;
    if (pname == GL_FOG_END) param = param * gFogMult;

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glFogf(%u, %f)") << pname << param);

    return glFogf(pname, param);
}

const GLubyte * GLAPIENTRY vglGetString (GLenum name)
{
    const GLubyte *result = glGetString(name);

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glGetString(%u) == %S") << name << result);

    return result;
}

void GLAPIENTRY vglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    // VoodooFrost->SetViewport(x, y, width, height)
    gViewWidth = width;
    gViewHeight = height;

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("glViewport(%i, %i, %i, %i)") << x << y << width << height);

    return glViewport(x, y, width, height);
}

HGLRC WINAPI vwglCreateContext(HDC hdc)
{
    HGLRC result = wglCreateContext(hdc);

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("wglCreateContext(%p) == %p") << hdc << result);

    return result;
}

BOOL WINAPI vwglDeleteContext(HGLRC hglrc)
{
    if (gSecondContext && gWindow)
    {
        //VoodooFrost->SetGLRC(nullptr);
        gSecondContext = false;
    }

    BOOL result = wglDeleteContext(hglrc);

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("wglDeleteContext(%p) == %i") << hglrc << result);

    return result;
}

PROC WINAPI vwglGetProcAddress(LPCSTR name)
{
    PROC result = wglGetProcAddress(name);

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("wglGetProcAddress(%S) == %p") << name << result);

    return result;
}

BOOL WINAPI vwglMakeCurrent(HDC hdc, HGLRC hglrc)
{
    BOOL result = wglMakeCurrent(hdc, hglrc);

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("wglMakeCurrent(%p, %p) == %i") << hdc << hglrc << result);

    if (!gSecondContext)
    {
        gSecondContext = true;
    }
    else if (!gFrostEnabled)
    {
        GLenum glewStatus = glewInit();

        if (glewStatus != GLEW_OK)
        {
            gpVoodooCore->GetLogger()->LogMessage(VSLog_ModError, VOODOO_FROST_NAME, Format("Unable to initialize GLEW (code %u).") << glewStatus);

            return result;
        }

        gWindow = WindowFromDC(hdc);

        TCHAR title[255];

        if (GetWindowText(gWindow, title, 255) > 0)
        {
            StringCchCat(title, 255, VSTR(" [ Voodoo Frost ]"));
            SetWindowText(gWindow, title);
        }

        VoodooFrost::FrostAdapter * adapter = nullptr;
        if (gpVoodooCore->GetAdapter()->QueryInterface(VoodooFrost::CLSID_FrostAdapter, (const IObject **)&adapter) && adapter)
        {
            adapter->SetDC(hdc);
            adapter->SetGLRC(hglrc);
        }

        gpVoodooCore->GetLogger()->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, L"OpenGL Driver Information:");
        gpVoodooCore->GetLogger()->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, Format("Vendor: %S") << glGetString(GL_VENDOR));
        gpVoodooCore->GetLogger()->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, Format("Renderer: %S") << glGetString(GL_RENDERER));
        gpVoodooCore->GetLogger()->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, Format("Version: %S") << glGetString(GL_VERSION));

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

    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("wglSwapLayerBuffers(%p, %u) == %i") << hdc << uint << result);

    // updateShaderVars()
    gDrawnGeometry = false;

    return result;
}
