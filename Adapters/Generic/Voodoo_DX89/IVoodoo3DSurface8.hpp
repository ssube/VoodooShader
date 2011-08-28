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

#ifndef VOODOO_DX89_SURFACE_HPP
#define VOODOO_DX89_SURFACE_HPP

#include "DX89_Module.hpp"

class IVoodoo3DSurface8
            : public IDirect3DSurface8
{
    IVoodoo3DDevice8 * mRealDevice;
    IDirect3DSurface9 * mRealSurface;

public:
    IVoodoo3DSurface8(IVoodoo3DDevice8 * device, IDirect3DSurface9 * realSurface)
            : mRealDevice(device), mRealSurface(realSurface)
    {

    }

    inline IDirect3DSurface9 * RealSurface()
    {
        return mRealSurface;
    }

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
    {
        return mRealSurface->QueryInterface(riid, ppvObj);
    }

    STDMETHOD_(ULONG,AddRef)()
    {
        return mRealSurface->AddRef();
    }

    STDMETHOD_(ULONG,Release)()
    {
        ULONG refCount = mRealSurface->Release();

        if (refCount == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return refCount;
        }
    }

    /*** IDirect3DSurface8 methods ***/
    STDMETHOD(GetDevice)(IDirect3DDevice8** ppDevice)
    {
        (*ppDevice) = (IDirect3DDevice8*)VoodooDevice;
        return D3D_OK;
    }

    STDMETHOD(SetPrivateData)(REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)
    {
        return mRealSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    STDMETHOD(GetPrivateData)(REFGUID refguid,void* pData,DWORD* pSizeOfData)
    {
        return mRealSurface->GetPrivateData(refguid, pData, pSizeOfData);
    }

    STDMETHOD(FreePrivateData)(REFGUID refguid)
    {
        return mRealSurface->FreePrivateData(refguid);
    }

    STDMETHOD(GetContainer)(REFIID riid,void** ppContainer)
    {
        return mRealSurface->GetContainer(riid, ppContainer);
    }

    STDMETHOD(GetDesc)(D3DSURFACE_DESC8 *pDesc)
    {
        D3DSURFACE_DESC9 rDesc;
        HRESULT hr = mRealSurface->GetDesc(&rDesc);
        if (SUCCEEDED(hr))
        {
            pDesc->Format = rDesc.Format;
            pDesc->Type = rDesc.Type;
            pDesc->Usage = rDesc.Usage;
            pDesc->Pool = rDesc.Pool;
            pDesc->Size = 0;
            pDesc->MultiSampleType = rDesc.MultiSampleType;
            pDesc->Width = rDesc.Width;
            pDesc->Height = rDesc.Height;
        }

        return hr;
    }

    STDMETHOD(LockRect)(D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
    {
        return mRealSurface->LockRect(pLockedRect, pRect, Flags);
    }

    STDMETHOD(UnlockRect)()
    {
        return mRealSurface->UnlockRect();
    }

};

#endif /*VOODOO_DX89_SURFACE_HPP*/
