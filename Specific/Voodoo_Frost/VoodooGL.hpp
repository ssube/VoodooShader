/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (c) 2010-2011 by Sean Sube
*
*
* This program is free software; you can redistribute it and/or modify it under the terms of the 
* GNU General Public License as published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#include "Frost_Adapter.hpp"
#include "Frost_Module.hpp"

const GLubyte * GLAPIENTRY vglGetString(GLenum name)
{
	const GLubyte * result = glGetString(name);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glGetString(%d) == %d\n")
		.With(name).With(result).Done();
#endif

	return result;
}

void GLAPIENTRY vglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glViewport(%d, %d, %d, %d)\n")
		.With(x).With(y).With(width).With(height).Done();
#endif

	return;
}

HGLRC vwglCreateContext(HDC hdc)
{
	HGLRC result = wglCreateContext(hdc);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: wglCreateContext(%d) == %d\n")
		.With(hdc).With(result).Done();
#endif
	
	return result;
}

BOOL vwglDeleteContext(HGLRC hglrc)
{
	BOOL result = wglDeleteContext(hglrc);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: wglDeleteContext(%d) == %d\n")
		.With(hglrc).With(result).Done();
#endif

	return result;
}

PROC vwglGetProcAddress(LPCSTR name)
{
	PROC result = wglGetProcAddress(name);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: wglGetProcAddress(%s) == %d\n")
		.With(name).With(result).Done();
#endif

	return result;
}

BOOL vwglMakeCurrent(HDC hdc, HGLRC hglrc)
{
	BOOL result = wglMakeCurrent(hdc, hglrc);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: wglMakeCurrent(%d, %d) == %d\n")
		.With(hdc).With(hglrc).With(result).Done();
#endif

	return result;
}

void GLAPIENTRY vglClear(GLbitfield mask)
{
	glClear(mask);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glClear(%d)\n")
		.With(mask).Done();
#endif

	return;
}

void GLAPIENTRY vglBegin(GLenum mode)
{
	glBegin(mode);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glBegin(%d)\n")
		.With(mode).Done();
#endif

	return;
}

void GLAPIENTRY vglEnd()
{
	glEnd();

#ifdef _DEBUG
	VoodooCore->GetLog->Log("Voodoo Frost: glEnd()\n");
#endif

	return;
}

BOOL GLAPIENTRY vwglSwapLayerBuffers(HDC hdc, UINT uint)
{
	BOOL result = wglSwapLayerBuffers(hdc, uint);

#ifdef _DEBUG
	VoodooCore->GetLog()->Log("Voodoo Frost: wglSwapLayerBuffers(%d, %d) == %d\n")
		.With(hdc).With(uint).Done();
#endif

	return result;
}

void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
	glBindTexture(target, texture);

#ifdef _DEBUG
	VoodooCore->GetLog()->Log("Voodoo Frost: glBindTexture(%d, %d)\n")
		.With(target).With(texture).Done();
#endif

	return;
}

void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{
	glDeleteTextures(n, textures);

#ifdef _DEBUG
	VoodooCore->GetLog()->Log("Voodoo Frost: glDeleteTextures(%d, %d)\n")
		.With(n).With(textures).Done();
#endif

	return;
}

void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	glDrawElements(mode, count, type, indices);

#ifdef _DEBUG
	VoodooCore->GetLog()->Log("Voodoo Frost: glDrawElements(%d, %d, %d, %d)\n")
		.With(mode).With(count).With(type).With(indices).Done();
#endif

	return;
}

void GLAPIENTRY vglEnable(GLenum cap)
{
	glEnable(cap);

#ifdef _DEBUG
	VoodooCore->GetLog()->Log("Voodoo Frost: glEnable(%d)\n")
		.With(cap).Done();
#endif
	
	return;
}

void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params)
{
	glFogfv(pname, params);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glFogfv(%d, %d)\n")
		.With(pname).With(params).Done();
#endif

	return;
}

void GLAPIENTRY glFogf(GLenum pname, GLfloat param)
{
	glFogf(pname, params);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glFogf(%d, %d)\n")
		.With(name).With(param).Done();
#endif

	return;
}