
#include "VoodooGL.hpp"

const GLubyte * GLAPIENTRY vglGetString(GLenum name)
{
    const GLubyte * result = glGetString(name);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glGetString(%d) == %d\n",
        name, result);
#endif

    return result;
}

void GLAPIENTRY vglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewport(x, y, width, height);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glViewport(%d, %d, %d, %d)\n",
        x, y, width, height);
#endif

    return;
}

HGLRC vwglCreateContext(HDC hdc)
{
    HGLRC result = wglCreateContext(hdc);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: wglCreateContext(%d) == %d\n",
        hdc, result);
#endif
    
    return result;
}

BOOL vwglDeleteContext(HGLRC hglrc)
{
    BOOL result = wglDeleteContext(hglrc);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: wglDeleteContext(%d) == %d\n",
        hglrc, result);
#endif

    return result;
}

PROC vwglGetProcAddress(LPCSTR name)
{
    PROC result = wglGetProcAddress(name);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: wglGetProcAddress(%s) == %d\n",
        name, result);
#endif

    return result;
}

BOOL vwglMakeCurrent(HDC hdc, HGLRC hglrc)
{
    BOOL result = wglMakeCurrent(hdc, hglrc);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: wglMakeCurrent(%d, %d) == %d\n",
        hdc, hglrc, result);
#endif

    return result;
}

void GLAPIENTRY vglClear(GLbitfield mask)
{
    glClear(mask);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glClear(%d)\n",
        mask);
#endif

    return;
}

void GLAPIENTRY vglBegin(GLenum mode)
{
    glBegin(mode);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glBegin(%d)\n",
        mode);
#endif

    return;
}

void GLAPIENTRY vglEnd()
{
    glEnd();

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glEnd()\n");
#endif

    return;
}

BOOL GLAPIENTRY vwglSwapLayerBuffers(HDC hdc, UINT uint)
{
    BOOL result = wglSwapLayerBuffers(hdc, uint);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: wglSwapLayerBuffers(%d, %d) == %d\n",
        hdc, uint);
#endif

    return result;
}

void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
    glBindTexture(target, texture);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glBindTexture(%d, %d)\n",
        target, texture);
#endif

    return;
}

void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{
    glDeleteTextures(n, textures);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glDeleteTextures(%d, %d)\n",
        n, textures);
#endif

    return;
}

void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    glDrawElements(mode, count, type, indices);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glDrawElements(%d, %d, %d, %d)\n",
        mode, count, type, indices);
#endif

    return;
}

void GLAPIENTRY vglEnable(GLenum cap)
{
    glEnable(cap);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glEnable(%d)\n",
        cap);
#endif
    
    return;
}

void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params)
{
    glFogfv(pname, params);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glFogfv(%d, %d)\n",
        pname, params);
#endif

    return;
}

void GLAPIENTRY vglFogf(GLenum pname, GLfloat param)
{
    glFogf(pname, param);

#ifdef _DEBUG
    VoodooCore->Log("Voodoo Frost: glFogf(%d, %d)\n",
        pname, param);
#endif

    return;
}