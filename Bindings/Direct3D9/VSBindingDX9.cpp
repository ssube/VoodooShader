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
#pragma once

#include "VSBindingDX9.hpp"
#include "VSEffectDX9.hpp"
#include "VSParameterDX9.hpp"
#include "VSTextureDX9.hpp"

#include "ConverterDX9.hpp"
#include "D3D9_Version.hpp"

#include "IL\ilut.h"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        VSBindingDX9::VSBindingDX9(_In_ ICore * pCore)
            : m_Refs(0), m_Core(pCore), m_Device(nullptr)
        {
            if (ilutRenderer(ILUT_DIRECT3D9) != IL_TRUE)
            {
                pCore->GetLogger()->LogMessage(VSLog_ModError, VOODOO_D3D9_NAME, 
                    VSTR("Unable to initialize ILUT for image loading."));
                m_ILUT = false;
            }
            else
            {
                m_ILUT = true;
            }
        }

        VSBindingDX9::~VSBindingDX9()
        {
            if (m_Device)
            {
                m_Device->Release();
            }
        }

        VOODOO_METHODDEF_(uint32_t, VSBindingDX9::AddRef)() CONST
        {
            return SAFE_INCREMENT(m_Refs);
        }

        VOODOO_METHODDEF_(uint32_t, VSBindingDX9::Release)() CONST
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

        VOODOO_METHODDEF(VSBindingDX9::QueryInterface)(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut)
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
                    *ppOut = static_cast<IBinding*>(this);
                }
                else if (refid == CLSID_VSTextureDX9)
                {
                    *ppOut = static_cast<VSBindingDX9*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return VSFERR_INVALIDUUID;
                }

                (*ppOut)->AddRef();
                return VSF_OK;
            }
        }

        VOODOO_METHODDEF_(String, VSBindingDX9::ToString)() CONST
        {
            return VSTR("VSBindingDX9()");
        }

        VOODOO_METHODDEF_(ICore *, VSBindingDX9::GetCore)() CONST
        {
            return m_Core;
        }

        VOODOO_METHODDEF(VSBindingDX9::Init)(uint32_t count, _In_count_(count) Variant * pParams)
        {
            if (count != 1 || !pParams)
            {
                return VSFERR_INVALIDPARAMS;
            }

            if (pParams[0].Type != VSUT_PVoid || !pParams[0].VPVoid)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Device = reinterpret_cast<LPDIRECT3DDEVICE9>(pParams[0].VPVoid);
            m_Device->AddRef();

            HRESULT errors = m_Device->CreateStateBlock(D3DSBT_ALL, &m_InitialState);
            assert(SUCCEEDED(errors));

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSBindingDX9::Reset)()
        {
            if (!m_Device) return VSFERR_INVALIDCALL;

            m_Device->Release();
            m_Device = nullptr;

            return VSF_OK;
        }

        VOODOO_METHODDEF_(IEffect *, VSBindingDX9::CreateEffect)(CONST String & source)
        {
            if (!m_Device)
            {
                return nullptr;
            }

            // Output/buffer
            std::string asource = source.ToStringA();
            LPD3DXEFFECT effect = NULL;
            LPD3DXBUFFER errors = NULL;

            HRESULT hr = D3DXCreateEffect(m_Device, asource.c_str(), asource.length(), NULL, NULL, 0, NULL, 
                &effect, &errors);
            if (FAILED(hr))
            {
                m_Core->GetLogger()->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    Format("Error compiling effect from source. Errors:\n%2%") << (LPCSTR)errors->GetBufferPointer());

                return nullptr;
            }

            IEffect * pEffect = new VSEffectDX9(this, effect);
            return pEffect;
        }

        VOODOO_METHODDEF_(IEffect *, VSBindingDX9::CreateEffectFromFile)(CONST IFile * pFile)
        {
            if (!m_Device)
            {
                return nullptr;
            }

            // Output/buffer
            LPD3DXEFFECT effect = NULL;
            LPD3DXBUFFER errors = NULL;

            HRESULT hr = D3DXCreateEffectFromFile(m_Device, pFile->GetPath().GetData(), NULL, NULL, 0, NULL, 
                &effect, &errors);
            if (FAILED(hr))
            {
                m_Core->GetLogger()->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    Format("Error compiling effect from file '%1%'. Errors:\n%2%") << 
                    pFile->GetPath() << (LPCSTR)errors->GetBufferPointer());

                return nullptr;
            }

            IEffect * pEffect = new VSEffectDX9(this, effect);
            return pEffect;
        }

        VOODOO_METHODDEF_(IParameter *, VSBindingDX9::CreateParameter)(CONST String & name, ParameterDesc desc)
        {
            IParameter * pParam = new VSParameterDX9(this, name, desc);
            return pParam;
        }

        VOODOO_METHODDEF_(ITexture *, VSBindingDX9::CreateTexture)(CONST String & name, TextureDesc desc)
        {
            if (!m_Device)
            {
                return nullptr;
            }

            DWORD usage = 0;
            if ((desc.Usage & VSTexFlag_AutoMip) > 0) usage |= D3DUSAGE_AUTOGENMIPMAP;
            if ((desc.Usage & VSTexFlag_Dynamic) > 0) usage |= D3DUSAGE_DYNAMIC;
            if ((desc.Usage & VSTexFlag_Target) > 0) usage |= D3DUSAGE_RENDERTARGET;

            D3DFORMAT format = ConverterDX9::ToD3DFormat(desc.Format);
            if (format == D3DFMT_UNKNOWN)
            {
                return nullptr;
            }

            LPDIRECT3DTEXTURE9 texture = nullptr;
            HRESULT hr = m_Device->CreateTexture(desc.Size.X, desc.Size.Y, desc.Levels, 
                usage, format, D3DPOOL_DEFAULT, &texture, nullptr);
            if (FAILED(hr))
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModWarning, VOODOO_D3D9_NAME, 
                    Format("Failed to create texture '%1%'.") << name);
                return nullptr;
            }

            ITexture * pTexture = new VSTextureDX9(this, name, texture);
            return pTexture;
        }

        VOODOO_METHODDEF_(ITexture *, VSBindingDX9::CreateTextureFromFile)(CONST String & name, IFile * pFile)
        {
            if (!pFile || !m_Device || !m_ILUT)
            {
                return nullptr;
            }

            LPDIRECT3DTEXTURE9 texture = nullptr;
            if (ilutD3D9TexFromFile(m_Device, pFile->GetPath().GetData(), &texture) != IL_TRUE)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModWarning, VOODOO_D3D9_NAME, 
                    Format("Failed to load texture '%1%' from file '%2%'.") << name << pFile->GetPath());
                return nullptr;
            }

            ITexture * pTexture = new VSTextureDX9(this, name, texture);
            return pTexture;
        }

        VOODOO_METHODDEF_(ITexture *, VSBindingDX9::CreateNullTexture)()
        {
            if (!m_Device)
            {
                return nullptr;
            }

            ITexture * pTexture = new VSTextureDX9(this);
            return pTexture;
        }

        VOODOO_METHODDEF_(void, VSBindingDX9::ResetState)()
        {
            m_InitialState->Apply();
        }

        VOODOO_METHODDEF_(void, VSBindingDX9::PushState)()
        {
            LPDIRECT3DSTATEBLOCK9 newBlock = nullptr;
            m_Device->CreateStateBlock(D3DSBT_ALL, &newBlock);
            newBlock->AddRef();
            m_StateStack.push(newBlock);
        }

        VOODOO_METHODDEF_(void, VSBindingDX9::PopState)()
        {
            LPDIRECT3DSTATEBLOCK9 oldBlock = m_StateStack.top();
            oldBlock->Apply();
            m_StateStack.pop();
            oldBlock->Release();
        }
    }
}
