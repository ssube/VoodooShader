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

#include "VoodooFramework.hpp"

#include <tchar.h>

namespace VoodooShader
{
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
        if (_tcsnicmp(format, VSTR("TF_"), 3) != 0)
        {
            return TF_Unknown;
        }

        const wchar_t * formatBody = format + 3;

        if (formatBody[0] == VSTR('D'))
        {
            // Depth format
            if (_tcsnicmp(formatBody, VSTR("D16"), 3) == 0) return TF_D16;
            if (_tcsnicmp(formatBody, VSTR("D32"), 3) == 0) return TF_D32;
        }
        else
        {
            if (_tcsnicmp(formatBody, VSTR("RGB5"),    4) == 0) return TF_RGB5;
            if (_tcsnicmp(formatBody, VSTR("RGB5A1"),  6) == 0) return TF_RGB5A1;
            if (_tcsnicmp(formatBody, VSTR("RGB8"),    4) == 0) return TF_RGB8;
            if (_tcsnicmp(formatBody, VSTR("RGBA8"),   5) == 0) return TF_RGBA8;
            if (_tcsnicmp(formatBody, VSTR("RGB10A2"), 7) == 0) return TF_RGB10A2;
            if (_tcsnicmp(formatBody, VSTR("RGBA16F"), 7) == 0) return TF_RGBA16F;
            if (_tcsnicmp(formatBody, VSTR("RGBA32F"), 7) == 0) return TF_RGBA32F;
        }

        return TF_Unknown;
    }

    uint32_t Converter::ToComponents(_In_ ParameterType type)
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
            return 1;
        case PT_Sampler2D:
            return 2;
        case PT_Sampler3D:
            return 3;
        case PT_Struct:
        case PT_Unknown:
        default:
            return 0;
        }
    }

#define CASESTRING(x) case PREFIX##x: return VOODOO_TOSTRING(x)

    const wchar_t * Converter::ToString(_In_ TextureFormat tf)
    {
        switch (tf)
        {
#define PREFIX TF_
            CASESTRING(RGB5);
            CASESTRING(RGB5A1);
            CASESTRING(RGB8);
            CASESTRING(RGBA8);
            CASESTRING(RGB10A2);
            CASESTRING(RGBA16F);
            CASESTRING(RGBA32F);
            CASESTRING(D16);
            CASESTRING(D32);
#undef PREFIX
        case TF_Unknown:
        default:
            return VSTR("Unknown");
        }
    }

    const wchar_t * Converter::ToString(_In_ ParameterType pt)
    {
        switch (pt)
        {
#define PREFIX PT_
            CASESTRING(Sampler1D);
            CASESTRING(Sampler2D);
            CASESTRING(Sampler3D);
            CASESTRING(Float1x1);
            CASESTRING(Float1x2);
            CASESTRING(Float1x3);
            CASESTRING(Float1x4);
            CASESTRING(Float2x1);
            CASESTRING(Float2x2);
            CASESTRING(Float2x3);
            CASESTRING(Float2x4);
            CASESTRING(Float3x1);
            CASESTRING(Float3x2);
            CASESTRING(Float3x3);
            CASESTRING(Float3x4);
            CASESTRING(Float4x1);
            CASESTRING(Float4x2);
            CASESTRING(Float4x3);
            CASESTRING(Float4x4);
            CASESTRING(Struct);
#undef PREFIX
        case PT_Unknown:
        default:
            return VSTR("Unknown");
        }
    }

    const wchar_t * Converter::ToString(_In_ ParameterCategory cat)
    {
        switch (cat)
        {
#define PREFIX PC_
            CASESTRING(Float);
            CASESTRING(Sampler);
            CASESTRING(Struct);
#undef PREFIX
        case PC_Unknown:
        default:
            return VSTR("Unknown");
        }
    }

    const wchar_t * Converter::ToString(ProgramProfile profile)
    {
#define PREFIX PP_
        switch (profile)
        {
            CASESTRING(ps_1_1);
            CASESTRING(ps_1_2);
            CASESTRING(ps_1_3);
            CASESTRING(ps_1_4);
            CASESTRING(ps_2_0);
            CASESTRING(ps_3_0);
            CASESTRING(vs_1_1);
            CASESTRING(vs_2_0);
            CASESTRING(vs_3_0);
            CASESTRING(glsl);
            CASESTRING(glsl12);
            CASESTRING(arb1);
        case PP_Unknown:
        default:
            return VSTR("Unknown");
        }
#undef PREFIX
    }

    const wchar_t * Converter::ToString(ProgramStage stage)
    {
#define PREFIX PS_
        switch (stage)
        {
            CASESTRING(Vertex);
            CASESTRING(Pixel);
            CASESTRNIG(Geometry);
            CASESTRING(Domain);
            CASESTRING(Hull);
            CASESTRING(Compute);
        case PS_Unknown:
        default:
            return VSTR("Unknown");
        }
#undef PREFIX
    }

#undef CASESTRING
}
