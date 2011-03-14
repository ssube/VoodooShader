/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (c) 2010-2011 by Sean Sube
*
*
* This program is free software; you can redistribute it and/or modify it under the terms of the 
* GNU General Public License as published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#ifndef VOODOO_GEM_DEVICE_HPP
#define VOODOO_GEM_DEVICE_HPP

#include "Gem_Includes.hpp"
#include "Gem_Version.hpp"

#include "Gem_Module.hpp"
#include "IVoodoo3DSurface8.hpp"
#include "IVoodoo3DTexture8.hpp"

using namespace VoodooShader;

static HRESULT DefaultErrorCode = D3DERR_INVALIDCALL;

/**
* The core Voodoo wrapper class for D3D8 devices. This class is responsible for processing,
* translating and handling most API calls. Due to this, it must be as optimized as possible
* (considering the circumstances) and should contain only tested code.
* 
* @note The various IVoodoo3D8 interfaces provide an application-opaque wrapper that actually
*          implements a Direct3D 8.9 layer (8 to 9 translation). For use with D3D9 applications, the
*          IVoodoo3D9 interface set should be used.         
*/
class IVoodoo3DDevice8 
    : public IDirect3DDevice8
{
    /**
    * A pointer to the true underlying IDirect3DDevice9 object wrapped by this IVoodoo interface.            
    */
    IDirect3DDevice9 * mRealDevice;

    D3DPRESENT_PARAMETERS mParams;

    IDirect3D9 * mRealObject;

    std::set<DWORD> mVertexShaders;
    std::set<DWORD> mPixelShaders;

    DWORD mCurrentVertexShader;
    DWORD mCurrentPixelShader;

    UINT mLastBaseIndex;

public:
    /**
    * The default, public constructor for IVoodoo3D objects.            
    */
    IVoodoo3DDevice8(IDirect3DDevice9 * realDevice, D3DPRESENT_PARAMETERS pp)
        : mRealDevice(realDevice), mParams(pp)
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::IVoodoo3DDevice8(%p) == %p",
            realDevice, this
        );
    }

    // IVoodoo3DDevice8 methods
    STDMETHOD_(D3DPRESENT_PARAMETERS, GetPresentParams)()
    {
        return mParams;
    }

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj)
    {
        return mRealDevice->QueryInterface(riid, ppvObj);
    }

    STDMETHOD_(ULONG, AddRef)()
    {
        ULONG refCount = mRealDevice->AddRef();

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::AddRef() == %d",
        refCount);
#endif

        return refCount;
    }

    STDMETHOD_(ULONG, Release)()
    {
        ULONG refCount = mRealDevice->Release();

        if ( refCount == 0 )
        {
            delete this;
            return 0;
        } else {
            return refCount;
        }
    }

    // IDirect3DDevice8 methods
    STDMETHOD(ApplyStateBlock)(DWORD Token)
    {
#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::ApplyStateBlock == UNUSED");
#endif
        return DefaultErrorCode;
    }

    STDMETHOD(BeginScene)()
    {
        HRESULT hr = mRealDevice->BeginScene();

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::BeginScene() == %d",
            hr);
#endif

        return hr;
    }

    STDMETHOD(BeginStateBlock)()
    {
        HRESULT hr = mRealDevice->BeginStateBlock();

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::BeginStateBlock() == %d",
            hr);
#endif

        return hr;
    }

    STDMETHOD(CaptureStateBlock)(DWORD Token)
    {
#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::CaptureStateBlock == UNUSED");
#endif

        return DefaultErrorCode;
    }

    STDMETHOD(Clear)
    (
        DWORD Count,
        const D3DRECT * pRects,
        DWORD Flags,
        D3DCOLOR Color,
        float Z,
        DWORD Stencil
    )
    {
        HRESULT hr = mRealDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::Clear(%d, %p, %d, %d, %f, %d) == %d",
            Count, pRects, Flags, Color, Z, Stencil, hr
        );

        return hr;
    }

    STDMETHOD(TestCooperativeLevel)
    (
        
    )
    {
        HRESULT hr = mRealDevice->TestCooperativeLevel();

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::TestCooperativeLevel() == %d",
            hr
        );

        return hr;
    }


    STDMETHOD_(UINT, GetAvailableTextureMem)
    (
        
    )
    {
        UINT mem = mRealDevice->GetAvailableTextureMem();

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetAvailableTextureMem() == %d",
            mem
        );

        return mem;
    }


    STDMETHOD(ResourceManagerDiscardBytes)
    (
        DWORD Bytes
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::ResourceManagerDiscardBytes(%d) == UNUSED",
            Bytes
        );

        return DefaultErrorCode;
    }


    STDMETHOD(GetDirect3D)
    (
        IDirect3D8 ** ppD3D8
    )
    {
        (*ppD3D8) = (IDirect3D8*)VoodooObject;

        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::GetDirect3D(%p) == D3D_OK",
        *ppD3D8);

        return D3D_OK;
    }


    STDMETHOD(GetDeviceCaps)
    (
        D3DCAPS8 * pCaps
    )
    {
        D3DCAPS9 rCaps;

        HRESULT hr = mRealDevice->GetDeviceCaps(&rCaps);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetDeviceCaps(%p) == %d",
            pCaps, hr
        );

        if ( SUCCEEDED(hr) )
        {
            memcpy(pCaps, &rCaps, sizeof(D3DCAPS8));
        }

        return hr;
    }


    STDMETHOD(GetDisplayMode)
    (
        D3DDISPLAYMODE * pMode
    )
    {
        HRESULT hr = mRealDevice->GetDisplayMode(0, pMode);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::GetDisplayMode(%p) == %d",
        pMode, hr);
#endif

        return hr;
    }


    STDMETHOD(GetCreationParameters)
    (
        D3DDEVICE_CREATION_PARAMETERS * pParameters
    )
    {
        HRESULT hr = mRealDevice->GetCreationParameters(pParameters);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::GetCreationParameters(%p) == %d",
        pParameters, hr);
