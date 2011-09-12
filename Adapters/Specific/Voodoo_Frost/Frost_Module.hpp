

#ifndef VOODOO_NWS_MODULE_HPP
#define VOODOO_NWS_MODULE_HPP

#define VOODOO_IMPORT
#include "VoodooFramework.hpp"

// GLEW OpenGL extensions
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GL/gl.h"
#include "Cg/cgGL.h"

#include "Frost_Version.hpp"

namespace VoodooShader
{
 namespace Frost
{
 class Adapter;

 Version API_ModuleVersion(void);

 int API_ClassCount(void);

 const char *API_ClassInfo(_In_ int number);

 IObject *API_ClassCreate(_In_ int number, _In_ Core *core);
}
}
extern VoodooShader::Core * VoodooCore;
extern VoodooShader::ILoggerRef VoodooLogger;
extern VoodooShader::Frost::Adapter * VoodooFrost;
extern VoodooShader::ShaderRef TestShader;

extern GLint gDepthTexture;
extern GLint gThisFrame;
extern GLint gLastFrame;
extern GLint gLastShader;
extern GLint gLastPass;
extern GLsizei gViewWidth;
extern GLsizei gViewHeight;
#endif /* VOODOO_NWS_MODULE_HPP */
