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

#include "Gem_Module.hpp"
#include "IVoodoo3DSurface8.hpp"
#include "IVoodoo3DTexture8.hpp"

static HRESULT DefaultErrorCode = D3DERR_INVALIDCALL;

/**
* The core Voodoo wrapper class for D3D8 devices. This class is responsible for processing,
* translating and handling most API calls. Due to this, it must be as optimized as possible
* (considering the circumstances) and should contain only tested code.
* 
* @note The various IVoodoo3D8 interfaces provide an application-opaque wrapper that actually
* 		 implements a Direct3D 8.9 layer (8 to 9 translation). For use with D3D9 applications, the
* 		 IVoodoo3D9 interface set should be used.         
*/
class IVoodoo3DDevice8 
	: public IDirect3DDevice8
{
	/**
	* A pointer to the true underlying IDirect3DDevice9 object wrapped by this IVoodoo interface.            
	*/
	IDirect3DDevice9 * mRealDevice;

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
	IVoodoo3DDevice8(IDirect3DDevice9 * realDevice)
		: mRealDevice(realDevice)
	{
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::IVoodoo3DDevice8(%d) == %d\n",
			realDevice, this);
#endif
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::AddRef() == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::ApplyStateBlock == UNUSED\n");
#endif
		return DefaultErrorCode;
	}

	STDMETHOD(BeginScene)()
	{
		HRESULT hr = mRealDevice->BeginScene();

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::BeginScene() == %d\n",
			hr);
#endif

		return hr;
	}

	STDMETHOD(BeginStateBlock)()
	{
		HRESULT hr = mRealDevice->BeginStateBlock();

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::BeginStateBlock() == %d\n",
			hr);
#endif

		return hr;
	}

	STDMETHOD(CaptureStateBlock)(DWORD Token)
	{
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CaptureStateBlock == UNUSED\n");
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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::Clear(%d, %d, %d, %d, %d, %d) == %d\n",
			Count, pRects, Flags, Color, Z, Stencil, 
			hr);
#endif

		return hr;
	}

	STDMETHOD(TestCooperativeLevel)
	(
		
	)
	{
		HRESULT hr = mRealDevice->TestCooperativeLevel();

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::TestCooperativeLevel() == %d\n",
		hr);
#endif

		return hr;
	}


	STDMETHOD_(UINT, GetAvailableTextureMem)
	(
		
	)
	{
		UINT mem = mRealDevice->GetAvailableTextureMem();

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetAvailableTextureMem() == %d\n",
		mem);
#endif

		return mem;
	}


	STDMETHOD(ResourceManagerDiscardBytes)
	(
		DWORD Bytes
	)
	{
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::ResourceManagerDiscardBytes(%d) == UNUSED\n",
		Bytes);
#endif

		return DefaultErrorCode;
	}


	STDMETHOD(GetDirect3D)
	(
		IDirect3D8 ** ppD3D8
	)
	{
		(*ppD3D8) = (IDirect3D8*)VoodooObject;

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetDirect3D(%d) == D3D_OK\n",
		*ppD3D8);
#endif

		return D3D_OK;
	}


	STDMETHOD(GetDeviceCaps)
	(
		D3DCAPS8 * pCaps
	)
	{
		D3DCAPS9 rCaps;

		HRESULT hr = mRealDevice->GetDeviceCaps(&rCaps);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetDeviceCaps(%d) == %d\n",
		pCaps, hr);
#endif

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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetDisplayMode(%d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetCreationParameters(%d) == %d\n",
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
		//! @todo Will need tweaked when a IVoodoo3DSurface is implemented
		IVoodoo3DSurface8 * rCursor = (IVoodoo3DSurface8*)pCursorBitmap;

		HRESULT hr = mRealDevice->SetCursorProperties(XHotSpot, YHotSpot, rCursor->RealSurface());

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetCursorProperties(%d, %d, %d) == %d\n",
		XHotSpot, YHotSpot, pCursorBitmap, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetCursorPosition(int X,int Y,DWORD Flags)\n",
		X, Y, Flags);
#endif
	}


	STDMETHOD_(BOOL, ShowCursor)
	(
		BOOL bShow
	)
	{
		BOOL show = mRealDevice->ShowCursor(bShow);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::ShowCursor(%d) == %d\n",
		bShow, show);