#endif

        return hr;
    }


    STDMETHOD(SetCursorProperties)
    (
        UINT XHotSpot,
        UINT YHotSpot,
        IDirect3DSurface8 * pCursorBitmap
    )
    {
        IVoodoo3DSurface8 * rCursor = (IVoodoo3DSurface8*)pCursorBitmap;

        HRESULT hr = mRealDevice->SetCursorProperties(XHotSpot, YHotSpot, rCursor->RealSurface());

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetCursorProperties(%d, %d, %p) == %d",
            XHotSpot, YHotSpot, pCursorBitmap, hr
        );

        return hr;
    }


    STDMETHOD_(void, SetCursorPosition)
    (
        int X,
        int Y,
        DWORD Flags
    )
    {
        mRealDevice->SetCursorPosition(X, Y, Flags);

        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::SetCursorPosition(%d, %d, %u)",
        X, Y, Flags);
    }


    STDMETHOD_(BOOL, ShowCursor)
    (
        BOOL bShow
    )
    {
        BOOL show = mRealDevice->ShowCursor(bShow);

        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::ShowCursor(%d) == %d",
        bShow, show);

        return show;
    }


    STDMETHOD(CreateAdditionalSwapChain)
    (
        D3DPRESENT_PARAMETERS8 * pPresentationParameters,
        IDirect3DSwapChain8 ** pSwapChain
    )
    {
        HRESULT hr = mRealDevice->CreateAdditionalSwapChain
        (
            (D3DPRESENT_PARAMETERS*)pPresentationParameters, 
            (IDirect3DSwapChain9**)pSwapChain
        );

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::CreateAdditionalSwapChain(%d, %d) == %d",
        pPresentationParameters, pSwapChain, hr);
#endif

        return hr;
    }


    STDMETHOD(Reset)
    (
        D3DPRESENT_PARAMETERS8 * pPresentationParameters
    )
    {
        HRESULT hr = mRealDevice->Reset((D3DPRESENT_PARAMETERS*)pPresentationParameters);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::Reset(%d) == %d",
        pPresentationParameters, hr);
