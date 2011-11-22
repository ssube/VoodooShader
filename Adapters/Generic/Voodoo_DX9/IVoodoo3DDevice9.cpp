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

#include "IVoodoo3DDevice9.hpp"

#include "DX9_Adapter.hpp"
#include "DX9_Version.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        IVoodoo3DDevice9::IVoodoo3DDevice9(IDirect3D9 * pVoodooObject, IDirect3DDevice9 * pRealDevice) :
            m_d3d(pVoodooObject), mRealDevice(pRealDevice)
        { };

        /* IUnknown methods */
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::QueryInterface(THIS_ REFIID riid, void **ppvObj)
        {
            return mRealDevice->QueryInterface(riid, ppvObj);
        }

        ULONG STDMETHODCALLTYPE IVoodoo3DDevice9::AddRef(THIS)
        {
            return mRealDevice->AddRef();
        }

        ULONG STDMETHODCALLTYPE IVoodoo3DDevice9::Release(THIS)
        {
            ULONG count = mRealDevice->Release();
            if (0 == count) delete this;

            return count;
        }

        /* IDirect3DDevice9 methods */
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::TestCooperativeLevel (THIS)
        {
            return mRealDevice->TestCooperativeLevel();
        }

        UINT STDMETHODCALLTYPE IVoodoo3DDevice9::GetAvailableTextureMem(THIS)
        {
            return mRealDevice->GetAvailableTextureMem();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::EvictManagedResources(THIS)
        {
            return mRealDevice->EvictManagedResources();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetDirect3D(THIS_ IDirect3D9 **ppD3D9)
        {

            // Let the device validate the incoming pointer for us
            HRESULT hr = mRealDevice->GetDirect3D(ppD3D9);
            if (SUCCEEDED(hr)) *ppD3D9 = m_d3d;

            return hr;
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetDeviceCaps(THIS_ D3DCAPS9 * pCaps)
        {
            return mRealDevice->GetDeviceCaps(pCaps);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetDisplayMode(THIS_ UINT iSwapChain, D3DDISPLAYMODE * pMode)
        {
            return mRealDevice->GetDisplayMode(iSwapChain, pMode);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetCreationParameters(THIS_ D3DDEVICE_CREATION_PARAMETERS * pParameters)
        {
            return mRealDevice->GetCreationParameters(pParameters);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetCursorProperties(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap)
        {
            return mRealDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
        }

        void STDMETHODCALLTYPE IVoodoo3DDevice9::SetCursorPosition(THIS_ int X, int Y, DWORD Flags)
        {
            mRealDevice->SetCursorPosition(X, Y, Flags);
        }

        BOOL STDMETHODCALLTYPE IVoodoo3DDevice9::ShowCursor(THIS_ BOOL bShow)
        {
            return mRealDevice->ShowCursor(bShow);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateAdditionalSwapChain
            (
            THIS_ D3DPRESENT_PARAMETERS * pPresentationParameters,
            IDirect3DSwapChain9 **pSwapChain
            )
        {
            return mRealDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetSwapChain(THIS_ UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain)
        {
            return mRealDevice->GetSwapChain(iSwapChain, pSwapChain);
        }

        UINT STDMETHODCALLTYPE IVoodoo3DDevice9::GetNumberOfSwapChains(THIS)
        {
            return mRealDevice->GetNumberOfSwapChains();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::Reset(THIS_ D3DPRESENT_PARAMETERS * pPresentationParameters)
        {
            return mRealDevice->Reset(pPresentationParameters);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::Present
        (
            THIS_ CONST RECT * pSourceRect,
            CONST RECT * pDestRect,
            HWND hDestWindowOverride,
            CONST RGNDATA * pDirtyRegion
        )
        {
            if (gpVoodooCore && testShader)
            {
                ILoggerRef logger = gpVoodooCore->GetLogger();
                IAdapterRef adapter = gpVoodooCore->GetAdapter();

                // IDirect3DBaseTexture9 * tex;
                // IDirect3DSurface9 * rts;
                // ;
                // mRealDevice->GetTexture(0, &tex);
                // mRealDevice->GetRenderTarget(0, &rts);
                HRESULT hr = mRealDevice->StretchRect(backbufferSurf, nullptr, surface_ThisFrame, nullptr, D3DTEXF_NONE);
                if (FAILED(hr))
                {
                    logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to stretch backbuffer to scratch texture.");
                }

                hr = mRealDevice->SetRenderTarget(0, backbufferSurf);
                if (FAILED(hr))
                {
                    logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to set render target.");
                }

                VoodooShader::ITechnique* tech = testShader->GetDefaultTechnique();
                VoodooShader::IPass* pass = tech->GetPass(0);

                adapter->SetPass(pass);

                adapter->DrawGeometry(4, nullptr);

                adapter->SetPass();
            }

            // mRealDevice->SetRenderTarget(0, rts);
            // mRealDevice->SetTexture(0, tex);
            return mRealDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetBackBuffer
        (
            THIS_ UINT iSwapChain,
            UINT iBackBuffer,
            D3DBACKBUFFER_TYPE Type,
            IDirect3DSurface9 **ppBackBuffer
        )
        {
            return mRealDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetRasterStatus(THIS_ UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus)
        {
            return mRealDevice->GetRasterStatus(iSwapChain, pRasterStatus);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetDialogBoxMode(THIS_ BOOL bEnableDialogs)
        {
            return mRealDevice->SetDialogBoxMode(bEnableDialogs);
        }

        void STDMETHODCALLTYPE IVoodoo3DDevice9::SetGammaRamp(THIS_ UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp)
        {
            return mRealDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
        }

        void STDMETHODCALLTYPE IVoodoo3DDevice9::GetGammaRamp(THIS_ UINT iSwapChain, D3DGAMMARAMP * pRamp)
        {
            return mRealDevice->GetGammaRamp(iSwapChain, pRamp);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateTexture
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
            return mRealDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateVolumeTexture
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
            return mRealDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateCubeTexture
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
            return mRealDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateVertexBuffer
        (
            THIS_ UINT Length,
            DWORD Usage,
            DWORD FVF,
            D3DPOOL Pool,
            IDirect3DVertexBuffer9 **ppVertexBuffer,
            HANDLE * pSharedHandle
        )
        {
            return mRealDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateIndexBuffer
        (
            THIS_ UINT Length,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DIndexBuffer9 **ppIndexBuffer,
            HANDLE * pSharedHandle
        )
        {
            return mRealDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateRenderTarget
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
            return mRealDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateDepthStencilSurface
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
            return mRealDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::UpdateSurface
        (
            THIS_ IDirect3DSurface9 * pSourceSurface,
            CONST RECT * pSourceRect,
            IDirect3DSurface9 * pDestinationSurface,
            CONST POINT * pDestPoint
        )
        {
            return mRealDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::UpdateTexture
        (
            THIS_ IDirect3DBaseTexture9 * pSourceTexture,
            IDirect3DBaseTexture9 * pDestinationTexture
        )
        {
            return mRealDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetRenderTargetData(THIS_ IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface)
        {
            return mRealDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetFrontBufferData(THIS_ UINT iSwapChain, IDirect3DSurface9 * pDestSurface)
        {
            return mRealDevice->GetFrontBufferData(iSwapChain, pDestSurface);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::StretchRect
            (
            THIS_ IDirect3DSurface9 * pSourceSurface,
            CONST RECT * pSourceRect,
            IDirect3DSurface9 * pDestSurface,
            CONST RECT * pDestRect,
            D3DTEXTUREFILTERTYPE Filter
            )
        {
            return mRealDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::ColorFill(THIS_ IDirect3DSurface9 * pSurface, CONST RECT * pRect, D3DCOLOR color)
        {
            return mRealDevice->ColorFill(pSurface, pRect, color);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateOffscreenPlainSurface
            (
            THIS_ UINT Width,
            UINT Height,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DSurface9 **ppSurface,
            HANDLE * pSharedHandle
            )
        {
            return mRealDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetRenderTarget(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget)
        {
            return mRealDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetRenderTarget(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget)
        {
            return mRealDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetDepthStencilSurface(THIS_ IDirect3DSurface9 * pNewZStencil)
        {
            return mRealDevice->SetDepthStencilSurface(pNewZStencil);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetDepthStencilSurface(THIS_ IDirect3DSurface9 **ppZStencilSurface)
        {
            return mRealDevice->GetDepthStencilSurface(ppZStencilSurface);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::BeginScene(THIS)
        {
            return mRealDevice->BeginScene();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::EndScene(THIS)
        {
            return mRealDevice->EndScene();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::Clear(THIS_ DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
        {
            return mRealDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
        {
            return mRealDevice->SetTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetTransform(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix)
        {
            return mRealDevice->GetTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::MultiplyTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix)
        {
            return mRealDevice->MultiplyTransform(State, pMatrix);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetViewport(THIS_ CONST D3DVIEWPORT9 * pViewport)
        {
            return mRealDevice->SetViewport(pViewport);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetViewport(THIS_ D3DVIEWPORT9 * pViewport)
        {
            return mRealDevice->GetViewport(pViewport);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetMaterial(THIS_ CONST D3DMATERIAL9 * pMaterial)
        {
            return mRealDevice->SetMaterial(pMaterial);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetMaterial(THIS_ D3DMATERIAL9 * pMaterial)
        {
            return mRealDevice->GetMaterial(pMaterial);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetLight(THIS_ DWORD Index, CONST D3DLIGHT9 * pLight)
        {
            return mRealDevice->SetLight(Index, pLight);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetLight(THIS_ DWORD Index, D3DLIGHT9 * pLight)
        {
            return mRealDevice->GetLight(Index, pLight);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::LightEnable(THIS_ DWORD Index, BOOL Enable)
        {
            return mRealDevice->LightEnable(Index, Enable);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetLightEnable(THIS_ DWORD Index, BOOL * pEnable)
        {
            return mRealDevice->GetLightEnable(Index, pEnable);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetClipPlane(THIS_ DWORD Index, CONST float *pPlane)
        {
            return mRealDevice->SetClipPlane(Index, pPlane);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetClipPlane(THIS_ DWORD Index, float *pPlane)
        {
            return mRealDevice->GetClipPlane(Index, pPlane);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetRenderState(THIS_ D3DRENDERSTATETYPE State, DWORD Value)
        {
            return mRealDevice->SetRenderState(State, Value);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetRenderState(THIS_ D3DRENDERSTATETYPE State, DWORD * pValue)
        {
            return mRealDevice->GetRenderState(State, pValue);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateStateBlock(THIS_ D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB)
        {
            return mRealDevice->CreateStateBlock(Type, ppSB);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::BeginStateBlock(THIS)
        {
            return mRealDevice->BeginStateBlock();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::EndStateBlock(THIS_ IDirect3DStateBlock9 **ppSB)
        {
            return mRealDevice->EndStateBlock(ppSB);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetClipStatus(THIS_ CONST D3DCLIPSTATUS9 * pClipStatus)
        {
            return mRealDevice->SetClipStatus(pClipStatus);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetClipStatus(THIS_ D3DCLIPSTATUS9 * pClipStatus)
        {
            return mRealDevice->GetClipStatus(pClipStatus);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetTexture(THIS_ DWORD Stage, IDirect3DBaseTexture9 **ppTexture)
        {
            return mRealDevice->GetTexture(Stage, ppTexture);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetTexture(THIS_ DWORD Stage, IDirect3DBaseTexture9 * pTexture)
        {
            return mRealDevice->SetTexture(Stage, pTexture);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetTextureStageState(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue)
        {
            return mRealDevice->GetTextureStageState(Stage, Type, pValue);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetTextureStageState(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
        {
            return mRealDevice->SetTextureStageState(Stage, Type, Value);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetSamplerState(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue)
        {
            return mRealDevice->GetSamplerState(Sampler, Type, pValue);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetSamplerState(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
        {
            return mRealDevice->SetSamplerState(Sampler, Type, Value);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::ValidateDevice(THIS_ DWORD * pNumPasses)
        {
            return mRealDevice->ValidateDevice(pNumPasses);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetPaletteEntries(THIS_ UINT PaletteNumber, CONST PALETTEENTRY * pEntries)
        {
            return mRealDevice->SetPaletteEntries(PaletteNumber, pEntries);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetPaletteEntries(THIS_ UINT PaletteNumber, PALETTEENTRY * pEntries)
        {
            return mRealDevice->GetPaletteEntries(PaletteNumber, pEntries);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetCurrentTexturePalette(THIS_ UINT PaletteNumber)
        {
            return mRealDevice->SetCurrentTexturePalette(PaletteNumber);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetCurrentTexturePalette(THIS_ UINT * PaletteNumber)
        {
            return mRealDevice->GetCurrentTexturePalette(PaletteNumber);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetScissorRect(THIS_ CONST RECT * pRect)
        {
            return mRealDevice->SetScissorRect(pRect);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetScissorRect(THIS_ RECT * pRect)
        {
            return mRealDevice->GetScissorRect(pRect);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetSoftwareVertexProcessing(THIS_ BOOL bSoftware)
        {
            return mRealDevice->SetSoftwareVertexProcessing(bSoftware);
        }

        BOOL STDMETHODCALLTYPE IVoodoo3DDevice9::GetSoftwareVertexProcessing(THIS)
        {
            return mRealDevice->GetSoftwareVertexProcessing();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetNPatchMode(THIS_ float nSegments)
        {
            return mRealDevice->SetNPatchMode(nSegments);
        }

        float STDMETHODCALLTYPE IVoodoo3DDevice9::GetNPatchMode(THIS)
        {
            return mRealDevice->GetNPatchMode();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::DrawPrimitive(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
        {
            return mRealDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::DrawIndexedPrimitive
            (
            THIS_ D3DPRIMITIVETYPE PrimitiveType,
            INT BaseVertexIndex,
            UINT MinVertexIndex,
            UINT NumVertices,
            UINT startIndex,
            UINT primCount
            )
        {
            return mRealDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::DrawPrimitiveUP
            (
            THIS_ D3DPRIMITIVETYPE PrimitiveType,
            UINT PrimitiveCount,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
            )
        {
            return mRealDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::DrawIndexedPrimitiveUP
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
            return mRealDevice->DrawIndexedPrimitiveUP
                (
                PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat,
                pVertexStreamZeroData, VertexStreamZeroStride
                );
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::ProcessVertices
            (
            THIS_ UINT SrcStartIndex,
            UINT DestIndex,
            UINT VertexCount,
            IDirect3DVertexBuffer9 * pDestBuffer,
            IDirect3DVertexDeclaration9 * pVertexDecl,
            DWORD Flags
            )
        {
            return mRealDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateVertexDeclaration
            (
            THIS_ CONST D3DVERTEXELEMENT9 * pVertexElements,
            IDirect3DVertexDeclaration9 **ppDecl
            )
        {
            return mRealDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9 * pDecl)
        {
            return mRealDevice->SetVertexDeclaration(pDecl);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetVertexDeclaration(THIS_ IDirect3DVertexDeclaration9 **ppDecl)
        {
            return mRealDevice->GetVertexDeclaration(ppDecl);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetFVF(THIS_ DWORD FVF)
        {
            return mRealDevice->SetFVF(FVF);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetFVF(THIS_ DWORD * pFVF)
        {
            return mRealDevice->GetFVF(pFVF);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateVertexShader(THIS_ CONST DWORD * pFunction, IDirect3DVertexShader9 **ppShader)
        {
            return mRealDevice->CreateVertexShader(pFunction, ppShader);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetVertexShader(THIS_ IDirect3DVertexShader9 * pShader)
        {
            return mRealDevice->SetVertexShader(pShader);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetVertexShader(THIS_ IDirect3DVertexShader9 **ppShader)
        {
            return mRealDevice->GetVertexShader(ppShader);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetVertexShaderConstantF(THIS_ UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
        {
            return mRealDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetVertexShaderConstantF(THIS_ UINT StartRegister, float *pConstantData, UINT Vector4fCount)
        {
            return mRealDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetVertexShaderConstantI(THIS_ UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
        {
            return mRealDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetVertexShaderConstantI(THIS_ UINT StartRegister, int *pConstantData, UINT Vector4iCount)
        {
            return mRealDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetVertexShaderConstantB(THIS_ UINT StartRegister, CONST BOOL * pConstantData, UINT boolCount)
        {
            return mRealDevice->SetVertexShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetVertexShaderConstantB(THIS_ UINT StartRegister, BOOL * pConstantData, UINT boolCount)
        {
            return mRealDevice->GetVertexShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetStreamSource
            (
            THIS_ UINT StreamNumber,
            IDirect3DVertexBuffer9 * pStreamData,
            UINT OffsetInBytes,
            UINT Stride
            )
        {
            return mRealDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetStreamSource
            (
            THIS_ UINT StreamNumber,
            IDirect3DVertexBuffer9 **ppStreamData,
            UINT * pOffsetInBytes,
            UINT * pStride
            )
        {
            return mRealDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetStreamSourceFreq(THIS_ UINT StreamNumber, UINT Setting)
        {
            return mRealDevice->SetStreamSourceFreq(StreamNumber, Setting);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetStreamSourceFreq(THIS_ UINT StreamNumber, UINT * pSetting)
        {
            return mRealDevice->GetStreamSourceFreq(StreamNumber, pSetting);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetIndices(THIS_ IDirect3DIndexBuffer9 * pIndexData)
        {
            return mRealDevice->SetIndices(pIndexData);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetIndices(THIS_ IDirect3DIndexBuffer9 **ppIndexData)
        {
            return mRealDevice->GetIndices(ppIndexData);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreatePixelShader(THIS_ CONST DWORD * pFunction, IDirect3DPixelShader9 **ppShader)
        {
            return mRealDevice->CreatePixelShader(pFunction, ppShader);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetPixelShader(THIS_ IDirect3DPixelShader9 * pShader)
        {
            return mRealDevice->SetPixelShader(pShader);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetPixelShader(THIS_ IDirect3DPixelShader9 **ppShader)
        {
            return mRealDevice->GetPixelShader(ppShader);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetPixelShaderConstantF(THIS_ UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount)
        {
            return mRealDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetPixelShaderConstantF (THIS_ UINT StartRegister, float *pConstantData, UINT Vector4fCount)
        {
            return mRealDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetPixelShaderConstantI(THIS_ UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount)
        {
            return mRealDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetPixelShaderConstantI(THIS_ UINT StartRegister, int *pConstantData, UINT Vector4iCount)
        {
            return mRealDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::SetPixelShaderConstantB(THIS_ UINT StartRegister, CONST BOOL * pConstantData, UINT boolCount)
        {
            return mRealDevice->SetPixelShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::GetPixelShaderConstantB(THIS_ UINT StartRegister, BOOL * pConstantData, UINT boolCount)
        {
            return mRealDevice->GetPixelShaderConstantB(StartRegister, pConstantData, boolCount);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::DrawRectPatch(THIS_ UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO * pRectPatchInfo)
        {
            return mRealDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::DrawTriPatch(THIS_ UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO * pTriPatchInfo)
        {
            return mRealDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
        }
        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::DeletePatch(THIS_ UINT Handle)
        {
            return mRealDevice->DeletePatch(Handle);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DDevice9::CreateQuery(THIS_ D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery)
        {
            return mRealDevice->CreateQuery(Type, ppQuery);
        }
    }
}