#endif

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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateAdditionalSwapChain(%d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::Reset(%d) == %d\n",
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
		HRESULT shr = mRealDevice->StretchRect(backbufferSurf, NULL, surface_ThisFrame, NULL, D3DTEXF_NONE);

		if ( FAILED(shr) )
		{
			VoodooCore->Log("Voodoo Gem: Failed to stretch backbuffer to scratch texture.\n");
		}

		/*
		shr = mRealDevice->SetRenderTarget(0, backbufferSurf);
		if ( FAILED(shr) )
		{
			VoodooCore->Log("Voodoo Gem: Failed to set render target.\n");
		}

		VoodooShader::TechniqueRef tech = testShader->GetDefaultTechnique();
		VoodooShader::PassRef pass = tech->GetPass(0);

		VoodooGem->BindPass(pass);

		VoodooGem->DrawQuad(NULL);

		VoodooGem->UnbindPass();
		*/

		VoodooGem->DrawShader(testShader);

		// Present call

		HRESULT hr = mRealDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::Present(%d, %d, %d, %d) == %d\n",
		pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, 
		hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetBackBuffer(%d, %d, %d) == %d\n",
		BackBuffer, Type, ppBackBuffer, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetRasterStatus(%d) == %d\n",
		pRasterStatus, hr);
#endif

		return hr;
	}

	STDMETHOD_(void, SetGammaRamp)
	(
		DWORD Flags,
		const D3DGAMMARAMP * pRamp
	)
	{
		mRealDevice->SetGammaRamp(0, Flags, pRamp);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetRasterStatus(%d, %d) == void\n",
		Flags, pRamp);
#endif

		return;
	}

	STDMETHOD_(void, GetGammaRamp)
	(
		D3DGAMMARAMP * pRamp
	)
	{
		mRealDevice->GetGammaRamp(0, pRamp);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetRasterStatus(%d) == void\n",
			pRamp);
#endif

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
			rtt = false;
			hr = mRealDevice->CreateTexture
			(
				Width, Height, Levels, Usage, Format, 
				Pool, &rTexture, NULL
			);
		}

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateTexture(%d, %d, %d, %d, %d, %d, %d) == %d\n",
			Width, Height, Levels, Usage, Format, Pool, ppTexture, 
			hr);
#endif

		// One succeeded, the texture exists. We need to register it with the Voodoo core.
		if ( SUCCEEDED(hr) )
		{
			//VoodooCore->CreateTexture()
			//! @todo Set up the core texture registration for Voodoo Gem and track filenames somehow
			// 
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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateVolumeTexture(%d, %d, %d, %d, %d, %d, %d, %d) == %d\n",
		Width, Height, Depth, Levels, Usage, Format, Pool, 
		*ppVolumeTexture, hr);
#endif

		//! @todo Set up the core volume texture registration for Voodoo Gem
		if ( SUCCEEDED(hr) )
		{
			IVoodoo3DTexture8 * wTexture = new IVoodoo3DTexture8(this, (IDirect3DTexture9*)rTexture);
			(*ppVolumeTexture) = (IDirect3DCubeTexture8*)wTexture;
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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateCubeTexture(%d, %d, %d, %d, %d, %d) == %d\n",
		EdgeLength, Levels, Usage, Format, Pool, *ppCubeTexture, 
		hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateVertexBuffer(%d, %d, %d, %d, %d) == %d\n",
		Length, Usage, FVF, Pool, *ppVertexBuffer, 
		hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateIndexBuffer(%d, %d, %d, %d, %d) == %d\n",
		Length, Usage, Format, Pool, *ppIndexBuffer, 
		hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateRenderTarget(%d, %d, %d, %d, %d, %d) == %d\n",
		Width, Height, Format, MultiSample, Lockable, *ppSurface, 
		hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateDepthStencilSurface(%d, %d, %d, %d, %d) == %d\n",
		Width, Height, Format, MultiSample, *ppSurface, 
		hr);
#endif

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
	 * 			same. <em>(non-standard behavior)</em>
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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateImageSurface(%d, %d, %d, %d) == %d\n",
		Width, Height, Format, *ppSurface, hr);
