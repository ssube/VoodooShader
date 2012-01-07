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

#include "DX9_Adapter.hpp"
// Voodoo DX9 Adapter
#include "DX9_Converter.hpp"
#include "DX9_Texture.hpp"
#include "DX9_Version.hpp"
// CVoodoo3D
#include "CVoodoo3D8.hpp"
#include "CVoodoo3D9.hpp"
// Voodoo Core
#include "Format.hpp"
#include "Support.inl"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        DX9Adapter::DX9Adapter(ICore * pCore) :
            m_Refs(0), m_Core(pCore),
            m_SdkVersion(D3D_SDK_VERSION), m_Device(nullptr), m_VertDecl(nullptr), m_VertDeclT(nullptr),
            m_BoundPass(nullptr), m_BackBuffer(nullptr)
        {
            gpVoodooCore = m_Core;
            gpVoodooLogger = gpVoodooCore->GetLogger();
        };

        DX9Adapter::~DX9Adapter()
        {
            if (m_Device)
            {
                m_Device->Release();
            }
        }

        uint32_t VOODOO_METHODTYPE DX9Adapter::AddRef() const
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE DX9Adapter::Release() const
        {
            uint32_t value = SAFE_DECREMENT(m_Refs);

            if (value == 0)
            {
                delete this;
            }

            return value;
        }

        bool VOODOO_METHODTYPE DX9Adapter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_DX9Adapter;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                }
                else if (clsid == IID_IAdapter)
                {
                    *ppOut = static_cast<const IAdapter*>(this);
                }
                else if (clsid == CLSID_DX9Adapter)
                {
                    *ppOut = static_cast<const DX9Adapter*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String VOODOO_METHODTYPE DX9Adapter::ToString() const
        {
            return VSTR("DX9Adapter()");
        }

        ICore * VOODOO_METHODTYPE DX9Adapter::GetCore() const
        {
            return m_Core;
        }

        bool VOODOO_METHODTYPE DX9Adapter::LoadPass(_In_ IPass * const pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pPass)
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Attempting to load null pass."));
                return false;
            }
            else
            {
                return true;
            }

            /*IPassRef pass(pPass);

            CGprogram vertProg = pass->GetProgram(PS_Vertex);
            CGprogram fragProg = pass->GetProgram(PS_Fragment);
            HRESULT hr = S_OK;

            if (cgIsProgram(vertProg))
            {
                hr = cgD3D9LoadProgram(vertProg, CG_TRUE, 0);
                if (!SUCCEEDED(hr))
                {
                    logger->LogMessage
                    (
                        LL_ModError, VOODOO_DX89_NAME,
                        Format("Error loading vertex program from '%1%': %2%.") << pass << hr
                    );

                    return false;
                }
            }

            if (cgIsProgram(fragProg))
            {
                hr = cgD3D9LoadProgram(fragProg, CG_TRUE, 0);
                if (!SUCCEEDED(hr))
                {
                    logger->LogMessage
                    (
                        LL_ModError, VOODOO_DX89_NAME,
                        Format("Error loading fragment program from '%1%': %2%.") << pass << hr
                    );
                    return false;
                }
            }

            logger->LogMessage(LL_ModInfo, VOODOO_DX89_NAME, Format("Successfully loaded programs from '%1%'.") << pass);
            return true;*/
        }

        bool VOODOO_METHODTYPE DX9Adapter::UnloadPass(_In_ IPass * const pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pPass)
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Attempting to load null pass."));
                return false;
            }
            else
            {
                return true;
            }

            /*IPassRef pass(pPass);

            CGprogram vertProg = pass->GetProgram(PS_Vertex);
            CGprogram fragProg = pass->GetProgram(PS_Fragment);
            HRESULT hr = S_OK;

            if (cgIsProgram(vertProg))
            {
                hr = cgD3D9UnloadProgram(vertProg);
                if (!SUCCEEDED(hr))
                {
                    logger->LogMessage
                    (
                        LL_ModError, VOODOO_DX89_NAME,
                        Format("Error unloading vertex program from '%1%': %2%.") << pass << hr
                    );

                    return false;
                }
            }

            if (cgIsProgram(fragProg))
            {
                hr = cgD3D9UnloadProgram(fragProg);
                if (!SUCCEEDED(hr))
                {
                    logger->LogMessage
                    (
                        LL_ModError, VOODOO_DX89_NAME,
                        Format("Error loading fragment program from '%1%': %2%.") << pass << hr
                    );
                    return false;
                }
            }

            return true;*/
        }

        bool VOODOO_METHODTYPE DX9Adapter::SetPass(_In_ IPass * const pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            m_Device->CreateStateBlock(D3DSBT_ALL, &m_PassState);
            m_CleanState->Apply();

            if (!pPass)
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Unable to set null pass."));
                return false;
            }

            if (m_BoundPass && m_BoundPass != pPass)
            {
                logger->LogMessage(LL_ModWarning, VOODOO_DX89_NAME, VSTR("Setting pass without resetting previously bound pass."));
            }

            cgSetPassState(pPass->GetCgPass());

