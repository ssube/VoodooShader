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

#ifndef VOODOO_GEM_MODULE_HPP
#define VOODOO_GEM_MODULE_HPP

#include "Gem_Includes.hpp"
#include "Gem_Converter.hpp"
#include "Gem_Version.hpp"

#include "MGE/FauxD3D8Header.hpp"

namespace VoodooShader
{
    namespace Gem
    {
        class Adapter;
    }
}

_Check_return_ void * Gem_D3D8Create(_In_ UINT sdkVersion);

extern HRESULT DefaultErrorCode;

extern D3DCAPS8 d3d8Caps;

extern VoodooShader::Core * VoodooCore;
extern VoodooShader::Gem::Adapter * VoodooGem;

extern IVoodoo3D8 * VoodooObject;
extern IVoodoo3DDevice8 * VoodooDevice;

extern VoodooShader::ShaderRef testShader;

extern TextureTuple gBackbuffer;
extern TextureTuple gScratch;
extern TextureTuple gThisFrame;
extern std::map<VoodooShader::String, TextureTuple> gTextures;

extern VoodooShader::ParameterRef gMatrixView, gMatrixProj, gMatrixWorld;

extern D3DPRESENT_PARAMETERS gParams;

#endif /*VOODOO_GEM_MODULE_HPP*/