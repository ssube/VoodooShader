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
#pragma once

#include "DX89_Module.hpp"

#include "IVoodoo3DSurface8.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        IVoodoo3DTexture8::IVoodoo3DTexture8(IVoodoo3DDevice8 * pDevice, IDirect3DTexture9 * pTexture) :
            mDevice(pDevice), mRealTexture(pTexture)
        {
            VoodooLogger->Log
            (
                LL_Debug, VOODOO_DX89_NAME,
                "IVoodoo3DTexture8::IVoodoo3DTexture8(%p, %p) == %p",
                device, texture, this
            );
        }

        inline IDirect3DTexture9 * IVoodoo3DTexture8::RealTexture() { return mRealTexture; }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::QueryInterface(REFIID riid, void **ppvObj)
        {
            return mRealTexture->QueryInterface(riid, ppvObj);
        }

        ULONG STDMETHODCALLTYPE IVoodoo3DTexture8::AddRef()
        {
            return mRealTexture->AddRef();
        }
        ULONG STDMETHODCALLTYPE IVoodoo3DTexture8::Release()
        {
            ULONG refCount = mRealTexture->Release();

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

        // IDirect3DBaseTexture8 methods
        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::GetDevice(IDirect3DDevice8 **ppDevice)
        {
            (*ppDevice) = (IDirect3DDevice8 *) mDevice;
            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::SetPrivateData(REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags)
        {
            return mRealTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::GetPrivateData(REFGUID refguid, void *pData, DWORD * pSizeOfData)
        {
            return mRealTexture->GetPrivateData(refguid, pData, pSizeOfData);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::FreePrivateData(REFGUID refguid)
        {
            return mRealTexture->FreePrivateData(refguid);
        }

        DWORD STDMETHODCALLTYPE IVoodoo3DTexture8::SetPriority(DWORD PriorityNew)
        {
            return mRealTexture->SetPriority(PriorityNew);
        }

        DWORD STDMETHODCALLTYPE IVoodoo3DTexture8::GetPriority()
        {
            return mRealTexture->GetPriority();
        }

        void STDMETHODCALLTYPE IVoodoo3DTexture8::PreLoad()
        {
            return mRealTexture->PreLoad();
        }

        D3DRESOURCETYPE STDMETHODCALLTYPE IVoodoo3DTexture8::GetType()
        {
            return mRealTexture->GetType();
        }

        DWORD STDMETHODCALLTYPE IVoodoo3DTexture8::SetLOD(DWORD LODNew)
        {
            return mRealTexture->SetLOD(LODNew);
        }

        DWORD STDMETHODCALLTYPE IVoodoo3DTexture8::GetLOD()
        {
            return mRealTexture->GetLOD();
        }

        DWORD STDMETHODCALLTYPE IVoodoo3DTexture8::GetLevelCount()
        {
            return mRealTexture->GetLevelCount();
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::GetLevelDesc(UINT Level, D3DSURFACE_DESC8 * pDesc)
        {
            D3DSURFACE_DESC9 rDesc;
            HRESULT hr = mRealTexture->GetLevelDesc(Level, &rDesc);
            if (SUCCEEDED(hr))
            {
                memcpy(pDesc, &rDesc, sizeof(D3DSURFACE_DESC8));
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::GetSurfaceLevel(UINT Level, IDirect3DSurface8 **ppSurfaceLevel)
        {
            IDirect3DSurface9 *rSurface;
            HRESULT hr = mRealTexture->GetSurfaceLevel(Level, &rSurface);
            if (SUCCEEDED(hr))
            {
                IVoodoo3DSurface8 *newSurface = new IVoodoo3DSurface8(mDevice, rSurface);
                (*ppSurfaceLevel) = (IDirect3DSurface8 *) newSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::LockRect(UINT Level, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags)
        {
            return mRealTexture->LockRect(Level, pLockedRect, pRect, Flags);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::UnlockRect(UINT Level)
        {
            return mRealTexture->UnlockRect(Level);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DTexture8::AddDirtyRect(CONST RECT * pDirtyRect)
        {
            return mRealTexture->AddDirtyRect(pDirtyRect);
        }
    }
}