#ifdef _DEBUG
            CGerror cgerr = cgGetError();
            assert(cgerr == CG_NO_ERROR);
            HRESULT cgd3derr = cgD3D9GetLastError();
            assert(cgd3derr == D3D_OK);
#endif

            // Bind render targets
            for (uint32_t i = 0; i < 4; ++i)
            {
                ITexture * pTarget = pPass->GetTarget(i);
                this->SetTarget(i, pTarget);
            }

            m_BoundPass = pPass;

            return true;
        }

        IPass * VOODOO_METHODTYPE DX9Adapter::GetPass() CONST
        {
            return m_BoundPass.get();
        }

        bool VOODOO_METHODTYPE DX9Adapter::ResetPass(_In_ IPass * pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pPass)
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Unable to reset null pass."));
                return false;
            }

            if (m_BoundPass && m_BoundPass != pPass)
            {
                logger->LogMessage(LL_ModWarning, VOODOO_DX89_NAME, VSTR("Resetting pass different than the previously bound pass."));
            }

            cgResetPassState(pPass->GetCgPass());

#ifdef _DEBUG
            CGerror cgerr = cgGetError();
            assert(cgerr == CG_NO_ERROR);
            HRESULT cgd3derr = cgD3D9GetLastError();
            assert(cgd3derr == D3D_OK);