#endif

        return hr;
    }

    STDMETHOD(Present)
    (
        CONST RECT * pSourceRect,
        CONST RECT * pDestRect,
        HWND hDestWindowOverride,
        CONST RGNDATA * pDirtyRegion
    )
    {
        HRESULT shr = mRealDevice->StretchRect(gBackbuffer.RawSurface, NULL, gThisFrame.RawSurface, NULL, D3DTEXF_NONE);

        if ( FAILED(shr) )
        {
            VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "Failed to stretch backbuffer to scratch texture.");
        }

        VoodooGem->DrawShader(testShader);

        // Present call
        HRESULT hr = mRealDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::Present(%d, %d, %d, %d) == %d",
            pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, hr
        );

        return hr;
    }

    STDMETHOD(GetBackBuffer)
    (
        UINT BackBuffer,
        D3DBACKBUFFER_TYPE Type,
        IDirect3DSurface8 ** ppBackBuffer
    )
    {
        IDirect3DSurface9 * rBackbuffer;
        HRESULT hr = mRealDevice->GetBackBuffer(0, BackBuffer, Type, &rBackbuffer);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetBackBuffer(%d, %d, %d) == %d",
            BackBuffer, Type, ppBackBuffer, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DSurface8 * wBackbuffer = new IVoodoo3DSurface8(this, rBackbuffer);
            (*ppBackBuffer) = (IDirect3DSurface8*)wBackbuffer;
        }

        return hr;
    }

    STDMETHOD(GetRasterStatus)
    (
        D3DRASTER_STATUS * pRasterStatus
    )
    {
        HRESULT hr = mRealDevice->GetRasterStatus(0, pRasterStatus);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetRasterStatus(%p) == %d",
            pRasterStatus, hr
        );

        return hr;
    }

    STDMETHOD_(void, SetGammaRamp)
    (
        DWORD Flags,
        const D3DGAMMARAMP * pRamp
    )
    {
        mRealDevice->SetGammaRamp(0, Flags, pRamp);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetRasterStatus(%d, %d) == void",
            Flags, pRamp
        );

        return;
    }

    STDMETHOD_(void, GetGammaRamp)
    (
        D3DGAMMARAMP * pRamp
    )
    {
        mRealDevice->GetGammaRamp(0, pRamp);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetRasterStatus(%d) == void",
            pRamp
        );

        return;
    }

    /**
     * Creates a texture in the IVoodoo runtime. The new texture must be registered with the core
     * and, for shader purposes, we should attempt to create it as a render-target.            
     */
    STDMETHOD(CreateTexture)
    (
        UINT Width,
        UINT Height,
        UINT Levels,
        DWORD Usage,
        D3DFORMAT Format,
        D3DPOOL Pool,
        IDirect3DTexture8 ** ppTexture
    )
    {
        IDirect3DTexture9 * rTexture;
        HRESULT hr = D3DERR_INVALIDCALL;

        // If not a render-target, attempt to create it as one
        bool rtt = true;

        if ( !( Usage & D3DUSAGE_RENDERTARGET ) )
        {
            hr = mRealDevice->CreateTexture
            (
                Width, Height, Levels, Usage | D3DUSAGE_RENDERTARGET, Format, 
                Pool, &rTexture, NULL
            );
        }

        // That failed, create as a standard texture
        if ( FAILED(hr) )
        {
            VoodooCore->Log(LL_Warning, VOODOO_GEM_NAME, "Unable to create texture as a render target (%d).", hr);

            rtt = false;
            hr = mRealDevice->CreateTexture
            (
                Width, Height, Levels, Usage, Format, Pool, &rTexture, NULL
            );
        }

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateTexture(%d, %d, %d, %d, %d, %d, %d) == %d",
            Width, Height, Levels, Usage, Format, Pool, ppTexture, hr
        );

        // One succeeded, the texture exists. We need to register it with the Voodoo core.
        if ( SUCCEEDED(hr) )
        {
            //! @todo Set up the core texture registration for Voodoo Gem and track filenames, hook CreateFile

            IVoodoo3DTexture8 * wTexture = new IVoodoo3DTexture8(this, rTexture);
            (*ppTexture) = (IDirect3DTexture8*)wTexture;
        }

        return hr;
    }


    STDMETHOD(CreateVolumeTexture)
    (
        UINT Width,
        UINT Height,
        UINT Depth,
        UINT Levels,
        DWORD Usage,
        D3DFORMAT Format,
        D3DPOOL Pool,
        IDirect3DVolumeTexture8 ** ppVolumeTexture
    )
    {
        IDirect3DVolumeTexture9 * rTexture;
        HRESULT hr = mRealDevice->CreateVolumeTexture
        (
            Width, Height, Depth, Levels, Usage, Format,
            Pool, &rTexture, NULL
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateVolumeTexture(%d, %d, %d, %d, %d, %d, %d, %p) == %d",
            Width, Height, Depth, Levels, Usage, Format, Pool, *ppVolumeTexture, hr
        );

        //! @todo Set up the core volume texture registration for Voodoo Gem
        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DTexture8 * wTexture = new IVoodoo3DTexture8(this, (IDirect3DTexture9*)rTexture);
            (*ppVolumeTexture) = (IDirect3DVolumeTexture8*)wTexture;
        }

        return hr;
    }

    STDMETHOD(CreateCubeTexture)
    (
        UINT EdgeLength,
        UINT Levels,
        DWORD Usage,
        D3DFORMAT Format,
        D3DPOOL Pool,
        IDirect3DCubeTexture8 ** ppCubeTexture
    )
    {
        IDirect3DCubeTexture9 * rTexture;
        HRESULT hr = mRealDevice->CreateCubeTexture
        (
            EdgeLength, Levels, Usage, Format, Pool, 
            &rTexture, NULL
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateCubeTexture(%u, %u, %u, %u, %u, %p) == %d",
            EdgeLength, Levels, Usage, Format, Pool, *ppCubeTexture, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DTexture8 * wTexture = new IVoodoo3DTexture8(this, (IDirect3DTexture9*)rTexture);
            (*ppCubeTexture) = (IDirect3DCubeTexture8*)wTexture;
        }

        return hr;
    }


    STDMETHOD(CreateVertexBuffer)
    (
        UINT Length,
        DWORD Usage,
        DWORD FVF,
        D3DPOOL Pool,
        IDirect3DVertexBuffer8 ** ppVertexBuffer
    )
    {
        HRESULT hr = mRealDevice->CreateVertexBuffer
        (
            Length, Usage, FVF, Pool, (IDirect3DVertexBuffer9**)ppVertexBuffer, NULL
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateVertexBuffer(%u, %u, %u, %u, %p) == %d",
            Length, Usage, FVF, Pool, *ppVertexBuffer, hr
        );

        return hr;
    }


    STDMETHOD(CreateIndexBuffer)
    (
        UINT Length,
        DWORD Usage,
        D3DFORMAT Format,
        D3DPOOL Pool,
        IDirect3DIndexBuffer8 ** ppIndexBuffer
    )
    {
        HRESULT hr = mRealDevice->CreateIndexBuffer
        (
            Length, Usage, Format, Pool, (IDirect3DIndexBuffer9**)ppIndexBuffer, NULL
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateIndexBuffer(%u, %u, %u, %u, %p) == %d",
            Length, Usage, Format, Pool, *ppIndexBuffer, hr
        );

        return hr;
    }


    /**
     * This function creates a stand-alone render target surface in the original D3D8 and 9 setups.
     * For Voodoo's purposes, this should create a render target texture if at all possible, then
     * retrieve the surface from that texture. 
     * 
     * @todo Find a way to force CreateRenderTarget to make RT textures and find a way to name them.
     */
    STDMETHOD(CreateRenderTarget)
    (
        UINT Width,
        UINT Height,
        D3DFORMAT Format,
        D3DMULTISAMPLE_TYPE MultiSample,
        BOOL Lockable,
        IDirect3DSurface8 ** ppSurface
    )
    {
        IDirect3DSurface9 * rSurface;
        HRESULT hr = mRealDevice->CreateRenderTarget
        (
            Width, Height, Format, MultiSample, 0, Lockable, &rSurface, NULL
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateRenderTarget(%u, %u, %u, %u, %u, %p) == %d",
            Width, Height, Format, MultiSample, Lockable, *ppSurface, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DSurface8 * wSurface = new IVoodoo3DSurface8(this, rSurface);
            (*ppSurface) = (IDirect3DSurface8*)wSurface;
        }

        return hr;
    }


    STDMETHOD(CreateDepthStencilSurface)
    (
        UINT Width,
        UINT Height,
        D3DFORMAT Format,
        D3DMULTISAMPLE_TYPE MultiSample,
        IDirect3DSurface8 ** ppSurface
    )
    {
        IDirect3DSurface9 * rSurface;

        HRESULT hr = mRealDevice->CreateDepthStencilSurface
        (
            Width, Height, Format, MultiSample, 0, 0, &rSurface, NULL
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateDepthStencilSurface(%u, %u, %u, %u, %p) == %d",
            Width, Height, Format, MultiSample, *ppSurface, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DSurface8 * wSurface = new IVoodoo3DSurface8(this, rSurface);
            (*ppSurface) = (IDirect3DSurface8*)wSurface;
        }

        return hr;
    }

    /**
     * Creates a D3D8 image surface, a sort of placeholder surface. Because D3D9 has no equivalent,
     * this is redirected to CreateOffscreenPlainSurface at some point.
     * 
     * @warning This uses a different call than the D3D8 original, and may not perform exactly the
     *             same. <em>(non-standard behavior)</em>
     * @todo Test the pool these should be created in. May be default, managed or sysmem, not sure yet.
     */
    STDMETHOD(CreateImageSurface)
    (
        UINT Width,
        UINT Height,
        D3DFORMAT Format,
        IDirect3DSurface8 ** ppSurface
    )
    {
        IDirect3DSurface9 * rSurface;

        HRESULT hr = mRealDevice->CreateOffscreenPlainSurface
        (
            Width, Height, Format, D3DPOOL_DEFAULT, &rSurface, NULL
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateImageSurface(%u, %u, %u, %p) == %d",
            Width, Height, Format, *ppSurface, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DSurface8 * wSurface = new IVoodoo3DSurface8(this, rSurface);
            (*ppSurface) = (IDirect3DSurface8*)wSurface;
        }

        return hr;
    }

    /**
     * Copies rectangular areas of data from one surface to another. There is no such function in
     * D3D9, it instead requires each texture to be stretched separately. It is, however, possible
     * to translate the call as D3D8 applies some very specific restrictions, including a lack of
     * stretching (means the rectangles must be the same dimensions, but at different points) and
     * the same format and surface properties.         
     * 
     * @warning For testing purposes, this only copies the first given rectangle. This is easy
     *             enough to fix, but I want to do more testing first. <em>(non-standard behavior)</em>
     * @todo Test and then make this perform identical to the D3D8 version.
     */
    STDMETHOD(CopyRects)
    (
        IDirect3DSurface8 * pSourceSurface,
        CONST RECT * pSourceRectsArray,
        UINT cRects,
        IDirect3DSurface8 * pDestinationSurface,
        CONST POINT * pDestPointsArray
    )
    {
        IVoodoo3DSurface8 * wSource = (IVoodoo3DSurface8*)pSourceSurface;
        IVoodoo3DSurface8 * wDest = (IVoodoo3DSurface8*)pDestinationSurface;

        HRESULT hr = mRealDevice->StretchRect
        (
            wSource->RealSurface(), pSourceRectsArray, 
            wDest->RealSurface(), pSourceRectsArray, D3DTEXF_NONE);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CopyRects(%p, %p, %u, %p, %p) == %d",
            pSourceSurface, pSourceRectsArray, cRects, pDestinationSurface, pDestPointsArray, hr
        );

        return hr;
    }


    STDMETHOD(UpdateTexture)
    (
        IDirect3DBaseTexture8 * pSourceTexture,
        IDirect3DBaseTexture8 * pDestinationTexture
    )
    {
        IVoodoo3DTexture8 * wSource = (IVoodoo3DTexture8*)pSourceTexture;
        IVoodoo3DTexture8 * wDest   = (IVoodoo3DTexture8*)pDestinationTexture;

        HRESULT hr = mRealDevice->UpdateTexture
        (
            wSource->RealTexture(), wDest->RealTexture()
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::UpdateTexture(%p, %p) == %d",
            pSourceTexture, pDestinationTexture, hr
        );

        return hr;
    }

    /**
     * Retrieves the front-buffer surface from the D3D8 device. D3D9 has no equivalent call (kinda).            
     */
    STDMETHOD(GetFrontBuffer)
    (
        IDirect3DSurface8 * pDestSurface
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetFrontBuffer(%p) == UNUSED",
            pDestSurface
        );

        return DefaultErrorCode;
    }

    /**
     * Sets a new render-target surface and depth/stencil buffer. D3D9 does not combine these calls,
     * so this automatically splits the calls and returns the combined results.
     */
    STDMETHOD(SetRenderTarget)
    (
        IDirect3DSurface8 * pRenderTarget,
        IDirect3DSurface8 * pNewZStencil
    )
    {
        IVoodoo3DSurface8 * wRender     = (IVoodoo3DSurface8*)pRenderTarget;
        IVoodoo3DSurface8 * wStencil    = (IVoodoo3DSurface8*)pNewZStencil;

        HRESULT hr  = mRealDevice->SetRenderTarget(0, wRender->RealSurface());
                hr |= mRealDevice->SetDepthStencilSurface(wStencil->RealSurface());

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetRenderTarget(%p, %p) == %d",
            pRenderTarget, pNewZStencil, hr
        );

        return hr;
    }

    STDMETHOD(GetRenderTarget)
    (
        IDirect3DSurface8 ** ppRenderTarget
    )
    {
        IDirect3DSurface9 * rRenderTarget;
        HRESULT hr = mRealDevice->GetRenderTarget(0, &rRenderTarget);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetRenderTarget(%p) == %d",
            *ppRenderTarget, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DSurface8 * wRenderTarget = new IVoodoo3DSurface8(this, rRenderTarget);
            (*ppRenderTarget) = (IDirect3DSurface8*)wRenderTarget;
        }

        return hr;
    }


    STDMETHOD(GetDepthStencilSurface)
    (
        IDirect3DSurface8 ** ppZStencilSurface
    )
    {
        IDirect3DSurface9 * rZStencilSurface;
        HRESULT hr = mRealDevice->GetDepthStencilSurface(&rZStencilSurface);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetDepthStencilSurface(%p) == %d",
            *ppZStencilSurface, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DSurface8 * wZStencilSurface = new IVoodoo3DSurface8(this, rZStencilSurface);
            (*ppZStencilSurface) = (IDirect3DSurface8*)wZStencilSurface;
        }

        return hr;
    }

    STDMETHOD(EndScene)()
    {
        HRESULT hr = mRealDevice->EndScene();

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::EndScene() == %d",
            hr
        );

        return hr;
    }

    STDMETHOD(SetTransform)
    (
        D3DTRANSFORMSTATETYPE State,
        CONST D3DMATRIX * pMatrix
    )
    {
        HRESULT hr = mRealDevice->SetTransform(State, pMatrix);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetTransform(%u, %p) == %d\n",
            State, pMatrix, hr
        );

        // Update cached matrices as necessary
        if ( SUCCEEDED(hr) )
        {
            D3DMATRIX * matrix = NULL;

            if ( State == D3DTS_VIEW )
            {
                matrix = (D3DMATRIX*) gMatrixView->GetFloat();
            } else if ( State == D3DTS_PROJECTION ) {
                matrix = (D3DMATRIX*) gMatrixProj->GetFloat();
            } else if ( State == D3DTS_WORLD ) {
                matrix = (D3DMATRIX*) gMatrixWorld->GetFloat();
            }

            if ( matrix )
            {
                memcpy(matrix, pMatrix, sizeof(D3DMATRIX));
            }
        }

        return hr;
    }


    STDMETHOD(GetTransform)
    (
        D3DTRANSFORMSTATETYPE State,
        D3DMATRIX * pMatrix
    )
    {
        HRESULT hr = mRealDevice->GetTransform(State, pMatrix);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetTransform(%u, %p) == %d",
            State, pMatrix, hr
        );

        return hr;
    }


    STDMETHOD(MultiplyTransform)
    (
        D3DTRANSFORMSTATETYPE State,
        CONST D3DMATRIX * pMatrix
    )
    {
        HRESULT hr = mRealDevice->MultiplyTransform(State, pMatrix);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::MultiplyTransform(%u, %p) == %d",
            State, pMatrix, hr
        );

        if ( SUCCEEDED(hr) )
        {
            D3DMATRIX * matrix = NULL;

            if ( State == D3DTS_VIEW )
            {
                matrix = (D3DMATRIX*) gMatrixView->GetFloat();
            } else if ( State == D3DTS_PROJECTION ) {
                matrix = (D3DMATRIX*) gMatrixProj->GetFloat();
            } else if ( State == D3DTS_WORLD ) {
                matrix = (D3DMATRIX*) gMatrixWorld->GetFloat();
            }

            if ( matrix )
            {
                mRealDevice->GetTransform(State, matrix);
                //matrix = D3DXMatrixMultiply(NULL, (const D3DXMATRIX*)matrix, pMatrix);
            } else {
                VoodooCore->Log
                (
                    LL_Info,
                    VOODOO_GEM_NAME,
                    "Unrecognized matrix set (%u).",
                    State
                );
            }
        }

        return hr;
    }


    STDMETHOD(SetViewport)
    (
        CONST D3DVIEWPORT8 * pViewport
    )
    {
        HRESULT hr = mRealDevice->SetViewport((CONST D3DVIEWPORT9 *)pViewport);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetViewport(%p) == %d",
            pViewport, hr
        );

        return hr;
    }


    STDMETHOD(GetViewport)
    (
        D3DVIEWPORT8 * pViewport
    )
    {
        HRESULT hr = mRealDevice->GetViewport((D3DVIEWPORT9*)pViewport);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetViewport(%p) == %d",
            pViewport, hr
        );

        return hr;
    }


    STDMETHOD(SetMaterial)
    (
        CONST D3DMATERIAL8 * pMaterial
    )
    {
        HRESULT hr = mRealDevice->SetMaterial((CONST D3DMATERIAL9*) pMaterial);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetMaterial(%p) == %d",
            pMaterial, hr
        );

        return hr;
    }


    STDMETHOD(GetMaterial)
    (
        D3DMATERIAL8 * pMaterial
    )
    {
        HRESULT hr = mRealDevice->GetMaterial((D3DMATERIAL9*) pMaterial);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetMaterial(%p) == %d",
            pMaterial, hr
        );

        return hr;
    }


    STDMETHOD(SetLight)
    (
        DWORD Index,
        CONST D3DLIGHT8 * Light
    )
    {
        HRESULT hr = mRealDevice->SetLight(Index, (CONST D3DLIGHT9*)Light);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetLight(%u, %p) == %d",
            Index, Light, hr
        );

        return hr;
    }


    STDMETHOD(GetLight)
    (
        DWORD Index,
        D3DLIGHT8 * Light
    )
    {
        HRESULT hr = mRealDevice->GetLight(Index, (D3DLIGHT9*)Light);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetLight(%u, %p) == %d",
            Index, Light, hr
        );

        return hr;
    }


    STDMETHOD(LightEnable)
    (
        DWORD Index,
        BOOL Enable
    )
    {
        HRESULT hr = mRealDevice->LightEnable(Index, Enable);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::LightEnable(%u, %d) == %d",
            Index, Enable, hr
        );

        return hr;
    }


    STDMETHOD(GetLightEnable)
    (
        DWORD Index,
        BOOL * pEnable
    )
    {
        HRESULT hr = mRealDevice->GetLightEnable(Index, pEnable);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetLightEnable(%u, %d) == %d",
            Index, *pEnable, hr
        );

        return hr;
    }


    STDMETHOD(SetClipPlane)
    (
        DWORD Index,
        CONST float * pPlane
    )
    {
        HRESULT hr = mRealDevice->SetClipPlane(Index, pPlane);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetClipPlane(%u, %p) == %d",
            Index, pPlane, hr
        );

        return hr;
    }


    STDMETHOD(GetClipPlane)
    (
        DWORD Index,
        float * pPlane
    )
    {
        HRESULT hr = mRealDevice->GetClipPlane(Index, pPlane);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetClipPlane(%u, %d) == %d",
            Index, pPlane, hr
        );

        return hr;
    }


    STDMETHOD(SetRenderState)
    (
        D3DRENDERSTATETYPE State,
        DWORD Value
    )
    {
        HRESULT hr = mRealDevice->SetRenderState(State, Value);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetRenderState(%u, %u) == %d",
            State, Value, hr
        );

        return hr;
    }


    STDMETHOD(GetRenderState)
    (
        D3DRENDERSTATETYPE State,
        DWORD * pValue
    )
    {
        HRESULT hr = mRealDevice->GetRenderState(State, pValue);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetRenderState(%u, %u) == %d",
            State, *pValue, hr
        );

        return hr;
    }

    /**
     * Due to differences between the D3D8 and D3D9 APIs, this function is currently non-
     * functional.        
     * @todo Diagnose state blocks, see if they can be translated.
     */
    STDMETHOD(EndStateBlock)
    (
        DWORD * pToken
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::EndStateBlock(%u) == UNUSED",
            *pToken
        );

        return DefaultErrorCode;
    }


    /**
     * This function does not exist in the D3D9 API.            
     */
    STDMETHOD(DeleteStateBlock)
    (
        DWORD Token
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::DeleteStateBlock(%u) == UNUSED",
            Token
        );

        return DefaultErrorCode;
    }


    /**
    * Due to differences between the D3D8 and D3D9 APIs, this function is currently non-
    * functional.            
    */
    STDMETHOD(CreateStateBlock)
    (
        D3DSTATEBLOCKTYPE Type,
        DWORD * pToken
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateStateBlock(%u, %u) == UNUSED",
            Type, *pToken
        );

        return DefaultErrorCode;
    }


    STDMETHOD(SetClipStatus)
    (
        CONST D3DCLIPSTATUS8 * pClipStatus
    )
    {
        HRESULT hr = mRealDevice->SetClipStatus((CONST D3DCLIPSTATUS9*)pClipStatus);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetClipStatus(%p) == %d",
            pClipStatus, hr
        );

        return hr;
    }


    STDMETHOD(GetClipStatus)
    (
        D3DCLIPSTATUS8 * pClipStatus
    )
    {
        HRESULT hr = mRealDevice->GetClipStatus((D3DCLIPSTATUS9*)pClipStatus);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetClipStatus(%p) == %d",
            pClipStatus, hr
        );

        return hr;
    }


    STDMETHOD(GetTexture)
    (
        DWORD Stage,
        IDirect3DBaseTexture8 ** ppTexture
    )
    {
        IDirect3DBaseTexture9 * rTexture;
        HRESULT hr = mRealDevice->GetTexture(Stage, &rTexture);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetTexture(%u, %p) == %d",
            Stage, *ppTexture, hr
        );

        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DTexture8 * wTexture = new IVoodoo3DTexture8(this, (IDirect3DTexture9*)rTexture);
            (*ppTexture) = (IDirect3DBaseTexture8*)wTexture;
        }

        return hr;
    }


    STDMETHOD(SetTexture)
    (
        DWORD Stage,
        IDirect3DBaseTexture8 * pTexture
    )
    {
        HRESULT hr = D3D_OK;

        if ( pTexture )
        {
            IVoodoo3DTexture8 * wTexture = (IVoodoo3DTexture8*)pTexture;
            hr = mRealDevice->SetTexture(Stage, (IDirect3DBaseTexture9*)wTexture->RealTexture());
        } else {
            hr = mRealDevice->SetTexture(Stage, NULL);
        }

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetTexture(%u, %p) == %d",
            Stage, pTexture, hr
        );

        return hr;
    }


    STDMETHOD(GetTextureStageState)
    (
        DWORD Stage,
        D3DTEXTURESTAGESTATETYPE Type,
        DWORD * pValue
    )
    {
        HRESULT hr = mRealDevice->GetTextureStageState(Stage, Type, pValue);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetTextureStageState(%u, %u, %p) == %d",
            Stage, Type, pValue, hr
        );

        return hr;
    }


    STDMETHOD(SetTextureStageState)
    (
        DWORD Stage,
        D3DTEXTURESTAGESTATETYPE Type,
        DWORD Value
    )
    {
        HRESULT hr = mRealDevice->SetTextureStageState(Stage, Type, Value);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetTextureStageState(%u, %u, %u) == %d",
            Stage, Type, Value, hr
        );

        return hr;
    }


    STDMETHOD(ValidateDevice)
    (
        DWORD * pNumPasses
    )
    {
        HRESULT hr = mRealDevice->ValidateDevice(pNumPasses);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::ValidateDevice(%u) == %d",
            *pNumPasses, hr
        );

        return hr;
    }

    /**
     * No D3D9 equivalent.            
     */
    STDMETHOD(GetInfo)
    (
        DWORD DevInfoID,
        void * pDevInfoStruct,
        DWORD DevInfoStructSize
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetInfo(%u, %p, %u) == UNUSED",
            DevInfoID, pDevInfoStruct, DevInfoStructSize
        );

        return DefaultErrorCode;
    }

    STDMETHOD(SetPaletteEntries)
    (
        UINT PaletteNumber,
        CONST PALETTEENTRY * pEntries
    )
    {
        HRESULT hr = mRealDevice->SetPaletteEntries(PaletteNumber, pEntries);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetPaletteEntries(%u, %p) == %d",
            PaletteNumber, pEntries, hr
        );

        return hr;
    }

    STDMETHOD(GetPaletteEntries)
    (
        UINT PaletteNumber,
        PALETTEENTRY * pEntries
    )
    {
        HRESULT hr = mRealDevice->GetPaletteEntries(PaletteNumber, pEntries);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetPaletteEntries(%u, %p) == %d",
            PaletteNumber, pEntries, hr
        );

        return hr;
    }


    STDMETHOD(SetCurrentTexturePalette)
    (
        UINT PaletteNumber
    )
    {
        HRESULT hr = mRealDevice->SetCurrentTexturePalette(PaletteNumber);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetCurrentTexturePalette(%u) == %d",
            PaletteNumber, hr
        );

        return hr;
    }


    STDMETHOD(GetCurrentTexturePalette)
    (
        UINT * PaletteNumber
    )
    {
        HRESULT hr = mRealDevice->GetCurrentTexturePalette(PaletteNumber);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetCurrentTexturePalette(%u) == %d",
            *PaletteNumber, hr
        );

        return hr;
    }


    STDMETHOD(DrawPrimitive)
    (
        D3DPRIMITIVETYPE PrimitiveType,
        UINT StartVertex,
        UINT PrimitiveCount
    )
    {
        HRESULT hr = mRealDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::DrawPrimitive(%u, %u, %u) == %d",
            PrimitiveType, StartVertex, PrimitiveCount, hr
        );

        return hr;
    }

    //! @todo Check the value of the start index in the D3D9 call.
    STDMETHOD(DrawIndexedPrimitive)
    (
        D3DPRIMITIVETYPE PrimitiveType,
        UINT minIndex,
        UINT NumVertices,
        UINT startIndex,
        UINT primCount
    )
    {
        HRESULT hr = mRealDevice->DrawIndexedPrimitive(PrimitiveType, mLastBaseIndex, minIndex, NumVertices, startIndex, primCount);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::DrawIndexedPrimitive(%u, %u, %u, %u, %u) == %d",
            PrimitiveType, minIndex, NumVertices, startIndex, primCount, hr
        );

        return hr;
    }

    STDMETHOD(DrawPrimitiveUP)
    (
        D3DPRIMITIVETYPE PrimitiveType,
        UINT PrimitiveCount,
        CONST void * pVertexStreamZeroData,
        UINT VertexStreamZeroStride
    )
    {
        HRESULT hr = mRealDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::DrawPrimitiveUP(%u, %u, %p, %u) == %d",
            PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride, hr
        );

        return hr;
    }

    STDMETHOD(DrawIndexedPrimitiveUP)
    (
        D3DPRIMITIVETYPE PrimitiveType,
        UINT MinVertexIndex,
        UINT NumVertexIndices,
        UINT PrimitiveCount,
        CONST void * pIndexData,
        D3DFORMAT IndexDataFormat,
        CONST void * pVertexStreamZeroData,
        UINT VertexStreamZeroStride
    )
    {
        HRESULT hr = mRealDevice->DrawIndexedPrimitiveUP
        (
            PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, 
            IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::DrawIndexedPrimitiveUP(%d, %d, %d, %d, %d, %d, %d, %d) == %d",
            PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, 
            pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride, 
            hr
        );

        return hr;
    }

    STDMETHOD(ProcessVertices)
    (
        UINT SrcStartIndex,
        UINT DestIndex,
        UINT VertexCount,
        IDirect3DVertexBuffer8* pDestBuffer,
        DWORD Flags
    )
    {
        HRESULT hr = mRealDevice->ProcessVertices
        (
            SrcStartIndex, DestIndex, VertexCount,(IDirect3DVertexBuffer9*)pDestBuffer, NULL, Flags
        );

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::ProcessVertices(%u, %u, %u, %p, %u) == %d",
            SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags, hr
        );

        return hr;
    }

    STDMETHOD(CreateVertexShader)
    (
        CONST DWORD * pDeclaration,
        CONST DWORD * pFunction,
        DWORD * pHandle,
        DWORD Usage
    )
    {
        HRESULT hr = mRealDevice->CreateVertexShader(pFunction, (IDirect3DVertexShader9**)pHandle);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::CreateVertexShader(%u, %u, %u, %u) == %d",
            *pDeclaration, *pFunction, *pHandle, Usage, hr
        );

        if ( SUCCEEDED(hr) )
        {
            mVertexShaders.insert(*pHandle);
        }

        return hr;
    }


    STDMETHOD(SetVertexShader)
    (
        DWORD Handle
    )
    {
        //! @todo Test the shader handling further; appears to function properly
        HRESULT hr = D3D_OK;

        if ( mVertexShaders.find(Handle) == mVertexShaders.end() )
        {
            // Not a shader, FVF
            hr = mRealDevice->SetFVF(Handle);
        } else {
            hr = mRealDevice->SetVertexShader((IDirect3DVertexShader9*)Handle);
        }

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetVertexShader(%u) == %d",
            Handle, hr
        );

        return hr;
    }


    STDMETHOD(GetVertexShader)
    (
        DWORD * pHandle
    )
    {
        (*pHandle) = mCurrentVertexShader;

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetVertexShader(%d) == D3D_OK",
            *pHandle
        );

        return D3D_OK;
    }

    /**
     * D3D9 has no equivalent function.            
     */
    STDMETHOD(DeleteVertexShader)
    (
        DWORD Handle
    )
    {
        HRESULT hr = D3D_OK;

        std::set<DWORD>::iterator vShader = mVertexShaders.find(Handle);
        if ( vShader != mVertexShaders.end() )
        {
            mVertexShaders.erase(vShader);
        } else {
            hr = D3DERR_INVALIDCALL;
        }

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::DeleteVertexShader(%u) == %d",
            Handle, hr
        );

        return D3D_OK;
    }

    STDMETHOD(SetVertexShaderConstant)
    (
        DWORD Register,
        CONST void * pConstantData,
        DWORD ConstantCount
    )
    {
        HRESULT hr = mRealDevice->SetVertexShaderConstantF(Register, (const float*)pConstantData, ConstantCount);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetVertexShaderConstant(%u, %p, %u) == %d",
            Register, pConstantData, ConstantCount, hr
        );

        return hr;
    }

    STDMETHOD(GetVertexShaderConstant)
    (
        DWORD Register,
        void * pConstantData,
        DWORD ConstantCount
    )
    {
        HRESULT hr = mRealDevice->GetVertexShaderConstantF(Register, (float*)pConstantData, ConstantCount);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetVertexShaderConstant(%u, %p, %u) == %d",
            Register, pConstantData, ConstantCount, hr
        );

        return hr;
    }


    STDMETHOD(GetVertexShaderDeclaration)
    (
        DWORD Handle,
        void * pData,
        DWORD * pSizeOfData
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetVertexShaderDeclaration(%u, %p, %u) == UNUSED",
            Handle, pData, *pSizeOfData
        );

        return DefaultErrorCode;
    }


    STDMETHOD(GetVertexShaderFunction)
    (
        DWORD Handle,
        void * pData,
        DWORD * pSizeOfData
    )
    {
        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetVertexShaderFunction(%u, %p, %u) == UNUSED",
            Handle, pData, *pSizeOfData
        );

        return DefaultErrorCode;
    }

    /**
     * @note DX8 does not use the concept of stream source offset, so IVoodoo3DDevice8::SetStreamSource always
     *          0 as the offset. 
     */
    STDMETHOD(SetStreamSource)
    (
        UINT StreamNumber,
        IDirect3DVertexBuffer8 * pStreamData,
        UINT Stride
    )
    {
        HRESULT hr = mRealDevice->SetStreamSource(StreamNumber,(IDirect3DVertexBuffer9*)pStreamData, 0, Stride);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::SetStreamSource(%u, %p, %u) == %d",
            StreamNumber, pStreamData, Stride, hr
        );

        return hr;
    }

    /**
     * @note DX8 does not use the concept of stream source offset, so IVoodoo3DDevice8::GetStreamSource
     *          discards the offset during retrieval. Since SetStreamSource uses 0 as the offset, there
     *          should be no issues with this.
     * @todo Verify that stream offset is not present in DX8, patch if otherwise.
     */
    STDMETHOD(GetStreamSource)
    (
        UINT StreamNumber,
        IDirect3DVertexBuffer8 ** ppStreamData,
        UINT * pStride
    )
    {
        UINT offset;
        HRESULT hr = mRealDevice->GetStreamSource(StreamNumber,(IDirect3DVertexBuffer9**)ppStreamData, &offset, pStride);

        VoodooCore->Log
        (
            LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DDevice8::GetStreamSource(%u, %p, %u) == %d",
            StreamNumber, *ppStreamData, *pStride, hr
        );

        return hr;
    }


    STDMETHOD(SetIndices)
    (
        IDirect3DIndexBuffer8 * pIndexData,
        UINT BaseVertexIndex
    )
    {
        HRESULT hr = mRealDevice->SetIndices((IDirect3DIndexBuffer9*)pIndexData);
        mLastBaseIndex = BaseVertexIndex;

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::SetIndices(%d, %d) == %d\n",
        pIndexData, BaseVertexIndex, hr);
