
#ifndef VOODOO_NWS_MODULE_HPP
#define VOODOO_NWS_MODULE_HPP

#define VOODOO_IMPORT
#include "VoodooFramework.hpp"

// GLEW OpenGL extensions
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GL/gl.h"
#include "Cg/cgGL.h"

#include "EasyHook.h"

// Class prototypes
class HookManager;

extern VoodooShader::Core * VoodooCore;
extern VoodooShader::Adapter * VoodooFrost;
extern HookManager * VoodooHooker;

#endif /*VOODOO_NWS_MODULE_HPP*/
