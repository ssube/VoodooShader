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

#include "CVoodoo3DDevice8.hpp"
// CVoodoo3D8
#include "CVoodoo3DSurface8.hpp"
#include "CVoodoo3DTexture8.hpp"
#include "CVoodoo3D8.hpp"
// Voodoo DX89
#include "DX9_Version.hpp"
// Voodoo Core
#include "StringFormat.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        CVoodoo3DDevice8::CVoodoo3DDevice8(CVoodoo3D8 * pObject, IDirect3DDevice9 * pRealDevice) :
            m_RealDevice(pRealDevice), m_Object(pObject),
            m_CurrentVertexShader(0), m_CurrentPixelShader(0), m_LastBaseIndex(0)
        {
            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CVoodoo3DDevice8(%p) == %p") << realDevice << this);

            if (m_RealDevice) m_RealDevice->AddRef();
            //if (m_Object) m_Object->AddRef();
        }

        CVoodoo3DDevice8::~CVoodoo3DDevice8()
        {
            //if (m_Object) m_Object->Release();
            //m_Object = nullptr;

            if (m_RealDevice) m_RealDevice->Release();
            m_RealDevice = nullptr;
        }

        // IUnknown methods
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::QueryInterface(REFIID riid, void **ppvObj)
        {
            if (!ppvObj)
            {
                return D3DERR_INVALIDCALL;
            }
            else if (riid == IID_IUnknown)
            {
                *ppvObj = this;
                ((IUnknown*)(*ppvObj))->AddRef();
                return D3D_OK;
            }
            else if (riid == IID_IDirect3DDevice8)
            {
                *ppvObj = this;
                ((IDirect3DDevice9*)(*ppvObj))->AddRef();
                return D3D_OK;
            }
            else
            {
                *ppvObj = nullptr;
                return D3DERR_INVALIDCALL;
            }
        }
        ULONG STDMETHODCALLTYPE CVoodoo3DDevice8::AddRef()
        {
            ULONG count = ++m_Refs;

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::AddRef() == %u") << count);

            return count;
        }
        ULONG STDMETHODCALLTYPE CVoodoo3DDevice8::Release()
        {
            ULONG count = --m_Refs;

            if (count == 0)
            {
                delete this;
            }

            return count;
        }

        // IDirect3DDevice8 methods
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ApplyStateBlock(DWORD Token)
        {
            UNREFERENCED_PARAMETER(Token);
            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::ApplyStateBlock(%u) == UNUSED") << Token);

            return D3DERR_INVALIDCALL;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::BeginScene()
        {
            HRESULT hr = m_RealDevice->BeginScene();

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::BeginScene() == %d") << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::BeginStateBlock()
        {
            HRESULT hr = m_RealDevice->BeginStateBlock();

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::BeginStateBlock() == %d") << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CaptureStateBlock(DWORD Token)
        {
            UNREFERENCED_PARAMETER(Token);
            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CaptureStateBlock(%u) == UNUSED") << Token);

            return D3DERR_INVALIDCALL;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::Clear(DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
        {
            HRESULT hr = m_RealDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::Clear(%u, %p, %u, %u, %f, %u) == %d") << Count << 
                pRects << Flags << Color << Z << Stencil << hr
            );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::TestCooperativeLevel()
        {
            HRESULT hr = m_RealDevice->TestCooperativeLevel();

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::TestCooperativeLevel() == %d") << hr);

            return hr;
        }

        UINT STDMETHODCALLTYPE CVoodoo3DDevice8::GetAvailableTextureMem()
        {
            UINT mem = m_RealDevice->GetAvailableTextureMem();

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetAvailableTextureMem() == %u") << mem);

            return mem;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ResourceManagerDiscardBytes(DWORD Bytes)
        {
            UNREFERENCED_PARAMETER(Bytes);
            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::ResourceManagerDiscardBytes(%u) == UNUSED") << Bytes);

            return D3DERR_INVALIDCALL;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDirect3D(IDirect3D8 **ppD3D8)
        {
            (*ppD3D8) = (IDirect3D8 *)m_Object;

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetDirect3D(%p) == D3D_OK") << *ppD3D8);

            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDeviceCaps(D3DCAPS8 *pCaps)
        {
            D3DCAPS9 rCaps;

            HRESULT hr = m_RealDevice->GetDeviceCaps(&rCaps);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetDeviceCaps(%p) == %d") << pCaps << hr);

            if (SUCCEEDED(hr))
            {
                memcpy(pCaps, &rCaps, sizeof(D3DCAPS8));
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDisplayMode(D3DDISPLAYMODE *pMode)
        {
            HRESULT hr = m_RealDevice->GetDisplayMode(0, pMode);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetDisplayMode(%p) == %d") << pMode << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
        {
            HRESULT hr = m_RealDevice->GetCreationParameters(pParameters);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetCreationParameters(%p) == %d") << pParameters << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap)
        {
            CVoodoo3DSurface8 *rCursor = (CVoodoo3DSurface8 *) pCursorBitmap;

            HRESULT hr = m_RealDevice->SetCursorProperties(XHotSpot, YHotSpot, rCursor->RealSurface());

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetCursorProperties(%u, %u, %p) == %d") << XHotSpot << YHotSpot << pCursorBitmap << hr
            );

            return hr;
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice8::SetCursorPosition(UINT X, UINT Y, DWORD Flags)
        {
            m_RealDevice->SetCursorPosition(X, Y, Flags);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetCursorPosition(%i, %i, %u)") << X << Y << Flags);
        }

        BOOL STDMETHODCALLTYPE CVoodoo3DDevice8::ShowCursor(BOOL bShow)
        {
            BOOL show = m_RealDevice->ShowCursor(bShow);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::ShowCursor(%i) == %d") << bShow << show);

            return show;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateAdditionalSwapChain
            (
            D3DPRESENT_PARAMETERS8  *pPresentationParameters,
            IDirect3DSwapChain8 **pSwapChain
            )
        {
            HRESULT hr = m_RealDevice->CreateAdditionalSwapChain
                ((D3DPRESENT_PARAMETERS *) pPresentationParameters, (IDirect3DSwapChain9 **) pSwapChain);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::CreateAdditionalSwapChain(%p, %p) == %d") << pPresentationParameters << *pSwapChain << hr
            );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::Reset(D3DPRESENT_PARAMETERS8 *pPresentationParameters)
        {
            HRESULT hr = m_RealDevice->Reset((D3DPRESENT_PARAMETERS *) pPresentationParameters);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::Reset(%p) == %d") << pPresentationParameters << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::Present
        (
            CONST RECT *pSourceRect,
            CONST RECT   *pDestRect,
            HWND hDestWindowOverride,
            CONST RGNDATA *pDirtyRegion
        )
        {
            HRESULT hr = m_RealDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::Present(%p, %p, %p, %p) == %d") << pSourceRect << pDestRect << 
                hDestWindowOverride << pDirtyRegion << hr
            );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer)
        {
            IDirect3DSurface9 *rBackbuffer;
            HRESULT hr = m_RealDevice->GetBackBuffer(0, BackBuffer, Type, &rBackbuffer);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetBackBuffer(%u, %u, %p) == %d") << BackBuffer << Type << *ppBackBuffer << hr
            );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wBackbuffer = new CVoodoo3DSurface8(this, rBackbuffer);
                (*ppBackBuffer) = (IDirect3DSurface8 *) wBackbuffer;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetRasterStatus(D3DRASTER_STATUS *pRasterStatus)
        {
            HRESULT hr = m_RealDevice->GetRasterStatus(0, pRasterStatus);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetRasterStatus(%p) == %d") << pRasterStatus << hr);

            return hr;
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice8::SetGammaRamp(DWORD Flags, const D3DGAMMARAMP *pRamp)
        {
            m_RealDevice->SetGammaRamp(0, Flags, pRamp);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetRasterStatus(%u, %p)") << Flags << pRamp);

            return;
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice8::GetGammaRamp(D3DGAMMARAMP *pRamp)
        {
            m_RealDevice->GetGammaRamp(0, pRamp);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetRasterStatus(%p)") << pRamp);

            return;
        }

        /**
         * Creates a texture in the CVoodoo3D8 runtime. The new texture must be registered with the core and, for shader 
         * purposes, we should attempt to create it as a render-target.
         */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateTexture
        (
            UINT width,
            UINT height,
            UINT levels,
            DWORD usage,
            D3DFORMAT format,
            D3DPOOL pool,
            IDirect3DTexture8 **ppTexture
        )
        {
            IDirect3DTexture9 * rTexture = nullptr;
            HRESULT hr = D3DERR_INVALIDCALL;

            // If not a render-target, attempt to create it as one
            if (pool == D3DPOOL_DEFAULT && !(usage & D3DUSAGE_RENDERTARGET))
            {
                hr = m_RealDevice->CreateTexture(width, height, levels, usage | D3DUSAGE_RENDERTARGET, format, pool, &rTexture, NULL);
            }

            // That failed, create as a standard texture
            if (FAILED(hr))
            {
                hr = m_RealDevice->CreateTexture(width, height, levels, usage, format, pool, &rTexture, NULL);
            }

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::CreateTexture(%u, %u, %u, %u, %u, %u, %p) == %d") << width << height << levels <<
                    usage << format << pool << *ppTexture << hr
            );

            // One succeeded, the texture exists. We need to register it with the Voodoo core.
            if (SUCCEEDED(hr))
            {
                // ! @todo Set up the core texture registration for Voodoo DX8.9 and track
                // filenames somehow
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, rTexture);
                (*ppTexture) = (IDirect3DTexture8 *) wTexture;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateVolumeTexture
        (
            UINT width,
            UINT height,
            UINT depth,
            UINT levels,
            DWORD usage,
            D3DFORMAT format,
            D3DPOOL pool,
            IDirect3DVolumeTexture8 **ppVolumeTexture
        )
        {
            IDirect3DVolumeTexture9 *rTexture;
            HRESULT hr = m_RealDevice->CreateVolumeTexture(width, height, depth, levels, usage, format, pool, &rTexture, NULL);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::CreateVolumeTexture(%u, %u, %u, %u, %u, %u, %u, %p) == %d") << width << height <<
                    depth << levels << usage << format << pool << *ppVolumeTexture << hr
            );

            // ! @todo Set up the core volume texture registration for Voodoo DX8.9
            if (SUCCEEDED(hr))
            {
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, (IDirect3DTexture9 *) rTexture);
                (*ppVolumeTexture) = (IDirect3DVolumeTexture8 *) wTexture;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateCubeTexture
        (
            UINT edgeLength,
            UINT levels,
            DWORD usage,
            D3DFORMAT format,
            D3DPOOL pool,
            IDirect3DCubeTexture8 **ppCubeTexture
        )
        {
            IDirect3DCubeTexture9 *rTexture;
            HRESULT hr = m_RealDevice->CreateCubeTexture(edgeLength, levels, usage, format, pool, &rTexture, NULL);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::CreateCubeTexture(%u, %u, %u, %u, %u, %p) == %d") << edgeLength << levels <<
                    usage << format << pool << *ppCubeTexture << hr
            );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, (IDirect3DTexture9 *) rTexture);
                (*ppCubeTexture) = (IDirect3DCubeTexture8 *) wTexture;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateVertexBuffer
            (
            UINT Length,
            DWORD Usage,
            DWORD FVF,
            D3DPOOL Pool,
            IDirect3DVertexBuffer8 **ppVertexBuffer
            )
        {
            HRESULT hr = m_RealDevice->CreateVertexBuffer
                (Length, Usage, FVF, Pool, (IDirect3DVertexBuffer9 **) ppVertexBuffer, NULL);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CreateVertexBuffer(%u, %u, %u, %u, %p) == %d") <<
                    Length << Usage << FVF << Pool << *ppVertexBuffer << hr
                );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateIndexBuffer
            (
            UINT length,
            DWORD usage,
            D3DFORMAT format,
            D3DPOOL pool,
            IDirect3DIndexBuffer8 **ppIndexBuffer
            )
        {
            HRESULT hr = m_RealDevice->CreateIndexBuffer
                (length, usage, format, pool, (IDirect3DIndexBuffer9 **) ppIndexBuffer, nullptr);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CreateIndexBuffer(%u, %u, %u, %u, %p) == %d") <<
                length << usage << format << pool << *ppIndexBuffer << hr
            );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateRenderTarget
            (
            UINT Width,
            UINT Height,
            D3DFORMAT format,
            D3DMULTISAMPLE_TYPE MultiSample,
            BOOL Lockable,
            IDirect3DSurface8 **ppSurface
            )
        {
            IDirect3DSurface9 *rSurface;
            HRESULT hr = m_RealDevice->CreateRenderTarget(Width, Height, format, MultiSample, 0, Lockable, &rSurface, nullptr);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::CreateRenderTarget(%u, %u, %u, %u, %u, %p) == %d") << Width << Height << format <<
                    MultiSample << Lockable << *ppSurface << hr
            );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wSurface = new CVoodoo3DSurface8(this, rSurface);
                (*ppSurface) = (IDirect3DSurface8 *) wSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateDepthStencilSurface
            (
            UINT Width,
            UINT Height,
            D3DFORMAT format,
            D3DMULTISAMPLE_TYPE MultiSample,
            IDirect3DSurface8 **ppSurface
            )
        {
            IDirect3DSurface9 *rSurface;

            HRESULT hr = m_RealDevice->CreateDepthStencilSurface(Width, Height, format, MultiSample, 0, 0, &rSurface, nullptr);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::CreateDepthStencilSurface(%u, %u, %u, %u, %p) == %d") << Width << Height << 
                    format << MultiSample << *ppSurface << hr
            );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wSurface = new CVoodoo3DSurface8(this, rSurface);
                (*ppSurface) = (IDirect3DSurface8 *) wSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateImageSurface(UINT Width, UINT Height, D3DFORMAT format, IDirect3DSurface8 **ppSurface)
        {
            IDirect3DSurface9 *rSurface;

            HRESULT hr = m_RealDevice->CreateOffscreenPlainSurface(Width, Height, format, D3DPOOL_DEFAULT, &rSurface, nullptr);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CreateImageSurface(%u, %u, %u, %p) == %d") << 
                    Width << Height << format << *ppSurface << hr
            );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wSurface = new CVoodoo3DSurface8(this, rSurface);
                (*ppSurface) = (IDirect3DSurface8 *) wSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CopyRects
            (
            IDirect3DSurface8  *pSourceSurface,
            CONST RECT   *pSourceRectsArray,
            UINT cRects,
            IDirect3DSurface8 *pDestinationSurface,
            CONST POINT *pDestPointsArray
            )
        {
            UNREFERENCED_PARAMETER(pDestPointsArray);
            UNREFERENCED_PARAMETER(cRects);

            CVoodoo3DSurface8 *wSource = (CVoodoo3DSurface8 *) pSourceSurface;
            CVoodoo3DSurface8 *wDest = (CVoodoo3DSurface8 *) pDestinationSurface;

            HRESULT hr = m_RealDevice->StretchRect
                (wSource->RealSurface(), pSourceRectsArray, wDest->RealSurface(), pSourceRectsArray, D3DTEXF_NONE);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CopyRects(%p, %p, %u, %p, %p) == %d") <<
                    pSourceSurface << pSourceRectsArray << cRects << pDestinationSurface << pDestPointsArray << hr
            );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::UpdateTexture(IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture)
        {
            CVoodoo3DTexture8 *wSource = (CVoodoo3DTexture8 *) pSourceTexture;
            CVoodoo3DTexture8 *wDest = (CVoodoo3DTexture8 *) pDestinationTexture;

            HRESULT hr = m_RealDevice->UpdateTexture(wSource->RealTexture(), wDest->RealTexture());

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::UpdateTexture(%p, %p) == %d") << pSourceTexture << pDestinationTexture << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetFrontBuffer(IDirect3DSurface8 *pDestSurface)
        {
            UNREFERENCED_PARAMETER(pDestSurface);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetFrontBuffer(%p) == UNUSED") << pDestSurface);

            return D3DERR_INVALIDCALL;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetRenderTarget(IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8  *pNewZStencil)
        {
            CVoodoo3DSurface8 *wRender = (CVoodoo3DSurface8 *) pRenderTarget;
            CVoodoo3DSurface8 *wStencil = (CVoodoo3DSurface8 *) pNewZStencil;

            HRESULT hr = m_RealDevice->SetRenderTarget(0, wRender->RealSurface());
            hr |= m_RealDevice->SetDepthStencilSurface(wStencil->RealSurface());

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetRenderTarget(%p, %p) == %d") << pRenderTarget << pNewZStencil << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetRenderTarget(IDirect3DSurface8 **ppRenderTarget)
        {
            IDirect3DSurface9 *rRenderTarget;
            HRESULT hr = m_RealDevice->GetRenderTarget(0, &rRenderTarget);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetRenderTarget(%p) == %d") << *ppRenderTarget << hr);

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wRenderTarget = new CVoodoo3DSurface8(this, rRenderTarget);
                (*ppRenderTarget) = (IDirect3DSurface8 *) wRenderTarget;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDepthStencilSurface(IDirect3DSurface8 **ppZStencilSurface)
        {
            IDirect3DSurface9 *rZStencilSurface;
            HRESULT hr = m_RealDevice->GetDepthStencilSurface(&rZStencilSurface);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetDepthStencilSurface(%p) == %d") << *ppZStencilSurface << hr);

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wZStencilSurface = new CVoodoo3DSurface8(this, rZStencilSurface);
                (*ppZStencilSurface) = (IDirect3DSurface8 *) wZStencilSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::EndScene()
        {
            HRESULT hr = m_RealDevice->EndScene();

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::EndScene() == %d") << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
        {
            HRESULT hr = m_RealDevice->SetTransform(State, pMatrix);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetTransform(%d, %p) == %d") << State << pMatrix << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix)
        {
            HRESULT hr = m_RealDevice->GetTransform(State, pMatrix);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetTransform(%d, %p) == %d") << State << pMatrix << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
        {
            HRESULT hr = m_RealDevice->MultiplyTransform(State, pMatrix);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::MultiplyTransform(%d, %p) == %d") << State << pMatrix << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetViewport(CONST D3DVIEWPORT8 *pViewport)
        {
            HRESULT hr = m_RealDevice->SetViewport((CONST D3DVIEWPORT9 *) pViewport);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetViewport(%p) == %d") << pViewport << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetViewport(D3DVIEWPORT8 *pViewport)
        {
            HRESULT hr = m_RealDevice->GetViewport((D3DVIEWPORT9 *) pViewport);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetViewport(%p) == %d") << pViewport << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetMaterial(CONST D3DMATERIAL8 *pMaterial)
        {
            HRESULT hr = m_RealDevice->SetMaterial((CONST D3DMATERIAL9 *) pMaterial);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetMaterial(%p) == %d") << pMaterial << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetMaterial(D3DMATERIAL8 *pMaterial)
        {
            HRESULT hr = m_RealDevice->GetMaterial((D3DMATERIAL9 *) pMaterial);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetMaterial(%p) == %d") << pMaterial << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetLight(DWORD Index, CONST D3DLIGHT8 *Light)
        {
            HRESULT hr = m_RealDevice->SetLight(Index, (CONST D3DLIGHT9 *) Light);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetLight(%d, %p) == %d") << Index << Light << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetLight(DWORD Index, D3DLIGHT8 *Light)
        {
            HRESULT hr = m_RealDevice->GetLight(Index, (D3DLIGHT9 *) Light);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetLight(%d, %p) == %d") << Index << Light << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::LightEnable(DWORD Index, BOOL Enable)
        {
            HRESULT hr = m_RealDevice->LightEnable(Index, Enable);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::LightEnable(%d, %d) == %d") << Index << Enable << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetLightEnable(DWORD Index, BOOL *pEnable)
        {
            HRESULT hr = m_RealDevice->GetLightEnable(Index, pEnable);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetLightEnable(%d, %p) == %d") << Index << pEnable << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetClipPlane(DWORD Index, CONST float *pPlane)
        {
            HRESULT hr = m_RealDevice->SetClipPlane(Index, pPlane);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetClipPlane(%d, %p) == %d") << Index << pPlane << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetClipPlane(DWORD Index, float *pPlane)
        {
            HRESULT hr = m_RealDevice->GetClipPlane(Index, pPlane);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetClipPlane(%d, %p) == %d") << Index << pPlane << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
        {
            HRESULT hr = m_RealDevice->SetRenderState(State, Value);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetRenderState(%d, %d) == %d") << State << Value << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue)
        {
            HRESULT hr = m_RealDevice->GetRenderState(State, pValue);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetRenderState(%d, %p) == %d") << State << pValue << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::EndStateBlock(DWORD *pToken)
        {
            UNREFERENCED_PARAMETER(pToken);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::EndStateBlock(%p)") << pToken);

            return D3DERR_INVALIDCALL;

            /**
             * HRESULT hr = m_RealDevice->EndStateBlock(pToken);
             * #ifdef _DEBUG gpVoodooLogger->Log("Voodoo DX8.9:
             * CVoodoo3DDevice8::EndStateBlock(%d) == %d", pToken, hr);
             * #endif return hr;
             */
        }

        /* This function does not exist in the D3D9 API. */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeleteStateBlock(DWORD Token)
        {
            UNREFERENCED_PARAMETER(Token);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::DeleteStateBlock(%d)") << Token);

            return D3DERR_INVALIDCALL;

            /**
            * HRESULT hr = m_RealDevice->DeleteStateBlock(Token);
            * #ifdef _DEBUG gpVoodooLogger->Log("Voodoo DX8.9:
            * CVoodoo3DDevice8::DeleteStateBlock(%d) == %d", Token, hr);
            * #endif return hr;
            */
        }

        /**
        * Due to differences between the D3D8 and D3D9 APIs, this function is currently
        * non- functional
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateStateBlock(D3DSTATEBLOCKTYPE Type, DWORD *pToken)
        {
            UNREFERENCED_PARAMETER(Type);
            UNREFERENCED_PARAMETER(pToken);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CreateStateBlock(%d, %p)") << Type << pToken);

            return D3DERR_INVALIDCALL;

            /**
            * HRESULT hr = m_RealDevice->CreateStateBlock(Type, pToken);
            * #ifdef _DEBUG gpVoodooLogger->Log("Voodoo DX8.9:
            * CVoodoo3DDevice8::CreateStateBlock(%d, %d) == %d", Type, pToken, hr);
            * #endif return hr;
            */
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetClipStatus(CONST D3DCLIPSTATUS8 *pClipStatus)
        {
            HRESULT hr = m_RealDevice->SetClipStatus((CONST D3DCLIPSTATUS9 *) pClipStatus);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetClipStatus(%p) == %d") << pClipStatus << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetClipStatus(D3DCLIPSTATUS8 *pClipStatus)
        {
            HRESULT hr = m_RealDevice->GetClipStatus((D3DCLIPSTATUS9 *) pClipStatus);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetClipStatus(%p) == %d") << pClipStatus << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetTexture(DWORD Stage, IDirect3DBaseTexture8 **ppTexture)
        {
            IDirect3DBaseTexture9 *rTexture;
            HRESULT hr = m_RealDevice->GetTexture(Stage, &rTexture);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetTexture(%d, %p) == %d") << Stage << *ppTexture << hr);

            if (SUCCEEDED(hr))
            {
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, (IDirect3DTexture9 *) rTexture);
                (*ppTexture) = (IDirect3DBaseTexture8 *) wTexture;
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetTexture(DWORD Stage, IDirect3DBaseTexture8 *pTexture)
        {
            HRESULT hr = D3D_OK;

            if (pTexture)
            {
                CVoodoo3DTexture8 *wTexture = (CVoodoo3DTexture8 *) pTexture;
                hr = m_RealDevice->SetTexture(Stage, (IDirect3DBaseTexture9 *) wTexture->RealTexture());
            }
            else
            {
                hr = m_RealDevice->SetTexture(Stage, nullptr);
            }

            m_RealDevice->SetSamplerState(Stage, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            m_RealDevice->SetSamplerState(Stage, D3DSAMP_MINFILTER, D3DTEXF_POINT);
            m_RealDevice->SetSamplerState(Stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetTexture(%d, %p) == %d") << Stage << pTexture << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue)
        {
            HRESULT hr = m_RealDevice->GetTextureStageState(Stage, Type, pValue);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetTextureStageState(%d, %d, %p) == %d") << Stage << Type << pValue << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
        {
            HRESULT hr = m_RealDevice->SetTextureStageState(Stage, Type, Value);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetTextureStageState(%d, %d, %d) == %d") << Stage << Type << Value << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ValidateDevice(DWORD *pNumPasses)
        {
            HRESULT hr = m_RealDevice->ValidateDevice(pNumPasses);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::ValidateDevice(%d) == %d") << *pNumPasses << hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetInfo(DWORD DevInfoID, void *pDevInfoStruct, DWORD DevInfoStructSize)
        {
            UNREFERENCED_PARAMETER(DevInfoID);
            UNREFERENCED_PARAMETER(pDevInfoStruct);
            UNREFERENCED_PARAMETER(DevInfoStructSize);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::GetInfo(%d, %p, %d)") << DevInfoID << pDevInfoStruct << DevInfoStructSize
            );

            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries)
        {
            HRESULT hr = m_RealDevice->SetPaletteEntries(PaletteNumber, pEntries);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetPaletteEntries(%d, %p) == %d") << PaletteNumber << pEntries << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
        {
            HRESULT hr = m_RealDevice->GetPaletteEntries(PaletteNumber, pEntries);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetPaletteEntries(%d, %p) == %d") << PaletteNumber << pEntries << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetCurrentTexturePalette(UINT PaletteNumber)
        {
            HRESULT hr = m_RealDevice->SetCurrentTexturePalette(PaletteNumber);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetCurrentTexturePalette(%d) == %d") << PaletteNumber << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetCurrentTexturePalette(UINT *PaletteNumber)
        {
            HRESULT hr = m_RealDevice->GetCurrentTexturePalette(PaletteNumber);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetCurrentTexturePalette(%d) == %d") << *PaletteNumber << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
        {
            HRESULT hr = m_RealDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::DrawPrimitive(%d, %d, %d) == %d") << PrimitiveType << StartVertex << PrimitiveCount << hr
            );

            return hr;
        }

        // ! @todo Check the value of the start index in the D3D9 call.
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawIndexedPrimitive
        (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT minIndex,
            UINT NumVertices,
            UINT startIndex,
            UINT primCount
        )
        {
            HRESULT hr = m_RealDevice->DrawIndexedPrimitive
                (PrimitiveType, m_LastBaseIndex, minIndex, NumVertices, startIndex, primCount);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::DrawIndexedPrimitive(%d, %d, %d, %d, %d) == %d") << PrimitiveType << minIndex <<
                    NumVertices << startIndex << primCount << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawPrimitiveUP
            (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT PrimitiveCount,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
            )
        {
            HRESULT hr = m_RealDevice->DrawPrimitiveUP
                (PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::DrawPrimitiveUP(%d, %d, %p, %d) == %d") << PrimitiveType << PrimitiveCount <<
                    pVertexStreamZeroData << VertexStreamZeroStride << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawIndexedPrimitiveUP
        (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT MinVertexIndex,
            UINT NumVertexIndices,
            UINT PrimitiveCount,
            CONST void *pIndexData,
            D3DFORMAT IndexDataFormat,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
        )
        {
            HRESULT hr = m_RealDevice->DrawIndexedPrimitiveUP
            (
                PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, IndexDataFormat,
                pVertexStreamZeroData, VertexStreamZeroStride
            );

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::DrawIndexedPrimitiveUP(%d, %d, %d, %d, %p, %d, %p, %d) == %d") << PrimitiveType <<
                    MinVertexIndex << NumVertexIndices << PrimitiveCount << pIndexData << IndexDataFormat <<
                    pVertexStreamZeroData << VertexStreamZeroStride << hr
            );

            return hr;
        }

        // ! @todo Check the parameter of the vertex decl in the D3D9 call. Taken from MGE, so...
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ProcessVertices
            (
            UINT SrcStartIndex,
            UINT DestIndex,
            UINT VertexCount,
            IDirect3DVertexBuffer8 *pDestBuffer,
            DWORD Flags
            )
        {
            HRESULT hr = m_RealDevice->ProcessVertices
                (SrcStartIndex, DestIndex, VertexCount, (IDirect3DVertexBuffer9 *) pDestBuffer, nullptr, Flags);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::ProcessVertices(%d, %d, %d, %p, %d) == %d") << SrcStartIndex << DestIndex <<
                    VertexCount << pDestBuffer << Flags << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateVertexShader(CONST DWORD *pDeclaration, CONST DWORD *pFunction, DWORD  *pHandle, DWORD Usage)
        {
            UNREFERENCED_PARAMETER(pDeclaration);
            UNREFERENCED_PARAMETER(Usage);

            HRESULT hr = m_RealDevice->CreateVertexShader(pFunction, (IDirect3DVertexShader9 **) pHandle);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::CreateVertexShader(%p, %p, %p, %d) == %d") << pDeclaration << pFunction <<
                    pHandle << Usage << hr
            );

            if (SUCCEEDED(hr))
            {
                m_VertexShaders.insert(*pHandle);
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetVertexShader(DWORD Handle)
        {
            HRESULT hr = D3D_OK;

            if (m_VertexShaders.find(Handle) == m_VertexShaders.end())
            {

                // Not a shader, FVF
                hr = m_RealDevice->SetFVF(Handle);
            }
            else
            {
                hr = m_RealDevice->SetVertexShader((IDirect3DVertexShader9 *) Handle);
            }

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, 
                StringFormat("CVoodoo3DDevice8::SetVertexShader(%d) == %d") << Handle << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShader(DWORD *pHandle)
        {
            // HRESULT hr = m_RealDevice->GetVertexShader((IDirect3DVertexShader9**)pHandle);
            (*pHandle) = m_CurrentVertexShader;

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetVertexShader(%p) == D3D_OK") << pHandle);

            return D3D_OK;
        }

        /* D3D9 has no equivalent function. */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeleteVertexShader(DWORD Handle)
        {
            HRESULT hr = D3D_OK; // m_RealDevice->DeleteVertexShader(DWORD Handle);

            std::set<DWORD>::iterator vShader = m_VertexShaders.find(Handle);
            if (vShader != m_VertexShaders.end())
            {
                m_VertexShaders.erase(vShader);
            }
            else
            {
                hr = D3DERR_INVALIDCALL;
            }

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::DeleteVertexShader(%d) == %d") << Handle << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetVertexShaderConstant(DWORD Register, CONST void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->SetVertexShaderConstantF(Register, (const float *) pConstantData, ConstantCount);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetVertexShaderConstant(%d, %p, %d) == %d") << Register << pConstantData <<
                ConstantCount << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShaderConstant(DWORD Register, void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->GetVertexShaderConstantF(Register, (float *) pConstantData, ConstantCount);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetVertexShaderConstant(%d, %p, %d) == %d") << Register << pConstantData <<
                ConstantCount << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShaderDeclaration(DWORD Handle, void *pData, DWORD *pSizeOfData)
        {
            UNREFERENCED_PARAMETER(Handle);
            UNREFERENCED_PARAMETER(pData);
            UNREFERENCED_PARAMETER(pSizeOfData);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetVertexShaderDeclaration(%d, %p, %d)") << Handle << pData << *pSizeOfData
            );

            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShaderFunction(DWORD Handle, void *pData, DWORD *pSizeOfData)
        {
            UNREFERENCED_PARAMETER(Handle);
            UNREFERENCED_PARAMETER(pData);
            UNREFERENCED_PARAMETER(pSizeOfData);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetVertexShaderFunction(%d, %p, %d)") << Handle << pData << *pSizeOfData
            );

            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8 *pStreamData, UINT Stride)
        {
            HRESULT hr = m_RealDevice->SetStreamSource(StreamNumber, (IDirect3DVertexBuffer9 *) pStreamData, 0, Stride);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetStreamSource(%d, %p, %d) == %d") << StreamNumber << pStreamData << Stride << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, UINT *pStride)
        {
            UINT offset;
            HRESULT hr = m_RealDevice->GetStreamSource
                (StreamNumber, (IDirect3DVertexBuffer9 **) ppStreamData, &offset, pStride);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetStreamSource(%d, %p, %d) == %d") << StreamNumber << *ppStreamData << *pStride << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetIndices(IDirect3DIndexBuffer8 *pIndexData, UINT BaseVertexIndex)
        {
            HRESULT hr = m_RealDevice->SetIndices((IDirect3DIndexBuffer9 *) pIndexData);
            m_LastBaseIndex = BaseVertexIndex;

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetIndices(%p, %d) == %d") << pIndexData << BaseVertexIndex << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetIndices(IDirect3DIndexBuffer8 **ppIndexData, UINT   *pBaseVertexIndex)
        {
            HRESULT hr = m_RealDevice->GetIndices((IDirect3DIndexBuffer9 **) ppIndexData);
            (*pBaseVertexIndex) = m_LastBaseIndex;

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetIndices(%p, %d) == %d") << *ppIndexData << *pBaseVertexIndex << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreatePixelShader(CONST DWORD *pFunction, DWORD  *pHandle)
        {
            HRESULT hr = m_RealDevice->CreatePixelShader(pFunction, (IDirect3DPixelShader9 **) pHandle);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::CreatePixelShader(%p, %d) == %d") << pFunction << *pHandle << hr);

            if (SUCCEEDED(hr))
            {
                m_PixelShaders.insert(*pHandle);
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetPixelShader(DWORD Handle)
        {

            // ! @todo Test the shader setup
            HRESULT hr = D3D_OK;

            if (m_PixelShaders.find(Handle) == m_PixelShaders.end())
            {

                // ! @todo Check if this is write, D3D8 docs have nothing
                hr = m_RealDevice->SetPixelShader(nullptr);
            }
            else
            {
                hr = m_RealDevice->SetPixelShader((IDirect3DPixelShader9 *) Handle);
            }

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetPixelShader(%d) == %d") << Handle << hr);

            if (SUCCEEDED(hr))
            {
                m_CurrentPixelShader = Handle;
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPixelShader(DWORD *pHandle)
        {
            (*pHandle) = m_CurrentPixelShader;

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetPixelShader(%d) == D3D_OK") << *pHandle);


            return D3D_OK;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeletePixelShader(DWORD Handle)
        {
            HRESULT hr = D3D_OK; // m_RealDevice->DeletePixelShader(DWORD Handle);

            std::set<DWORD>::iterator pShader = m_PixelShaders.find(Handle);
            if (pShader != m_PixelShaders.end())
            {
                m_PixelShaders.erase(pShader);
            }
            else
            {
                // hr = D3DERR_INVALIDCALL;
            }

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::DeletePixelShader(%d) == %d") << Handle << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetPixelShaderConstant(DWORD Register, CONST void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->SetPixelShaderConstantF(Register, (float *) pConstantData, ConstantCount);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::SetPixelShaderConstant(%d, %p, %d) == %d") << Register << pConstantData <<
                ConstantCount << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPixelShaderConstant(DWORD Register, void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->GetPixelShaderConstantF(Register, (float *) pConstantData, ConstantCount);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetPixelShaderConstant(%d, %p, %d) == %d") << Register << pConstantData <<
                ConstantCount << hr
            );

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPixelShaderFunction(DWORD Handle, void *pData, DWORD *pSizeOfData)
        {
            UNREFERENCED_PARAMETER(Handle);
            UNREFERENCED_PARAMETER(pData);
            UNREFERENCED_PARAMETER(pSizeOfData);

            VOODOO_API_LOG
            (
                VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::GetPixelShaderFunction(%d, %p, %p)") << Handle << pData <<
                pSizeOfData
            );

            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo)
        {
            HRESULT hr = m_RealDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::DrawRectPatch(%d, %p, %p) == %d") << Handle << pNumSegs << pRectPatchInfo << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
        {
            HRESULT hr = m_RealDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::DrawTriPatch(%d, %p, %p) == %d") << Handle << pNumSegs << pTriPatchInfo << hr);

            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeletePatch(UINT Handle)
        {
            HRESULT hr = m_RealDevice->DeletePatch(Handle);

            VOODOO_API_LOG(VSLog_PlugDebug, VOODOO_DX89_NAME, StringFormat("CVoodoo3DDevice8::DeletePatch(%d) == %d") << Handle << hr);

            return hr;
        }

        IDirect3DDevice9 * STDMETHODCALLTYPE CVoodoo3DDevice8::VSGetRealDevice()
        {
            return m_RealDevice;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::VSSetRealDevice(IDirect3DDevice9 * pDev)
        {
            if (m_RealDevice == pDev) return D3D_OK;

            if (m_RealDevice) m_RealDevice->Release();
            m_RealDevice = pDev;
            if (m_RealDevice) m_RealDevice->AddRef();

            return D3D_OK;
        }
    }
}
