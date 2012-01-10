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

#include "VSCompiler.hpp"
#include "VSEffectDX9.hpp"
#include "VSTechniqueDX9.hpp"
#include "VSPassDX9.hpp"
#include "VSParameterDX9.hpp"

#include <d3dx9effect.h>

namespace VoodooShader
{
    ParameterType VOODOO_METHODTYPE VSCompilerDX9::GetParameterType(const D3DXPARAMETER_DESC & desc)
    {
        // Get the category first
        switch (desc.Type)
        {
        case D3DXPT_FLOAT:
            return (ParameterType)((desc.Rows << 1) & 0xF0 | (desc.Columns) & 0x0F);
        case D3DXPT_SAMPLER1D:
            return PT_Sampler1D;
        case D3DXPT_SAMPLER2D:
            return PT_Sampler2D;
        case D3DXPT_SAMPLER3D:
            return PT_Sampler3D;
        default:
            return PT_Unknown;
        }
    }

    ParameterCategory VOODOO_METHODTYPE VSCompilerDX9::GetParameterCategory(D3DXPARAMETER_TYPE type)
    {
        switch (type)
        {
        case D3DXPT_BOOL:
            return PC_Bool;
        case D3DXPT_INT:
            return PC_Int;
        case D3DXPT_FLOAT:
            return PC_Float;
        case D3DXPT_STRING:
            return PC_String;
        case D3DXPT_SAMPLER1D:
        case D3DXPT_SAMPLER2D:
        case D3DXPT_SAMPLER3D:
        case D3DXPT_SAMPLERCUBE:
            return PC_Sampler;
    }
}