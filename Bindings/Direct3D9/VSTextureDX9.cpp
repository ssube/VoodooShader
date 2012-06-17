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
#include "VSBindingDX9.hpp"
#include "ConverterDX9.hpp"
#include "D3D9_Version.hpp"
// Voodoo Core
#include "StringFormat.hpp"
// DevIL
#include "IL\ilut.h"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        VSTextureDX9::VSTextureDX9(_In_ VSBindingDX9 * pBinding) :
			m_Refs(0), m_Binding(pBinding), m_DXTexture(nullptr), m_DXSurface(nullptr),
			m_BoundSourceSlot(VOODOO_TEXTURE_INVALID), m_BoundTargetSlot(VOODOO_TEXTURE_INVALID)
        {

        }

        VSTextureDX9::VSTextureDX9(_In_ VSBindingDX9 * pBinding, _In_ CONST String & name, _In_ IDirect3DTexture9 * pTexture) :
            m_Refs(0), m_Binding(pBinding), m_Name(name), m_DXTexture(pTexture), m_DXSurface(nullptr), 
            m_BoundSourceSlot(VOODOO_TEXTURE_INVALID), m_BoundTargetSlot(VOODOO_TEXTURE_INVALID)
        {
            if (!m_Binding) Throw(VOODOO_D3D9_NAME, VSTR("Unable to create texture with no binding."), nullptr);

            m_Core = m_Binding->m_Core;

            if (!m_Core) Throw(VOODOO_D3D9_NAME, VSTR("Unable to create texture from binding with no core."), nullptr);
            if (!m_DXTexture) Throw(VOODOO_D3D9_NAME, VSTR("Unable to create texture with no hardware texture."), m_Core);
            
            if (m_DXTexture)
            {
                m_DXTexture->AddRef();
                m_DXTexture->GetSurfaceLevel(0, &m_DXSurface);

                this->GetTexDesc();
            }
        }

        VSTextureDX9::~VSTextureDX9()
        {
            if (m_DXTexture)
            {
                m_DXTexture->Release();
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

        VoodooResult VOODOO_METHODTYPE VSTextureDX9::QueryInterface(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut)
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
            return StringFormat("VSTextureDX9(%s)") << m_Name;
        }

        ICore * VOODOO_METHODTYPE VSTextureDX9::GetCore() CONST
        {
            return m_Core;
        }

        String VOODOO_METHODTYPE VSTextureDX9::GetName() CONST
        {
            return m_Name;
        }

        VoodooResult VOODOO_METHODTYPE VSTextureDX9::GetProperty(_In_ CONST Uuid propid, _Out_ Variant * pValue) CONST
        {
            if (!pValue) return VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3D9Texture)
            {
                pValue->Type = VSUT_PVoid;
                pValue->Components = 0;
                pValue->VPVoid = m_DXTexture;
                return VSF_OK;
            }
            else if (propid == PropIds::D3D9Surface)
            {
                pValue->Type = VSUT_PVoid;
                pValue->Components = 0;
                pValue->VPVoid = m_DXSurface;
                return VSF_OK;
            }

			*pValue = CreateVariant();
            return VSFERR_INVALIDPARAMS;
        }

        VoodooResult VOODOO_METHODTYPE VSTextureDX9::SetProperty(_In_ CONST Uuid propid, _In_ Variant * pValue)
        {
            UNREFERENCED_PARAMETER(propid);

            if (!pValue) return VSFERR_INVALIDPARAMS;

            return VSFERR_INVALIDCALL;
        }

        VOODOO_METHODDEF(VSTextureDX9::Bind)(_In_ TextureMode mode, _In_ uint32_t index)
        {
            if (mode == VSTexMode_Source)
            {
                if (index >= 8)
                {
                    return VSFERR_INVALIDPARAMS;
                }
                else if ((m_Binding->m_BoundSourceTexture[index] &&
                         m_Binding->m_BoundSourceTexture[index] != this) || 
                         m_BoundSourceSlot != VOODOO_TEXTURE_INVALID)
                {
                    return VSFERR_INVALIDCALL;
                }

                m_Binding->m_Device->GetTexture(index, &m_ReplacedSource);
                if (SUCCEEDED(m_Binding->m_Device->SetTexture(index, m_DXTexture)))
                {
                    m_Binding->m_BoundSourceTexture[index] = this;
                    m_BoundSourceSlot = index;
                    return VSF_OK;
                }
                else
                {
                    return VSF_FAIL;
                }
            }
            else if (mode == VSTexMode_Target)
            {
                if (index >= 4)
                {
                    return VSFERR_INVALIDPARAMS;
                }
                else if ((m_Binding->m_BoundTargetTexture[index] &&
                        m_Binding->m_BoundTargetTexture[index] != this) || 
                         m_BoundTargetSlot != VOODOO_TEXTURE_INVALID)
                {
                    return VSFERR_INVALIDCALL;
                }

                m_Binding->m_Device->GetRenderTarget(index, &m_ReplacedTarget);
                if (SUCCEEDED(m_Binding->m_Device->SetRenderTarget(index, m_DXSurface)))
                {
                    m_Binding->m_BoundTargetTexture[index] = this;
                    m_BoundTargetSlot = index;
                    return VSF_OK;
                }
                else
                {
                    return VSF_FAIL;
                }
            }
            else
            {
                return VSFERR_INVALIDPARAMS;
            }
        }

        VOODOO_METHODDEF(VSTextureDX9::Reset)(_In_ TextureMode mode)
        {
            if (mode == VSTexMode_Source)
            {
                if (m_BoundSourceSlot == VOODOO_TEXTURE_INVALID || 
                    m_BoundSourceSlot >= 8 ||
                    m_Binding->m_BoundSourceTexture[m_BoundSourceSlot] != this)
                {
                    return VSFERR_INVALIDCALL;
                }

                if (SUCCEEDED(m_Binding->m_Device->SetTexture(m_BoundSourceSlot, m_ReplacedSource)))
                {
                    m_BoundSourceSlot = VOODOO_TEXTURE_INVALID;
                    m_ReplacedSource = nullptr;
                    return VSF_OK;
                }
                else
                {
                    return VSF_FAIL;
                }
            }
            else if (mode == VSTexMode_Target)
            {
                if (m_BoundTargetSlot == VOODOO_TEXTURE_INVALID || 
                    m_BoundTargetSlot >= 4 ||
                    m_Binding->m_BoundTargetTexture[m_BoundTargetSlot] != this)
                {
                    return VSFERR_INVALIDCALL;
                }

                if (SUCCEEDED(m_Binding->m_Device->SetRenderTarget(m_BoundTargetSlot, m_ReplacedTarget)))
                {
                    m_BoundTargetSlot = VOODOO_TEXTURE_INVALID;
                    m_ReplacedTarget = nullptr;
                    return VSF_OK;
                }
                else
                {
                    return VSF_FAIL;
                }
            }
            else
            {
                return VSFERR_INVALIDPARAMS;
            }
        }

        TextureDesc VOODOO_METHODTYPE VSTextureDX9::GetDesc() CONST
        {
            return m_Desc;
        }

        void VSTextureDX9::GetTexDesc()
        {
            ZeroMemory(&m_Desc, sizeof(TextureDesc));

            if (!m_DXTexture) return;

            if (m_DXTexture->GetLevelCount() > 0)
            {
                m_Desc.Usage = TextureFlags(m_Desc.Usage | VSTexFlag_MipMaps);
            }

            // Test for 2D texture
            D3DSURFACE_DESC desc;
            if (SUCCEEDED(m_DXTexture->GetLevelDesc(0, &desc)))
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
        }

        VOODOO_METHODDEF(VSTextureDX9::StretchRect)(_In_ ITexture * pSource, _In_ CONST Rect source, _In_ CONST Rect dest)
        {
            if (!pSource) return VSFERR_INVALIDPARAMS;

            VSTextureDX9 * pOther = nullptr;
            if (FAILED(pSource->QueryInterface(CLSID_VSTextureDX9, (IObject**)&pOther)) || !pOther)
            {
                return VSFERR_INVALIDPARAMS;
            }

            if (!pOther->m_DXTexture)
            {
                return VSFERR_INVALIDPARAMS;
            }

            LPDIRECT3DDEVICE9 pDevice = nullptr;
            if (FAILED(m_DXTexture->GetDevice(&pDevice)) || !pDevice)
            {
                return VSFERR_APIERROR;
            }

            RECT src = {source.Min.X, source.Min.Y, source.Max.X, source.Max.Y};
            RECT dst = {dest.Min.X, dest.Min.Y, dest.Max.X, dest.Max.Y};

            if (SUCCEEDED(pDevice->StretchRect(pOther->m_DXSurface, &src, m_DXSurface, &dst, D3DTEXF_NONE)))
            {
                return VSF_OK;
            }

            if (FAILED(m_DXTexture->AddDirtyRect(&dst)))
            {
                return VSFERR_APIERROR;
            }

            if (SUCCEEDED(pDevice->UpdateTexture(pOther->m_DXTexture, m_DXTexture)))
            {
                return VSF_OK;
            }
            else
            {
                return VSFERR_APIERROR;
            }
        }

        VOODOO_METHODDEF(VSTextureDX9::CopyVolume)(_In_ ITexture * pSource, _In_ CONST Box source, _In_ CONST UInt3 dest)
        {
            UNREFERENCED_PARAMETER(pSource);
            UNREFERENCED_PARAMETER(source);
            UNREFERENCED_PARAMETER(dest);

            return VSFERR_NOTIMPLEMENTED;
        }
    }
}
