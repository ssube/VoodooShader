/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

#include "CVoodoo3DSurface8.hpp"
#include "CVoodoo3DDevice8.hpp"

#include "DX9_Version.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        CVoodoo3DSurface8::CVoodoo3DSurface8(CVoodoo3DDevice8 * pDevice, IDirect3DSurface9 * pRealSurface) :
            m_Refs(0), m_Device(pDevice), m_RealSurface(pRealSurface)
        {
            if (m_RealSurface) m_RealSurface->AddRef();
            //if (m_Device) m_Device->AddRef();
        }

        CVoodoo3DSurface8::~CVoodoo3DSurface8()
        {
            //if (m_Device) m_Device->Release();
            //m_Device = nullptr;

            if (m_RealSurface) m_RealSurface->Release();
            m_RealSurface = nullptr;
        }

        /* IUnknown methods */
        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::QueryInterface(REFIID riid, void **ppvObj)
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
            else if (riid == IID_IDirect3DSurface8)
            {
                *ppvObj = this;
                ((IDirect3DSurface8*)(*ppvObj))->AddRef();
                return D3D_OK;
            }
            else
            {
                *ppvObj = nullptr;
                return D3DERR_INVALIDCALL;
            }
        }
        ULONG STDMETHODCALLTYPE CVoodoo3DSurface8::AddRef()
        {
            return ++m_Refs;
        }
        ULONG STDMETHODCALLTYPE CVoodoo3DSurface8::Release()
        {
            ULONG count = --m_Refs;

            if (count == 0)
            {
                delete this;
            }

            return count;
        }

        /* IDirect3DSurface8 methods */
        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::GetDevice(IDirect3DDevice8 **ppDevice)
        {
            (*ppDevice) = (IDirect3DDevice8 *)m_Device;
            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::SetPrivateData(REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags)
        {
            return m_RealSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::GetPrivateData(REFGUID refguid, void *pData, DWORD * pSizeOfData)
        {
            return m_RealSurface->GetPrivateData(refguid, pData, pSizeOfData);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::FreePrivateData(REFGUID refguid)
        {
            return m_RealSurface->FreePrivateData(refguid);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::GetContainer(REFIID riid, void **ppContainer)
        {
            return m_RealSurface->GetContainer(riid, ppContainer);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::GetDesc(D3DSURFACE_DESC8 * pDesc)
        {
            D3DSURFACE_DESC rDesc;
            HRESULT hr = m_RealSurface->GetDesc(&rDesc);
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

        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::LockRect(D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags)
        {
            return m_RealSurface->LockRect(pLockedRect, pRect, Flags);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DSurface8::UnlockRect()
        {
            return m_RealSurface->UnlockRect();
        }
    }
}
