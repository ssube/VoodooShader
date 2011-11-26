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
        class IVoodoo3DSurface8 :
            public IDirect3DSurface8
        {
        public:
            IVoodoo3DSurface8(IVoodoo3DDevice8 *device, IDirect3DSurface9 *realSurface);

            inline IDirect3DSurface9 *RealSurface() { return mRealSurface; }

            /* IUnknown methods */
            STDMETHOD(QueryInterface)(REFIID riid, void **ppvObj);
            STDMETHOD_(ULONG, AddRef)();
            STDMETHOD_(ULONG, Release)();

            /* IDirect3DSurface8 methods */
            STDMETHOD(GetDevice)(IDirect3DDevice8 **ppDevice);
            STDMETHOD(SetPrivateData)(REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags);
            STDMETHOD(GetPrivateData)(REFGUID refguid, void *pData, DWORD * pSizeOfData);
            STDMETHOD(FreePrivateData)(REFGUID refguid);
            STDMETHOD(GetContainer)(REFIID riid, void **ppContainer);
            STDMETHOD(GetDesc)(D3DSURFACE_DESC8 * pDesc);
            STDMETHOD(LockRect)(D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags);
            STDMETHOD(UnlockRect)();

        private:
            IVoodoo3DDevice8 * mRealDevice;
            IDirect3DSurface9 * mRealSurface;
        };
    }
}
