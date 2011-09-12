

#include "VoodooGL.hpp"
#include "Frost_Adapter.hpp"

HWND gNwnWindow = NULL;
bool gSecondContext = false;
bool gFrostEnabled = false;
bool gDrawnGeometry = false;
float gFogMult = 99.0f;

using namespace VoodooShader;

/**
 =======================================================================================================================
 * Begins a draw operation and sets the expected vertex order. @param mode Vertex order (GL_QUADS, etc) @implements
 * glBegin
 =======================================================================================================================
 */
void GLAPIENTRY vglBegin(GLenum mode)
{
 gDrawnGeometry = true;

 /**
  * gThisFrame.DrawnGeometry = true if ( mode == GL_QUADS &&
  * !glIsEnabled(GL_LIGHTING) ) gThisFrame.DrawnShadows = true materials.unbind()
  */
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glBegin(%u)", mode);

 return glBegin(mode);
}

/**
 =======================================================================================================================
 * Binds a texture to the given target for use in draw ops. @param target The target to bind to (often GL_TEXTURE_2D).
 * @param texture The texture ID to be bound (must be a valid ID). @note This function also binds material shaders as
 * appropriate. If texture is linked to a Shader, the default technique will be retrieved and set as the active effect.
 * @implements glBindTexture
 =======================================================================================================================
 */
void GLAPIENTRY vglBindTexture(GLenum target, GLuint texture)
{
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glBindTexture(%u, %u)", target, texture);

 /**
  * if ( texture != 0 && texture != lastTexture ) { lastTexture = texture;
  * find Technique from Materials[texture] if ( Technique is valid ) { setup
  * variables on Technique get first Pass from Technique bind Pass gBoundPass =
  * Pass } } else if ( gBoundPass ) { unbind gBoundPass gBoundPass = null }
  */
 return glBindTexture(target, texture);
}

#define MASK_ALL (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT)
#define MASK_HUD (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglClear(GLbitfield mask)
{
 if (VoodooFrost && mask == MASK_HUD && glIsEnabled(GL_LIGHT0))
 {

  /*~~~~~~~~~~~~~~~~~~~~*/
  /*~~~~~~~~~~~~~~~~~~~~*/
  GLint prevTexture = 0;
  /*~~~~~~~~~~~~~~~~~~~~*/

  /*~~~~~~~~~~~~~~~~~~~~*/
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
  VoodooFrost->DrawShader(TestShader);
 }

 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glClear(%u)", mask);

 return glClear(mask);
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglDeleteTextures(GLsizei n, const GLuint *textures)
{

 /**
  * for ( GLsizei i = 0;
  * i < n;
  * ++i ) gMaterials.remove(i);
  */
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glDeleteTextures(%i, %p)", n, textures);

 return glDeleteTextures(n, textures);
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
 gDrawnGeometry = true;

 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glDrawElements(%u, %i, %u, %p)", mode, count, type, indices);

 return glDrawElements(mode, count, type, indices);
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglEnable(GLenum cap)
{
 if (cap == GL_FOG && gFogMult > 0) return;

 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glEnable(%u)", cap);

 return glEnable(cap);
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglEnd(void)
{
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glEnd()");

 return glEnd();
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglFogfv(GLenum pname, const GLfloat *params)
{
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glFogfv(%u, %p)", pname, params);

 return glFogfv(pname, params);
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglFogf(GLenum pname, GLfloat param)
{
 if (pname == GL_FOG_START) param = param * gFogMult;
 if (pname == GL_FOG_END) param = param * gFogMult;

 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glFogf(%u, %f)", pname, param);

 return glFogf(pname, param);
}

const GLubyte * GLAPIENTRY vglGetString (GLenum name)
{
 const GLubyte *result = glGetString(name);

 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glGetString(%u) == %s", name, result);

 return result;
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
void GLAPIENTRY vglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{

 // VoodooFrost->SetViewport(x, y, width, height)
 gViewWidth = width;
 gViewHeight = height;

 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "glViewport(%i, %i, %i, %i)", x, y, width, height);

 return glViewport(x, y, width, height);
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
HGLRC WINAPI vwglCreateContext(HDC hdc)
{

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 HGLRC result = wglCreateContext(hdc);
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "wglCreateContext(%p) == %p", hdc, result);

 return result;
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
BOOL WINAPI vwglDeleteContext(HGLRC hglrc)
{
 if (gSecondContext && gNwnWindow)
 {
  VoodooFrost->SetGLRC(NULL);
  gSecondContext = false;
 }

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 BOOL result = wglDeleteContext(hglrc);
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "wglDeleteContext(%p) == %i", hglrc, result);

 return result;
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
PROC WINAPI vwglGetProcAddress(LPCSTR name)
{

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 PROC result = wglGetProcAddress(name);
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "wglGetProcAddress(%s) == %p", name, result);

 return result;
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
BOOL WINAPI vwglMakeCurrent(HDC hdc, HGLRC hglrc)
{

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 BOOL result = wglMakeCurrent(hdc, hglrc);
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "wglMakeCurrent(%p, %p) == %i", hdc, hglrc, result);

 if (!gSecondContext)
 {
  gSecondContext = true;
 }
 else if (!gFrostEnabled)
 {

  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  GLenum glewStatus = glewInit();
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  if (glewStatus != GLEW_OK)
  {
   VoodooLogger->Log(LL_Error, VOODOO_FROST_NAME, "Unable to initialize GLEW (code %u).", glewStatus);

   return result;
  }

  gNwnWindow = WindowFromDC(hdc);

  /*~~~~~~~~~~~~~~*/
  /*~~~~~~~~~~~~~~*/
  char title[64];
  /*~~~~~~~~~~~~~~*/

  /*~~~~~~~~~~~~~~*/
  if (GetWindowTextA(gNwnWindow, title, 64) > 0)
  {
   strcat_s(title, " [ Voodoo Frost ]");
   SetWindowTextA(gNwnWindow, title);
  }

  VoodooFrost->SetDC(hdc);
  VoodooFrost->SetGLRC(hglrc);

  VoodooLogger->Log
   (
    LL_Info,
    VOODOO_FROST_NAME,
    "OpenGL driver information:&lt;br /&gt;\nVendor: %s&lt;br /&gt;\nRenderer: %s&lt;br /&gt;\nVersion: %s",
    glGetString(GL_VENDOR),
    glGetString(GL_RENDERER),
    glGetString(GL_VERSION)
   );

  /*~~~~~~~~~~~~~~~~~~~~*/
  /*~~~~~~~~~~~~~~~~~~~~*/
  GLint viewportInfo[4];
  /*~~~~~~~~~~~~~~~~~~~~*/

  /*~~~~~~~~~~~~~~~~~~~~*/
  glGetIntegerv(GL_VIEWPORT, viewportInfo);
  gViewWidth = viewportInfo[2];
  gViewHeight = viewportInfo[3];

  gFrostEnabled = true;
 }

 return result;
}

/**
 =======================================================================================================================
 *
 =======================================================================================================================
 */
BOOL WINAPI vwglSwapLayerBuffers(HDC hdc, UINT uint)
{

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 BOOL result = wglSwapLayerBuffers(hdc, uint);
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 VoodooLogger->Log(LL_Debug, VOODOO_FROST_NAME, "wglSwapLayerBuffers(%p, %u) == %i", hdc, uint, result);

 // updateShaderVars()
 gDrawnGeometry = false;

 return result;
}
