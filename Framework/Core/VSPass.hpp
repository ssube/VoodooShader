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

namespace VoodooShader
{
    /**
     * @clsid e6f312a3-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSPass, IPass, ({0xA3, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        VSPass(_In_ ITechnique * pTechnique, _In_ pugi::xml_node passNode);
        ~VSPass();

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
        VOODOO_METHOD_(ITechnique *, GetTechnique)() CONST;
        VOODOO_METHOD_(IProgram *, GetProgram)(_In_ const ProgramStage stage) CONST;
        VOODOO_METHOD(Compile)(const CompileFlags flags);

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        VSTechnique * m_Technique;
        VariantMap m_Properties;

        pugi::xml_node m_Node;
        TextureVector m_Targets;

        IProgramRef m_VertexShader;
        IProgramRef m_PixelShader;
        IProgramRef m_GeometryShader;
        IProgramRef m_DomainShader;
        IProgramRef m_HullShader;
        IProgramRef m_ComputeShader;
    };
}