#endif

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
	 * 			enough to fix, but I want to do more testing first. <em>(non-standard behavior)</em>
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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CopyRects(%d, %d, %d, %d, %d) == %d\n",
		pSourceSurface, pSourceRectsArray, cRects, pDestinationSurface, 
		pDestPointsArray, hr);
#endif

		return hr;
	}


	STDMETHOD(UpdateTexture)
	(
		IDirect3DBaseTexture8 * pSourceTexture,
		IDirect3DBaseTexture8 * pDestinationTexture
	)
	{
		IVoodoo3DTexture8 * wSource = (IVoodoo3DTexture8*)pSourceTexture;
		IVoodoo3DTexture8 * wDest = (IVoodoo3DTexture8*)pDestinationTexture;

		HRESULT hr = mRealDevice->UpdateTexture
		(
			wSource->RealTexture(), wDest->RealTexture()
		);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::UpdateTexture(%d, %d) == %d\n",
		pSourceTexture, pDestinationTexture, hr);
#endif

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
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetFrontBuffer(%d) == UNUSED\n",
		pDestSurface);
#endif

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
		IVoodoo3DSurface8 * wRender = (IVoodoo3DSurface8*)pRenderTarget;
		IVoodoo3DSurface8 * wStencil = (IVoodoo3DSurface8*)pNewZStencil;

		HRESULT hr = mRealDevice->SetRenderTarget(0, wRender->RealSurface());
		hr |= mRealDevice->SetDepthStencilSurface(wStencil->RealSurface());

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetRenderTarget(%d, %d) == %d\n",
		pRenderTarget, pNewZStencil, hr);
#endif

		return hr;
	}

	STDMETHOD(GetRenderTarget)
	(
		IDirect3DSurface8 ** ppRenderTarget
	)
	{
		IDirect3DSurface9 * rRenderTarget;
		HRESULT hr = mRealDevice->GetRenderTarget(0, &rRenderTarget);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetRenderTarget(%d) == %d\n",
		*ppRenderTarget, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetDepthStencilSurface(%d) == %d\n",
		*ppZStencilSurface, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::EndScene() == %d\n",
		hr);
#endif

		return hr;
	}

	STDMETHOD(SetTransform)
	(
		D3DTRANSFORMSTATETYPE State,
		CONST D3DMATRIX * pMatrix
	)
	{
		HRESULT hr = mRealDevice->SetTransform(State, pMatrix);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetTransform(%d, %d) == %d\n",
		State, pMatrix, hr);
#endif

		return hr;
	}


	STDMETHOD(GetTransform)
	(
		D3DTRANSFORMSTATETYPE State,
		D3DMATRIX * pMatrix
	)
	{
		HRESULT hr = mRealDevice->GetTransform(State, pMatrix);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetTransform(%d, %d) == %d\n",
		State, pMatrix, hr);
#endif

		return hr;
	}


	STDMETHOD(MultiplyTransform)
	(
		D3DTRANSFORMSTATETYPE State,
		CONST D3DMATRIX * pMatrix
	)
	{
		HRESULT hr = mRealDevice->MultiplyTransform(State, pMatrix);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::MultiplyTransform(%d, %d) == %d\n",
		State, pMatrix, hr);
#endif

		return hr;
	}


	STDMETHOD(SetViewport)
	(
		CONST D3DVIEWPORT8 * pViewport
	)
	{
		HRESULT hr = mRealDevice->SetViewport((CONST D3DVIEWPORT9 *)pViewport);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetViewport(%d) == %d\n",
		pViewport, hr);
#endif

		return hr;
	}


	STDMETHOD(GetViewport)
	(
		D3DVIEWPORT8 * pViewport
	)
	{
		HRESULT hr = mRealDevice->GetViewport((D3DVIEWPORT9*)pViewport);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetViewport(%d) == %d\n",
		pViewport, hr);
#endif

		return hr;
	}


	STDMETHOD(SetMaterial)
	(
		CONST D3DMATERIAL8 * pMaterial
	)
	{
		HRESULT hr = mRealDevice->SetMaterial((CONST D3DMATERIAL9*) pMaterial);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetMaterial(%d) == %d\n",
		pMaterial, hr);
