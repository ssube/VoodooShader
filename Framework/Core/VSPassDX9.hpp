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

#include "VoodooFramework.hpp"
#include "VoodooInternal.hpp"

#include <d3dx9effect.h>

namespace VoodooShader
{
    /**
     * @clsid e6f312a3-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSPassDX9, IPass, ({0xA3, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
        friend VSCompilerDX9;

    public:
        VSPassDX9(_In_ VSTechniqueDX9 * pTechnique, _In_ LPD3DXEFFECT pEffect, _In_ D3DXHANDLE pPassHandle);
        ~VSPassDX9();

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid refid, _Deref_out_opt_ const IObject ** ppOut) CONST;
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD_(String, GetName)() CONST;
        VOODOO_METHOD(GetProperty)(const String & name, _In_ Variant * pValue) CONST;
        VOODOO_METHOD(SetProperty)(const String & name, _In_ const Variant & value);

        VOODOO_METHOD_(ITexture *, GetTarget)(uint32_t index) CONST;
        VOODOO_METHOD(SetTarget)(uint32_t index, ITexture * pTarget);
        VOODOO_METHOD(GetShader)(ShaderStage stage, _In_ Variant * pShader) CONST;
        VOODOO_METHOD_(ITechnique *, GetTechnique)() CONST;

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        VSTechniqueDX9 * m_Technique;
        VariantMap m_Properties;
        TextureVector m_Targets;

        LPD3DXEFFECT m_DXEffect;
        D3DXHANDLE m_DXHandle;

        LPDIRECT3DVERTEXSHADER9 m_VertexShader;
        LPDIRECT3DPIXELSHADER9  m_PixelShader;
    };
}
