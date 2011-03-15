
#include "VoodooGL.hpp"

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
    glViewport(x, y, width, height);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glViewport(%i, %i, %i, %i)",
        x, y, width, height
    );

    return;
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
    glClear(mask);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glClear(%u)",
        mask
    );

    return;
}

void GLAPIENTRY vglBegin(GLenum mode)
{
    glBegin(mode);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glBegin(%u)",
        mode
    );

    return;
}

void GLAPIENTRY vglEnd()
{
    glEnd();

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glEnd()"
    );

    return;
}

BOOL GLAPIENTRY vwglSwapLayerBuffers(HDC hdc, UINT uint)
{
    BOOL result = wglSwapLayerBuffers(hdc, uint);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "wglSwapLayerBuffers(%p, %u) == %i",
        hdc, uint
    );

    return result;
}

void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
    glBindTexture(target, texture);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glBindTexture(%u, %u)",
        target, texture
    );

    return;
}

void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{
    glDeleteTextures(n, textures);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glDeleteTextures(%i, %p)",
        n, textures
    );

    return;
}

void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    glDrawElements(mode, count, type, indices);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glDrawElements(%u, %i, %u, %p)",
        mode, count, type, indices
    );

    return;
}

void GLAPIENTRY vglEnable(GLenum cap)
{
    glEnable(cap);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glEnable(%u)",
        cap
    );
    
    return;
}

void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params)
{
    glFogfv(pname, params);

    VoodooCore->Log
    (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glFogfv(%u, %p)",
        pname, params
    );

    return;
}

void GLAPIENTRY vglFogf(GLenum pname, GLfloat param)
{
    glFogf(pname, param);

    VoodooCore->Log
   (
        LL_Debug,
        VOODOO_FROST_NAME,
        "glFogf(%u, %f)",
        pname, param
    );

    return;
}