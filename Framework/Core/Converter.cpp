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

#include "Converter.hpp"

namespace VoodooShader
{
    CGtype Converter::ToCGType(_In_ ParameterType pt)
    {
        switch (pt)
        {
        // Vectors
        case PT_Float1x1:
            return CG_FLOAT;
        case PT_Float1x2:
            return CG_FLOAT2;
        case PT_Float1x3:
            return CG_FLOAT3;
        case PT_Float1x4:
            return CG_FLOAT4;
        case PT_Float2x1:
            return CG_FLOAT2x1;
        case PT_Float2x2:
            return CG_FLOAT2x2;
        case PT_Float2x3:
            return CG_FLOAT2x3;
        case PT_Float2x4:
            return CG_FLOAT2x4;
        case PT_Float3x1:
            return CG_FLOAT3x1;
        case PT_Float3x2:
            return CG_FLOAT3x2;
        case PT_Float3x3:
            return CG_FLOAT3x3;
        case PT_Float3x4:
            return CG_FLOAT3x4;
        case PT_Float4x1:
            return CG_FLOAT4x1;
        case PT_Float4x2:
            return CG_FLOAT4x2;
        case PT_Float4x3:
            return CG_FLOAT4x3;
        case PT_Float4x4:
            return CG_FLOAT4x4;
        // Samplers
        case PT_Sampler1D:
            return CG_SAMPLER1D;
        case PT_Sampler2D:
            return CG_SAMPLER2D;
        case PT_Sampler3D:
            return CG_SAMPLER3D;
        // Structs
        case PT_Struct:
            return CG_STRUCT;
        // Other
        case PT_Unknown:
        default:
            return CG_UNKNOWN_TYPE;
        }
    }

    ParameterType Converter::ToParameterType(_In_ CGtype pt)
    {
        switch (pt)
        {
        case CG_FLOAT:
        case CG_FLOAT1:
        case CG_FLOAT1x1:
            return PT_Float1x1;
        case CG_FLOAT2:
        case CG_FLOAT1x2:
            return PT_Float1x2;
        case CG_FLOAT3:
        case CG_FLOAT1x3:
            return PT_Float1x3;
        case CG_FLOAT4:
        case CG_FLOAT1x4:
            return PT_Float1x4;
        case CG_FLOAT2x1:
            return PT_Float2x1;
        case CG_FLOAT2x2:
            return PT_Float2x2;
        case CG_FLOAT2x3:
            return PT_Float2x3;
        case CG_FLOAT2x4:
            return PT_Float2x4;
        case CG_FLOAT3x1:
            return PT_Float3x1;
        case CG_FLOAT3x2:
            return PT_Float3x2;
        case CG_FLOAT3x3:
            return PT_Float3x3;
        case CG_FLOAT3x4:
            return PT_Float3x4;
        case CG_FLOAT4x1:
            return PT_Float4x1;
        case CG_FLOAT4x2:
            return PT_Float4x2;
        case CG_FLOAT4x3:
            return PT_Float4x3;
        case CG_FLOAT4x4:
            return PT_Float4x4;
        case CG_SAMPLER1D:
            return PT_Sampler1D;
        case CG_SAMPLER2D:
            return PT_Sampler2D;
        case CG_SAMPLER3D:
            return PT_Sampler3D;
        case CG_STRUCT:
            return PT_Struct;
        case CG_UNKNOWN_TYPE:
        default:
            return PT_Unknown;
        }
    }

    ParameterCategory Converter::ToParameterCategory(_In_ ParameterType type)
    {
        switch (type)
        {
        case PT_Float1x1:
        case PT_Float1x2:
        case PT_Float1x3:
        case PT_Float1x4:
        case PT_Float2x1:
        case PT_Float2x2:
        case PT_Float2x3:
        case PT_Float2x4:
        case PT_Float3x1:
        case PT_Float3x2:
        case PT_Float3x3:
        case PT_Float3x4:
        case PT_Float4x1:
        case PT_Float4x2:
        case PT_Float4x3:
        case PT_Float4x4:
            return PC_Float;
        case PT_Sampler1D:
        case PT_Sampler2D:
        case PT_Sampler3D:
            return PC_Sampler;
        case PT_Unknown:
        default:
            return PC_Unknown;
        }
    }

