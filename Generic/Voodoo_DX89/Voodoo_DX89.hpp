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

#define VOODOO_IMPORT
#include "VoodooFramework.hpp"

#include "../Voodoo_DX9/DX9_Module.hpp"
#include "../Voodoo_DX9/DX9_Converter.hpp"

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "D3D9.lib")
#pragma comment(lib, "D3dx9.lib")

#include "MGE/FauxD3D8Header.hpp"

#define GEMCALL(type) type _stdcall

interface IVoodoo3D8;

extern VoodooShader::Core * VoodooCore;
extern VoodooShader::DirectX9::Adapter * VoodooDX9;

typedef IDirect3D8 * (__stdcall * D3DFunc8)(UINT);

