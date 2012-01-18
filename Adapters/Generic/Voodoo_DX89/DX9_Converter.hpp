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

#include "VoodooFramework.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        class VOODOO_API_DX9 DX9_Converter
        {
        public:
            static inline D3DFORMAT ToD3DFormat(TextureFormat tf)
            {
                switch (tf)
                {
                case VSFmtRGB5:
                    return D3DFMT_X1R5G5B5;
                case VSFmtRGB5A1:
                    return D3DFMT_A1R5G5B5;
                case VSFmtRGB8:
                    return D3DFMT_X8R8G8B8;
                case VSFmtRGBA8:
                    return D3DFMT_A8R8G8B8;
                case VSFmtRGB10A2:
                    return D3DFMT_A2B10G10R10;
                case VSFmtRGBA16F:
                    return D3DFMT_A16B16G16R16F;
                case VSFmtRGBA32F:
                    return D3DFMT_A32B32G32R32F;
                case VSFmtD16:
                    return D3DFMT_D16;
                case VSFmtD32:
                    return D3DFMT_D32;
                case VSFmtUnknown:
                default:
                    return D3DFMT_UNKNOWN;
                }
            }

            static inline TextureFormat ToTextureFormat(D3DFORMAT tf)
            {
                switch (tf)
                {
                case D3DFMT_X1R5G5B5:
                    return VSFmtRGB5;
                case D3DFMT_A1R5G5B5:
                    return VSFmtRGB5A1;
                case D3DFMT_X8R8G8B8:
                    return VSFmtRGB8;
                case D3DFMT_A8R8G8B8:
                    return VSFmtRGBA8;
                case D3DFMT_A2B10G10R10:
                    return VSFmtRGB10A2;
                case D3DFMT_A16B16G16R16F:
                    return VSFmtRGBA16F;
                case D3DFMT_A32B32G32R32F:
                    return VSFmtRGBA32F;
                case D3DFMT_D16:
                    return VSFmtD16;
                case D3DFMT_D32:
                    return VSFmtD32;
                case D3DFMT_UNKNOWN:
                default:
                    return VSFmtUnknown;
                }
            }
        };
    }
}