#endif

        return hr;
    }


    STDMETHOD(GetIndices)
    (
        IDirect3DIndexBuffer8 ** ppIndexData,
        UINT * pBaseVertexIndex
    )
    {
        HRESULT hr = mRealDevice->GetIndices((IDirect3DIndexBuffer9**) ppIndexData);
        (*pBaseVertexIndex) = mLastBaseIndex;

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::GetIndices(%d, %d) == %d\n",
        *ppIndexData, *pBaseVertexIndex, hr);
#endif

        return hr;
    }


    STDMETHOD(CreatePixelShader)
    (
        CONST DWORD * pFunction,
        DWORD * pHandle
    )
    {
        HRESULT hr = mRealDevice->CreatePixelShader(pFunction, (IDirect3DPixelShader9**)pHandle);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::CreatePixelShader(%d, %d) == %d\n",
        pFunction, *pHandle, hr);
#endif

        if ( SUCCEEDED(hr) )
        {
            mPixelShaders.insert(*pHandle);
        }

        return hr;
    }


    STDMETHOD(SetPixelShader)
    (
        DWORD Handle
    )
    {
        //! @todo Test the shader setup
        HRESULT hr = D3D_OK;

        if ( mPixelShaders.find(Handle) == mPixelShaders.end() )
        {
            //! @todo Check if this is write, D3D8 docs have nothing
            hr = mRealDevice->SetPixelShader(NULL);
        } else {
            hr = mRealDevice->SetPixelShader((IDirect3DPixelShader9*)Handle);
        }

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::SetPixelShader(%d) == %d\n",
            Handle, hr);
