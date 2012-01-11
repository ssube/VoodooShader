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

#define DONOTHING(a) a
#define CASESTRING(x) case DONOTHING(PREFIX) ## x: return VOODOO_TOSTRING(x)

#define PREFIX TF_
    const wchar_t * Converter::ToString(_In_ TextureFormat tf)
    {
        switch (tf)
        {
            CASESTRING(RGB5);
            CASESTRING(RGB5A1);
            CASESTRING(RGB8);
            CASESTRING(RGBA8);
            CASESTRING(RGB10A2);
            CASESTRING(RGBA16F);
            CASESTRING(RGBA32F);
            CASESTRING(D16);
            CASESTRING(D32);
        case TF_Unknown:
        default:
            return VSTR("Unknown");
        }
    }
#undef PREFIX

#define PREFIX PT_
    const wchar_t * Converter::ToString(_In_ ParameterType pt)
    {
        switch (pt)
        {
            CASESTRING(Bool);
            CASESTRING(Int);
            CASESTRING(Float);
            CASESTRING(String);
            CASESTRING(Sampler1D);
            CASESTRING(Sampler2D);
            CASESTRING(Sampler3D);
            CASESTRING(SamplerCube);
        case PT_Unknown:
        default:
            return VSTR("Unknown");
        }
    }
#undef PREFIX

#define PREFIX SS_
    const wchar_t * Converter::ToString(ShaderStage stage)
    {
        switch (stage)
        {
            CASESTRING(Vertex);
            CASESTRING(Pixel);
            CASESTRING(Geometry);
            CASESTRING(Domain);
            CASESTRING(Hull);
            CASESTRING(Compute);
        case SS_Unknown:
        default:
            return VSTR("Unknown");
        }
    }
#undef PREFIX

#undef CASESTRING
#undef TOKENCAT
}
