/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (C) 2010 by Sean Sube
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

#include <set>

#define VOODOO_IMPORT
#include "VoodooFramework.hpp"

#define D3D_DEBUG_INFO
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "D3D9.lib")
#pragma comment(lib, "D3dx9.lib")

#include "Cg/cgD3D9.h"
#pragma comment(lib, "cgD3D9.lib")

#ifndef VOODOO_IMPORT_Gem
#	define VOODOO_API_Gem __declspec(dllexport)
#else
#	define VOODOO_API_Gem __declspec(dllimport)
#endif

#include "MGE/FauxD3D8Header.hpp"

class IVoodoo3D8;
class IVoodoo3DDevice8;
class IVoodoo3DSurface8;

extern D3DCAPS8 d3d8Caps;

extern VoodooShader::Core * VoodooCore;
extern VoodooShader::Adapter * VoodooGem;

extern IVoodoo3D8 * VoodooObject;
extern IVoodoo3DDevice8 * VoodooDevice;

extern VoodooShader::ShaderRef testShader;

extern IDirect3DSurface9 * backbufferSurf;

extern IDirect3DSurface9 * scratchSurface;
extern IDirect3DTexture9 * scratchTexture;

extern VoodooShader::TextureRef texture_ThisFrame;
extern IDirect3DSurface9 * surface_ThisFrame;

typedef IDirect3D8 * (__stdcall *D3DFunc8)(UINT);

