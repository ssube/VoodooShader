/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

#include "Voodoo_D3D9.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        class ConverterDX9
        {
        public:
            static inline D3DFORMAT ToD3DFormat(TextureFormat tf)
            {
                switch (tf)
                {
                case VSFmt_RGB5A1:
                    return D3DFMT_A1R5G5B5;
                case VSFmt_RGB8:
                    return D3DFMT_X8R8G8B8;
                case VSFmt_RGBA8:
                    return D3DFMT_A8R8G8B8;
                case VSFmt_RGB10A2:
                    return D3DFMT_A2B10G10R10;
                case VSFmt_RGBA16F:
                    return D3DFMT_A16B16G16R16F;
                case VSFmt_RGBA32F:
                    return D3DFMT_A32B32G32R32F;
                case VSFmt_D16:
                    return D3DFMT_D16;
                case VSFmt_D24:
                    return D3DFMT_D24X8;
                case VSFmt_D24S8:
                    return D3DFMT_D24S8;
                case VSFmt_D32:
                    return D3DFMT_D32;
                case VSFmt_Unknown:
                case VSFmt_Null:
                case VSFmt_DMax:
                default:
                    return D3DFMT_UNKNOWN;
                }
            }

            static inline TextureFormat ToTextureFormat(D3DFORMAT tf)
            {
                switch (tf)
                {
                case D3DFMT_A1R5G5B5:
                    return VSFmt_RGB5A1;
                case D3DFMT_X8R8G8B8:
                    return VSFmt_RGB8;
                case D3DFMT_A8R8G8B8:
                    return VSFmt_RGBA8;
                case D3DFMT_A2B10G10R10:
                    return VSFmt_RGB10A2;
                case D3DFMT_A16B16G16R16F:
                    return VSFmt_RGBA16F;
                case D3DFMT_A32B32G32R32F:
                    return VSFmt_RGBA32F;
                case D3DFMT_D16:
                    return VSFmt_D16;
                case D3DFMT_D24X8:
                    return VSFmt_D24;
                case D3DFMT_D24S8:
                    return VSFmt_D24S8;
                case D3DFMT_D32:
                    return VSFmt_D32;
                case D3DFMT_UNKNOWN:
                default:
                    return VSFmt_Unknown;
                }
            }
        };
    }
}
