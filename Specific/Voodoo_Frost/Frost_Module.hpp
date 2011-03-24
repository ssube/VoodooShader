
#ifndef VOODOO_NWS_MODULE_HPP
#define VOODOO_NWS_MODULE_HPP

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

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

        Version API_ModuleVersion();

        int API_ClassCount();

        const char * API_ClassInfo
        (
            _In_ int number
        );

        IObject * API_ClassCreate
        (
            _In_ int number, 
            _In_ Core * core
        );
    }
}

extern VoodooShader::Core * VoodooCore;
extern VoodooShader::Frost::Adapter * VoodooFrost;

#endif /*VOODOO_NWS_MODULE_HPP*/
