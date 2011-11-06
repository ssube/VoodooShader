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

#include "Includes.hpp"

#include "IAdapter.hpp"

namespace VoodooShader
{
    /* e6f3129a-05af-11e1-9e05-005056c00008 */
    DEFINE_CLSID(VSAdapter, 0x9A, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);

    class VSAdapter :
        public IAdapter
    {
    public:
        VSAdapter(ICore * pCore);
        virtual ~VSAdapter(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual bool LoadPass(_In_ IPass * const pPass);
        virtual bool UnloadPass(_In_ IPass * const pPass);
        virtual void SetPass(_In_opt_ IPass * const pPass);
        virtual IPass * GetPass(void) const;
        virtual bool SetTarget(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget);
        virtual ITexture * GetTarget(_In_ const uint32_t index) const;
        virtual ITexture * CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc);
        virtual ITexture * LoadTexture(_In_ IFile * const pFile, _In_opt_ const TextureRegion * pRegion);
        virtual void DrawGeometry(_In_ const uint32_t count, _In_count_(count) const VertexStruct * const pVertexData);
        virtual void DrawShader(_In_ IShader * const pShader);
        virtual void ApplyParameter(_In_ IParameter * const pParam);
        virtual void SetProperty(_In_ const String & property, _In_ const String & value);
        virtual String GetProperty(_In_ const String & property) const;
        virtual bool ConnectTexture(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture);
        virtual void HandleError(_In_opt_ CGcontext const pContext, _In_ uint32_t error);

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
    };
}