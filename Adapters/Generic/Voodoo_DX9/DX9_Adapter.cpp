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

#include "DX9_Adapter.hpp"

#include "DX9_Converter.hpp"
#include "DX9_Version.hpp"

#include "IVoodoo3D9.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        DX9Adapter::DX9Adapter(ICore * pCore) :
            m_Refs(0), m_Core(pCore), 
            m_Device(nullptr), m_VertDecl(nullptr), m_VertDeclT(nullptr), m_BoundPass(nullptr)
        { 
            m_RenderTarget[0] = m_RenderTarget[1] = m_RenderTarget[2] = m_RenderTarget[3] = nullptr;
            gpVoodooCore = pCore;
        };

        DX9Adapter::~DX9Adapter()
        {
            if (m_Device)
            {
                m_Device->Release();
            }
        }

        uint32_t DX9Adapter::AddRef(void) const throw()
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t DX9Adapter::Release(void) const throw()
        {
            uint32_t value = SAFE_DECREMENT(m_Refs);

            if (value == 0)
            {
                delete this;
            }

            return value;
        }

        bool DX9Adapter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw()
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_DX9Adapter;
                    return true;
                } else {
                    return false;
                }
            } else {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                } else if (clsid == IID_IAdapter) {
                    *ppOut = static_cast<const IAdapter*>(this);
                } else if (clsid == CLSID_DX9Adapter) {
                    *ppOut = static_cast<const DX9Adapter*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String DX9Adapter::ToString(void) const throw()
        {
            return L"DX9Adapter()";
        }

        ICore * DX9Adapter::GetCore(void) const throw()
        {
            return m_Core;
        }

        bool DX9Adapter::LoadPass(_In_ IPass * const pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pPass)
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Attempting to load null pass.");
                return false;
            }

            IPassRef pass(pPass);
            
            CGprogram vertProg = pass->GetProgram(PS_Vertex);
            CGprogram fragProg = pass->GetProgram(PS_Fragment);
            HRESULT hr = S_OK;

            if (cgIsProgram(vertProg))
            {
                hr = cgD3D9LoadProgram(vertProg, CG_TRUE, 0);
                if (!SUCCEEDED(hr))
                {
                    logger->Log
                    (
                        LL_ModError, VOODOO_DX9_NAME, 
                        L"Error loading vertex program from '%s': %d.\n",
                        pass->GetName().GetData(),
                        hr
                    );

                    return false;
                }
            }

            if (cgIsProgram(fragProg))
            {
                hr = cgD3D9LoadProgram(fragProg, CG_TRUE, 0);
                if (!SUCCEEDED(hr))
                {
                    logger->Log
                    (
                        LL_ModError, VOODOO_DX9_NAME,
                        L"Error loading fragment program from '%s': %d.\n",
                        pass->GetName().GetData(),
                        hr
                    );
                    return false;
                }
            }

            logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Successfully loaded programs from '%s'.", pass->GetName().GetData());
            return true;
        }

        bool DX9Adapter::UnloadPass(_In_ IPass * const pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pPass)
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Attempting to load null pass.");
                return false;
            }

            IPassRef pass(pPass);

            CGprogram vertProg = pass->GetProgram(PS_Vertex);
            CGprogram fragProg = pass->GetProgram(PS_Fragment);
            HRESULT hr = S_OK;

            if (cgIsProgram(vertProg))
            {
                hr = cgD3D9UnloadProgram(vertProg);
                if (!SUCCEEDED(hr))
                {
                    logger->Log
                        (
                        LL_ModError, VOODOO_DX9_NAME, 
                        L"Error loading vertex program from '%s': %d.\n",
                        pass->GetName().GetData(),
                        hr
                        );

                    return false;
                }
            }

            if (cgIsProgram(fragProg))
            {
                hr = cgD3D9UnloadProgram(fragProg);
                if (!SUCCEEDED(hr))
                {
                    logger->Log
                        (
                        LL_ModError, VOODOO_DX9_NAME,
                        L"Error loading fragment program from '%s': %d.\n",
                        pass->GetName().GetData(),
                        hr
                        );
                    return false;
                }
            }

            return true;
        }

        bool DX9Adapter::SetPass(_In_opt_ IPass * const pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pPass)
            {
                if (m_BoundPass)
                {
                    cgResetPassState(m_BoundPass->GetCgPass());
                }
                return false;
            }

            m_BoundPass = pPass;

            cgSetPassState(m_BoundPass->GetCgPass());

            return true;
        }

        IPass * DX9Adapter::GetPass(void) const
        {
            return m_BoundPass.get();
        }

        bool DX9Adapter::SetTarget(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pTarget)
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Attempting to set null render target %d.", index);
                return false;
            } else if (index > 4) {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Invalid render target index %d.", index);
                return false;
            }

            m_RenderTarget[index] = pTarget;

            IDirect3DTexture9 * pD3D9Tex = reinterpret_cast<IDirect3DTexture9*>(pTarget->GetData());

            if (!pD3D9Tex)
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Attempting to bind texture '%s' to render target %d, null data.", pTarget->GetName().GetData(), index);
                return false;
            }

            pD3D9Tex->AddRef();

            IDirect3DSurface9 * pD3D9Surf = nullptr;
            HRESULT result = pD3D9Tex->GetSurfaceLevel(0, &pD3D9Surf);
            if (!SUCCEEDED(result))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to get surface of texture '%s', data %p: %d", pTarget->GetName().GetData(), pD3D9Tex, result);
                pD3D9Tex->Release();
                return false;
            }

            result = m_Device->SetRenderTarget(index, pD3D9Surf);
            if (!SUCCEEDED(result))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Device failed to bind texture '%s' to render target %d: %d", pTarget->GetName().GetData(), index, result);
                pD3D9Tex->Release();
                return false;
            } else {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Bound texture '%s' to render target %d: %d", pTarget->GetName().GetData(), index, result);
            }

            pD3D9Surf->Release();
            pD3D9Tex->Release();

            return false;
        }

        ITexture * DX9Adapter::GetTarget(_In_ const uint32_t index) const
        {
            if (index > 4) 
            {
                m_Core->GetLogger()->Log(LL_ModError, VOODOO_DX9_NAME, L"Invalid render target index %d.", index);
                return false;
            }

            return m_RenderTarget[index].get();
        }

        bool DX9Adapter::CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc, _Inout_ ITexture * const pTexture)
        {
            IDirect3DTexture9 * tex = nullptr;
            D3DFORMAT fmt = DX9_Converter::ToD3DFormat(pDesc->Format);

            HRESULT hr = m_Device->CreateTexture
            (
                pDesc->Size.X,
                pDesc->Size.Y,
                pDesc->Mipmaps?0:1,
                D3DUSAGE_RENDERTARGET,
                fmt,
                D3DPOOL_DEFAULT,
                &tex,
                nullptr
            );

            if (SUCCEEDED(hr))
            {
                pTexture->SetData(reinterpret_cast<void*>(tex));
            }
            else
            {
                const char * error = cgD3D9TranslateHRESULT(hr);

                m_Core->GetLogger()->Log(LL_ModError, VOODOO_DX9_NAME, L"Error creating texture %s: %s", name, error);
                return nullptr;
            }

            return true;
        }

        bool DX9Adapter::LoadTexture(_In_ IImage * const pFile, _In_opt_ const TextureRegion * pRegion, _Inout_ ITexture * const pTexture)
        {
            //! @todom Load texture data into memory.
            return this->CreateTexture(pFile->GetPath(), reinterpret_cast<const TextureDesc *>(pRegion), pTexture);
        }

        bool DX9Adapter::DrawGeometry
        (
            _In_ const uint32_t count, 
            _In_count_(count) VertexStruct * const pVertexData, 
            _In_ const VertexFlags flags
        )
        { 
            IDirect3DVertexBuffer9 * sourceBuffer;
            UINT sourceOffset, sourceStride;
            DWORD sourceFVF, zEnabled, aEnabled, cullMode;
            IDirect3DVertexDeclaration9 * sourceDecl;

            m_Device->GetStreamSource(0, &sourceBuffer, &sourceOffset, &sourceStride);
            m_Device->GetVertexDeclaration(&sourceDecl);
            m_Device->GetFVF(&sourceFVF);
            m_Device->GetRenderState(D3DRS_ZENABLE, &zEnabled);
            m_Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &aEnabled);
            m_Device->GetRenderState(D3DRS_CULLMODE, &cullMode);

            if (flags & VF_Transformed)
            {
                m_Device->SetVertexDeclaration(m_VertDeclT);
            } else {
                m_Device->SetVertexDeclaration(m_VertDecl);
            }
            m_Device->SetFVF(0);
            m_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
            m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

            if (pVertexData)
            {
                HRESULT hr = m_Device->BeginScene();

                if (SUCCEEDED(hr))
                {
                    if (flags & VF_Buffer)
                    {
                        IDirect3DVertexBuffer9 * vbuffer = reinterpret_cast<IDirect3DVertexBuffer9*>(pVertexData);
                        m_Device->SetStreamSource(0, vbuffer, 0, sizeof(VertexStruct));
                        m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, count);
                    } else {
                        m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, count, pVertexData, sizeof(VertexStruct));
                    }

                    m_Device->EndScene();
                }
                else
                {
                    m_Core->GetLogger()->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to draw geometry.");
                }
            }

            m_Device->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
            m_Device->SetVertexDeclaration(sourceDecl);
            m_Device->SetFVF(sourceFVF);
            m_Device->SetRenderState(D3DRS_ZENABLE, zEnabled);
            m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, aEnabled);
            m_Device->SetRenderState(D3DRS_CULLMODE, cullMode);

            return true;
        }

        bool DX9Adapter::ApplyParameter(_In_ IParameter * const pParam)
        {
            if (!pParam)
            {
                m_Core->GetLogger()->Log(LL_ModError, VOODOO_DX9_NAME, L"Attempting to apply null parameter.");
                return false;
            }

            IParameterRef param(pParam);

            switch (Converter::ToParameterCategory(param->GetType()))
            {
            case PC_Float:
                cgD3D9SetUniform(param->GetCgParameter(), param->GetScalar());
                break;
            case PC_Sampler:
                cgD3D9SetTextureParameter(param->GetCgParameter(), reinterpret_cast<IDirect3DTexture9 *>(param->GetTexture()->GetData()));
                break;
            case PC_Unknown:
            default:
                m_Core->GetLogger()->Log(LL_ModError, VOODOO_DX9_NAME, L"Unable to bind parameter %s of unknown type.", param->GetName().GetData());
                return false;
            }

            return true;
        }

        bool DX9Adapter::SetProperty(_In_ const wchar_t * name, _In_ Variant & value)
        {
            String strname(name);

            if (strname != L"IDirect3D9")
            {
                return false;
            } else if (value.Type != UT_PVoid) {
                // Invalid type
                return false;
            } else {
                IDirect3D9 * origObj = reinterpret_cast<IDirect3D9 *>(value.Value.VPVoid);

                IVoodoo3D9 * fakeObj = new IVoodoo3D9(origObj);

                value.Value.VPVoid = fakeObj;

                return true;
            }
        }

        Variant DX9Adapter::GetProperty(_In_ const wchar_t * property) const
        {
            Variant retVar = {UT_None, nullptr};
            return retVar;
        }

        bool DX9Adapter::ConnectTexture(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture)
        {
            if (Converter::ToParameterCategory(pParam->GetType()) == PC_Sampler)
            {
                IDirect3DTexture9 * texObj = reinterpret_cast<IDirect3DTexture9 *>(pTexture->GetData());
                CGparameter texParam = pParam->GetCgParameter();

                cgD3D9SetTextureParameter(texParam, texObj);
                m_Core->GetLogger()->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Bound texture %s to parameter %s.\n", pTexture->GetName().GetData(), pParam->GetName().GetData());
                return true;
            }
            else
            {
                m_Core->GetLogger()->Log(LL_ModError, VOODOO_DX9_NAME, L"Invalid texture binding, parameter %s is not a sampler.\n", pParam->GetName().GetData());
                return false;
            }
        }

        bool DX9Adapter::HandleError(_In_opt_ CGcontext const pContext, _In_ uint32_t error)
        {
            //! @todo Handle errors as needed.
            return false;
        }

        bool DX9Adapter::SetDXDevice(IDirect3DDevice9 * pDevice)
        {
            if (pDevice == m_Device)
            {
                return true;
            }
            else if (pDevice == nullptr || (pDevice && m_Device))
            {
                if (m_Device) m_Device->Release();
                if (m_VertDecl) m_VertDecl->Release();
                if (m_VertDeclT) m_VertDeclT->Release();

                if (!pDevice) return true;
            }

            m_Device = pDevice;
            m_Device->AddRef();

            ILoggerRef logger = m_Core->GetLogger();
            CGcontext context = m_Core->GetCgContext();

            HRESULT errors = cgD3D9SetDevice(m_Device);

            if (!SUCCEEDED(errors))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Unable to set Cg D3D9 device.");
            }
            else
            {
                logger->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Cg D3D9 device set successfully.");
            }

            cgD3D9EnableDebugTracing(CG_TRUE);

            cgD3D9SetManageTextureParameters(context, CG_TRUE);

            cgD3D9RegisterStates(context);

            errors = cgD3D9GetLastError();

            if (!SUCCEEDED(errors))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Errors registering Cg states.");
            }
            else
            {
                logger->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Cg states registered successfully.");
            }

            // Setup profiles
            CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
            CGprofile bestVert = cgD3D9GetLatestVertexProfile();

            const char * bestVertStr = cgGetProfileString(bestVert);
            const char * bestFragStr = cgGetProfileString(bestFrag);

            logger->Log
            (
                LL_ModInfo, VOODOO_DX9_NAME,
                L"Detected the following profiles:\nVertex: %S\nFragment: %S",
                bestVertStr, bestFragStr
            );

            // Create vertex declaration
            D3DVERTEXELEMENT9 vertDeclElems[] =
            {
                { 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                { 0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
                { 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
                { 0, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
                D3DDECL_END()
            };

            errors = m_Device->CreateVertexDeclaration(vertDeclElems, &m_VertDecl);

            if (!SUCCEEDED(errors))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Unable to create standard vertex declaration.");
            }

            vertDeclElems[0].Usage = D3DDECLUSAGE_POSITIONT;

            errors = m_Device->CreateVertexDeclaration(vertDeclElems, &m_VertDeclT);

            if (!SUCCEEDED(errors))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Unable to create transformed vertex declaration.");
            }

            // Get params
            D3DVIEWPORT9 viewport;

            m_Device->GetViewport(&viewport);

            float fx = (viewport.Width  / 2) + 0.5f;
            float fy = (viewport.Height / 2) + 0.5f;

            logger->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Prepping for %d by %d target.", viewport.Width, viewport.Height);

            // Create fullscreen vbuffer
            errors = m_Device->CreateVertexBuffer
            (
                6 * sizeof(VertexStruct), 0, 0, D3DPOOL_DEFAULT, &gpFSQuadVerts, nullptr
            );

            if (FAILED(errors))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to create vertex buffer.");
            }

            VertexStruct fsVertData[4];

            ZeroMemory(fsVertData, sizeof(VertexStruct) * 4);

            fsVertData[0].Position.X = -0.5f;
            fsVertData[0].Position.Y = -0.5f;
            fsVertData[0].Position.Z =  0.5f;
            fsVertData[1].Position.X =    fx;
            fsVertData[1].Position.Y = -0.5f;
            fsVertData[1].Position.Z =  0.5f;
            fsVertData[2].Position.X = -0.5f;
            fsVertData[2].Position.Y =    fy;
            fsVertData[2].Position.Z =  0.5f;
            fsVertData[3].Position.X =    fx;
            fsVertData[3].Position.Y =    fy;
            fsVertData[3].Position.Z =  0.5f;

            fsVertData[0].TexCoord[0].X = 0.0f;
            fsVertData[0].TexCoord[0].Y = 0.0f;
            fsVertData[1].TexCoord[0].X = 1.0f;
            fsVertData[1].TexCoord[0].Y = 0.0f;
            fsVertData[2].TexCoord[0].X = 0.0f;
            fsVertData[2].TexCoord[0].Y = 1.0f;
            fsVertData[3].TexCoord[0].X = 1.0f;
            fsVertData[3].TexCoord[0].Y = 1.0f;

            VertexStruct * pVertices = nullptr;
            errors = gpFSQuadVerts->Lock(0, sizeof(VertexStruct) * 6, (void**)&pVertices, 0);

            if (FAILED(errors))
            {
                logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to lock vertex buffer to fsquad.");
            } else {
                memcpy(pVertices,   fsVertData,   sizeof(VertexStruct) * 3);
                memcpy(pVertices+3, fsVertData+1, sizeof(VertexStruct) * 3);

                gpFSQuadVerts->Unlock();
            }

            return true;
        }
    }
}