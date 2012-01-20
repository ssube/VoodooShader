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

#include "VSTextureDX9.hpp"
// Voodoo D3D9
#include "ConverterDX9.hpp"
// Voodoo Core
#include "Format.hpp"
// DevIL
#include "IL\ilut.h"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        VSTextureDX9::VSTextureDX9(ICore * pCore, String name, IDirect3DTexture9 * pTexture) :
            m_Refs(0), m_Core(pCore), m_Name(name), m_Texture(pTexture), m_Surface(nullptr)
        {
            if (pTexture)
            {
                m_Texture->AddRef();
                m_Texture->GetSurfaceLevel(0, &m_Surface);

                this->GetTexDesc();
            }
        }

        VSTextureDX9::~VSTextureDX9()
        {
            if (m_Texture)
            {
                m_Texture->Release();
            }
        }

        uint32_t VOODOO_METHODTYPE VSTextureDX9::AddRef() CONST
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE VSTextureDX9::Release() CONST
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

        VoodooResult VOODOO_METHODTYPE VSTextureDX9::QueryInterface(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut)
        {
            if (!ppOut)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else
            {
                if (refid == IID_IObject)
                {
                    *ppOut = static_cast<IObject*>(this);
                }
                else if (refid == IID_ITexture)
                {
                    *ppOut = static_cast<ITexture*>(this);
                }
                else if (refid == CLSID_VSTextureDX9)
                {
                    *ppOut = static_cast<VSTextureDX9*>(this);
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

        String VOODOO_METHODTYPE VSTextureDX9::ToString() CONST
        {
            return Format("VSTextureDX9(%s)") << m_Name;
        }

        ICore * VOODOO_METHODTYPE VSTextureDX9::GetCore() CONST
        {
            return m_Core;
        }

        String VOODOO_METHODTYPE VSTextureDX9::GetName() CONST
        {
            return m_Name;
        }

        VoodooResult VOODOO_METHODTYPE VSTextureDX9::GetProperty(const Uuid propid, _In_ Variant * pValue) CONST
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
                pValue->VPVoid = m_Surface;
                return VSF_OK;
            }

            return VSFERR_INVALIDPARAMS;
        }

        VoodooResult VOODOO_METHODTYPE VSTextureDX9::SetProperty(const Uuid propid, _In_ Variant * pValue)
        {
            UNREFERENCED_PARAMETER(propid);

            if (!pValue) return VSFERR_INVALIDPARAMS;

            return VSFERR_INVALIDCALL;
        }

        TextureDesc VOODOO_METHODTYPE VSTextureDX9::GetDesc() CONST
        {
            return m_Desc;
        }

        void VSTextureDX9::GetTexDesc()
        {
            ZeroMemory(&m_Desc, sizeof(TextureDesc));

            if (!m_Texture) return;

            if (m_Texture->GetLevelCount() > 0)
            {
                m_Desc.Usage = TextureFlags(m_Desc.Usage | VSTexFlag_MipMaps);
            }

            // Test for 2D texture
            D3DSURFACE_DESC desc;
            if (SUCCEEDED(m_Texture->GetLevelDesc(0, &desc)))
            {
                m_Desc.Size.X = desc.Width;
                m_Desc.Size.Y = desc.Height;
                m_Desc.Size.Z = 0;
                if (desc.Usage == D3DUSAGE_RENDERTARGET)
                {
                    m_Desc.Usage = TextureFlags(m_Desc.Usage | VSTexFlag_Target);
                }
                m_Desc.Format = ConverterDX9::ToTextureFormat(desc.Format);
            }
        }        VOODOO_METHODDEF(VSTextureDX9::StretchRect)(_In_ ITexture * pSource, _In_ CONST Rect source, CONST Rect dest)
        {
            if (!pSource) return VSFERR_INVALIDPARAMS;

            VSTextureDX9 * pOther = nullptr;
            if (FAILED(pSource->QueryInterface(CLSID_VSTextureDX9, (IObject**)&pOther)) || !pOther)
            {
                return VSFERR_INVALIDPARAMS;
            }

            if (!pOther->m_Texture)
            {
                return VSFERR_INVALIDPARAMS;
            }

            LPDIRECT3DDEVICE9 pDevice = nullptr;
            if (FAILED(m_Texture->GetDevice(&pDevice)) || !pDevice)
            {
                return VSFERR_APIERROR;
            }

            RECT src = {source.Min.X, source.Min.Y, source.Max.X, source.Max.Y};
            RECT dst = {dest.Min.X, dest.Min.Y, dest.Max.X, dest.Max.Y};

            if (SUCCEEDED(pDevice->StretchRect(pOther->m_Surface, &src, m_Surface, &dst, D3DTEXF_NONE)))
            {
                return VSF_OK;
            }

            if (FAILED(m_Texture->AddDirtyRect(&dst)))
            {
                return VSFERR_APIERROR;
            }

            if (SUCCEEDED(pDevice->UpdateTexture(pOther->m_Texture, m_Texture)))
            {
                return VSF_OK;
            }
            else
            {
                return VSFERR_APIERROR;
            }
        }

        VOODOO_METHODDEF(VSTextureDX9::CopyVolume)(_In_ ITexture * pSource, _In_ CONST Box source, CONST UInt3 dest)
        {
            UNREFERENCED_PARAMETER(pSource);
            UNREFERENCED_PARAMETER(source);
            UNREFERENCED_PARAMETER(dest);

            return VSFERR_NOTIMPLEMENTED;
        }
    }
}
