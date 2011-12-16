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

#include "DX9_Module.hpp"

#include "DX8_ThinHeader.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        /**
         * The core Voodoo3D8 wrapper class for D3D8 objects. This class is responsible for the primary abstraction from
         * D3D8 to D3D9. This class is responsible for creating devices, giving it control over what is returned to the
         * engine.
         *
         * @note The various CVoodoo3D8 interfaces provide an application-opaque wrapper that actually implements a Direct3D
         *      8.9 layer (8 to 9 translation). For use with D3D9 applications, the CVoodoo3D9 interface set must be used.
         */
        class CVoodoo3D8 :
            public IDirect3D8
        {
        public:
            CVoodoo3D8(UINT sdkVersion, IDirect3D9 * pRealObj);
            virtual STDMETHODCALLTYPE ~CVoodoo3D8();

            // IUnknown methods
            STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj);
            STDMETHOD_(ULONG, AddRef)();
            STDMETHOD_(ULONG, Release)();

            // IDirect3D8 methods
            STDMETHOD(CheckDepthStencilMatch)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                D3DFORMAT AdapterFormat,
                D3DFORMAT RenderTargetFormat,
                D3DFORMAT DepthStencilFormat
            );
            STDMETHOD(CheckDeviceFormat)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                D3DFORMAT AdapterFormat,
                DWORD Usage,
                D3DRESOURCETYPE RType,
                D3DFORMAT CheckFormat
            );
            /**
             * @note Direct3D8 doesn't seem to support the concept of multisampling quality levels, or won't recognize them
             *      in this function. They are not passed back because of this.
             */
            STDMETHOD(CheckDeviceMultiSampleType)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                D3DFORMAT SurfaceFormat,
                BOOL Windowed,
                D3DMULTISAMPLE_TYPE MultiSampleType
            );
            STDMETHOD(CheckDeviceType)
            (
                UINT Adapter,
                D3DDEVTYPE CheckType,
                D3DFORMAT DisplayFormat,
                D3DFORMAT BackBufferFormat,
                BOOL Windowed
            );
            STDMETHOD(CreateDevice)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                HWND hFocusWindow,
                DWORD BehaviorFlags,
                D3DPRESENT_PARAMETERS8 * pPresentationParameters,
                IDirect3DDevice8 **ppReturnedDeviceInterface
            );
            STDMETHOD(EnumAdapterModes)(UINT Adapter, UINT Mode, D3DDISPLAYMODE * pMode);
            STDMETHOD_(UINT, GetAdapterCount)();
            STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, D3DDISPLAYMODE * pMode);
            /**
             * @note This function forcibly ignores WHQL levels
             */
            STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 * pIdentifier);
            STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter);
            STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter);
            STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 * pCaps);
            /**
             * This is a legacy function to register a software renderer into the DX8 system; however, the DX8 docs state
             * that it is unsupported. The DX9 reintroduces support for it. Since the Voodoo CVoodoo3D8 layer targets DX9,
             * the function is callable and will target the appropriate DX9 function. It will log a warning message, due to
             * technically being illegal.
             */
            STDMETHOD(RegisterSoftwareDevice)(void *pInitializeFunction);

        private:
            UINT m_SdkVersion;
            IDirect3D9 * m_RealObject;
        };
    }
}
