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

#include "DX9_Module.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {    
        /**
         * @par CLSID
         *      f84ac3c2-073f-11e1-83d4-005056c00008
         */
        VOODOO_CLASS(DX9Texture, ITexture, {0xC2, 0xC3, 0x4A, 0xF8, 0x3F, 0x07, 0xE1, 0x11, 0x83, 0xD4, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
        {
        public:
            DX9Texture(_In_ ICore * pCore, _In_ String name, _In_ IDirect3DBaseTexture9 * pTexture);
            virtual ~DX9Texture();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;

            VOODOO_METHOD_(String, GetName)() CONST;
            VOODOO_METHOD_(void *, GetData)() CONST;
            VOODOO_METHOD_(void *, SetData)(_In_ void * pData) ;
            VOODOO_METHOD_(const TextureDesc *, GetDesc)() CONST;

        private:
            void GetTexDesc();

            mutable uint32_t m_Refs;
            ICore * m_Core;
            String m_Name;

            IDirect3DBaseTexture9 * m_Texture;
            TextureDesc m_Desc;
        };
    }
}
