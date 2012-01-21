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

#include "DX9_Adapter.hpp"
#include "DX8_ThinHeader.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        /**
         * Implements a thin wrapper around D3D8 textures to make them compatible with the CVoodoo3D8 API. This internally
         * translates all calls into appropriate D3D9 calls and keeps a pointer to the internal IDirect3DTexture9 object.
         */
        class CVoodoo3DTexture8 :
            public IDirect3DTexture8
        {
        public:
            CVoodoo3DTexture8(CVoodoo3DDevice8 * pDevice, IDirect3DTexture9 * pTexture);

            inline IDirect3DTexture9 *RealTexture() { return m_RealTexture; }
            STDMETHOD(QueryInterface)(REFIID riid, void **ppvObj);
            STDMETHOD_(ULONG, AddRef)();
            STDMETHOD_(ULONG, Release)();

            // IDirect3DBaseTexture8 methods
            STDMETHOD(GetDevice)(IDirect3DDevice8 **ppDevice);
            STDMETHOD(SetPrivateData)(REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags);
            STDMETHOD(GetPrivateData)(REFGUID refguid, void *pData, DWORD * pSizeOfData);
            STDMETHOD(FreePrivateData)(REFGUID refguid);
            STDMETHOD_(DWORD, SetPriority)(DWORD PriorityNew);
            STDMETHOD_(DWORD, GetPriority)();
            STDMETHOD_(void, PreLoad)();
            STDMETHOD_(D3DRESOURCETYPE, GetType)();
            STDMETHOD_(DWORD, SetLOD)(DWORD LODNew);
            STDMETHOD_(DWORD, GetLOD)();
            STDMETHOD_(DWORD, GetLevelCount)();
            STDMETHOD(GetLevelDesc)(UINT Level, D3DSURFACE_DESC8 * pDesc);
            STDMETHOD(GetSurfaceLevel)(UINT Level, IDirect3DSurface8 **ppSurfaceLevel);
            STDMETHOD(LockRect)(UINT Level, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags);
            STDMETHOD(UnlockRect)(UINT Level);
            STDMETHOD(AddDirtyRect)(CONST RECT * pDirtyRect);

        private:
            CVoodoo3DDevice8 * m_Device;
            IDirect3DTexture9 * m_RealTexture;
        };
    }
}
