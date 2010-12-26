/*************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (C) 2010 by Sean Sube
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
\*************************************************************************************************/

#ifndef VOODOO_GEM_TEXTURE_HPP
#define VOODOO_GEM_TEXTURE_HPP
//! @todo Verify/compile/test

#include "IVoodoo3DSurface8.hpp"

/**
 * Implements a thin wrapper around D3D8 textures to make them compatible with the Voodoo API.
 * This internally translates all calls into appropriate D3D9 calls and keeps a pointer to the
 * internal IDirect3DTexture9 object.
 */
class IVoodoo3DTexture8
	: public IDirect3DTexture8
{
private:
	IVoodoo3DDevice8 * mDevice;
	IDirect3DTexture9 * mRealTexture;

public:
	IVoodoo3DTexture8(IVoodoo3DDevice8 * device, IDirect3DTexture9 * texture)
		: mDevice(device), mRealTexture(texture)
	{
#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo DX8.9: IVoodoo3DTexture8::IVoodoo3DTexture8(%d, %d) == %d\n")
			.With(device).With(texture).With(this).Done();
#endif
	}

	inline IDirect3DTexture9 * RealTexture()
	{
		return mRealTexture;
	}

	STDMETHOD(QueryInterface)
	(
	 	REFIID riid,
		void ** ppvObj
	)
	{
		return mRealTexture->QueryInterface(riid, ppvObj);
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		return mRealTexture->AddRef();
	}

	STDMETHOD_(ULONG,Release)()
	{
		ULONG refCount = mRealTexture->Release();

		if ( refCount == 0 )
		{
			delete this;
			return 0;
		} else {
			return refCount;
		}
	}

	// IDirect3DBaseTexture8 methods
	STDMETHOD(GetDevice)
	(
	 	IDirect3DDevice8 ** ppDevice
	)
	{
		(*ppDevice) = (IDirect3DDevice8*)mDevice;
		return D3D_OK;
	}

	STDMETHOD(SetPrivateData)
	(
	 	REFGUID refguid,
		CONST void * pData,
		DWORD SizeOfData,
		DWORD Flags
	)
	{
		return mRealTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
	}

	STDMETHOD(GetPrivateData)
	(
	 	REFGUID refguid,
		void * pData,
		DWORD * pSizeOfData
	)
	{
		return mRealTexture->GetPrivateData(refguid, pData, pSizeOfData);
	}

	STDMETHOD(FreePrivateData)
	(
	 	REFGUID refguid
	)
	{
		return mRealTexture->FreePrivateData(refguid);
	}

	STDMETHOD_(DWORD, SetPriority)
	(
	 	DWORD PriorityNew
	)
	{
		return mRealTexture->SetPriority(PriorityNew);
	}

	STDMETHOD_(DWORD, GetPriority)()
	{
		return mRealTexture->GetPriority();
	}

	STDMETHOD_(void, PreLoad)()
	{
		return mRealTexture->PreLoad();
	}

	STDMETHOD_(D3DRESOURCETYPE, GetType)()
	{
		return mRealTexture->GetType();
	}

	STDMETHOD_(DWORD, SetLOD)
	(
	 	DWORD LODNew
	)
	{
		return mRealTexture->SetLOD(LODNew);
	}

	STDMETHOD_(DWORD, GetLOD)()
	{
		return mRealTexture->GetLOD();
	}

	STDMETHOD_(DWORD, GetLevelCount)()
	{
		return mRealTexture->GetLevelCount();
	}

	STDMETHOD(GetLevelDesc)
	(
	 	UINT Level,
		D3DSURFACE_DESC8 * pDesc
	)
	{
		D3DSURFACE_DESC9 rDesc;
		HRESULT hr = mRealTexture->GetLevelDesc(Level, &rDesc);
		if ( SUCCEEDED(hr) )
		{
			memcpy(pDesc, &rDesc, sizeof(D3DSURFACE_DESC8));
		}

		return hr;
	}

	STDMETHOD(GetSurfaceLevel)
	(
	 	UINT Level,
		IDirect3DSurface8 ** ppSurfaceLevel
	)
	{
		IDirect3DSurface9 * rSurface;
		HRESULT hr = mRealTexture->GetSurfaceLevel(Level, &rSurface);
		if ( SUCCEEDED(hr) )
		{
			IVoodoo3DSurface8 * newSurface = new IVoodoo3DSurface8(mDevice, rSurface);
			(*ppSurfaceLevel) = (IDirect3DSurface8*)newSurface;
		}

		return hr;
	}

	STDMETHOD(LockRect)
	(
	 	UINT Level,
		D3DLOCKED_RECT * pLockedRect,
		CONST RECT * pRect,
		DWORD Flags
	)
	{
		return mRealTexture->LockRect(Level, pLockedRect, pRect, Flags);
	}

	STDMETHOD(UnlockRect)
	(
	 	UINT Level
	)
	{
		return mRealTexture->UnlockRect(Level);
	}

	STDMETHOD(AddDirtyRect)
	(
	 	CONST RECT * pDirtyRect
	)
	{
		return mRealTexture->AddDirtyRect(pDirtyRect);
	}
};

#endif /*VOODOO_GEM_TEXTURE_HPP*/
