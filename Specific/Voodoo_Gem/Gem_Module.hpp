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
#include "Gem_Version.hpp"

#include "MGE/FauxD3D8Header.hpp"

#ifndef VOODOO_IMPORT_GEM
#    define VOODOO_API_GEM __declspec(dllexport)
#else
#    define VOODOO_API_GEM __declspec(dllimport)
#endif

namespace VoodooShader
{
    namespace Gem
    {
        class Adapter;
    }
}

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

class IVoodoo3D8;
class IVoodoo3DDevice8;
class IVoodoo3DSurface8;
class IVoodoo3DTexture8;

_Check_return_ void * WINAPI Gem_D3D8Create(_In_ UINT sdkVersion);
__out HANDLE WINAPI Gem_CreateFileA
(
    __in LPCSTR lpFileName, 
    __in DWORD dwDesiredAccess, 
    __in DWORD dwShareMode, 
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
    __in DWORD dwCreationDisposition, 
    __in DWORD dwFlagsAndAttributes, 
    __in_opt HANDLE hTemplateFile
);

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

extern VoodooShader::String gLastFilename;
extern bool gNextTexture;

#endif /*VOODOO_GEM_MODULE_HPP*/