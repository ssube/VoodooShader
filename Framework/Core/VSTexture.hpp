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

#include "ITexture.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_uuids 
     */
    DEFINE_CLSID(VSTexture) = {0xA6, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};
    
    /**
     * @par CLSID
     *      e6f312a6-05af-11e1-9e05-005056c00008
     */
    class VSTexture :
        public ITexture
    {
    public:
        VSTexture(_In_ ICore * pCore, _In_ String name, _In_ const TextureDesc * pDesc, _In_opt_ void * pTexture = nullptr);
        virtual ~VSTexture();

        VOODOO_METHODCALL_(uint32_t, AddRef)() CONST;
        VOODOO_METHODCALL_(uint32_t, Release)() CONST;
        VOODOO_METHODCALL(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHODCALL_(String, ToString)() CONST;
        VOODOO_METHODCALL_(ICore *, GetCore)() CONST;

        VOODOO_METHODCALL_(String, GetName)() CONST;
        VOODOO_METHODCALL_(void *, GetData)() const;
        VOODOO_METHODCALL_(void *, SetData)(_In_ void * pData) ;
        VOODOO_METHODCALL_(const TextureDesc *, GetDesc)() const;

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        void * m_Data;
        TextureDesc m_Desc;
    };
}
