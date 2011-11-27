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
        DEFINE_CLSID(DX9Adapter) = {0xC1, 0xC3, 0x4A, 0xF8, 0x3F, 0x07, 0xE1, 0x11, 0x83, 0xD4, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

        class DX9Adapter : 
            public VoodooShader::IAdapter
        {
        public:
            DX9Adapter(ICore * pCore);
            virtual ~DX9Adapter();

            VOODOO_METHODCALL_(uint32_t, AddRef)() CONST;
            VOODOO_METHODCALL_(uint32_t, Release)() CONST;
            VOODOO_METHODCALL(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
            VOODOO_METHODCALL_(String, ToString)() CONST;
            VOODOO_METHODCALL_(ICore *, GetCore)() CONST;

            VOODOO_METHODCALL(LoadPass)(_In_ IPass * const pPass);
            VOODOO_METHODCALL(UnloadPass)(_In_ IPass * const pPass);
            VOODOO_METHODCALL(SetPass)(_In_opt_ IPass * const pPass);
            VOODOO_METHODCALL_(IPass *, GetPass)() CONST;
            VOODOO_METHODCALL(SetTarget)(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget);
            VOODOO_METHODCALL_(ITexture *, GetTarget)(_In_ const uint32_t index) CONST;
            VOODOO_METHODCALL(CreateTexture)(_In_ const String & name, _In_ const TextureDesc * pDesc, _Inout_ ITexture * const pTexture);
            VOODOO_METHODCALL(LoadTexture)(_In_ IImage * const pFile, _In_opt_ const TextureRegion * pRegion, _Inout_ ITexture * const pTexture);
            VOODOO_METHODCALL_(bool, DrawGeometry)
            (
                _In_ const uint32_t count, 
                _In_count_(count) VertexStruct * const pVertexData, 
                _In_ const VertexFlags flags
            );
            VOODOO_METHODCALL(ApplyParameter)(_In_ IParameter * const pParam);
            VOODOO_METHODCALL(SetProperty)(_In_ const wchar_t * name, _In_ Variant & value);
            VOODOO_METHODCALL(GetProperty)(_In_ const wchar_t * name, _In_ Variant & value) const;
            VOODOO_METHODCALL(ConnectTexture)(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture);
            VOODOO_METHODCALL(HandleError)(_In_opt_ CGcontext const pContext, _In_ uint32_t error);

            bool SetDXDevice(IDirect3DDevice9 * pDevice);

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;

            UINT m_SdkVersion;
            IDirect3DDevice9 * m_Device;
            IDirect3DVertexDeclaration9 * m_VertDecl;
            IDirect3DVertexDeclaration9 * m_VertDeclT;

            IPassRef m_BoundPass;

            ITextureRef m_RenderTarget[4];
        };
    };
}
