/*************************************************************************************************\
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
\*************************************************************************************************/

#ifndef VOODOO_GEM_TEXTURE_HPP
#define VOODOO_GEM_TEXTURE_HPP

#include "Gem_Includes.hpp"
#include "Gem_Version.hpp"

#include "IVoodoo3DSurface8.hpp"

using namespace VoodooShader;

/**
 * Implements a thin wrapper around D3D8 textures to make them compatible with the Voodoo API.
 * This internally translates all calls into appropriate D3D9 calls and keeps a pointer to the
 * internal IDirect3DTexture9 object.
 */
class IVoodoo3DTexture8
    : public IDirect3DTexture8
{
private:
    IVoodoo3DDevice8 * m_Device;
    IDirect3DTexture9 * m_RealTexture;
    String mName;
    TextureRef mVoodooTexture;

public:
    IVoodoo3DTexture8(IVoodoo3DDevice8 * device, IDirect3DTexture9 * texture)
        : m_Device(device), m_RealTexture(texture), mName(gLastFilename)
    {
        VoodooLogger->Log
        (
            VoodooShader::LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DTexture8::IVoodoo3DTexture8(%p, %p) == %p",
            device, texture, this
        );

        if ( gNextTexture )
        {
            gNextTexture = false;

            try
            {
                mVoodooTexture = VoodooCore->AddTexture(mName, this);
            } catch ( std::exception & exc ) {
                UNREFERENCED_PARAMETER(exc);
            }
        }
    }

    ~IVoodoo3DTexture8()
    {
        VoodooCore->RemoveTexture(mName);
    }

    inline IDirect3DTexture9 * RealTexture()
    {
        return m_RealTexture;
    }

    STDMETHOD(QueryInterface)
    (
         REFIID riid,
        void ** ppvObj
    )
    {
        return m_RealTexture->QueryInterface(riid, ppvObj);
    }

    STDMETHOD_(ULONG,AddRef)()
    {
        return m_RealTexture->AddRef();
    }

    STDMETHOD_(ULONG,Release)()
    {
        ULONG refCount = m_RealTexture->Release();

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
        (*ppDevice) = (IDirect3DDevice8*)m_Device;
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
        return m_RealTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
    }

    STDMETHOD(GetPrivateData)
    (
         REFGUID refguid,
        void * pData,
        DWORD * pSizeOfData
    )
    {
        return m_RealTexture->GetPrivateData(refguid, pData, pSizeOfData);
    }

    STDMETHOD(FreePrivateData)
    (
         REFGUID refguid
    )
    {
        return m_RealTexture->FreePrivateData(refguid);
    }

    STDMETHOD_(DWORD, SetPriority)
    (
         DWORD PriorityNew
    )
    {
        return m_RealTexture->SetPriority(PriorityNew);
    }

    STDMETHOD_(DWORD, GetPriority)()
    {
        return m_RealTexture->GetPriority();
    }

    STDMETHOD_(void, PreLoad)()
    {
        return m_RealTexture->PreLoad();
    }

    STDMETHOD_(D3DRESOURCETYPE, GetType)()
    {
        return m_RealTexture->GetType();
    }

    STDMETHOD_(DWORD, SetLOD)
    (
         DWORD LODNew
    )
    {
        return m_RealTexture->SetLOD(LODNew);
    }

    STDMETHOD_(DWORD, GetLOD)()
    {
        return m_RealTexture->GetLOD();
    }

    STDMETHOD_(DWORD, GetLevelCount)()
    {
        return m_RealTexture->GetLevelCount();
    }

    STDMETHOD(GetLevelDesc)
    (
         UINT Level,
        D3DSURFACE_DESC8 * pDesc
    )
    {
        D3DSURFACE_DESC rDesc;
        HRESULT hr = m_RealTexture->GetLevelDesc(Level, &rDesc);
        if ( SUCCEEDED(hr) )
        {
            memcpy(pDesc, &rDesc, sizeof(D3DSURFACE_DESC8));
        }

        return hr;
    }

    STDMETHOD(GetSurfaceLevel)
    (
        UINT level,
        IDirect3DSurface8 ** ppSurfaceLevel
    )
    {
        IDirect3DSurface9 * rSurface;
        HRESULT hr = m_RealTexture->GetSurfaceLevel(level, &rSurface);
        if ( SUCCEEDED(hr) )
        {
            IVoodoo3DSurface8 * newSurface = new IVoodoo3DSurface8(m_Device, rSurface);
            (*ppSurfaceLevel) = (IDirect3DSurface8*)newSurface;
        }

        VoodooLogger->Log
        (
            VoodooShader::LL_Debug, 
            VOODOO_GEM_NAME, 
            "IVoodoo3DTexture8::GetSurfaceLevel(%u, %p) == %d",
            level, *ppSurfaceLevel, hr
        );

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
        return m_RealTexture->LockRect(Level, pLockedRect, pRect, Flags);
    }

    STDMETHOD(UnlockRect)
    (
         UINT Level
    )
    {
        return m_RealTexture->UnlockRect(Level);
    }

    STDMETHOD(AddDirtyRect)
    (
         CONST RECT * pDirtyRect
    )
    {
        return m_RealTexture->AddDirtyRect(pDirtyRect);
    }
};

#endif /*VOODOO_GEM_TEXTURE_HPP*/
