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

#include "DX9_Texture.hpp"

#include "DX9_Converter.hpp"

#include "Format.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        DX9Texture::DX9Texture(ICore * pCore, String name, IDirect3DTexture9 * pTexture) :
            m_Refs(0), m_Core(pCore), m_Name(name), m_Texture(pTexture), m_Surface(nullptr)
        {
            if (pTexture)
            {
                m_Texture->AddRef();
                m_Texture->GetSurfaceLevel(0, &m_Surface);

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
            }
            else
            {
                return m_Refs;
            }
        }

        VoodooResult VOODOO_METHODTYPE DX9Texture::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const IObject ** ppOut) CONST
        {
            if (!ppOut)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else
            {
                if (refid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                }
                else if (refid == IID_ITexture)
                {
                    *ppOut = static_cast<const ITexture*>(this);
                }
                else if (refid == CLSID_DX9Texture)
                {
                    *ppOut = static_cast<const DX9Texture*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return false;
                }

                (*ppOut)->AddRef();
                return true;
            }
        }

        String VOODOO_METHODTYPE DX9Texture::ToString() CONST
        {
            return Format("DX9Texture(%s)") << m_Name;
        }

        ICore * VOODOO_METHODTYPE DX9Texture::GetCore() CONST
        {
            return m_Core;
        }

        String VOODOO_METHODTYPE DX9Texture::GetName() CONST
        {
            return m_Name;
        }

        VoodooResult VOODOO_METHODTYPE DX9Texture::GetProperty(const Uuid propid, _In_ Variant * pValue) CONST
        {
            if (!pValue) return VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3D9Texture)
            {
                pValue->Type = UT_PVoid;
                pValue->Components = 0;
                pValue->VPVoid = m_Texture;
                return VSF_OK;
            }
            else if (propid == PropIds::D3D9Surface)
            {
                pValue->Type = UT_PVoid;
                pValue->Components = 0;
                pValue->VPVoid = m_Texture;
                return VSF_OK;
            }

            return VSFERR_INVALIDPARAMS;
        }

        VoodooResult VOODOO_METHODTYPE DX9Texture::SetProperty(const Uuid propid, _In_ Variant * pValue)
        {
            UNREFERENCED_PARAMETER(propid);

            if (!pValue) return VSFERR_INVALIDPARAMS;

            return VSFERR_INVALIDCALL;
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
