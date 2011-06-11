
#ifndef VOODOO_NWS_MODULE_HPP
#define VOODOO_NWS_MODULE_HPP

// GLEW OpenGL extensions
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GL/gl.h"
#include "Cg/cgGL.h"

#include "VoodooFramework.hpp"
#include "VoodooSupport.hpp"

#include "Frost_Version.hpp"

namespace VoodooShader
{
    namespace Frost
    {
        class Adapter;
    }
}

extern LPVOODOOCORE VoodooCore;
extern LPVOODOOLOGGER VoodooLogger;
extern VoodooShader::Frost::Adapter * VoodooFrost;
extern LPVOODOOSHADER TestShader;

extern GLint gDepthTexture;
extern GLint gThisFrame;
extern GLint gLastFrame;
extern GLint gLastShader;
extern GLint gLastPass;
extern GLsizei gViewWidth;
extern GLsizei gViewHeight;

#endif /*VOODOO_NWS_MODULE_HPP*/
