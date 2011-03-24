
#include "VoodooGL.hpp"

using namespace VoodooShader;

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

HGLRC vwglCreateContext(HDC hdc)
{
    HGLRC result = wglCreateContext(hdc);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "wglCreateContext(%p) == %p",
        hdc, result
    );
    
    return result;
}

BOOL vwglDeleteContext(HGLRC hglrc)
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

PROC vwglGetProcAddress(LPCSTR name)
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

BOOL vwglMakeCurrent(HDC hdc, HGLRC hglrc)
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

BOOL GLAPIENTRY vwglSwapLayerBuffers(HDC hdc, UINT uint)
{
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

void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glBindTexture(%u, %u)",
        target, texture
    );

    return glBindTexture(target, texture);
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
    
    return glEnable(cap);;
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