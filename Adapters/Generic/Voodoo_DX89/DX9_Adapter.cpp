/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

// Voodoo DX89
#include "DX9_Adapter.hpp"
#include "DX9_Version.hpp"
// CVoodoo3D
#include "CVoodoo3D8.hpp"
#include "CVoodoo3DDevice8.hpp"
#include "CVoodoo3D9.hpp"
// Voodoo Core
#include "StringFormat.hpp"
#include "Support.inl"

VoodooShader::EffectRef   testEffect = nullptr;

IDirect3DSurface9       * target_Color = nullptr;
IDirect3DSurface9       * target_Depth = nullptr;

VoodooShader::TextureRef  texture_Frame0 = nullptr;
IDirect3DSurface9       * surface_Frame0 = nullptr;
VoodooShader::TextureRef  texture_Pass0 = nullptr;
IDirect3DSurface9       * surface_Pass0 = nullptr;
VoodooShader::TextureRef  texture_Depth = nullptr;
IDirect3DSurface9       * surface_Depth = nullptr;
