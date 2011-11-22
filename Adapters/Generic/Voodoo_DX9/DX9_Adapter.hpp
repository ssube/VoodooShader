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

        class DX9Adapter
            : VoodooShader::IAdapter
        {
        public:
            DX9Adapter(ICore * pCore);
            virtual ~DX9Adapter();

            virtual uint32_t AddRef(void) const throw();
            virtual uint32_t Release(void) const throw();
            virtual bool QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
            virtual String ToString(void) const throw();
            virtual ICore * GetCore(void) const throw();

            virtual bool LoadPass(_In_ IPass * const pPass);
            virtual bool UnloadPass(_In_ IPass * const pPass);
            virtual bool SetPass(_In_opt_ IPass * const pPass);
            virtual IPass * GetPass(void) const;
            virtual bool SetTarget(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget);
            virtual ITexture * GetTarget(_In_ const uint32_t index) const;
            virtual bool CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc, _Inout_ ITexture * const pTexture);
            virtual bool LoadTexture(_In_ IImage * const pFile, _In_opt_ const TextureRegion * pRegion, _Inout_ ITexture * const pTexture);
            virtual bool DrawGeometry
            (
                _In_ const uint32_t count, 
                _In_count_(count) VertexStruct * const pVertexData, 
                _In_ const VertexFlags flags
            );
            virtual bool ApplyParameter(_In_ IParameter * const pParam);
            virtual bool SetProperty(_In_ const wchar_t * name, _In_ Variant & value);
            virtual Variant GetProperty(_In_ const wchar_t * property) const;
            virtual bool ConnectTexture(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture);
            virtual bool HandleError(_In_opt_ CGcontext const pContext, _In_ uint32_t error);

            virtual bool SetDXDevice(IDirect3DDevice9 * pDevice);

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;

            IDirect3DDevice9 * m_Device;
            IDirect3DVertexDeclaration9 * m_VertDecl;
            IDirect3DVertexDeclaration9 * m_VertDeclT;

            IPassRef m_BoundPass;

            ITextureRef m_RenderTarget[4];
        };
    };
}
