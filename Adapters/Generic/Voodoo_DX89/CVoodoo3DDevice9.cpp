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

#include "CVoodoo3DDevice9.hpp"
#include "CVoodoo3D9.hpp"

#include "DX9_Version.hpp"

#define PREVENT_MANAGED_POOL() //if (Pool == D3DPOOL_MANAGED) Pool = D3DPOOL_DEFAULT

namespace VoodooShader
{
    namespace VoodooDX9
    {
        CVoodoo3DDevice9::CVoodoo3DDevice9(CVoodoo3D9 * pObject, IDirect3DDevice9 * pRealDevice) :
            m_Refs(0), m_RealDevice(pRealDevice), m_Object(pObject),
            m_VertDecl(nullptr), m_VertDeclT(nullptr), m_FSQuadVerts(nullptr), m_BackBuffer(nullptr)
        {
            if (m_RealDevice) m_RealDevice->AddRef();
            if (m_Object) m_Object->AddRef();
        };

        CVoodoo3DDevice9::~CVoodoo3DDevice9()
        {
            if (m_Object) m_Object->Release();
            m_Object = nullptr;

            if (m_RealDevice) m_RealDevice->Release();
            m_RealDevice = nullptr;
        }

        /* IUnknown methods */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::QueryInterface(REFIID riid, void **ppvObj)
        {
            return m_RealDevice->QueryInterface(riid, ppvObj);
        }

        ULONG STDMETHODCALLTYPE CVoodoo3DDevice9::AddRef()
        {
            return ++m_Refs;
        }

        ULONG STDMETHODCALLTYPE CVoodoo3DDevice9::Release()
        {
            ULONG count = --m_Refs;

            if (count == 0)
            {
                delete this;
            }

            return count;
        }

        /* IDirect3DDevice9 methods */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::TestCooperativeLevel()
        {
            return m_RealDevice->TestCooperativeLevel();
        }

