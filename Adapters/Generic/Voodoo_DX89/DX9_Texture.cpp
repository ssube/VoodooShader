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

#include "DX9_Texture.hpp"

#include "DX9_Converter.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        DX9Texture::DX9Texture(ICore * pCore, String name, IDirect3DBaseTexture9 * pTexture) :
            m_Refs(0), m_Core(pCore), m_Name(name), m_Texture(pTexture)
        {
            if (pTexture)
            {
                m_Texture->AddRef();

                this->GetTexDesc();
            }
        }

        DX9Texture::~DX9Texture()
        {
            if (m_Texture)
            {
                m_Texture->Release();
            }
        }

        uint32_t VOODOO_METHODTYPE DX9Texture::AddRef() CONST
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE DX9Texture::Release() CONST
        {
            if (SAFE_DECREMENT(m_Refs) == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VOODOO_METHODTYPE DX9Texture::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = IID_ITexture;
                    return true;
                } else {
                    return false;
                }
            } else {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                } else if (clsid == IID_ITexture) {
                    *ppOut = static_cast<const ITexture*>(this);
                } else if (clsid == CLSID_DX9Texture) {
                    *ppOut = static_cast<const DX9Texture*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String VOODOO_METHODTYPE DX9Texture::ToString() CONST
        {
            return String::Format(L"DX9Texture(%s)", m_Name.GetData());
        }

        ICore * VOODOO_METHODTYPE DX9Texture::GetCore() CONST
        {
            return m_Core;
        }

        String VOODOO_METHODTYPE DX9Texture::GetName() CONST
        {
            return m_Name;
        }

        void * VOODOO_METHODTYPE DX9Texture::GetData() CONST
        {
            return m_Texture;
        }

        void * VOODOO_METHODTYPE DX9Texture::SetData(_In_ void * pData)
        {
            void * oldData = m_Texture;
            m_Texture = reinterpret_cast<IDirect3DBaseTexture9*>(pData);
            this->GetTexDesc();
            return oldData;
        }

        TextureDesc VOODOO_METHODTYPE DX9Texture::GetDesc() CONST
        {
            return m_Desc;
        }

        void DX9Texture::GetTexDesc()
        {
            ZeroMemory(&m_Desc, sizeof(TextureDesc));

            if (!m_Texture) return;

            m_Desc.Mipmaps = m_Texture->GetLevelCount() > 1;

            // Test for 2D texture
            IDirect3DTexture9 * pTex = nullptr;
            if (SUCCEEDED(m_Texture->QueryInterface(IID_IDirect3DTexture9, (void**)&pTex)))
            {
                D3DSURFACE_DESC desc;
                pTex->GetLevelDesc(0, &desc);

                m_Desc.Size.X = desc.Width;
                m_Desc.Size.Y = desc.Height;
                m_Desc.Size.Z = 0;
                m_Desc.RenderTarget = desc.Usage == D3DUSAGE_RENDERTARGET;
                m_Desc.Format = DX9_Converter::ToTextureFormat(desc.Format);

                pTex->Release();
            }

            // Test for 3D texture
            IDirect3DVolumeTexture9 * pVTex = nullptr;
            if (SUCCEEDED(m_Texture->QueryInterface(IID_IDirect3DVolumeTexture9, (void**)&pVTex)))
            {
                D3DVOLUME_DESC desc;
                pVTex->GetLevelDesc(0, &desc);

                m_Desc.Size.X = desc.Width;
                m_Desc.Size.Y = desc.Height;
                m_Desc.Size.Z = desc.Depth;
                m_Desc.RenderTarget = desc.Usage == D3DUSAGE_RENDERTARGET;
                m_Desc.Format = DX9_Converter::ToTextureFormat(desc.Format);

                pTex->Release();
            }
        }
    }
}