#endif

        if ( SUCCEEDED(hr) )
        {
            mCurrentPixelShader = Handle;
        }

        return hr;
    }

    STDMETHOD(GetPixelShader)
    (
        DWORD * pHandle
    )
    {
        (*pHandle) = mCurrentPixelShader;

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::GetPixelShader(%d) == D3D_OK\n",
        *pHandle);
#endif

        return D3D_OK;
    }

    STDMETHOD(DeletePixelShader)
    (
        DWORD Handle
    )
    {
        HRESULT hr = D3D_OK; //mRealDevice->DeletePixelShader(DWORD Handle);
        std::set<DWORD>::iterator pShader = mPixelShaders.find(Handle);
        if ( pShader != mPixelShaders.end() )
        {
            mPixelShaders.erase(pShader);
        } else {
            //hr = D3DERR_INVALIDCALL;
        }

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::DeletePixelShader(%d) == %d\n",
        Handle, hr);
#endif

        return hr;
    }


    STDMETHOD(SetPixelShaderConstant)
    (
        DWORD Register,
        CONST void * pConstantData,
        DWORD ConstantCount
    )
    {
        HRESULT hr = mRealDevice->SetPixelShaderConstantF(Register, (float*)pConstantData, ConstantCount);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::SetPixelShaderConstant(%d, %d, %d) == %d\n",
        Register, pConstantData, ConstantCount, hr);
