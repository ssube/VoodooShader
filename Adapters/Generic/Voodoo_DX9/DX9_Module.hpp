/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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

#define VOODOO_IMPORT
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

#include "D3D9.h"

#include "Cg/cgD3D9.h"
#pragma comment(lib, "cgD3D9.lib")

#ifndef VOODOO_IMPORT_DX9
#define VOODOO_API_DX9 __declspec(dllexport)
#else
#define VOODOO_API_DX9 __declspec(dllimport)
#endif

namespace VoodooShader
{
    namespace VoodooDX9
    {
        class DX9Adapter;

        class IVoodoo3D9;
        class IVoodoo3DDevice9;
    }
}

extern VoodooShader::ICore * gpVoodooCore;

extern VoodooShader::VoodooDX9::IVoodoo3D9 *VoodooObject;
extern VoodooShader::VoodooDX9::IVoodoo3DDevice9 *VoodooDevice;

extern VoodooShader::IShader * testShader;

extern IDirect3DVertexBuffer9 * gpFSQuadVerts;
extern IDirect3DSurface9 *backbufferSurf;

extern IDirect3DSurface9 *surface_ThisFrame;
extern VoodooShader::ITexture* texture_ThisFrame;

typedef IDirect3D9 * (__stdcall * D3DFunc9) (UINT);

const VoodooShader::Version * VOODOO_CALL API_VersionFunc();
const uint32_t VOODOO_CALL API_CountFunc();
const wchar_t * VOODOO_CALL API_InfoFunc(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * refid);
VoodooShader::IObject * VOODOO_CALL API_CreateFunc(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore);
