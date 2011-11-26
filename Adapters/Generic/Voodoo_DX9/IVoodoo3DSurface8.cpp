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

namespace VoodooShader
{
    namespace VoodooDX8
    {
        IVoodoo3DSurface8(IVoodoo3DDevice8 *device, IDirect3DSurface9 *realSurface) :
            mRealDevice(device), mRealSurface(realSurface)
        { }

        inline IDirect3DSurface9 *RealSurface() { return mRealSurface; }

        /* IUnknown methods */
        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::QueryInterface(REFIID riid, void **ppvObj)
        {
            return mRealSurface->QueryInterface(riid, ppvObj);
        }
        ULONG STDMETHODCALLTYPE IVoodoo3DSurface8::AddRef()
        {
            return mRealSurface->AddRef();
        }
        ULONG STDMETHODCALLTYPE IVoodoo3DSurface8::Release()
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

        /* IDirect3DSurface8 methods */
        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::GetDevice(IDirect3DDevice8 **ppDevice)
        {
            (*ppDevice) = (IDirect3DDevice8 *) VoodooDevice;
            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::SetPrivateData(REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags)
        {
            return mRealSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::GetPrivateData(REFGUID refguid, void *pData, DWORD * pSizeOfData)
        {
            return mRealSurface->GetPrivateData(refguid, pData, pSizeOfData);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::FreePrivateData(REFGUID refguid)
        {
            return mRealSurface->FreePrivateData(refguid);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::GetContainer(REFIID riid, void **ppContainer)
        {
            return mRealSurface->GetContainer(riid, ppContainer);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::GetDesc(D3DSURFACE_DESC8 * pDesc)
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

        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::LockRect(D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags)
        {
            return mRealSurface->LockRect(pLockedRect, pRect, Flags);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3DSurface8::UnlockRect()
        {
            return mRealSurface->UnlockRect();
        }
    }
}