        UINT STDMETHODCALLTYPE CVoodoo3DDevice9::GetAvailableTextureMem()
        {
            return m_RealDevice->GetAvailableTextureMem();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::EvictManagedResources()
        {
            return m_RealDevice->EvictManagedResources();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDirect3D(IDirect3D9 **ppD3D9)
        {
            if (!ppD3D9) return D3DERR_INVALIDCALL;

            *ppD3D9 = (IDirect3D9*)m_Object;

            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDeviceCaps(D3DCAPS9 * pCaps)
        {
            return m_RealDevice->GetDeviceCaps(pCaps);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE * pMode)
        {
            return m_RealDevice->GetDisplayMode(iSwapChain, pMode);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS * pParameters)
        {
            return m_RealDevice->GetCreationParameters(pParameters);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap)
        {
            return m_RealDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags)
        {
            m_RealDevice->SetCursorPosition(X, Y, Flags);
        }

        BOOL STDMETHODCALLTYPE CVoodoo3DDevice9::ShowCursor(BOOL bShow)
        {
            return m_RealDevice->ShowCursor(bShow);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateAdditionalSwapChain
            (
            D3DPRESENT_PARAMETERS * pPresentationParameters,
            IDirect3DSwapChain9 **pSwapChain
            )
        {
            return m_RealDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain)
        {
            return m_RealDevice->GetSwapChain(iSwapChain, pSwapChain);
        }

        UINT STDMETHODCALLTYPE CVoodoo3DDevice9::GetNumberOfSwapChains()
        {
            return m_RealDevice->GetNumberOfSwapChains();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::Reset(D3DPRESENT_PARAMETERS * pPresentationParameters)
        {
            return m_RealDevice->Reset(pPresentationParameters);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::Present
        (
            CONST RECT * pSourceRect,
            CONST RECT * pDestRect,
            HWND hDestWindowOverride,
            CONST RGNDATA * pDirtyRegion
        )
        {
            if (gpVoodooCore && testEffect)
            {
                LoggerRef logger = gpVoodooCore->GetLogger();

                HRESULT hr = m_RealDevice->StretchRect(m_BackBuffer, nullptr, surface_Frame0, nullptr, D3DTEXF_NONE);
                if (FAILED(hr))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, "Failed to stretch backbuffer to scratch texture.");
                }

                VoodooShader::TechniqueRef tech = testEffect->Bind();
                if (tech)
                {
                    uint32_t passCount = tech->GetPassCount();
                    for (uint32_t i = 0; i < passCount; ++i)
                    {
                        VoodooShader::PassRef pass = tech->GetPass(i);
                        if (pass)
                        {
                            hr = m_RealDevice->StretchRect(m_BackBuffer, nullptr, surface_Pass0, nullptr, D3DTEXF_NONE);
                            if (FAILED(hr))
                            {
                                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, "Failed to stretch backbuffer to scratch texture.");
                            }

                            pass->Bind();
                            this->VSDrawFSQuad();
                            pass->Reset();
                        }
                    }
                    testEffect->Reset();
                }
            }

            return m_RealDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetBackBuffer
        (
            UINT iSwapChain,
            UINT iBackBuffer,
            D3DBACKBUFFER_TYPE Type,
            IDirect3DSurface9 **ppBackBuffer
        )
        {
            return m_RealDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus)
        {
            return m_RealDevice->GetRasterStatus(iSwapChain, pRasterStatus);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
        {
            return m_RealDevice->SetDialogBoxMode(bEnableDialogs);
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp)
        {
            return m_RealDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP * pRamp)
        {
            return m_RealDevice->GetGammaRamp(iSwapChain, pRamp);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateTexture
        (
            UINT Width,
            UINT Height,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DTexture9 **ppTexture,
            HANDLE * pSharedHandle
        )
        {
            PREVENT_MANAGED_POOL();
            return m_RealDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateVolumeTexture
            (
            UINT Width,
            UINT Height,
            UINT Depth,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DVolumeTexture9 **ppVolumeTexture,
            HANDLE * pSharedHandle
            )
        {
            PREVENT_MANAGED_POOL();
            return m_RealDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateCubeTexture
        (
            UINT EdgeLength,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DCubeTexture9 **ppCubeTexture,
            HANDLE * pSharedHandle
        )
        {
            PREVENT_MANAGED_POOL();
            return m_RealDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateVertexBuffer
        (
            UINT Length,
            DWORD Usage,
            DWORD FVF,
            D3DPOOL Pool,
            IDirect3DVertexBuffer9 **ppVertexBuffer,
            HANDLE * pSharedHandle
        )
        {
            PREVENT_MANAGED_POOL();
            return m_RealDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateIndexBuffer
        (
            UINT Length,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DIndexBuffer9 **ppIndexBuffer,
            HANDLE * pSharedHandle
        )
        {
            PREVENT_MANAGED_POOL();
            return m_RealDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateRenderTarget
            (
            UINT Width,
            UINT Height,
            D3DFORMAT Format,
            D3DMULTISAMPLE_TYPE MultiSample,
            DWORD MultisampleQuality,
            BOOL Lockable,
            IDirect3DSurface9 **ppSurface,
            HANDLE * pSharedHandle
            )
        {
            return m_RealDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateDepthStencilSurface
        (
            UINT Width,
            UINT Height,
            D3DFORMAT Format,
            D3DMULTISAMPLE_TYPE MultiSample,
            DWORD MultisampleQuality,
            BOOL Discard,
            IDirect3DSurface9 **ppSurface,
            HANDLE * pSharedHandle
        )
        {
            return m_RealDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::UpdateSurface
        (
            IDirect3DSurface9 * pSourceSurface,
            CONST RECT * pSourceRect,
            IDirect3DSurface9 * pDestinationSurface,
            CONST POINT * pDestPoint
        )
        {
            return m_RealDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::UpdateTexture
        (
            IDirect3DBaseTexture9 * pSourceTexture,
            IDirect3DBaseTexture9 * pDestinationTexture
        )
        {
            return m_RealDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRenderTargetData(IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface)
        {
            return m_RealDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9 * pDestSurface)
        {
            return m_RealDevice->GetFrontBufferData(iSwapChain, pDestSurface);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::StretchRect
            (
            IDirect3DSurface9 * pSourceSurface,
            CONST RECT * pSourceRect,
            IDirect3DSurface9 * pDestSurface,
            CONST RECT * pDestRect,
            D3DTEXTUREFILTERTYPE Filter
            )
        {
            return m_RealDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::ColorFill(IDirect3DSurface9 * pSurface, CONST RECT * pRect, D3DCOLOR color)
        {
            return m_RealDevice->ColorFill(pSurface, pRect, color);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateOffscreenPlainSurface
            (
            UINT Width,
            UINT Height,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DSurface9 **ppSurface,
            HANDLE * pSharedHandle
            )
        {
            return m_RealDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget)
        {
            return m_RealDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget)
        {
            return m_RealDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetDepthStencilSurface(IDirect3DSurface9 * pNewZStencil)
        {
            return m_RealDevice->SetDepthStencilSurface(pNewZStencil);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface)
        {
            return m_RealDevice->GetDepthStencilSurface(ppZStencilSurface);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::BeginScene()
        {
            return m_RealDevice->BeginScene();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::EndScene()
        {
            return m_RealDevice->EndScene();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::Clear(DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
        {
            return m_RealDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
        {
            return m_RealDevice->SetTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix)
        {
            return m_RealDevice->GetTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
        {
            return m_RealDevice->MultiplyTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetViewport(CONST D3DVIEWPORT9 * pViewport)
        {
            return m_RealDevice->SetViewport(pViewport);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetViewport(D3DVIEWPORT9 * pViewport)
        {
            return m_RealDevice->GetViewport(pViewport);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetMaterial(CONST D3DMATERIAL9 * pMaterial)
        {
            return m_RealDevice->SetMaterial(pMaterial);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetMaterial(D3DMATERIAL9 * pMaterial)
        {
            return m_RealDevice->GetMaterial(pMaterial);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9 * pLight)
        {
            return m_RealDevice->SetLight(Index, pLight);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetLight(DWORD Index, D3DLIGHT9 * pLight)
        {
            return m_RealDevice->GetLight(Index, pLight);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::LightEnable(DWORD Index, BOOL Enable)
        {
            return m_RealDevice->LightEnable(Index, Enable);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetLightEnable(DWORD Index, BOOL * pEnable)
        {
            return m_RealDevice->GetLightEnable(Index, pEnable);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetClipPlane(DWORD Index, CONST float *pPlane)
        {
            return m_RealDevice->SetClipPlane(Index, pPlane);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetClipPlane(DWORD Index, float *pPlane)
        {
            return m_RealDevice->GetClipPlane(Index, pPlane);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
        {
            return m_RealDevice->SetRenderState(State, Value);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD * pValue)
        {
            return m_RealDevice->GetRenderState(State, pValue);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB)
        {
            return m_RealDevice->CreateStateBlock(Type, ppSB);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::BeginStateBlock()
        {
            return m_RealDevice->BeginStateBlock();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::EndStateBlock(IDirect3DStateBlock9 **ppSB)
        {
            return m_RealDevice->EndStateBlock(ppSB);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9 * pClipStatus)
        {
            return m_RealDevice->SetClipStatus(pClipStatus);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetClipStatus(D3DCLIPSTATUS9 * pClipStatus)
        {
            return m_RealDevice->GetClipStatus(pClipStatus);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9 **ppTexture)
        {
            return m_RealDevice->GetTexture(Stage, ppTexture);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9 * pTexture)
        {
            return m_RealDevice->SetTexture(Stage, pTexture);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue)
        {
            return m_RealDevice->GetTextureStageState(Stage, Type, pValue);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
        {
            return m_RealDevice->SetTextureStageState(Stage, Type, Value);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue)
        {
            return m_RealDevice->GetSamplerState(Sampler, Type, pValue);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
        {
            return m_RealDevice->SetSamplerState(Sampler, Type, Value);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::ValidateDevice(DWORD * pNumPasses)
        {
            return m_RealDevice->ValidateDevice(pNumPasses);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY * pEntries)
        {
            return m_RealDevice->SetPaletteEntries(PaletteNumber, pEntries);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY * pEntries)
        {
            return m_RealDevice->GetPaletteEntries(PaletteNumber, pEntries);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
        {
            return m_RealDevice->SetCurrentTexturePalette(PaletteNumber);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetCurrentTexturePalette(UINT * PaletteNumber)
        {
            return m_RealDevice->GetCurrentTexturePalette(PaletteNumber);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetScissorRect(CONST RECT * pRect)
        {
            return m_RealDevice->SetScissorRect(pRect);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetScissorRect(RECT * pRect)
        {
            return m_RealDevice->GetScissorRect(pRect);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
        {
            return m_RealDevice->SetSoftwareVertexProcessing(bSoftware);
        }

        BOOL STDMETHODCALLTYPE CVoodoo3DDevice9::GetSoftwareVertexProcessing()
        {
            return m_RealDevice->GetSoftwareVertexProcessing();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetNPatchMode(float nSegments)
        {
            return m_RealDevice->SetNPatchMode(nSegments);
        }

        float STDMETHODCALLTYPE CVoodoo3DDevice9::GetNPatchMode()
        {
            return m_RealDevice->GetNPatchMode();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
        {
            return m_RealDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawIndexedPrimitive
            (
            D3DPRIMITIVETYPE PrimitiveType,
            INT BaseVertexIndex,
            UINT MinVertexIndex,
            UINT NumVertices,
            UINT startIndex,
            UINT primCount
            )
        {
            return m_RealDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawPrimitiveUP
            (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT PrimitiveCount,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
            )
        {
            return m_RealDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawIndexedPrimitiveUP
            (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT MinVertexIndex,
            UINT NumVertices,
            UINT PrimitiveCount,
            CONST void *pIndexData,
            D3DFORMAT IndexDataFormat,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
            )
        {
            return m_RealDevice->DrawIndexedPrimitiveUP
                (
                PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat,
                pVertexStreamZeroData, VertexStreamZeroStride
                );
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::ProcessVertices
            (
            UINT SrcStartIndex,
            UINT DestIndex,
            UINT VertexCount,
            IDirect3DVertexBuffer9 * pDestBuffer,
            IDirect3DVertexDeclaration9 * pVertexDecl,
            DWORD Flags
            )
        {
            return m_RealDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateVertexDeclaration
            (
            CONST D3DVERTEXELEMENT9 * pVertexElements,
            IDirect3DVertexDeclaration9 **ppDecl
            )
        {
            return m_RealDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9 * pDecl)
        {
            return m_RealDevice->SetVertexDeclaration(pDecl);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9 **ppDecl)
        {
            return m_RealDevice->GetVertexDeclaration(ppDecl);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetFVF(DWORD FVF)
        {
            return m_RealDevice->SetFVF(FVF);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetFVF(DWORD * pFVF)
        {
            return m_RealDevice->GetFVF(pFVF);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateVertexShader(CONST DWORD * pFunction, IDirect3DVertexShader9 **ppShader)
        {
            return m_RealDevice->CreateVertexShader(pFunction, ppShader);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShader(IDirect3DVertexShader9 * pShader)
        {
            return m_RealDevice->SetVertexShader(pShader);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShader(IDirect3DVertexShader9 **ppShader)
        {
            return m_RealDevice->GetVertexShader(ppShader);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->SetVertexShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->GetVertexShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetStreamSource
            (
            UINT StreamNumber,
            IDirect3DVertexBuffer9 * pStreamData,
            UINT OffsetInBytes,
            UINT Stride
            )
        {
            return m_RealDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetStreamSource
            (
            UINT StreamNumber,
            IDirect3DVertexBuffer9 **ppStreamData,
            UINT * pOffsetInBytes,
            UINT * pStride
            )
        {
            return m_RealDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
        {
            return m_RealDevice->SetStreamSourceFreq(StreamNumber, Setting);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT * pSetting)
        {
            return m_RealDevice->GetStreamSourceFreq(StreamNumber, pSetting);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetIndices(IDirect3DIndexBuffer9 * pIndexData)
        {
            return m_RealDevice->SetIndices(pIndexData);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetIndices(IDirect3DIndexBuffer9 **ppIndexData)
        {
            return m_RealDevice->GetIndices(ppIndexData);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreatePixelShader(CONST DWORD * pFunction, IDirect3DPixelShader9 **ppShader)
        {
            return m_RealDevice->CreatePixelShader(pFunction, ppShader);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShader(IDirect3DPixelShader9 * pShader)
        {
            return m_RealDevice->SetPixelShader(pShader);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShader(IDirect3DPixelShader9 **ppShader)
        {
            return m_RealDevice->GetPixelShader(ppShader);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShaderConstantF (UINT StartRegister, float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->SetPixelShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->GetPixelShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO * pRectPatchInfo)
        {
            return m_RealDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO * pTriPatchInfo)
        {
            return m_RealDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DeletePatch(UINT Handle)
        {
            return m_RealDevice->DeletePatch(Handle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery)
        {
            return m_RealDevice->CreateQuery(Type, ppQuery);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::VSSetupDevice()
        {
            if (m_VertDecl) m_VertDecl->Release();
            if (m_VertDeclT) m_VertDeclT->Release();

            LoggerRef logger = gpVoodooCore->GetLogger();

            Variant deviceVar = CreateVariant(this);
            VoodooResult vr = gpVoodooCore->Bind(VSProfile_D3D9, 1, &deviceVar);
            assert(SUCCEEDED(vr));

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
                StringFormat("Detected latest profiles as %1% and %2%.") << bestVertStr << bestFragStr
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

            HRESULT errors = m_RealDevice->CreateVertexDeclaration(vertDeclElems, &m_VertDecl);

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
                StringFormat("Prepping for %1% by %2% target.") << (uint32_t)viewport.Width << (uint32_t)viewport.Height
            );

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
                sizeof(fsVertData), D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &m_FSQuadVerts, NULL
            );

            if (FAILED(errors))
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to create vertex buffer."));
            }

            VertexDesc * pVertices = nullptr;
            errors = m_FSQuadVerts->Lock(0, 0, (void**)&pVertices, 0);

            if (SUCCEEDED(errors))
            {
                memcpy(pVertices, fsVertData, sizeof(fsVertData));
                m_FSQuadVerts->Unlock();
            }
            else
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to lock vertex buffer to fsquad."));
            }

            TextureDesc bufferTextureDesc;
            bufferTextureDesc.Size.X = viewport.Width;
            bufferTextureDesc.Size.Y = viewport.Height;
            bufferTextureDesc.Size.Z = 0;
            bufferTextureDesc.Levels = 0;
            bufferTextureDesc.Usage = VSTexFlag_Target;
            bufferTextureDesc.Format = VSFmt_RGBA8; //VoodooShader::VoodooDX9::ConverterDX9::ToTextureFormat(pPP->BackBufferFormat);

            // Get buffer
            HRESULT hrt = m_RealDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer);

            if (SUCCEEDED(hrt))
            {
                logger->LogMessage(VSLog_ModInfo, VOODOO_DX89_NAME, L"Cached backbuffer surface.");
            }
            else
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, L"Failed to retrieve backbuffer surface.");
            }

            texture_Frame0 = gpVoodooCore->CreateTexture(L":frame0", bufferTextureDesc);
            if (texture_Frame0)
            {
                Variant texvar = CreateVariant();
                if (SUCCEEDED(texture_Frame0->GetProperty(PropIds::D3D9Surface, &texvar)))
                {
                    surface_Frame0 = reinterpret_cast<IDirect3DSurface9 *>(texvar.VPVoid);
                    logger->LogMessage(VSLog_ModInfo, VOODOO_DX89_NAME, L"Cached :frame0 surface.");
                }
                else
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, L"Failed to cache :frame0 surface.");
                }
            }

            texture_Pass0 = gpVoodooCore->CreateTexture(L":pass0", bufferTextureDesc);
            if (texture_Pass0)
            {
                Variant texvar = CreateVariant();
                if (SUCCEEDED(texture_Pass0->GetProperty(PropIds::D3D9Surface, &texvar)))
                {
                    surface_Pass0 = reinterpret_cast<IDirect3DSurface9 *>(texvar.VPVoid);
                    logger->LogMessage(VSLog_ModInfo, VOODOO_DX89_NAME, L"Cached :pass0 surface.");
                }
                else
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, L"Failed to cache :pass0 surface.");
                }
            }

            ParameterDesc rcpres_desc = {VSPT_Float, 1, 2, 0};
            IParameter * lpparam_rcpres = gpVoodooCore->CreateParameter(L"rcpres", rcpres_desc);

            try
            {
                IFile * shaderFile = gpVoodooCore->GetFileSystem()->GetFile(L"test.fx");
                if (shaderFile)
                {
                    testEffect = gpVoodooCore->CreateEffect(shaderFile);
                }
            }
            catch (std::exception & exc)
            {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, StringFormat("Error loading shader: %1%") << exc.what());
            }

            if (lpparam_rcpres)
            {
                Float4 rcpres_val = {1.0f / viewport.Width, 1.0f / viewport.Height, 0, 0};
                lpparam_rcpres->SetVector(rcpres_val);
            }

            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::VSDrawFSQuad()
        {
            HRESULT hr;

            // Set the necessary states
            hr = m_RealDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
            hr = m_RealDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
            hr = m_RealDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
            hr = m_RealDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

            m_RealDevice->SetVertexDeclaration(m_VertDeclT);
            m_RealDevice->SetStreamSource(0, m_FSQuadVerts, 0, sizeof(VertexDesc));
            hr = m_RealDevice->BeginScene();
            if (SUCCEEDED(hr))
            {
                m_RealDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
                m_RealDevice->EndScene();
            }

            return D3D_OK;
        }

        IDirect3DDevice9 * STDMETHODCALLTYPE CVoodoo3DDevice9::VSGetRealDevice()
        {
            return m_RealDevice;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::VSSetRealDevice(IDirect3DDevice9 * pDev)
        {
            if (m_RealDevice == pDev) return D3D_OK;

            if (m_RealDevice) m_RealDevice->Release();
            m_RealDevice = pDev;
            if (m_RealDevice) m_RealDevice->AddRef();

            return D3D_OK;        
        }
    }
}