#endif

            m_BoundPass = nullptr;

            m_PassState->Apply();
            m_PassState->Release();

            return true;
        }

        bool DX9Adapter::SetTarget(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (index > 3) {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Invalid render target index %1%.") << index);
                return false;
            }

            m_RenderTarget[index] = pTarget;

            HRESULT result;

            if (!pTarget)
            {
                if (index == 0)
                {
                    result = m_Device->SetRenderTarget(index, m_BackBuffer);

                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(LL_ModDebug, VOODOO_DX89_NAME, Format("Bound backbuffer to render target %1%.") << index);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Device failed to bind backbuffer to render target %1%: %2%") << index << result);
                        return false;
                    }
                }
                else
                {
                    result = m_Device->SetRenderTarget(index, NULL);

                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(LL_ModDebug, VOODOO_DX89_NAME, Format("Bound null texture to render target %1%.") << index);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Device failed to bind null texture to render target %1%: %2%") << index << result);
                        return false;
                    }
                }
            }
            else
            {
                IDirect3DTexture9 * pD3D9Tex = reinterpret_cast<IDirect3DTexture9*>(pTarget->GetData());

                if (!pD3D9Tex)
                {
                    result = m_Device->SetRenderTarget(index, nullptr);
                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(LL_ModDebug, VOODOO_DX89_NAME, Format("Bound null texture data to render target %1%: %2%") << index << result);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Device failed to bind null texture data to render target %1%: %2%") << index << result);
                        return false;
                    }
                }

                pD3D9Tex->AddRef();

                IDirect3DSurface9 * pD3D9Surf = nullptr;
                result = pD3D9Tex->GetSurfaceLevel(0, &pD3D9Surf);

                if (!SUCCEEDED(result))
                {
                    logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Failed to get surface of texture '%1%', data %2%: %3%") << pTarget << pD3D9Tex << result);
                    pD3D9Surf->Release();
                    pD3D9Tex->Release();
                    return false;
                }

                result = m_Device->SetRenderTarget(index, pD3D9Surf);

                pD3D9Surf->Release();
                pD3D9Tex->Release();

                if (!SUCCEEDED(result))
                {
                    logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Device failed to bind texture '%1%' to render target %2%: %3%") << pTarget << index << result);
                    return false;
                }
                else
                {
                    logger->LogMessage(LL_ModDebug, VOODOO_DX89_NAME, Format("Bound texture '%1%' to render target %2%: %3%") << pTarget << index << result);
                    return true;
                }
            }
        }

        ITexture * VOODOO_METHODTYPE DX9Adapter::GetTarget(_In_ const uint32_t index) CONST
        {
            if (index > 3)
            {
                m_Core->GetLogger()->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Invalid render target index %1%.") << index);
                return false;
            }

            return m_RenderTarget[index].get();
        }

        ITexture * VOODOO_METHODTYPE DX9Adapter::CreateTexture(_In_ const String & name, _In_ const TextureDesc pDesc)
        {
            IDirect3DTexture9 * tex = nullptr;
            D3DFORMAT fmt = DX9_Converter::ToD3DFormat(pDesc.Format);

            HRESULT hr = m_Device->CreateTexture
            (
                pDesc.Size.X,
                pDesc.Size.Y,
                pDesc.Mipmaps ? 0 : 1,
                pDesc.RenderTarget ? D3DUSAGE_RENDERTARGET : 0,
                fmt,
                D3DPOOL_DEFAULT,
                &tex,
                nullptr
            );

            if (SUCCEEDED(hr))
            {
                ITexture * pTex = new DX9Texture(m_Core, name, tex);
                return pTex;
            }
            else
            {
                const char * error = cgD3D9TranslateHRESULT(hr);

                m_Core->GetLogger()->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("Error creating texture %1%: %2%") << name << error);
                return nullptr;
            }
        }

        bool VOODOO_METHODTYPE DX9Adapter::LoadTexture(_In_ IImage * const pFile, _In_ const TextureRegion pRegion, _Inout_ ITexture * const pTexture)
        {
            UNREFERENCED_PARAMETER(pFile);
            UNREFERENCED_PARAMETER(pRegion);
            UNREFERENCED_PARAMETER(pTexture);

            //! @todo Load texture data into memory.
            return false;
        }

        bool VOODOO_METHODTYPE DX9Adapter::DrawGeometry
        (
            _In_ const uint32_t offset,
            _In_ const uint32_t count,
            _In_ void * const pData,
            _In_ const VertexFlags flags
        )
        {
            if (!pData)
            {
                m_Core->GetLogger()->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("No geometry given to draw."));
                return false;
            }

            HRESULT hr;

            // Set the necessary states
            hr = m_Device->SetRenderState(D3DRS_CLIPPING, FALSE);
            hr = m_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
            hr = m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
            hr = m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

            if (flags & VF_Transformed)
            {
                m_Device->SetVertexDeclaration(m_VertDeclT);
            }
            else
            {
                m_Device->SetVertexDeclaration(m_VertDecl);
            }

            if (flags & VF_Buffer)
            {
                IDirect3DVertexBuffer9 * pVertexBuffer = reinterpret_cast<IDirect3DVertexBuffer9*>(pData);

                hr = m_Device->SetStreamSource(0, pVertexBuffer, 0, sizeof(VertexStruct));

                hr = m_Device->BeginScene();

                if (SUCCEEDED(hr))
                {
                    hr = m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, offset, count);
                    hr = m_Device->EndScene();
                }
                else
                {
                    gpVoodooCore->GetLogger()->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Failed to draw geometry."));
                    return false;
                }
            }
            else
            {
                hr = m_Device->BeginScene();

                if (SUCCEEDED(hr))
                {
                    hr = m_Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count, pData, sizeof(VertexStruct));
                    hr = m_Device->EndScene();
                }
                else
                {
                    gpVoodooCore->GetLogger()->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Failed to draw geometry."));
                    return false;
                }
            }

            return true;
        }

        bool VOODOO_METHODTYPE DX9Adapter::ApplyParameter(_In_ IParameter * const pParam)
        {
            if (!pParam)
            {
                m_Core->GetLogger()->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Attempting to apply null parameter."));
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
                m_Core->GetLogger()->LogMessage
                (
                    LL_ModError, VOODOO_DX89_NAME, 
                    Format("Unable to bind parameter %1% of unknown type.") << param
                );
                return false;
            }

            return true;
        }

        bool VOODOO_METHODTYPE DX9Adapter::SetProperty(_In_ const wchar_t * name, _In_ Variant * const value)
        {
            String strname(name);

            if (strname == VSTR("SdkVersion") && value->Type == UT_UInt32)
            {
                m_SdkVersion = value->VUInt32.X;
            } 
            else if (strname == VSTR("IDirect3D8") && value->Type == UT_PVoid) 
            {
                if (InterlockedCompareExchange(&gObjectLock, 1, 0) == 0)
                {
                    IDirect3D8 * origObj = reinterpret_cast<IDirect3D8 *>(value->VPVoid);
                    if (!origObj) return false;

                    // Create the wrapper object and build the caps cache from the original
                    VoodooDX8::CVoodoo3D8 * fakeObj = new VoodooDX8::CVoodoo3D8(m_SdkVersion, nullptr);
                    fakeObj->VSCacheCaps(origObj);
                    origObj->Release();
                
                    // Load system d3d9.
                    HMODULE d3d9 = nullptr;
                    typedef IDirect3D9 * (WINAPI * Type_Direct3DCreate9)(UINT);
                    Type_Direct3DCreate9 d3d9func = (Type_Direct3DCreate9)FindFunction(TEXT("d3d9.dll"), true, "Direct3DCreate9", &d3d9);
                    assert(d3d9func);

                    // Create the real object
                    IDirect3D9 * realObj = d3d9func(D3D_SDK_VERSION);
                    assert(realObj);

                    fakeObj->m_RealObject = realObj;
                    value->VPVoid = fakeObj;
                    return true;
                }
            } 
            else if (strname == VSTR("IDirect3D9") && value->Type == UT_PVoid) 
            {
                if (InterlockedCompareExchange(&gObjectLock, 1, 0) == 0)
                {
                    IDirect3D9 * origObj = reinterpret_cast<IDirect3D9 *>(value->VPVoid);
                    if (!origObj) return false;

                    CVoodoo3D9 * fakeObj = new CVoodoo3D9(m_SdkVersion, origObj);
                    value->VPVoid = fakeObj;
                    return true;
                }
            }

            return false;
        }

        bool VOODOO_METHODTYPE DX9Adapter::GetProperty(_In_ const wchar_t * name, _In_ Variant * const value) CONST
        {
            if (!name || !value) return false;

            String strname(name);

            if (strname == VSTR("SdkVersion"))
            {
                ZeroMemory(value, sizeof(Variant));
                value->Type = UT_UInt32;
                value->Components = 1;
                value->VUInt32.X = m_SdkVersion;
                return true;
            }

            return false;
        }

        bool VOODOO_METHODTYPE DX9Adapter::ConnectTexture(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture)
        {
            if (!pParam)
            {
                return false;
            }
            else if (Converter::ToParameterCategory(pParam->GetType()) == PC_Sampler)
            {
                IDirect3DTexture9 * texObj = nullptr;

                if (pTexture)
                {
                    texObj = reinterpret_cast<IDirect3DTexture9 *>(pTexture->GetData());
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_ModInfo, VOODOO_DX89_NAME, 
                        Format("Binding texture %1% to parameter %2%.") << pTexture << pParam
                    );
                } 
                else 
                {
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_ModInfo, VOODOO_DX89_NAME, 
                        Format("Binding null texture to parameter %1%.") << pParam);
                }

                CGparameter texParam = pParam->GetCgParameter();

                cgD3D9SetTextureParameter(texParam, texObj);
                return true;
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_ModError, VOODOO_DX89_NAME, 
                    Format("Invalid texture binding, parameter %1% is not a sampler.") << pParam
                );
                return false;
            }
        }

        bool VOODOO_METHODTYPE DX9Adapter::HandleError(_In_opt_ CGcontext const pContext, _In_ uint32_t error)
        {
            UNREFERENCED_PARAMETER(pContext);

            CGerror ecode = (CGerror)error;

            if (ecode == cgD3D9DebugTrace)
            {
                const char * emsg = cgD3D9TranslateCGerror(ecode);
                m_Core->GetLogger()->LogMessage(LL_ModError, VOODOO_DX89_NAME, Format("D3D9 Debug %1%: %2%") << ecode << emsg);
                return true;
            }

            return false;
        }

        bool VOODOO_METHODTYPE DX9Adapter::SetDXDevice(IDirect3DDevice9 * pDevice)
        {
            if (pDevice == m_Device)
            {
                return true;
            }
            else if (pDevice == nullptr || (pDevice && m_Device))
            {
                if (m_VertDecl) m_VertDecl->Release();
                if (m_VertDeclT) m_VertDeclT->Release();

                CGcontext context = m_Core->GetCgContext();
                if (cgIsContext(context))
                {
                    cgD3D9UnloadAllPrograms();
                    m_Core->SetCgContext(nullptr);
                }

                if (!pDevice) return true;
            }

            m_Device = pDevice;

            HRESULT errors = m_Device->CreateStateBlock(D3DSBT_ALL, &m_CleanState);

            ILoggerRef logger = m_Core->GetLogger();
            CGcontext context = m_Core->GetCgContext();

            if (!cgIsContext(context))
            {
                context = cgCreateContext();
                m_Core->SetCgContext(context);
            }

            errors = cgD3D9SetDevice(m_Device);

            if (!SUCCEEDED(errors))
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Unable to set Cg D3D9 device."));
            }
            else
            {
                logger->LogMessage(LL_ModInfo, VOODOO_DX89_NAME, VSTR("Cg D3D9 device set successfully."));
            }

            cgD3D9RegisterStates(context);

            cgD3D9EnableDebugTracing(CG_TRUE);

            cgD3D9SetManageTextureParameters(context, CG_TRUE);

            errors = cgD3D9GetLastError();

            if (!SUCCEEDED(errors))
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Errors registering Cg states."));
            }
            else
            {
                logger->LogMessage(LL_ModInfo, VOODOO_DX89_NAME, VSTR("Cg states registered successfully."));
            }

            // Setup profiles
            CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
            CGprofile bestVert = cgD3D9GetLatestVertexProfile();

            const char * bestVertStr = cgGetProfileString(bestVert);
            const char * bestFragStr = cgGetProfileString(bestFrag);

            logger->LogMessage
            (
                LL_ModInfo, VOODOO_DX89_NAME,
                Format("Detected latest profiles with %1% as vertex and %2% as fragment.") << bestVertStr << bestFragStr
            );

            // Create vertex declaration
            D3DVERTEXELEMENT9 vertDeclElems[] =
            {
                { 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                { 0, 16, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
                { 0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
                { 0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
                D3DDECL_END()
            };

            errors = m_Device->CreateVertexDeclaration(vertDeclElems, &m_VertDecl);

            if (!SUCCEEDED(errors))
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Unable to create standard vertex declaration."));
            }

            vertDeclElems[0].Usage = D3DDECLUSAGE_POSITIONT;

            errors = m_Device->CreateVertexDeclaration(vertDeclElems, &m_VertDeclT);

            if (!SUCCEEDED(errors))
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Unable to create transformed vertex declaration."));
            }

            // Get params
            D3DVIEWPORT9 viewport;

            m_Device->GetViewport(&viewport);

            logger->LogMessage
            (
                LL_ModInfo, VOODOO_DX89_NAME, 
                Format("Prepping for %1% by %2% target.") << (uint32_t)viewport.Width << (uint32_t)viewport.Height
            );

            // Get buffers
            if (!SUCCEEDED(m_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer))) { m_BackBuffer = nullptr; }

            // Create fullscreen vbuffer
            float ft = 15.0f;
            float fb = viewport.Height - 15.0f;
            float fl = 15.0f;
            float fr = viewport.Width - 15.0f;

            VertexStruct fsVertData[6] =
            {
            //    POSITION              COLOR                  TEXCOORD[0]               TEXCOORD[1]
                {{fl, fb, 0.5f, 1.0f}, {255, 255,   0,   0}, {{0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fl, ft, 0.5f, 1.0f}, {255,   0, 255,   0}, {{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fr, ft, 0.5f, 1.0f}, {255,   0,   0, 255}, {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fl, fb, 0.5f, 1.0f}, {255, 255,   0, 255}, {{0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fr, ft, 0.5f, 1.0f}, {255,   0,   0, 255}, {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fr, fb, 0.5f, 1.0f}, {255, 255,   0, 255}, {{1.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
            };

            errors = m_Device->CreateVertexBuffer
            (
                sizeof(fsVertData), D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &gpFSQuadVerts, NULL
            );

            if (FAILED(errors))
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Failed to create vertex buffer."));
            }

            VertexStruct * pVertices = nullptr;
            errors = gpFSQuadVerts->Lock(0, 0, (void**)&pVertices, 0);

            if (FAILED(errors))
            {
                logger->LogMessage(LL_ModError, VOODOO_DX89_NAME, VSTR("Failed to lock vertex buffer to fsquad."));
            }
            else
            {
                memcpy(pVertices, fsVertData, sizeof(fsVertData));
                gpFSQuadVerts->Unlock();
            }

            return true;
        }
    }
}