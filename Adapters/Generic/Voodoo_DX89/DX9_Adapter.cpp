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
#include "CVoodoo3DDevice8.hpp"
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
            m_SdkVersion(D3D_SDK_VERSION), m_FakeDevice(nullptr), m_RealDevice(nullptr), 
            m_VertDecl(nullptr), m_VertDeclT(nullptr), m_BackBuffer(nullptr),
            m_CleanState(nullptr), m_PassState(nullptr), m_BoundDXEffect(nullptr), m_BoundDXPass(nullptr)
        {
            gpVoodooCore = m_Core;
            gpVoodooLogger = gpVoodooCore->GetLogger();
        };

        DX9Adapter::~DX9Adapter()
        {
            if (m_RealDevice)
            {
                m_RealDevice->Release();
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

        VoodooResult VOODOO_METHODTYPE DX9Adapter::QueryInterface(const Uuid clsid, _Deref_out_opt_ const IObject ** ppOut) const
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
                    return VSFERR_INVALIDUUID;
                }

                (*ppOut)->AddRef();
                return VSF_OK;
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

        String VOODOO_METHODTYPE DX9Adapter::GetName() CONST
        {
            return VSTR("DX9Adapter");
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::SetProperty(const Uuid propid, _In_ Variant * pValue)
        {
            if (!pValue) return VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3DSdkVersion && pValue->Type == UT_UInt32)
            {
                m_SdkVersion = pValue->VUInt32.X;
                return VSF_OK;
            } 
            else if (propid == PropIds::D3D8Object && pValue->Type == UT_PVoid) 
            {
                if (InterlockedCompareExchange(&gObjectLock, 1, 0) == 0)
                {
                    IDirect3D8 * origObj = reinterpret_cast<IDirect3D8 *>(pValue->VPVoid);
                    if (!origObj) return VSFERR_INVALIDPARAMS;

                    // Create the wrapper object and build the caps cache from the original
                    VoodooDX8::CVoodoo3D8 * fakeObj = new VoodooDX8::CVoodoo3D8(m_SdkVersion, nullptr);
                    fakeObj->VSCacheCaps(origObj);
                    while (origObj->Release() > 0) {};
                
                    // Load system d3d9.
                    HMODULE d3d9 = nullptr;
                    typedef IDirect3D9 * (WINAPI * Type_Direct3DCreate9)(UINT);
                    Type_Direct3DCreate9 d3d9func = (Type_Direct3DCreate9)FindFunction(TEXT("d3d9.dll"), true, "Direct3DCreate9", &d3d9);
                    assert(d3d9func);

                    // Create the real object
                    IDirect3D9 * realObj = d3d9func(D3D_SDK_VERSION);
                    assert(realObj);

                    fakeObj->m_RealObject = realObj;
                    pValue->VPVoid = fakeObj;
                    return VSFOK_PROPERTYCHANGED;
                }
            } 
            else if (propid == PropIds::D3D9Object && pValue->Type == UT_PVoid) 
            {
                if (InterlockedCompareExchange(&gObjectLock, 1, 0) == 0)
                {
                    IDirect3D9 * origObj = reinterpret_cast<IDirect3D9 *>(pValue->VPVoid);
                    if (!origObj) return false;

                    CVoodoo3D9 * fakeObj = new CVoodoo3D9(m_SdkVersion, origObj);
                    pValue->VPVoid = fakeObj;
                    return VSFOK_PROPERTYCHANGED;
                }
            }
            else if (propid == PropIds::D3D8Device && pValue->Type == UT_PVoid)
            {
                if (m_FakeDevice) m_FakeDevice->Release();
                m_FakeDevice = reinterpret_cast<VoodooDX8::CVoodoo3DDevice8 *>(pValue->VPVoid);
                m_FakeDevice->AddRef();
                return VSF_OK;
            }
            else if (propid == PropIds::D3D9Device && pValue->Type == UT_PVoid)
            {
                if (m_RealDevice) m_RealDevice->Release();
                m_RealDevice = reinterpret_cast<LPDIRECT3DDEVICE9>(pValue->VPVoid);
                m_RealDevice->AddRef();
                this->SetupDevice();
                return VSF_OK;
            }

            return VSFERR_INVALIDPARAMS;
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::GetProperty(const Uuid propid, _In_ Variant * pValue) CONST
        {
            if (!pValue) return VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3DSdkVersion)
            {
                ZeroMemory(pValue, sizeof(Variant));
                pValue->Type = UT_UInt32;
                pValue->Components = 1;
                pValue->VUInt32.X = m_SdkVersion;
                return true;
            }
            else if (propid == PropIds::D3D8Device)
            {
                ZeroMemory(pValue, sizeof(Variant));
                pValue->Type = UT_PVoid;
                pValue->VPVoid = reinterpret_cast<LPVOID>(m_FakeDevice);
                return VSF_OK;
            }
            else if (propid == PropIds::D3D9Device)
            {
                ZeroMemory(pValue, sizeof(Variant));
                pValue->Type = UT_PVoid;
                pValue->VPVoid = reinterpret_cast<LPVOID>(m_RealDevice);
                return VSF_OK;
            }

            return VSFERR_PROPERTYNOTFOUND;
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::SetEffect(_In_ IEffect * const pEffect)
        {
            if (!pEffect)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else if (m_BoundEffect)
            {
                return VSFERR_INVALIDCALL;
            }

            Variant effectVar = CreateVariant();
            if (FAILED(pEffect->GetProperty(PropIds::D3DX9Effect, &effectVar)) || effectVar.Type != UT_PVoid)
            {
                return VSFERR_INVALIDPARAMS;
            }

            LPD3DXEFFECT pDXEffect = reinterpret_cast<LPD3DXEFFECT>(effectVar.VPVoid);

            if (!pDXEffect)
            {
                return VSFERR_INVALIDPARAMS;
            }

            UINT dxPass = 0;
            if (FAILED(pDXEffect->Begin(&dxPass, 0)))
            {
                return VSF_FAIL;
            }

            m_BoundDXEffect = pDXEffect;
            m_BoundDXEffect->AddRef();
            m_BoundEffect = pEffect;

            return VSF_OK;
        }

        IEffect * VOODOO_METHODTYPE DX9Adapter::GetEffect() CONST
        {
            return m_BoundEffect.get();
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::ResetEffect()
        {
            if (!m_BoundEffect || !m_BoundDXEffect)
            {
                return VSFERR_INVALIDCALL;
            }

            HRESULT hr = m_BoundDXEffect->End();
            if (FAILED(hr))
            {
                return VSF_FAIL;
            }

            m_BoundDXEffect->Release();
            m_BoundEffect = nullptr;

            return VSF_OK;
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::SetPass(_In_ IPass * const pPass)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!pPass)
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Unable to set null pass."));
                return false;
            }
            else if (m_BoundPass)
            {
                logger->LogMessage(VSLog_ModWarning, VOODOO_DX89_NAME, VSTR("Setting pass without resetting previously bound pass."));
                return VSFERR_INVALIDCALL;
            }
            else if (!m_BoundEffect)
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Unable to set pass with no effect set."));
                return VSFERR_INVALIDCALL;
            }
            else if (pPass->GetTechnique() != m_BoundEffect->GetDefaultTechnique())
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Unable to set pass, not from default technique of current effect."));
                return VSFERR_INVALIDPARAMS;
            }

            Variant propvar = CreateVariant();
            if (FAILED(pPass->GetProperty(PropIds::D3DX9PassId, &propvar)) || propvar.Type != UT_UInt32)
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to get hardware handle from pass %1%.") << pPass);
                return VSFERR_INVALIDPARAMS;
            }
            m_BoundDXPassId = propvar.VUInt32.X;

            m_RealDevice->CreateStateBlock(D3DSBT_ALL, &m_PassState);
            m_CleanState->Apply();

            HRESULT hr = m_BoundDXEffect->BeginPass(m_BoundDXPassId);
            if (FAILED(hr))
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to set pass %1% in hardware.") << pPass);
                return VSF_FAIL;
            }

            // Bind render targets
            for (uint32_t i = 0; i < 4; ++i)
            {
                ITexture * pTarget = pPass->GetTarget(i);
                this->SetTarget(i, pTarget);
            }

            m_BoundPass = pPass;

            return VSF_OK;
        }

        IPass * VOODOO_METHODTYPE DX9Adapter::GetPass() CONST
        {
            return m_BoundPass.get();
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::ResetPass()
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (!m_BoundPass || !m_BoundEffect)
            {
                return VSFOK_REDUNDANT;
            }

            HRESULT hr = m_BoundDXEffect->EndPass();
            if (FAILED(hr))
            {
                return VSF_FAIL;
            }

            m_BoundPass = nullptr;

            m_PassState->Apply();
            m_PassState->Release();

            return VSF_OK;
        }

        VoodooResult DX9Adapter::SetTarget(const uint32_t index, _In_opt_ ITexture * pTarget)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (index > 3) {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Invalid render target index %1%.") << index);
                return false;
            }

            m_RenderTarget[index] = pTarget;

            HRESULT result;

            if (!pTarget)
            {
                if (index == 0)
                {
                    result = m_RealDevice->SetRenderTarget(index, m_BackBuffer);

                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound backbuffer to render target %1%.") << index);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind backbuffer to render target %1%: %2%") << index << result);
                        return false;
                    }
                }
                else
                {
                    result = m_RealDevice->SetRenderTarget(index, NULL);

                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound null texture to render target %1%.") << index);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind null texture to render target %1%: %2%") << index << result);
                        return false;
                    }
                }
            }
            else
            {
                Variant texVar = CreateVariant();
                if (FAILED(pTarget->GetProperty(PropIds::D3D9Texture, &texVar)))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to retrieve hardware texture property from texture %1%.") << pTarget);
                    return VSFERR_INVALIDPARAMS;
                }

                IDirect3DTexture9 * pD3D9Tex = reinterpret_cast<IDirect3DTexture9*>(texVar.VPVoid);

                if (!pD3D9Tex)
                {
                    result = m_RealDevice->SetRenderTarget(index, nullptr);
                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound null texture data to render target %1%: %2%") << index << result);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind null texture data to render target %1%: %2%") << index << result);
                        return false;
                    }
                }

                pD3D9Tex->AddRef();

                IDirect3DSurface9 * pD3D9Surf = nullptr;
                result = pD3D9Tex->GetSurfaceLevel(0, &pD3D9Surf);

                if (!SUCCEEDED(result))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Failed to get surface of texture '%1%', data %2%: %3%") << pTarget << pD3D9Tex << result);
                    pD3D9Surf->Release();
                    pD3D9Tex->Release();
                    return false;
                }

                result = m_RealDevice->SetRenderTarget(index, pD3D9Surf);

                pD3D9Surf->Release();
                pD3D9Tex->Release();

                if (!SUCCEEDED(result))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind texture '%1%' to render target %2%: %3%") << pTarget << index << result);
                    return false;
                }
                else
                {
                    logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound texture '%1%' to render target %2%: %3%") << pTarget << index << result);
                    return true;
                }
            }
        }

        ITexture * VOODOO_METHODTYPE DX9Adapter::CreateTexture(_In_ const String & name, _In_ const TextureDesc pDesc)
        {
            IDirect3DTexture9 * tex = nullptr;
            D3DFORMAT fmt = DX9_Converter::ToD3DFormat(pDesc.Format);

            HRESULT hr = m_RealDevice->CreateTexture
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
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Error creating texture %1%: %2%") << name << hr);
                return nullptr;
            }
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::LoadTexture(_In_ IImage * const pFile, _In_ const TextureRegion pRegion, _Inout_ ITexture * const pTexture)
        {
            if (!pFile | !pTexture) return VSFERR_INVALIDPARAMS;

            TextureDesc texDesc = pTexture->GetDesc();
            TextureDesc fileDesc = pFile->GetDesc();

            if (texDesc.Format != pRegion.Format || fileDesc.Format != pRegion.Format) return VSFERR_INVALIDPARAMS;

            D3DFORMAT fmt = DX9_Converter::ToD3DFormat(pRegion.Format);
            if (fmt == D3DFMT_UNKNOWN) return VSFERR_INVALIDPARAMS;

            // Should succeed, take references
            IImageRef image = pFile;
            ITextureRef texture = pTexture;

            // Create a trash texture in system memory
            Variant destSurfVar = CreateVariant();
            if (FAILED(texture->GetProperty(PropIds::D3D9Surface, &destSurfVar)) || destSurfVar.Type != UT_PVoid)
            {
                return VSFERR_INVALIDPARAMS;
            }

            LPDIRECT3DSURFACE9 pDestSurf = reinterpret_cast<LPDIRECT3DSURFACE9>(destSurfVar.VPVoid);

            if (!pDestSurf)
            {
                return VSFERR_INVALIDPARAMS;
            }

            pDestSurf->AddRef();

            IDirect3DSurface9 * pTempSurf = nullptr;
            HRESULT hr = m_RealDevice->CreateOffscreenPlainSurface(pRegion.Size.X, pRegion.Size.Y, fmt, D3DPOOL_SYSTEMMEM, &pTempSurf, nullptr);
            if (FAILED(hr))
            {
                return VSFERR_APIERROR;
            }

            uint32_t bufferSize = pFile->GetData(pRegion, 0, nullptr);
            std::vector<char> buffer(bufferSize);
            pFile->GetData(pRegion, bufferSize, &buffer[0]);

            RECT rect;
            ZeroMemory(&rect, sizeof(RECT));

            rect.left = pRegion.Origin.X;
            rect.top = pRegion.Origin.Y;
            rect.right = pRegion.Size.X;
            rect.bottom = pRegion.Size.Y;

            D3DLOCKED_RECT lockRect;
            hr = pTempSurf->LockRect(&lockRect, &rect, D3DLOCK_DISCARD);

            for (uint32_t row = 0; row < pRegion.Size.Y; ++row)
            {
                char * dstBits = (char*)lockRect.pBits;
                dstBits += (lockRect.Pitch * row);

                char * srcBits = &buffer[0];
                srcBits += (lockRect.Pitch * row);

                memcpy(dstBits, srcBits, lockRect.Pitch);
            }

            hr = pTempSurf->UnlockRect();

            POINT o = {pRegion.Origin.X, pRegion.Origin.Y};
            hr = m_RealDevice->UpdateSurface(pTempSurf, &rect, pDestSurf, &o);

            return false;
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::DrawGeometry
        (
            _In_ const uint32_t offset,
            _In_ const uint32_t count,
            _In_ void * const pData,
            _In_ const VertexFlags flags
        )
        {
            if (!pData)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("No geometry given to draw."));
                return false;
            }

            HRESULT hr;

            // Set the necessary states
            hr = m_RealDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
            hr = m_RealDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
            hr = m_RealDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
            hr = m_RealDevice->SetRenderState(D3DRS_CULLMODE, D3DCUVSLog_NONE);

            if (flags & VF_Transformed)
            {
                m_RealDevice->SetVertexDeclaration(m_VertDeclT);
            }
            else
            {
                m_RealDevice->SetVertexDeclaration(m_VertDecl);
            }

            if (flags & VF_Buffer)
            {
                IDirect3DVertexBuffer9 * pVertexBuffer = reinterpret_cast<IDirect3DVertexBuffer9*>(pData);

                hr = m_RealDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VertexDesc));

                hr = m_RealDevice->BeginScene();

                if (SUCCEEDED(hr))
                {
                    hr = m_RealDevice->DrawPrimitive(D3DPT_TRIANGLELIST, offset, count);
                    hr = m_RealDevice->EndScene();
                }
                else
                {
                    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to draw geometry."));
                    return false;
                }
            }
            else
            {
                hr = m_RealDevice->BeginScene();

                if (SUCCEEDED(hr))
                {
                    hr = m_RealDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count, pData, sizeof(VertexDesc));
                    hr = m_RealDevice->EndScene();
                }
                else
                {
                    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to draw geometry."));
                    return false;
                }
            }

            return true;
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::BindTexture(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture)
        {
            if (!pParam || !pTexture)
            {
                return VSFERR_INVALIDPARAMS;
            }

            ParameterDesc desc = pParam->GetDesc();
            if (desc.Type < VSPT_Texture || desc.Type > VSPT_TextureCube)
            {
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_ModError, VOODOO_DX89_NAME, 
                    Format("Invalid texture binding, parameter %1% is not a texture parameter.") << pParam
                );
                return VSFERR_INVALIDPARAMS;
            }

            LPD3DXEFFECT pDXEffect;
            D3DXHANDLE pDXParam;
            LPDIRECT3DTEXTURE9 pDXTexture;

            Variant propVar = CreateVariant();
            if (FAILED(pParam->GetProperty(PropIds::D3DX9Effect, &propVar))) 
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to get hardware effect from parameter %1%.") << pParam);
                return VSFERR_INVALIDPARAMS;
            }
            pDXEffect = reinterpret_cast<LPD3DXEFFECT>(propVar.VPVoid);
            if (!pDXEffect)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Parameter %1% has no hardware effect.") << pParam);
                return VSFERR_INVALIDPARAMS;
            }

            if (FAILED(pParam->GetProperty(PropIds::D3DX9Handle, &propVar)))
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to get hardware handle from parameter %1%.") << pParam);
                return VSFERR_INVALIDPARAMS;
            }
            pDXParam = reinterpret_cast<D3DXHANDLE>(propVar.VPVoid);
            if (!pDXParam)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Parameter %1% has no hardware handle.") << pParam);
                return VSFERR_INVALIDPARAMS;
            }

            if (FAILED(pTexture->GetProperty(PropIds::D3D9Texture, &propVar)))
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to get hardware texture from texture %1%.") << pTexture);
                return VSFERR_INVALIDPARAMS;
            }
            pDXTexture = reinterpret_cast<LPDIRECT3DTEXTURE9>(propVar.VPVoid);

            pDXEffect->AddRef();
            pDXTexture->AddRef();
            HRESULT hr = pDXEffect->SetTexture(pDXParam, pDXTexture);
            pDXEffect->Release();
            pDXTexture->Release();

            if (FAILED(hr))
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to bind texture %1% to parameter %2%.") << pTexture << pParam);
                return VSFERR_INVALIDCALL;
            }
            else
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound texture %1% to parameter %2%.") << pTexture << pParam);
                return VSF_OK;
            }
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::SetupDevice()
        {
            if (!m_RealDevice)
            {
                return VSFERR_INVALIDCALL;
            }
            
            if (m_VertDecl) m_VertDecl->Release();
            if (m_VertDeclT) m_VertDeclT->Release();

            HRESULT errors = m_RealDevice->CreateStateBlock(D3DSBT_ALL, &m_CleanState);

            ILoggerRef logger = m_Core->GetLogger();

            // Setup profiles
            const char * bestVertStr = D3DXGetVertexShaderProfile(m_RealDevice);
            const char * bestFragStr = D3DXGetPixelShaderProfile(m_RealDevice);

            if (!bestVertStr || !bestFragStr)
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Error detecting latest shader profiles."));
                return VSF_FAIL;
            }

            logger->LogMessage
            (
                VSLog_ModInfo, VOODOO_DX89_NAME,
                Format("Detected latest profiles as %1% and %2%.") << bestVertStr << bestFragStr
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

            errors = m_RealDevice->CreateVertexDeclaration(vertDeclElems, &m_VertDecl);

            if (!SUCCEEDED(errors))
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Unable to create standard vertex declaration."));
            }

            vertDeclElems[0].Usage = D3DDECLUSAGE_POSITIONT;

            errors = m_RealDevice->CreateVertexDeclaration(vertDeclElems, &m_VertDeclT);

            if (!SUCCEEDED(errors))
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Unable to create transformed vertex declaration."));
            }

            // Get params
            D3DVIEWPORT9 viewport;

            m_RealDevice->GetViewport(&viewport);

            logger->LogMessage
            (
                VSLog_ModInfo, VOODOO_DX89_NAME, 
                Format("Prepping for %1% by %2% target.") << (uint32_t)viewport.Width << (uint32_t)viewport.Height
            );

            // Get buffers
            if (!SUCCEEDED(m_RealDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer))) { m_BackBuffer = nullptr; }

            // Create fullscreen vbuffer
            float ft = -0.5f;
            float fb = viewport.Height - ft;
            float fl = -0.5f;
            float fr = viewport.Width - ft;

            VertexDesc fsVertData[6] =
            {
            //    POSITION              COLOR                  TEXCOORD[0]               TEXCOORD[1]
                {{fl, fb, 0.5f, 1.0f}, {255, 255,   0,   0}, {{0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fl, ft, 0.5f, 1.0f}, {255,   0, 255,   0}, {{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fr, ft, 0.5f, 1.0f}, {255,   0,   0, 255}, {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fl, fb, 0.5f, 1.0f}, {255, 255,   0, 255}, {{0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fr, ft, 0.5f, 1.0f}, {255,   0,   0, 255}, {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
                {{fr, fb, 0.5f, 1.0f}, {255, 255,   0, 255}, {{1.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}},
            };

            errors = m_RealDevice->CreateVertexBuffer
            (
                sizeof(fsVertData), D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &gpFSQuadVerts, NULL
            );

            if (FAILED(errors))
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to create vertex buffer."));
            }

            VertexDesc * pVertices = nullptr;
            errors = gpFSQuadVerts->Lock(0, 0, (void**)&pVertices, 0);

            if (FAILED(errors))
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to lock vertex buffer to fsquad."));
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