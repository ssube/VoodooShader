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
// System
#pragma warning(push,3)
#include <tchar.h>
#pragma warning(pop)

namespace VoodooShader
{
    TextureFormat Converter::ToTextureFormat(_In_ CONST wchar_t * format)
    {
        const wchar_t * formatBody = format;

        if (_tcsnicmp(format, VSTR("VSFmt_"), 6) == 0)
        {
            formatBody += 6;
        }

        if (formatBody[0] == VSTR('D'))
        {
            // Depth format
            if (_tcsnicmp(formatBody, VSTR("D16"), 3) == 0) return VSFmt_D16;
            if (_tcsnicmp(formatBody, VSTR("D24"), 3) == 0) return VSFmt_D24;
            if (_tcsnicmp(formatBody, VSTR("D24S8"), 5) == 0) return VSFmt_D24S8;
            if (_tcsnicmp(formatBody, VSTR("D32"), 3) == 0) return VSFmt_D32;
            if (_tcsnicmp(formatBody, VSTR("DMax"), 4) == 0) return VSFmt_DMax;
        }
        else if (formatBody[0] == VSTR('R'))
        {
            if (_tcsnicmp(formatBody, VSTR("RGB5A1"),  6) == 0) return VSFmt_RGB5A1;
            if (_tcsnicmp(formatBody, VSTR("RGB8"),    4) == 0) return VSFmt_RGB8;
            if (_tcsnicmp(formatBody, VSTR("RGBA8"),   5) == 0) return VSFmt_RGBA8;
            if (_tcsnicmp(formatBody, VSTR("RGB10A2"), 7) == 0) return VSFmt_RGB10A2;
            if (_tcsnicmp(formatBody, VSTR("RGBA16F"), 7) == 0) return VSFmt_RGBA16F;
            if (_tcsnicmp(formatBody, VSTR("RGBA32F"), 7) == 0) return VSFmt_RGBA32F;
        }
        else if (_tcsnicmp(formatBody, VSTR("Null"), 4) == 0)
        {
            return VSFmt_Null;
        }

        return VSFmt_Unknown;
    }

#define DONOTHING(a) a
#define CASESTRING(x) case DONOTHING(PREFIX) ## x: return VOODOO_TOSTRING(x)

#define PREFIX VSFmt_
    const wchar_t * Converter::ToString(_In_ TextureFormat tf)
    {
        switch (tf)
        {
            CASESTRING(Null);
            CASESTRING(RGB5A1);
            CASESTRING(RGB8);
            CASESTRING(RGBA8);
            CASESTRING(RGB10A2);
            CASESTRING(RGBA16F);
            CASESTRING(RGBA32F);
            CASESTRING(D16);
            CASESTRING(D24);
            CASESTRING(D24S8);
            CASESTRING(D32);
            CASESTRING(DMax);
        case VSFmt_Unknown:
        default:
            return VSTR("Unknown");
        }
    }
#undef PREFIX

#define PREFIX VSPT_
    const wchar_t * Converter::ToString(_In_ ParameterType pt)
    {
        switch (pt)
        {
            CASESTRING(Bool);
            CASESTRING(Int);
            CASESTRING(Float);
            CASESTRING(String);
            CASESTRING(Sampler);
            CASESTRING(Sampler1D);
            CASESTRING(Sampler2D);
            CASESTRING(Sampler3D);
            CASESTRING(SamplerCube);
            CASESTRING(Texture);
            CASESTRING(Texture1D);
            CASESTRING(Texture2D);
            CASESTRING(Texture3D);
            CASESTRING(TextureCube);
        case VSPT_Unknown:
        default:
            return VSTR("Unknown");
        }
    }
#undef PREFIX

#define PREFIX VSStage_
    const wchar_t * Converter::ToString(_In_ ShaderStage stage)
    {
        switch (stage)
        {
            CASESTRING(Vertex);
            CASESTRING(Pixel);
            CASESTRING(Geometry);
            CASESTRING(Domain);
            CASESTRING(Hull);
            CASESTRING(Compute);
            CASESTRING(All);
        case VSStage_Unknown:
        default:
            return VSTR("Unknown");
        }
    }
#undef PREFIX

#undef CASESTRING
#undef TOKENCAT
}
