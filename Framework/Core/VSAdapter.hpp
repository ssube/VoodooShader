/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
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
    class VSAdapter :
        public IAdapter
    {
    public:
        VSAdapter(ICore * pCore);
        virtual ~VSAdapter(void);

        virtual uint32_t AddRef() const;
        virtual uint32_t Release() const;
        virtual String ToString(void) const;
        virtual ICore * GetCore(void) const;

        virtual bool LoadPass(_In_ IPass * const pPass);
        virtual bool UnloadPass(_In_ IPass * const pPass);
        virtual void SetPass(_In_opt_ IPass * const pPass);
        virtual IPass * GetPass(void) const;
        virtual bool SetTarget(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget);
        virtual ITexture * GetTarget(_In_ const uint32_t index) const;
        virtual ITexture * CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc);
        virtual ITexture * LoadTexture(_In_ const String & name, _In_opt_ const TextureRegion * pRegion);
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