#endif

		return hr;
	}


	STDMETHOD(GetMaterial)
	(
		D3DMATERIAL8 * pMaterial
	)
	{
		HRESULT hr = mRealDevice->GetMaterial((D3DMATERIAL9*) pMaterial);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetMaterial(%d) == %d\n",
		pMaterial, hr);
#endif

		return hr;
	}


	STDMETHOD(SetLight)
	(
		DWORD Index,
		CONST D3DLIGHT8 * Light
	)
	{
		HRESULT hr = mRealDevice->SetLight(Index, (CONST D3DLIGHT9*)Light);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetLight(%d, %d) == %d\n",
		Index, Light, hr);
#endif

		return hr;
	}


	STDMETHOD(GetLight)
	(
		DWORD Index,
		D3DLIGHT8 * Light
	)
	{
		HRESULT hr = mRealDevice->GetLight(Index, (D3DLIGHT9*)Light);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetLight(%d, %d) == %d\n",
		Index, Light, hr);
#endif

		return hr;
	}


	STDMETHOD(LightEnable)
	(
		DWORD Index,
		BOOL Enable
	)
	{
		HRESULT hr = mRealDevice->LightEnable(Index, Enable);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::LightEnable(%d, %d) == %d\n",
		Index, Enable, hr);
#endif

		return hr;
	}


	STDMETHOD(GetLightEnable)
	(
		DWORD Index,
		BOOL * pEnable
	)
	{
		HRESULT hr = mRealDevice->GetLightEnable(Index, pEnable);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetLightEnable(%d, %d) == %d\n",
		Index, pEnable, hr);
#endif

		return hr;
	}


	STDMETHOD(SetClipPlane)
	(
		DWORD Index,
		CONST float * pPlane
	)
	{
		HRESULT hr = mRealDevice->SetClipPlane(Index, pPlane);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetClipPlane(%d, %d) == %d\n",
		Index, pPlane, hr);
#endif

		return hr;
	}


	STDMETHOD(GetClipPlane)
	(
		DWORD Index,
		float * pPlane
	)
	{
		HRESULT hr = mRealDevice->GetClipPlane(Index, pPlane);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetClipPlane(%d, %d) == %d\n",
		Index, pPlane, hr);
#endif

		return hr;
	}


	STDMETHOD(SetRenderState)
	(
		D3DRENDERSTATETYPE State,
		DWORD Value
	)
	{
		HRESULT hr = mRealDevice->SetRenderState(State, Value);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetRenderState(%d, %d) == %d\n",
		State, Value, hr);
#endif

		return hr;
	}


	STDMETHOD(GetRenderState)
	(
		D3DRENDERSTATETYPE State,
		DWORD * pValue
	)
	{
		HRESULT hr = mRealDevice->GetRenderState(State, pValue);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetRenderState(%d, %d) == %d\n",
		State, pValue, hr);
#endif

		return hr;
	}

	/**
	 * Due to differences between the D3D8 and D3D9 APIs, this function is currently non-
	 * functional.            
	 */
	STDMETHOD(EndStateBlock)
	(
		DWORD * pToken
	)
	{
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::EndStateBlock(%d) == UNUSED\n",
			pToken);
#endif

		return DefaultErrorCode;

		/*
		HRESULT hr = mRealDevice->EndStateBlock(pToken);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::EndStateBlock(%d) == %d\n",
		pToken, hr);
#endif

		return hr;
		*/
	}


	/**
	 * This function does not exist in the D3D9 API.            
	 */
	STDMETHOD(DeleteStateBlock)
	(
		DWORD Token
	)
	{
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DeleteStateBlock(%d) == UNUSED\n",
			Token);
#endif

		return DefaultErrorCode;

		/*
		HRESULT hr = mRealDevice->DeleteStateBlock(Token);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DeleteStateBlock(%d) == %d\n",
		Token, hr);
#endif

		return hr;
		*/
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
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateStateBlock(%d, %d) == UNUSED\n",
			Type, pToken);
