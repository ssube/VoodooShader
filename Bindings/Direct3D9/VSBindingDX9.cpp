/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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
            // Start up DevIL for D3D loading
            ilInit();
            iluInit();
            ilutInit();

            if (ilutRenderer(ILUT_DIRECT3D9) != IL_TRUE)
            {
                pCore->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_D3D9_NAME, 
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

        VOODOO_METHODDEF(VSBindingDX9::QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut)
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
                else if (refid == IID_IBinding)
                {
                    *ppOut = static_cast<IBinding*>(this);
                }
                else if (refid == CLSID_VSBindingDX9)
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

        VOODOO_METHODDEF(VSBindingDX9::Init)(_In_ CONST uint32_t count, _In_reads_(count) Variant * pParams)
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
            UNREFERENCED_PARAMETER(errors);

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSBindingDX9::Reset)()
        {
            if (!m_Device) return VSFERR_INVALIDCALL;

            m_Device->Release();
            m_Device = nullptr;

            return VSF_OK;
        }

        VOODOO_METHODDEF_(IEffect *, VSBindingDX9::CreateEffect)(_In_ CONST String & source)
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
                m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_D3D9_NAME, 
                    StringFormat("Error compiling effect from source. Errors:\n%2%") << (LPCSTR)errors->GetBufferPointer());

                return nullptr;
            }

            IEffect * pEffect = new VSEffectDX9(this, effect);
            return pEffect;
        }

        VOODOO_METHODDEF_(IEffect *, VSBindingDX9::CreateEffectFromFile)(_In_ CONST IFile * pFile)
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
                m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_D3D9_NAME, 
                    StringFormat("Error compiling effect from file '%1%'. Errors:\n%2%") << 
                    pFile->GetPath() << (LPCSTR)errors->GetBufferPointer());

                return nullptr;
            }

            IEffect * pEffect = new VSEffectDX9(this, effect);
            return pEffect;
        }

        VOODOO_METHODDEF_(IParameter *, VSBindingDX9::CreateParameter)(_In_ CONST String & name, _In_ ParameterDesc desc)
        {
            IParameter * pParam = new VSParameterDX9(this, name, desc);
            return pParam;
        }

        D3DFORMAT DepthFormats[] =
        {
            ((D3DFORMAT)(MAKEFOURCC('I','N','T','Z'))),
            ((D3DFORMAT)(MAKEFOURCC('D','F','2','4'))),
            ((D3DFORMAT)(MAKEFOURCC('D','F','1','6'))),
            ((D3DFORMAT)(MAKEFOURCC('R','A','W','Z'))),
            ((D3DFORMAT)0)
        };

        VOODOO_METHODDEF_(ITexture *, VSBindingDX9::CreateTexture)(_In_ CONST String & name, _In_ TextureDesc desc)
        {
            if (!m_Device)
            {
                return nullptr;
            }

            DWORD usage = 0;
            if ((desc.Usage & VSTexFlag_AutoMip) > 0)   usage |= D3DUSAGE_AUTOGENMIPMAP;
            if ((desc.Usage & VSTexFlag_Dynamic) > 0)   usage |= D3DUSAGE_DYNAMIC;
            if ((desc.Usage & VSTexFlag_Target) > 0)    usage |= D3DUSAGE_RENDERTARGET;

            if (desc.Format == VSFmt_D16 || desc.Format == VSFmt_D24 || desc.Format == VSFmt_D24S8 ||
                desc.Format == VSFmt_D32 || desc.Format == VSFmt_DMax)
            {
                usage = D3DUSAGE_DEPTHSTENCIL; // | D3DUSAGE_RENDERTARGET;
                desc.Levels = 1;
            }
            
            LPDIRECT3DTEXTURE9 texture = nullptr;

            if (desc.Format == VSFmt_DMax)
            {
                uint32_t index = 0;
                while (!texture && DepthFormats[index] != 0)
                {
                    texture = Impl_CreateTexture(desc, usage, DepthFormats[index]);
                    ++index;
                }
            }
            else
            {
                D3DFORMAT format = ConverterDX9::ToD3DFormat(desc.Format);
                if (format == D3DFMT_UNKNOWN)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to convert texture format %1%.") << desc.Format);

                    return nullptr;
                }

                texture = Impl_CreateTexture(desc, usage, format);
            }

            if (!texture)
            {
                m_Core->GetLogger()->LogMessage(VSLog_BindError, VOODOO_D3D9_NAME,
                    StringFormat("Unable to create texture %1% from %2%.") << name << desc);
                return nullptr;
            }

            VSTextureDX9 * pTexture = new VSTextureDX9(this, name, texture);
            return pTexture;
        }

        LPDIRECT3DTEXTURE9 VSBindingDX9::Impl_CreateTexture(_In_ TextureDesc & desc, _In_ DWORD usage, _In_ D3DFORMAT format)
        {
            LPDIRECT3DTEXTURE9 texture = nullptr;
            HRESULT hr = m_Device->CreateTexture(desc.Size.X, desc.Size.Y, desc.Levels, 
                usage, format, D3DPOOL_DEFAULT, &texture, NULL);
            if (FAILED(hr))
            {
                return nullptr;
            } 
            else
            {
                return texture;
            }
        }

        VOODOO_METHODDEF_(ITexture *, VSBindingDX9::CreateTextureFromFile)(_In_ CONST String & name, _In_ IFile * pFile)
        {
            if (!pFile || !m_Device) // || !m_ILUT)
            {
                return nullptr;
            }

            ILuint image = ilGenImage();
            ilBindImage(image);
            if (ilLoadImage(pFile->GetPath().GetData()) == IL_FALSE)
            {
                ilBindImage(0);
                ilDeleteImage(image);
                return nullptr;
            }

            ILinfo info;
            iluGetImageInfo(&info);
            if (info.Origin == IL_ORIGIN_LOWER_LEFT)
            {
                iluFlipImage();
            }

            LPDIRECT3DTEXTURE9 texture = nullptr;
            if (info.Depth > 1)
            {
                //texture = ilutD3D9VolumeTexture(m_Device);
                m_Core->GetLogger()->LogMessage(VSLog_BindError, VOODOO_D3D9_NAME, VSTR("Volume textures are not yet supported."));
            }
            else
            {
                texture = ilutD3D9Texture(m_Device);
            }

            if (!texture)
            {
                m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                    StringFormat("Failed to load texture '%1%' from file '%2%'.") << name << pFile->GetPath());
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
