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

#include "Frost_Texture.hpp"
// Voodoo Core
#include "Format.hpp"

namespace VoodooShader
{
    namespace VoodooFrost
    {
        FrostTexture::FrostTexture(ICore * pCore, String name, GLuint pTexture) :
            m_Refs(0), m_Core(pCore), m_Name(name), m_Texture(pTexture)
        {
            if (pTexture)
            {
                this->GetTexDesc();
            }
        }

        FrostTexture::~FrostTexture()
        {
        }

        uint32_t VOODOO_METHODTYPE FrostTexture::AddRef() CONST
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE FrostTexture::Release() CONST
        {
            if (SAFE_DECREMENT(m_Refs) == 0)
            {
                delete this;
                return 0;
            }
            else
            {
                return m_Refs;
            }
        }

        VoodooResult VOODOO_METHODTYPE FrostTexture::QueryInterface(_In_ Uuid clsid, _Deref_out_opt_ const IObject ** ppOut) CONST
        {
            if (!ppOut)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else
            {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                }
                else if (clsid == IID_ITexture)
                {
                    *ppOut = static_cast<const ITexture*>(this);
                }
                else if (clsid == CLSID_FrostTexture)
                {
                    *ppOut = static_cast<const FrostTexture*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return VSFERR_INVALIDPARAMS;
                }

                (*ppOut)->AddRef();
                return true;
            }
        }

        String VOODOO_METHODTYPE FrostTexture::ToString() CONST
        {
            return Format("FrostTexture(%1%)") << m_Name;
        }

        ICore * VOODOO_METHODTYPE FrostTexture::GetCore() CONST
        {
            return m_Core;
        }

        VOODOO_METHODDEF_(String, FrostTexture::GetName)() CONST
        {
            return m_Name;
        }

        VOODOO_METHODDEF(FrostTexture::GetProperty)(const String & name, _In_ Variant * pValue) CONST
        {
            return VSFERR_NOTIMPLEMENTED;
        }

        VOODOO_METHODDEF(FrostTexture::SetProperty)(const String & name, _In_ const Variant & value)
        {
            return VSFERR_NOTIMPLEMENTED;
        }

        void * VOODOO_METHODTYPE FrostTexture::GetData() CONST
        {
            return reinterpret_cast<void*>(m_Texture);
        }

        void * VOODOO_METHODTYPE FrostTexture::SetData(_In_ void * pData)
        {
            GLuint oldData = m_Texture;
            m_Texture = reinterpret_cast<GLuint>(pData);
            this->GetTexDesc();
            return reinterpret_cast<void*>(oldData);
        }

        TextureDesc VOODOO_METHODTYPE FrostTexture::GetDesc() CONST
        {
            return m_Desc;
        }

        void FrostTexture::GetTexDesc()
        {
            ZeroMemory(&m_Desc, sizeof(TextureDesc));

            if (!m_Texture) return;
        }
    }
}
