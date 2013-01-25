/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
#pragma once

#include "Voodoo_OGL.hpp"

namespace VoodooShader
{
    namespace Voodoo_OGL
    {
        class ConverterOGL
        {
        public:
            static inline GLenum ToOGLFormat(TextureFormat tf)
            {
                switch (tf)
                {
                case VSFmt_RGB5A1:
                    return GL_RGB5_A1;
                case VSFmt_RGB8:
                    return GL_RGB8;
                case VSFmt_RGBA8:
                    return GL_RGBA8;
                case VSFmt_RGB10A2:
                    return GL_RGB10_A2;
                case VSFmt_RGBA16F:
                    return GL_RGBA_16F;
                case VSFmt_RGBA32F:
                    return GL_RGBA_32F;
                case VSFmt_D16:
                    return GL_DEPTH_COMPONENT16;
                case VSFmt_D24:
                    return GL_DEPTH_COMPONENT24;
                case VSFmt_D24S8:
                    return GL_DEPTH24_STENCIL8;
                case VSFmt_D32:
                    return GL_DEPTH_COMPONENT32;
                case VSFmt_Unknown:
                case VSFmt_Null:
                case VSFmt_DMax:
                default:
                    return 0;
                }
            }

            static inline TextureFormat ToTextureFormat(GLenum tf)
            {
                switch (tf)
                {
                case GL_RGB5_A1:
                    return VSFmt_RGB5A1;
                case GL_RGB8:
                    return VSFmt_RGB8;
                case GL_RGBA8:
                    return VSFmt_RGBA8;
                case GL_RGB10_A2:
                    return VSFmt_RGB10A2;
                case GL_RGBA_16F:
                    return VSFmt_RGBA16F;
                case GL_RGBA_32F:
                    return VSFmt_RGBA32F;
                case GL_DEPTH_COMPONENT16:
                    return VSFmt_D16;
                case GL_DEPTH_COMPONENT24:
                    return VSFmt_D24;
                case GL_DEPTH24_STENCIL8:
                    return VSFmt_D24S8;
                case GL_DEPTH_COMPONENT32:
                    return VSFmt_D32;
                case 0:
                default:
                    return VSFmt_Unknown;
                }
            }
        };
    }
}