#endif

		return DefaultErrorCode;

		/*
		HRESULT hr = mRealDevice->CreateStateBlock(Type, pToken);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateStateBlock(%d, %d) == %d\n",
		Type, pToken, hr);
#endif

		return hr;
		*/
	}


	STDMETHOD(SetClipStatus)
	(
		CONST D3DCLIPSTATUS8 * pClipStatus
	)
	{
		HRESULT hr = mRealDevice->SetClipStatus((CONST D3DCLIPSTATUS9*)pClipStatus);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetClipStatus(%d) == %d\n",
		pClipStatus, hr);
#endif

		return hr;
	}


	STDMETHOD(GetClipStatus)
	(
		D3DCLIPSTATUS8 * pClipStatus
	)
	{
		HRESULT hr = mRealDevice->GetClipStatus((D3DCLIPSTATUS9*)pClipStatus);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetClipStatus(%d) == %d\n",
		pClipStatus, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetTexture(%d, %d) == %d\n",
		Stage, *ppTexture, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetTexture(%d, %d) == %d\n",
		Stage, pTexture, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetTextureStageState(%d, %d, %d) == %d\n",
		Stage, Type, pValue, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetTextureStageState(%d, %d, %d) == %d\n",
		Stage, Type, Value, hr);
#endif

		return hr;
	}


	STDMETHOD(ValidateDevice)
	(
		DWORD * pNumPasses
	)
	{
		HRESULT hr = mRealDevice->ValidateDevice(pNumPasses);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::ValidateDevice(%d) == %d\n",
		*pNumPasses, hr);
#endif

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
		//HRESULT hr = mRealDevice->GetInfo(DevInfoID, pDevInfoStruct, DevInfoStructSize);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetInfo(%d, %d, %d) == UNUSED\n",
		DevInfoID, pDevInfoStruct, DevInfoStructSize);
#endif

		return DefaultErrorCode;
	}

	STDMETHOD(SetPaletteEntries)
	(
		UINT PaletteNumber,
		CONST PALETTEENTRY * pEntries
	)
	{
		HRESULT hr = mRealDevice->SetPaletteEntries(PaletteNumber, pEntries);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetPaletteEntries(%d, %d) == %d\n",
		PaletteNumber, pEntries, hr);
#endif

		return hr;
	}

	STDMETHOD(GetPaletteEntries)
	(
		UINT PaletteNumber,
		PALETTEENTRY * pEntries
	)
	{
		HRESULT hr = mRealDevice->GetPaletteEntries(PaletteNumber, pEntries);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetPaletteEntries(%d, %d) == %d\n",
		PaletteNumber, pEntries, hr);
#endif

		return hr;
	}


	STDMETHOD(SetCurrentTexturePalette)
	(
		UINT PaletteNumber
	)
	{
		HRESULT hr = mRealDevice->SetCurrentTexturePalette(PaletteNumber);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetCurrentTexturePalette(%d) == %d\n",
		PaletteNumber, hr);
#endif

		return hr;
	}


	STDMETHOD(GetCurrentTexturePalette)
	(
		UINT * PaletteNumber
	)
	{
		HRESULT hr = mRealDevice->GetCurrentTexturePalette(PaletteNumber);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetCurrentTexturePalette(%d) == %d\n",
		*PaletteNumber, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DrawPrimitive(%d, %d, %d) == %d\n",
		PrimitiveType, StartVertex, PrimitiveCount, 
		hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DrawIndexedPrimitive(%d, %d, %d, %d, %d) == %d\n",
		PrimitiveType, minIndex, NumVertices, startIndex, primCount, 
		hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DrawPrimitiveUP(%d, %d, %d, %d) == %d\n",
		PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DrawIndexedPrimitiveUP(%d, %d, %d, %d, %d, %d, %d, %d) == %d\n",
		PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, 
		pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride, 
		hr);
#endif

		return hr;
	}

	//! @todo Check the parameter of the vertex decl in the D3D9 call. Taken from MGE, so...
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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::ProcessVertices(%d, %d, %d, %d, %d) == %d\n",
		SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags, 
		hr);
#endif

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
		//! @todo Set this up to keep a running log of shaders (probably a 
		//!			std::set<IDirect3DVertexShader9**>).
		HRESULT hr = mRealDevice->CreateVertexShader(pFunction, (IDirect3DVertexShader9**)pHandle);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreateVertexShader(%d, %d, %d, %d) == %d\n",
		pDeclaration, pFunction, pHandle, Usage, 
		hr);
