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

#include "VSBindingOGL.hpp"
#include "VSEffectOGL.hpp"
#include "VSParameterOGL.hpp"
#include "VSTextureOGL.hpp"

#include "ConverterOGL.hpp"
#include "OGL_Version.hpp"

#include "IL\ilut.h"

namespace VoodooShader
{
    namespace Voodoo_OGL
    {
        VSBindingOGL::VSBindingOGL(_In_ ICore * pCore)
            : m_Refs(0), m_Core(pCore), m_Device(nullptr)
        {
            // Start up DevIL for D3D loading
            ilInit();
            iluInit();
            ilutInit();

            if (ilutRenderer(ILUT_OPENGL) != IL_TRUE)
            {
                pCore->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_OGL_NAME, 
                    VSTR("Unable to initialize ILUT for image loading."));
                m_ILUT = false;
            }
            else
            {
                m_ILUT = true;
            }

            //! @todo Initialize Mojoshader
        }

        VSBindingOGL::~VSBindingOGL()
        {
            //! @todo Shutdown Mojoshader
        }

        VOODOO_METHODDEF_(uint32_t, VSBindingOGL::AddRef)() CONST
        {
            return SAFE_INCREMENT(m_Refs);
        }

        VOODOO_METHODDEF_(uint32_t, VSBindingOGL::Release)() CONST
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

        VOODOO_METHODDEF(VSBindingOGL::QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut)
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
                else if (refid == CLSID_VSBindingOGL)
                {
                    *ppOut = static_cast<VSBindingOGL*>(this);
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

        VOODOO_METHODDEF_(String, VSBindingOGL::ToString)() CONST
        {
            return VSTR("VSBindingOGL()");
        }

        VOODOO_METHODDEF_(ICore *, VSBindingOGL::GetCore)() CONST
        {
            return m_Core;
        }

        VOODOO_METHODDEF(VSBindingOGL::Init)(_In_ CONST uint32_t count, _In_reads_(count) Variant * pParams)
        {
            if (count != 2 || !pParams)
            {
                return VSFERR_INVALIDPARAMS;
            }

            if (pParams[0].Type != VSUT_PVoid || !pParams[0].VPVoid || pParams[1].Type != VSUT_PVoid || !pParams[1].VPVoid)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Context = reinterpret_cast<HGLRC>(pParams[0].VPVoid);
            m_Device = reinterpret_cast<HDC>(pParams[1].VPVoid);

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSBindingOGL::Reset)()
        {
            if (!m_Device || !m_Context)
            {
                return VSFERR_INVALIDCALL;
            }

            m_Device = nullptr;
            m_Context = nullptr;

            return VSF_OK;
        }

        VOODOO_METHODDEF_(IEffect *, VSBindingOGL::CreateEffect)(_In_ CONST String & source)
        {
            if (!m_Device)
            {
                return nullptr;
            }

            MOJOSHADER_parseEffect("arb1", )

            // Output/buffer
            std::string asource = source.ToStringA();
            LPD3DXEFFECT effect = NULL;
            LPD3DXBUFFER errors = NULL;

            HRESULT hr = D3DXCreateEffect(m_Device, asource.c_str(), asource.length(), NULL, NULL, 0, NULL, 
                &effect, &errors);
            if (FAILED(hr))
            {
                m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_OGL_NAME, 
                    StringFormat("Error compiling effect from source. Errors:\n%2%") << (LPCSTR)errors->GetBufferPointer());

                return nullptr;
            }

            IEffect * pEffect = new VSEffectOGL(this, effect);
            return pEffect;
        }

        VOODOO_METHODDEF_(IEffect *, VSBindingOGL::CreateEffectFromFile)(_In_ CONST IFile * pFile)
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
                m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_OGL_NAME, 
                    StringFormat("Error compiling effect from file '%1%'. Errors:\n%2%") << 
                    pFile->GetPath() << (LPCSTR)errors->GetBufferPointer());

                return nullptr;
            }

            IEffect * pEffect = new VSEffectOGL(this, effect);
            return pEffect;
        }

        VOODOO_METHODDEF_(IParameter *, VSBindingOGL::CreateParameter)(_In_ CONST String & name, _In_ ParameterDesc desc)
        {
            IParameter * pParam = new VSParameterOGL(this, name, desc);
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

        VOODOO_METHODDEF_(ITexture *, VSBindingOGL::CreateTexture)(_In_ CONST String & name, _In_ TextureDesc desc)
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
                D3DFORMAT format = ConverterOGL::ToD3DFormat(desc.Format);
                if (format == D3DFMT_UNKNOWN)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_OGL_NAME, 
                        StringFormat("Unable to convert texture format %1%.") << desc.Format);

                    return nullptr;
                }

                texture = Impl_CreateTexture(desc, usage, format);
            }

            if (!texture)
            {
                m_Core->GetLogger()->LogMessage(VSLog_BindError, VOODOO_OGL_NAME,
                    StringFormat("Unable to create texture %1% from %2%.") << name << desc);
                return nullptr;
            }

            VSTextureOGL * pTexture = new VSTextureOGL(this, name, texture);
            return pTexture;
        }

        LPDIRECT3DTEXTURE9 VSBindingOGL::Impl_CreateTexture(_In_ TextureDesc & desc, _In_ DWORD usage, _In_ D3DFORMAT format)
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

        VOODOO_METHODDEF_(ITexture *, VSBindingOGL::CreateTextureFromFile)(_In_ CONST String & name, _In_ IFile * pFile)
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
                //texture = ilutOGLVolumeTexture(m_Device);
                m_Core->GetLogger()->LogMessage(VSLog_BindError, VOODOO_OGL_NAME, VSTR("Volume textures are not yet supported."));
            }
            else
            {
                texture = ilutOGLTexture(m_Device);
            }

            if (!texture)
            {
                m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_OGL_NAME, 
                    StringFormat("Failed to load texture '%1%' from file '%2%'.") << name << pFile->GetPath());
                return nullptr;
            }

            ITexture * pTexture = new VSTextureOGL(this, name, texture);
            return pTexture;
        }

        VOODOO_METHODDEF_(ITexture *, VSBindingOGL::CreateNullTexture)()
        {
            if (!m_Device)
            {
                return nullptr;
            }

            ITexture * pTexture = new VSTextureOGL(this);
            return pTexture;
        }

        VOODOO_METHODDEF_(void, VSBindingOGL::ResetState)()
        {
            m_InitialState->Apply();
        }

        VOODOO_METHODDEF_(void, VSBindingOGL::PushState)()
        {
            LPDIRECT3DSTATEBLOCK9 newBlock = nullptr;
            m_Device->CreateStateBlock(D3DSBT_ALL, &newBlock);
            newBlock->AddRef();
            m_StateStack.push(newBlock);
        }

        VOODOO_METHODDEF_(void, VSBindingOGL::PopState)()
        {
            LPDIRECT3DSTATEBLOCK9 oldBlock = m_StateStack.top();
            oldBlock->Apply();
            m_StateStack.pop();
            oldBlock->Release();
        }
    }
}
