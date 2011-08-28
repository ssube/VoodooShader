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

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

#include <set>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "D3D9.lib")
#pragma comment(lib, "D3dx9.lib")

#include "Cg/cgD3D9.h"
#pragma comment(lib, "cgD3D9.lib")

#ifndef VOODOO_IMPORT_DX89
#    define VOODOO_API_DX89 __declspec(dllexport)
#else
#    define VOODOO_API_DX89 __declspec(dllimport)
#endif

#include "MGE/FauxD3D8Header.hpp"

class IVoodoo3D8;
class IVoodoo3DDevice8;
class IVoodoo3DSurface8;

extern D3DCAPS8 d3d8Caps;

extern VoodooShader::Core * VoodooCore;
extern VoodooShader::Adapter * VoodooDX89;

extern IVoodoo3D8 * VoodooObject;
extern IVoodoo3DDevice8 * VoodooDevice;

extern VoodooShader::ShaderRef testShader;

extern IDirect3DSurface9 * backbufferSurf;

extern VoodooShader::TextureRef texture_ThisFrame;
extern IDirect3DSurface9 * surface_ThisFrame;

typedef IDirect3D8 * (__stdcall *D3DFunc8)(UINT);