#endif

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
		//! @todo Test the shader setup
		HRESULT hr = D3D_OK;

		if ( mVertexShaders.find(Handle) == mVertexShaders.end() )
		{
			// Not a shader, FVF
			hr = mRealDevice->SetFVF(Handle);
		} else {
			hr = mRealDevice->SetVertexShader((IDirect3DVertexShader9*)Handle);
		}

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetVertexShader(%d) == %d\n",
		Handle, hr);
#endif

		return hr;
	}


	STDMETHOD(GetVertexShader)
	(
		DWORD * pHandle
	)
	{
		//HRESULT hr = mRealDevice->GetVertexShader((IDirect3DVertexShader9**)pHandle);
		(*pHandle) = mCurrentVertexShader;

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetVertexShader(%d) == D3D_OK\n",
		pHandle);
#endif

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
		HRESULT hr = D3D_OK; //mRealDevice->DeleteVertexShader(DWORD Handle);
		std::set<DWORD>::iterator vShader = mVertexShaders.find(Handle);
		if ( vShader != mVertexShaders.end() )
		{
			mVertexShaders.erase(vShader);
		} else {
			hr = D3DERR_INVALIDCALL;
		}

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DeleteVertexShader(%d) == %d\n",
		Handle, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetVertexShaderConstant(%d, %d, %d) == %d\n",
		Register, pConstantData, ConstantCount, hr);
#endif

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

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetVertexShaderConstant(%d, %d, %d) == %d\n",
		Register, pConstantData, ConstantCount, hr);
#endif

		return hr;
	}


	STDMETHOD(GetVertexShaderDeclaration)
	(
		DWORD Handle,
		void * pData,
		DWORD * pSizeOfData
	)
	{
#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetVertexShaderDeclaration(%d, %d, %d) == UNUSED\n",
		Handle, pData, pSizeOfData);
#endif

		return DefaultErrorCode;
	}


	STDMETHOD(GetVertexShaderFunction)
	(
		DWORD Handle,
		void * pData,
		DWORD * pSizeOfData
	)
	{
		//HRESULT hr = mRealDevice->GetVertexShaderFunction(Handle, pData, pSizeOfData);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetVertexShaderFunction(%d, %d, %d) == UNUSED\n",
		Handle, pData, pSizeOfData);
#endif

		return DefaultErrorCode;
	}


	STDMETHOD(SetStreamSource)
	(
		UINT StreamNumber,
		IDirect3DVertexBuffer8 * pStreamData,
		UINT Stride
	)
	{
		HRESULT hr = mRealDevice->SetStreamSource(StreamNumber,(IDirect3DVertexBuffer9*)pStreamData, 0, Stride);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetStreamSource(%d, %d, %d) == %d\n",
		StreamNumber, pStreamData, Stride, hr);
#endif

		return hr;
	}


	STDMETHOD(GetStreamSource)
	(
		UINT StreamNumber,
		IDirect3DVertexBuffer8 ** ppStreamData,
		UINT * pStride
	)
	{
		UINT offset;
		HRESULT hr = mRealDevice->GetStreamSource(StreamNumber,(IDirect3DVertexBuffer9**)ppStreamData, &offset, pStride);

#ifdef _DEBUG
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetStreamSource(%d, %d, %d) == %d\n",
		StreamNumber, *ppStreamData, *pStride, hr);
#endif

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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetIndices(%d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetIndices(%d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::CreatePixelShader(%d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetPixelShader(%d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetPixelShader(%d) == D3D_OK\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DeletePixelShader(%d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::SetPixelShaderConstant(%d, %d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetPixelShaderConstant(%d, %d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::GetPixelShaderFunction(%d, %d, %d) == UNUSED\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DrawRectPatch(%d, %d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DrawTriPatch(%d, %d, %d) == %d\n",
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
		VoodooCore->Log("Voodoo Gem: IVoodoo3DDevice8::DeletePatch(%d) == %d\n",
		Handle, hr);
#endif

		return hr;
	}


};

#endif /*VOODOO_GEM_DEVICE_HPP*/
