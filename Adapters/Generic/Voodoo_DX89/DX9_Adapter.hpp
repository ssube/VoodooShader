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

#include "DX9_Module.hpp"

#if defined(_DEBUG)
#   define D3D_DEBUG_INFO
#endif
#include "D3D9.h"
#include "D3DX9Shader.h"

#ifdef VOODOO_DX89_DEBUG3D8
#   define VOODOO_API_LOG(...) gpVoodooLogger->LogMessage(VSLog_PlugDebug, VOODOO_DX89_NAME, __VA_ARGS__)
#else
#   define VOODOO_API_LOG(...)
#endif

extern VoodooShader::EffectRef  testEffect;

extern IDirect3DSurface9 *      target_Color;
extern IDirect3DSurface9 *      target_Depth;

extern VoodooShader::TextureRef texture_Frame0;
extern IDirect3DSurface9 *      surface_Frame0;
extern VoodooShader::TextureRef texture_Pass0;
extern IDirect3DSurface9 *      surface_Pass0;
extern VoodooShader::TextureRef texture_Depth;
extern IDirect3DSurface9 *      surface_Depth;
extern IDirect3DSurface9 *      surface_Scratch;
extern VoodooShader::TextureRef texture_Scratch;
