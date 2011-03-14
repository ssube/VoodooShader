/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (c) 2010-2011 by Sean Sube
*
*
* This program is free software; you can redistribute it and/or modify it under the terms of the 
* GNU General Public License as published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#ifndef VOODOO_GEM_INCLUDES_HPP
#define VOODOO_GEM_INCLUDES_HPP

#include <set>
#include <tuple>

#define VOODOO_IMPORT
#include "VoodooFramework.hpp"

#ifdef _DEBUG
#   define D3D_DEBUG_INFO
#   define VOODOO_GEM_DEBUG_VALUE 1
#else
#   define VOODOO_GEM_DEBUG_VALUE 0
#endif

#include <d3d9.h>
#include <d3dx9.h>

#include "Cg/cgD3D9.h"

#ifndef VOODOO_IMPORT_GEM
#    define VOODOO_API_GEM __declspec(dllexport)
#else
#    define VOODOO_API_GEM __declspec(dllimport)
#endif

class IVoodoo3D8;
class IVoodoo3DDevice8;
class IVoodoo3DSurface8;
class IVoodoo3DTexture8;
/*

namespace VoodooShader
{
    namespace Gem
    {*/
        /**
        * Storage struct to hold associated Voodoo @ref Texture "Textures" and 
        * API texture objects. The RawTexture and RawSurface fields can hold an
        * OpenGL FBO and texture or DirectX texture and surface.
        */
        struct TextureTuple
        {
            VoodooShader::TextureRef Texture;
            LPDIRECT3DTEXTURE9 RawTexture;
            LPDIRECT3DSURFACE9 RawSurface;
        };
 /*   };
};*/

//typedef std::tuple<VoodooShader::TextureRef, LPDIRECT3DTEXTURE9, LPDIRECT3DSURFACE9> TextureTuple;

#endif /*VOODOO_GEM_INCLUDES_HPP*/