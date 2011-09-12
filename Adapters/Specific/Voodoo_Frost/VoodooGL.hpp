/**
 * \ This file is part of the Voodoo Shader Framework, a comprehensive shader
 * support library. Copyright (c) 2010-2011 by Sean Sube This program is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version. This
 * program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details. You
 * should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth
 * Floor, Boston, MA 02110-1301 US Support and more information may be found at
 * http://www.voodooshader.com, or by contacting the developer at
 * peachykeen@voodooshader.com \ 
 */
#include "Frost_Module.hpp"
#include <GL/glew.h>

void GLAPIENTRY vglBegin(GLenum mode);
void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture);
void GLAPIENTRY vglClear(GLbitfield mask);
void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures);
void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void GLAPIENTRY vglEnable(GLenum cap);
void GLAPIENTRY vglEnd(void);
void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params);
void GLAPIENTRY vglFogf(GLenum pname, GLfloat param);
const GLubyte * GLAPIENTRY vglGetString(GLenum name);
void GLAPIENTRY vglViewport(GLint x, GLint y, GLsizei width, GLsizei height);

HGLRC WINAPI vwglCreateContext(HDC hdc);
BOOL WINAPI vwglDeleteContext(HGLRC hglrc);
BOOL WINAPI vwglMakeCurrent(HDC hdc, HGLRC hglrc);
PROC WINAPI vwglGetProcAddress(LPCSTR name);
BOOL WINAPI vwglSwapLayerBuffers(HDC hdc, UINT uint);
