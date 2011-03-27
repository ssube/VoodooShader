
#include "VoodooGL.hpp"
#include "Frost_Adapter.hpp"

HWND gNwnWindow;

using namespace VoodooShader;

/**
 * Begins a draw operation and sets the expected vertex order.
 * 
 * @param mode Vertex order (GL_QUADS, etc)
 * 
 * @implements glBegin
 */
void GLAPIENTRY vglBegin(GLenum mode)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glBegin(%u)",
        mode
    );

    return glBegin(mode);
}

/**
 * Binds a texture to the given target for use in draw ops.
 * 
 * @param target The target to bind to (often GL_TEXTURE_2D).
 * @param texture The texture ID to be bound (must be a valid ID).
 * 
 * @note This function also binds material shaders as appropriate. If texture
 *       is linked to a Shader, the default technique will be retrieved and set
 *       as the active effect.
 *
 * @implements glBindTexture
 */
void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glBindTexture(%u, %u)",
        target, texture
    );

    /*
    find Technique from Materials[texture]
    if ( Technique is valid )
    {
        setup variables on Technique
        get first Pass from Technique
        bind Pass
    }
    */

    return glBindTexture(target, texture);
}

void GLAPIENTRY vglClear(GLbitfield mask)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glClear(%u)",
        mask
    );

    return glClear(mask);;
}

void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glDeleteTextures(%i, %p)",
        n, textures
    );

    return glDeleteTextures(n, textures);;
}

void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glDrawElements(%u, %i, %u, %p)",
        mode, count, type, indices
    );

    return glDrawElements(mode, count, type, indices);;
}

void GLAPIENTRY vglEnable(GLenum cap)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glEnable(%u)",
        cap
    );

    return glEnable(cap);
}

void GLAPIENTRY vglEnd()
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glEnd()"
    );

    return glEnd();
}

void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glFogfv(%u, %p)",
        pname, params
    );

    return glFogfv(pname, params);;
}

void GLAPIENTRY vglFogf(GLenum pname, GLfloat param)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glFogf(%u, %f)",
        pname, param
    );

    return glFogf(pname, param);;
}

const GLubyte * GLAPIENTRY vglGetString(GLenum name)
{
    const GLubyte * result = glGetString(name);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glGetString(%u) == %s",
        name, result
    );

    return result;
}

void GLAPIENTRY vglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glViewport(%i, %i, %i, %i)",
        x, y, width, height
    );

    return glViewport(x, y, width, height);;
}

HGLRC WINAPI vwglCreateContext(HDC hdc)
{
    HGLRC result = wglCreateContext(hdc);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "wglCreateContext(%p) == %p",
        hdc, result
    );

    gNwnWindow = WindowFromDC(hdc);
    char title[64];
    if ( GetWindowTextA(gNwnWindow, title, 64) > 0 )
    {
        strcat_s(title, " [ Voodoo Frost ]");
        SetWindowTextA(gNwnWindow, title);
    }

    VoodooFrost->SetDC(hdc);
    VoodooFrost->SetGLRC(result);
    
    return result;
}

BOOL WINAPI vwglDeleteContext(HGLRC hglrc)
{
    BOOL result = wglDeleteContext(hglrc);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "wglDeleteContext(%p) == %i",
        hglrc, result
    );

    return result;
}

PROC WINAPI vwglGetProcAddress(LPCSTR name)
{
    PROC result = wglGetProcAddress(name);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "wglGetProcAddress(%s) == %p",
        name, result
    );

    return result;
}

BOOL WINAPI vwglMakeCurrent(HDC hdc, HGLRC hglrc)
{
    BOOL result = wglMakeCurrent(hdc, hglrc);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "wglMakeCurrent(%p, %p) == %i",
        hdc, hglrc, result
    );

    return result;
}

BOOL WINAPI vwglSwapLayerBuffers(HDC hdc, UINT uint)
{
    if ( VoodooFrost && TestShader.get() )
    {
        VoodooFrost->DrawShader(TestShader);
    }

    BOOL result = wglSwapLayerBuffers(hdc, uint);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "wglSwapLayerBuffers(%p, %u) == %i",
        hdc, uint, result
    );

    return result;
}