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

#include "DX9_Version.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        CVoodoo3DDevice9::CVoodoo3DDevice9(CVoodoo3D9 * pObject, IDirect3DDevice9 * pRealDevice) :
            m_Object(pObject), m_RealDevice(pRealDevice),
            m_VertDecl(nullptr), m_VertDeclT(nullptr), m_FSQuadVerts(nullptr), m_BackBuffer(nullptr)
        {
        };

        CVoodoo3DDevice9::~CVoodoo3DDevice9()
        {
            m_RealDevice = nullptr;
            m_Object = nullptr;
        }

        /* IUnknown methods */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::QueryInterface(THIS_ REFIID riid, void **ppvObj)
        {
            return m_RealDevice->QueryInterface(riid, ppvObj);
        }

        ULONG STDMETHODCALLTYPE CVoodoo3DDevice9::AddRef(THIS)
        {
            return m_RealDevice->AddRef();
        }

        ULONG STDMETHODCALLTYPE CVoodoo3DDevice9::Release(THIS)
        {
            ULONG count = m_RealDevice->Release();

            if (count == 0)
            {
                delete this;
            }

            return count;
        }

        /* IDirect3DDevice9 methods */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::TestCooperativeLevel (THIS)
        {
            return m_RealDevice->TestCooperativeLevel();
        }

        UINT STDMETHODCALLTYPE CVoodoo3DDevice9::GetAvailableTextureMem(THIS)
        {
            return m_RealDevice->GetAvailableTextureMem();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::EvictManagedResources(THIS)
        {
            return m_RealDevice->EvictManagedResources();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDirect3D(THIS_ IDirect3D9 **ppD3D9)
        {
            // Let the device validate the incoming pointer for us
            if (!ppD3D9) return D3DERR_INVALIDCALL;

            *ppD3D9 = (IDirect3D9*)m_Object;

            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDeviceCaps(THIS_ D3DCAPS9 * pCaps)
        {
            return m_RealDevice->GetDeviceCaps(pCaps);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDisplayMode(THIS_ UINT iSwapChain, D3DDISPLAYMODE * pMode)
        {
            return m_RealDevice->GetDisplayMode(iSwapChain, pMode);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetCreationParameters(THIS_ D3DDEVICE_CREATION_PARAMETERS * pParameters)
        {
            return m_RealDevice->GetCreationParameters(pParameters);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetCursorProperties(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap)
        {
            return m_RealDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice9::SetCursorPosition(THIS_ int X, int Y, DWORD Flags)
        {
            m_RealDevice->SetCursorPosition(X, Y, Flags);
        }

        BOOL STDMETHODCALLTYPE CVoodoo3DDevice9::ShowCursor(THIS_ BOOL bShow)
        {
            return m_RealDevice->ShowCursor(bShow);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateAdditionalSwapChain
            (
            THIS_ D3DPRESENT_PARAMETERS * pPresentationParameters,
            IDirect3DSwapChain9 **pSwapChain
            )
        {
            return m_RealDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetSwapChain(THIS_ UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain)
        {
            return m_RealDevice->GetSwapChain(iSwapChain, pSwapChain);
        }

        UINT STDMETHODCALLTYPE CVoodoo3DDevice9::GetNumberOfSwapChains(THIS)
        {
            return m_RealDevice->GetNumberOfSwapChains();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::Reset(THIS_ D3DPRESENT_PARAMETERS * pPresentationParameters)
        {
            return m_RealDevice->Reset(pPresentationParameters);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::Present
        (
            THIS_ CONST RECT * pSourceRect,
            CONST RECT * pDestRect,
            HWND hDestWindowOverride,
            CONST RGNDATA * pDirtyRegion
        )
        {
            if (gpVoodooCore && testEffect)
            {
                ILoggerRef logger = gpVoodooCore->GetLogger();

                HRESULT hr = m_RealDevice->StretchRect(m_BackBuffer, nullptr, surface_Frame0, nullptr, D3DTEXF_NONE);
                if (FAILED(hr))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, "Failed to stretch backbuffer to scratch texture.");
                }

                VoodooShader::ITechniqueRef tech = testEffect->Bind();
                if (tech)
                {
                    uint32_t passCount = tech->GetPassCount();
                    for (uint32_t i = 0; i < passCount; ++i)
                    {
                        VoodooShader::IPassRef pass = tech->GetPass(i);
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
            THIS_ UINT iSwapChain,
            UINT iBackBuffer,
            D3DBACKBUFFER_TYPE Type,
            IDirect3DSurface9 **ppBackBuffer
        )
        {
            return m_RealDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRasterStatus(THIS_ UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus)
        {
            return m_RealDevice->GetRasterStatus(iSwapChain, pRasterStatus);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetDialogBoxMode(THIS_ BOOL bEnableDialogs)
        {
            return m_RealDevice->SetDialogBoxMode(bEnableDialogs);
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice9::SetGammaRamp(THIS_ UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp)
        {
            return m_RealDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice9::GetGammaRamp(THIS_ UINT iSwapChain, D3DGAMMARAMP * pRamp)
        {
            return m_RealDevice->GetGammaRamp(iSwapChain, pRamp);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateTexture
        (
            THIS_ UINT Width,
            UINT Height,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DTexture9 **ppTexture,
            HANDLE * pSharedHandle
        )
        {
            return m_RealDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateVolumeTexture
            (
            THIS_ UINT Width,
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
            return m_RealDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateCubeTexture
        (
            THIS_ UINT EdgeLength,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DCubeTexture9 **ppCubeTexture,
            HANDLE * pSharedHandle
        )
        {
            return m_RealDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateVertexBuffer
        (
            THIS_ UINT Length,
            DWORD Usage,
            DWORD FVF,
            D3DPOOL Pool,
            IDirect3DVertexBuffer9 **ppVertexBuffer,
            HANDLE * pSharedHandle
        )
        {
            return m_RealDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateIndexBuffer
        (
            THIS_ UINT Length,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DIndexBuffer9 **ppIndexBuffer,
            HANDLE * pSharedHandle
        )
        {
            return m_RealDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateRenderTarget
            (
            THIS_ UINT Width,
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
            THIS_ UINT Width,
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
            THIS_ IDirect3DSurface9 * pSourceSurface,
            CONST RECT * pSourceRect,
            IDirect3DSurface9 * pDestinationSurface,
            CONST POINT * pDestPoint
        )
        {
            return m_RealDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::UpdateTexture
        (
            THIS_ IDirect3DBaseTexture9 * pSourceTexture,
            IDirect3DBaseTexture9 * pDestinationTexture
        )
        {
            return m_RealDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRenderTargetData(THIS_ IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface)
        {
            return m_RealDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetFrontBufferData(THIS_ UINT iSwapChain, IDirect3DSurface9 * pDestSurface)
        {
            return m_RealDevice->GetFrontBufferData(iSwapChain, pDestSurface);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::StretchRect
            (
            THIS_ IDirect3DSurface9 * pSourceSurface,
            CONST RECT * pSourceRect,
            IDirect3DSurface9 * pDestSurface,
            CONST RECT * pDestRect,
            D3DTEXTUREFILTERTYPE Filter
            )
        {
            return m_RealDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::ColorFill(THIS_ IDirect3DSurface9 * pSurface, CONST RECT * pRect, D3DCOLOR color)
        {
            return m_RealDevice->ColorFill(pSurface, pRect, color);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateOffscreenPlainSurface
            (
            THIS_ UINT Width,
            UINT Height,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DSurface9 **ppSurface,
            HANDLE * pSharedHandle
            )
        {
            return m_RealDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetRenderTarget(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget)
        {
            return m_RealDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRenderTarget(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget)
        {
            return m_RealDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetDepthStencilSurface(THIS_ IDirect3DSurface9 * pNewZStencil)
        {
            return m_RealDevice->SetDepthStencilSurface(pNewZStencil);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetDepthStencilSurface(THIS_ IDirect3DSurface9 **ppZStencilSurface)
        {
            return m_RealDevice->GetDepthStencilSurface(ppZStencilSurface);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::BeginScene(THIS)
        {
            return m_RealDevice->BeginScene();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::EndScene(THIS)
        {
            return m_RealDevice->EndScene();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::Clear(THIS_ DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
        {
            return m_RealDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
        {
            return m_RealDevice->SetTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetTransform(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix)
        {
            return m_RealDevice->GetTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::MultiplyTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
        {
            return m_RealDevice->MultiplyTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetViewport(THIS_ CONST D3DVIEWPORT9 * pViewport)
        {
            return m_RealDevice->SetViewport(pViewport);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetViewport(THIS_ D3DVIEWPORT9 * pViewport)
        {
            return m_RealDevice->GetViewport(pViewport);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetMaterial(THIS_ CONST D3DMATERIAL9 * pMaterial)
        {
            return m_RealDevice->SetMaterial(pMaterial);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetMaterial(THIS_ D3DMATERIAL9 * pMaterial)
        {
            return m_RealDevice->GetMaterial(pMaterial);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetLight(THIS_ DWORD Index, CONST D3DLIGHT9 * pLight)
        {
            return m_RealDevice->SetLight(Index, pLight);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetLight(THIS_ DWORD Index, D3DLIGHT9 * pLight)
        {
            return m_RealDevice->GetLight(Index, pLight);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::LightEnable(THIS_ DWORD Index, BOOL Enable)
        {
            return m_RealDevice->LightEnable(Index, Enable);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetLightEnable(THIS_ DWORD Index, BOOL * pEnable)
        {
            return m_RealDevice->GetLightEnable(Index, pEnable);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetClipPlane(THIS_ DWORD Index, CONST float *pPlane)
        {
            return m_RealDevice->SetClipPlane(Index, pPlane);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetClipPlane(THIS_ DWORD Index, float *pPlane)
        {
            return m_RealDevice->GetClipPlane(Index, pPlane);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetRenderState(THIS_ D3DRENDERSTATETYPE State, DWORD Value)
        {
            return m_RealDevice->SetRenderState(State, Value);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetRenderState(THIS_ D3DRENDERSTATETYPE State, DWORD * pValue)
        {
            return m_RealDevice->GetRenderState(State, pValue);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateStateBlock(THIS_ D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB)
        {
            return m_RealDevice->CreateStateBlock(Type, ppSB);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::BeginStateBlock(THIS)
        {
            return m_RealDevice->BeginStateBlock();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::EndStateBlock(THIS_ IDirect3DStateBlock9 **ppSB)
        {
            return m_RealDevice->EndStateBlock(ppSB);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetClipStatus(THIS_ CONST D3DCLIPSTATUS9 * pClipStatus)
        {
            return m_RealDevice->SetClipStatus(pClipStatus);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetClipStatus(THIS_ D3DCLIPSTATUS9 * pClipStatus)
        {
            return m_RealDevice->GetClipStatus(pClipStatus);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetTexture(THIS_ DWORD Stage, IDirect3DBaseTexture9 **ppTexture)
        {
            return m_RealDevice->GetTexture(Stage, ppTexture);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetTexture(THIS_ DWORD Stage, IDirect3DBaseTexture9 * pTexture)
        {
            return m_RealDevice->SetTexture(Stage, pTexture);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetTextureStageState(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue)
        {
            return m_RealDevice->GetTextureStageState(Stage, Type, pValue);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetTextureStageState(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
        {
            return m_RealDevice->SetTextureStageState(Stage, Type, Value);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetSamplerState(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue)
        {
            return m_RealDevice->GetSamplerState(Sampler, Type, pValue);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetSamplerState(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
        {
            return m_RealDevice->SetSamplerState(Sampler, Type, Value);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::ValidateDevice(THIS_ DWORD * pNumPasses)
        {
            return m_RealDevice->ValidateDevice(pNumPasses);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPaletteEntries(THIS_ UINT PaletteNumber, CONST PALETTEENTRY * pEntries)
        {
            return m_RealDevice->SetPaletteEntries(PaletteNumber, pEntries);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPaletteEntries(THIS_ UINT PaletteNumber, PALETTEENTRY * pEntries)
        {
            return m_RealDevice->GetPaletteEntries(PaletteNumber, pEntries);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetCurrentTexturePalette(THIS_ UINT PaletteNumber)
        {
            return m_RealDevice->SetCurrentTexturePalette(PaletteNumber);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetCurrentTexturePalette(THIS_ UINT * PaletteNumber)
        {
            return m_RealDevice->GetCurrentTexturePalette(PaletteNumber);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetScissorRect(THIS_ CONST RECT * pRect)
        {
            return m_RealDevice->SetScissorRect(pRect);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetScissorRect(THIS_ RECT * pRect)
        {
            return m_RealDevice->GetScissorRect(pRect);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetSoftwareVertexProcessing(THIS_ BOOL bSoftware)
        {
            return m_RealDevice->SetSoftwareVertexProcessing(bSoftware);
        }

        BOOL STDMETHODCALLTYPE CVoodoo3DDevice9::GetSoftwareVertexProcessing(THIS)
        {
            return m_RealDevice->GetSoftwareVertexProcessing();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetNPatchMode(THIS_ float nSegments)
        {
            return m_RealDevice->SetNPatchMode(nSegments);
        }

        float STDMETHODCALLTYPE CVoodoo3DDevice9::GetNPatchMode(THIS)
        {
            return m_RealDevice->GetNPatchMode();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawPrimitive(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
        {
            return m_RealDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawIndexedPrimitive
            (
            THIS_ D3DPRIMITIVETYPE PrimitiveType,
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
            THIS_ D3DPRIMITIVETYPE PrimitiveType,
            UINT PrimitiveCount,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
            )
        {
            return m_RealDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawIndexedPrimitiveUP
            (
            THIS_ D3DPRIMITIVETYPE PrimitiveType,
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
            THIS_ UINT SrcStartIndex,
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
            THIS_ CONST D3DVERTEXELEMENT9 * pVertexElements,
            IDirect3DVertexDeclaration9 **ppDecl
            )
        {
            return m_RealDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9 * pDecl)
        {
            return m_RealDevice->SetVertexDeclaration(pDecl);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9 **ppDecl)
        {
            return m_RealDevice->GetVertexDeclaration(ppDecl);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetFVF(THIS_ DWORD FVF)
        {
            return m_RealDevice->SetFVF(FVF);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetFVF(THIS_ DWORD * pFVF)
        {
            return m_RealDevice->GetFVF(pFVF);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateVertexShader(THIS_ CONST DWORD * pFunction, IDirect3DVertexShader9 **ppShader)
        {
            return m_RealDevice->CreateVertexShader(pFunction, ppShader);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShader(THIS_ IDirect3DVertexShader9 * pShader)
        {
            return m_RealDevice->SetVertexShader(pShader);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShader(THIS_ IDirect3DVertexShader9 **ppShader)
        {
            return m_RealDevice->GetVertexShader(ppShader);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShaderConstantF(THIS_ UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShaderConstantF(THIS_ UINT StartRegister, float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShaderConstantI(THIS_ UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShaderConstantI(THIS_ UINT StartRegister, int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetVertexShaderConstantB(THIS_ UINT StartRegister, CONST BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->SetVertexShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetVertexShaderConstantB(THIS_ UINT StartRegister, BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->GetVertexShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetStreamSource
            (
            THIS_ UINT StreamNumber,
            IDirect3DVertexBuffer9 * pStreamData,
            UINT OffsetInBytes,
            UINT Stride
            )
        {
            return m_RealDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetStreamSource
            (
            THIS_ UINT StreamNumber,
            IDirect3DVertexBuffer9 **ppStreamData,
            UINT * pOffsetInBytes,
            UINT * pStride
            )
        {
            return m_RealDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetStreamSourceFreq(THIS_ UINT StreamNumber, UINT Setting)
        {
            return m_RealDevice->SetStreamSourceFreq(StreamNumber, Setting);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetStreamSourceFreq(THIS_ UINT StreamNumber, UINT * pSetting)
        {
            return m_RealDevice->GetStreamSourceFreq(StreamNumber, pSetting);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetIndices(THIS_ IDirect3DIndexBuffer9 * pIndexData)
        {
            return m_RealDevice->SetIndices(pIndexData);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetIndices(THIS_ IDirect3DIndexBuffer9 **ppIndexData)
        {
            return m_RealDevice->GetIndices(ppIndexData);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreatePixelShader(THIS_ CONST DWORD * pFunction, IDirect3DPixelShader9 **ppShader)
        {
            return m_RealDevice->CreatePixelShader(pFunction, ppShader);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShader(THIS_ IDirect3DPixelShader9 * pShader)
        {
            return m_RealDevice->SetPixelShader(pShader);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShader(THIS_ IDirect3DPixelShader9 **ppShader)
        {
            return m_RealDevice->GetPixelShader(ppShader);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShaderConstantF(THIS_ UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShaderConstantF (THIS_ UINT StartRegister, float *pConstantData, UINT Vector4fCount)
        {
            return m_RealDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShaderConstantI(THIS_ UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShaderConstantI(THIS_ UINT StartRegister, int *pConstantData, UINT Vector4iCount)
        {
            return m_RealDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::SetPixelShaderConstantB(THIS_ UINT StartRegister, CONST BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->SetPixelShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::GetPixelShaderConstantB(THIS_ UINT StartRegister, BOOL * pConstantData, UINT boolCount)
        {
            return m_RealDevice->GetPixelShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawRectPatch(THIS_ UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO * pRectPatchInfo)
        {
            return m_RealDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DrawTriPatch(THIS_ UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO * pTriPatchInfo)
        {
            return m_RealDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::DeletePatch(THIS_ UINT Handle)
        {
            return m_RealDevice->DeletePatch(Handle);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::CreateQuery(THIS_ D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery)
        {
            return m_RealDevice->CreateQuery(Type, ppQuery);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice9::VSSetupDevice()
        {
            if (m_VertDecl) m_VertDecl->Release();
            if (m_VertDeclT) m_VertDeclT->Release();

            ILoggerRef logger = gpVoodooCore->GetLogger();

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
                Format("Prepping for %1% by %2% target.") << (uint32_t)viewport.Width << (uint32_t)viewport.Height
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
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Error loading shader: %1%") << exc.what());
            }

            if (lpparam_rcpres)
            {
                Float4 rcpres_val = {1.0f / bbDesc.Width, 1.0f / bbDesc.Height, 0, 0};
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