    TextureFormat Converter::ToTextureFormat(_In_ const wchar_t * format)
    {
        if (_wcsnicmp(format, VSTR("TF_"), 3) != 0)
        {
            return TF_Unknown;
        }

        const wchar_t * formatBody = format + 3;

        if (formatBody[0] == VSTR('D'))
        {
            // Depth format
            if (_wcsnicmp(formatBody, VSTR("D16"), 3) == 0) return TF_D16;
            if (_wcsnicmp(formatBody, VSTR("D32"), 3) == 0) return TF_D32;
        }
        else
        {
            if (_wcsnicmp(formatBody, VSTR("RGB5"),    4) == 0) return TF_RGB5;
            if (_wcsnicmp(formatBody, VSTR("RGB5A1"),  6) == 0) return TF_RGB5A1;
            if (_wcsnicmp(formatBody, VSTR("RGB8"),    4) == 0) return TF_RGB8;
            if (_wcsnicmp(formatBody, VSTR("RGBA8"),   5) == 0) return TF_RGBA8;
            if (_wcsnicmp(formatBody, VSTR("RGB10A2"), 7) == 0) return TF_RGB10A2;
            if (_wcsnicmp(formatBody, VSTR("RGBA16F"), 7) == 0) return TF_RGBA16F;
            if (_wcsnicmp(formatBody, VSTR("RGBA32F"), 7) == 0) return TF_RGBA32F;
        }

        return TF_Unknown;
    }

    int Converter::ToComponents(_In_ ParameterType type)
    {
        switch (type)
        {
        case PT_Float1x1:
            return 1;
        case PT_Float1x2:
        case PT_Float2x1:
            return 2;
        case PT_Float1x3:
        case PT_Float3x1:
            return 3;
        case PT_Float1x4:
        case PT_Float2x2:
        case PT_Float4x1:
            return 4;
        case PT_Float2x3:
        case PT_Float3x2:
            return 6;
        case PT_Float2x4:
        case PT_Float4x2:
            return 8;
        case PT_Float3x3:
            return 9;
        case PT_Float3x4:
        case PT_Float4x3:
            return 12;
        case PT_Float4x4:
            return 16;
        case PT_Sampler1D:
        case PT_Sampler2D:
        case PT_Sampler3D:
        case PT_Struct:
        case PT_Unknown:
        default:
            return -1;
        }
    }

#define CASESTRING(x) case x: return VOODOO_META_TOSTRING(x)

    const wchar_t * Converter::ToString(_In_ TextureFormat tf)
    {
        switch (tf)
        {
            CASESTRING(TF_RGB5);
            CASESTRING(TF_RGB5A1);
            CASESTRING(TF_RGB8);
            CASESTRING(TF_RGBA8);
            CASESTRING(TF_RGB10A2);
            CASESTRING(TF_RGBA16F);
            CASESTRING(TF_RGBA32F);
            CASESTRING(TF_D16);
            CASESTRING(TF_D32);
        case TF_Unknown:
        default:
            return VSTR("TF_Unknown");
        }
    }

    const wchar_t * Converter::ToString(_In_ ParameterType pt)
    {
        switch (pt)
        {
            CASESTRING(PT_Sampler1D);
            CASESTRING(PT_Sampler2D);
            CASESTRING(PT_Sampler3D);
            CASESTRING(PT_Float1x1);
            CASESTRING(PT_Float1x2);
            CASESTRING(PT_Float1x3);
            CASESTRING(PT_Float1x4);
            CASESTRING(PT_Float2x1);
            CASESTRING(PT_Float2x2);
            CASESTRING(PT_Float2x3);
            CASESTRING(PT_Float2x4);
            CASESTRING(PT_Float3x1);
            CASESTRING(PT_Float3x2);
            CASESTRING(PT_Float3x3);
            CASESTRING(PT_Float3x4);
            CASESTRING(PT_Float4x1);
            CASESTRING(PT_Float4x2);
            CASESTRING(PT_Float4x3);
            CASESTRING(PT_Float4x4);
            CASESTRING(PT_Struct);
        case PT_Unknown:
        default:
            return VSTR("PT_Unknown");
        }
    }

    const wchar_t * Converter::ToString(_In_ ParameterCategory cat)
    {
        switch (cat)
        {
            CASESTRING(PC_Float);
            CASESTRING(PC_Sampler);
            CASESTRING(PC_Struct);
        case PC_Unknown:
        default:
            return VSTR("PC_Unknown");
        }
    }

#undef CASESTRING
}
