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
#pragma once

#include "ITechnique.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_uuids 
     */
    DEFINE_CLSID(VSTechnique) = {0xA5, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};
    
    /**
     * @par CLSID
     *      e6f312a5-05af-11e1-9e05-005056c00008
     */
    class VSTechnique :
        public ITechnique
    {
    public:
        VSTechnique(_In_ IShader * pShader, _In_ CGtechnique pCgTech);
        virtual ~VSTechnique();

        VOODOO_METHODCALL_(uint32_t, AddRef)() CONST;
        VOODOO_METHODCALL_(uint32_t, Release)() CONST;
        VOODOO_METHODCALL(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHODCALL_(String, ToString)() CONST;
        VOODOO_METHODCALL_(ICore *, GetCore)() CONST;

        VOODOO_METHODCALL_(String, GetName)() CONST;
        VOODOO_METHODCALL_(ITexture *, GetTarget)() CONST;
        VOODOO_METHODCALL_(const uint32_t, GetPassCount)() CONST;
        VOODOO_METHODCALL_(IPass *, GetPass)(_In_ const uint32_t index) CONST;
        VOODOO_METHODCALL_(IShader *, GetShader)() CONST;

        VOODOO_METHODCALL_(CGtechnique, GetCgTechnique)() CONST;

    private:
        void Link();

        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        ITextureRef m_Target;
        PassVector m_Passes;

        IShader * m_Shader;
        CGtechnique m_CgTechnique;
    };
}
