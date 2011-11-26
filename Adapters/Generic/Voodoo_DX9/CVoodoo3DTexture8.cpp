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

#include "CVoodoo3DTexture8.hpp"

#include "CVoodoo3DSurface8.hpp"

#include "DX9_Version.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        CVoodoo3DTexture8::CVoodoo3DTexture8(CVoodoo3DDevice8 * pDevice, IDirect3DTexture9 * pTexture) :
            m_Device(pDevice), m_RealTexture(pTexture)
        {
            gpVoodooLogger->Log
            (
                LL_ModDebug, VOODOO_DX9_NAME,
                L"CVoodoo3DTexture8::CVoodoo3DTexture8(%p, %p) == %p",
                pDevice, pTexture, this
            );
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::QueryInterface(REFIID riid, void **ppvObj)
        {
            return m_RealTexture->QueryInterface(riid, ppvObj);
        }

        ULONG STDMETHODCALLTYPE CVoodoo3DTexture8::AddRef()
        {
            return m_RealTexture->AddRef();
        }
        ULONG STDMETHODCALLTYPE CVoodoo3DTexture8::Release()
        {
            ULONG refCount = m_RealTexture->Release();

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
        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::GetDevice(IDirect3DDevice8 **ppDevice)
        {
            (*ppDevice) = (IDirect3DDevice8 *) m_Device;
            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::SetPrivateData(REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags)
        {
            return m_RealTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::GetPrivateData(REFGUID refguid, void *pData, DWORD * pSizeOfData)
        {
            return m_RealTexture->GetPrivateData(refguid, pData, pSizeOfData);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::FreePrivateData(REFGUID refguid)
        {
            return m_RealTexture->FreePrivateData(refguid);
        }

        DWORD STDMETHODCALLTYPE CVoodoo3DTexture8::SetPriority(DWORD PriorityNew)
        {
            return m_RealTexture->SetPriority(PriorityNew);
        }

        DWORD STDMETHODCALLTYPE CVoodoo3DTexture8::GetPriority()
        {
            return m_RealTexture->GetPriority();
        }

        void STDMETHODCALLTYPE CVoodoo3DTexture8::PreLoad()
        {
            return m_RealTexture->PreLoad();
        }

        D3DRESOURCETYPE STDMETHODCALLTYPE CVoodoo3DTexture8::GetType()
        {
            return m_RealTexture->GetType();
        }

        DWORD STDMETHODCALLTYPE CVoodoo3DTexture8::SetLOD(DWORD LODNew)
        {
            return m_RealTexture->SetLOD(LODNew);
        }

        DWORD STDMETHODCALLTYPE CVoodoo3DTexture8::GetLOD()
        {
            return m_RealTexture->GetLOD();
        }

        DWORD STDMETHODCALLTYPE CVoodoo3DTexture8::GetLevelCount()
        {
            return m_RealTexture->GetLevelCount();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::GetLevelDesc(UINT Level, D3DSURFACE_DESC8 * pDesc)
        {
            D3DSURFACE_DESC9 rDesc;
            HRESULT hr = m_RealTexture->GetLevelDesc(Level, &rDesc);
            if (SUCCEEDED(hr))
            {
                memcpy(pDesc, &rDesc, sizeof(D3DSURFACE_DESC8));
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::GetSurfaceLevel(UINT Level, IDirect3DSurface8 **ppSurfaceLevel)
        {
            IDirect3DSurface9 *rSurface;
            HRESULT hr = m_RealTexture->GetSurfaceLevel(Level, &rSurface);
            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *newSurface = new CVoodoo3DSurface8(m_Device, rSurface);
                (*ppSurfaceLevel) = (IDirect3DSurface8 *) newSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::LockRect(UINT Level, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags)
        {
            return m_RealTexture->LockRect(Level, pLockedRect, pRect, Flags);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::UnlockRect(UINT Level)
        {
            return m_RealTexture->UnlockRect(Level);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DTexture8::AddDirtyRect(CONST RECT * pDirtyRect)
        {
            return m_RealTexture->AddDirtyRect(pDirtyRect);
        }
    }
}