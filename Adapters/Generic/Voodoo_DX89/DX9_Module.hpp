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

#define VOODOO_IMPORT
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

#if defined(_DEBUG)
#   define D3D_DEBUG_INFO
#endif
#include "D3D9.h"
#include "D3DX9Shader.h"

#ifndef VOODOO_IMPORT_DX9
#   define VOODOO_API_DX9 __declspec(dllexport)
#else
#   define VOODOO_API_DX9 __declspec(dllimport)
#endif

#ifdef VOODOO_DX89_DEBUG3D8
#   define VOODOO_API_LOG(...) gpVoodooLogger->LogMessage(LL_ModDebug, VOODOO_DX89_NAME, __VA_ARGS__)
#else
#   define VOODOO_API_LOG(...)
#endif

namespace VoodooShader
{
    namespace VoodooDX8
    {
        class CVoodoo3D8;
        class CVoodoo3DDevice8;
        class CVoodoo3DTexture8;
        class CVoodoo3DSurface8;
    }

    namespace VoodooDX9
    {
        class DX9Adapter;

        class CVoodoo3D9;
        class CVoodoo3DDevice9;
    }
}

extern VoodooShader::ICoreRef gpVoodooCore;
extern VoodooShader::ILoggerRef gpVoodooLogger;

extern VoodooShader::IEffect * testEffect;

extern IDirect3DVertexBuffer9 * gpFSQuadVerts;
extern IDirect3DSurface9 *backbufferSurf;

extern IDirect3DSurface9 *surface_Frame0;
extern VoodooShader::ITexture* texture_Frame0;
extern IDirect3DSurface9 *surface_Pass0;
extern VoodooShader::ITexture* texture_Pass0;
extern IDirect3DSurface9 * surface_Scratch;
extern VoodooShader::ITexture* texture_Scratch;

extern volatile LONG gObjectLock;

typedef IDirect3D9 * (__stdcall * D3DFunc9) (UINT);

const VoodooShader::Version * VOODOO_CALLTYPE API_ModuleVersion();
const uint32_t VOODOO_CALLTYPE API_ClassCount();
const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * refid);
VoodooShader::IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore);