#endif

        return hr;
    }


    STDMETHOD(GetPixelShaderConstant)
    (
        DWORD Register,
        void * pConstantData,
        DWORD ConstantCount
    )
    {
        HRESULT hr = mRealDevice->GetPixelShaderConstantF(Register, (float*)pConstantData, ConstantCount);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::GetPixelShaderConstant(%d, %d, %d) == %d\n",
        Register, pConstantData, ConstantCount, hr);
#endif

        return hr;
    }


    STDMETHOD(GetPixelShaderFunction)
    (
        DWORD Handle,
        void * pData,
        DWORD * pSizeOfData
    )
    {
        //HRESULT hr = mRealDevice->GetPixelShaderFunction(Handle,void* pData,DWORD* pSizeOfData);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::GetPixelShaderFunction(%d, %d, %d) == UNUSED\n",
        Handle, pData, pSizeOfData);
#endif

        return DefaultErrorCode;
    }


    STDMETHOD(DrawRectPatch)
    (
        UINT Handle,
        CONST float * pNumSegs,
        CONST D3DRECTPATCH_INFO * pRectPatchInfo
    )
    {
        HRESULT hr = mRealDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::DrawRectPatch(%d, %d, %d) == %d\n",
        Handle, pNumSegs, pRectPatchInfo, hr);
#endif

        return hr;
    }


    STDMETHOD(DrawTriPatch)
    (
        UINT Handle,
        CONST float * pNumSegs,
        CONST D3DTRIPATCH_INFO * pTriPatchInfo
    )
    {
        HRESULT hr = mRealDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::DrawTriPatch(%d, %d, %d) == %d\n",
        Handle, pNumSegs, pTriPatchInfo, hr);
#endif

        return hr;
    }


    STDMETHOD(DeletePatch)
    (
        UINT Handle
    )
    {
        HRESULT hr = mRealDevice->DeletePatch(Handle);

#ifdef _DEBUG
        VoodooCore->Log(LL_Debug, VOODOO_GEM_NAME, "IVoodoo3DDevice8::DeletePatch(%d) == %d\n",
        Handle, hr);
#endif

        return hr;
    }


};

#endif /*VOODOO_GEM_DEVICE_HPP*/
