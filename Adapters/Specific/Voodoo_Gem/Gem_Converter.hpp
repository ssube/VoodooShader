/**
 * \ This file is part of the Voodoo Shader Framework, a comprehensive shader
 * support library. Copyright (c) 2010-2011 by Sean Sube This program is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version. This
 * program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details. You
 * should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth
 * Floor, Boston, MA 02110-1301 US Support and more information may be found at
 * http://www.voodooshader.com, or by contacting the developer at
 * peachykeen@voodooshader.com \ 
 */
#ifndef VOODOO_GEM_CONVERTER_HPP
#define VOODOO_GEM_CONVERTER_HPP

#include "Gem_Module.hpp"

namespace VoodooShader
{
 namespace Gem
{
 class VOODOO_API_GEM Gem_Converter
 {

/**
 -----------------------------------------------------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------------------------------------------------
 */
public:

  /**
   ===============================================================================================================
   *
   ===============================================================================================================
   */
  static inline D3DFORMAT ToD3DFormat(TextureFormat tf)
  {
   switch (tf)
   {
   case TF_RGB5:
    return D3DFMT_X1R5G5B5;

   case TF_RGB5A1:
    return D3DFMT_A1R5G5B5;

   case TF_RGB8:
    return D3DFMT_X8R8G8B8;

   case TF_RGBA8:
    return D3DFMT_A8R8G8B8;

   case TF_RGB10A2:
    return D3DFMT_A2B10G10R10;

   case TF_RGBA16F:
    return D3DFMT_A16B16G16R16F;

   case TF_RGBA32F:
    return D3DFMT_A32B32G32R32F;

   case TF_D16:
    return D3DFMT_D16;

   case TF_D32:
    return D3DFMT_D32;

   case TF_Unknown:
   default:
    return D3DFMT_UNKNOWN;
   }
  }

  /**
   ===============================================================================================================
   *
   ===============================================================================================================
   */
  static inline TextureFormat ToTextureFormat(D3DFORMAT tf)
  {
   switch (tf)
   {
   case D3DFMT_X1R5G5B5:
    return TF_RGB5;

   case D3DFMT_A1R5G5B5:
    return TF_RGB5A1;

   case D3DFMT_X8R8G8B8:
    return TF_RGB8;

   case D3DFMT_A8R8G8B8:
    return TF_RGBA8;

   case D3DFMT_A2B10G10R10:
    return TF_RGB10A2;

   case D3DFMT_A16B16G16R16F:
    return TF_RGBA16F;

   case D3DFMT_A32B32G32R32F:
    return TF_RGBA32F;

   case D3DFMT_D16:
    return TF_D16;

   case D3DFMT_D32:
    return TF_D32;

   case D3DFMT_UNKNOWN:
   default:
    return TF_Unknown;
   }
  }
 };
}
}
#endif /* VOODOO_GEM_CONVERTER_HPP */
