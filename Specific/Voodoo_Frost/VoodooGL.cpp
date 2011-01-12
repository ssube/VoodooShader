
#include "VoodooGL.hpp"

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
	VoodooCore->GetLog()->Log("Voodoo Frost: glEnd()\n");
#endif

	return;
}

BOOL GLAPIENTRY vwglSwapLayerBuffers(HDC hdc, UINT uint)
{
	BOOL result = wglSwapLayerBuffers(hdc, uint);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: wglSwapLayerBuffers(%d, %d) == %d\n")
		.With(hdc).With(uint).Done();
#endif

	return result;
}

void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
	glBindTexture(target, texture);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glBindTexture(%d, %d)\n")
		.With(target).With(texture).Done();
#endif

	return;
}

void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{
	glDeleteTextures(n, textures);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glDeleteTextures(%d, %d)\n")
		.With(n).With(textures).Done();
#endif

	return;
}

void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	glDrawElements(mode, count, type, indices);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glDrawElements(%d, %d, %d, %d)\n")
		.With(mode).With(count).With(type).With(indices).Done();
#endif

	return;
}

void GLAPIENTRY vglEnable(GLenum cap)
{
	glEnable(cap);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glEnable(%d)\n")
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

void GLAPIENTRY vglFogf(GLenum pname, GLfloat param)
{
	glFogf(pname, param);

#ifdef _DEBUG
	VoodooCore->GetLog()->Format("Voodoo Frost: glFogf(%d, %d)\n")
		.With(pname).With(param).Done();
#endif

	return;
}