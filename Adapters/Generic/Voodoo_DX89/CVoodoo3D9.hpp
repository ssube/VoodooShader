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

#include "DX9_Adapter.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        class CVoodoo3D9 :
            public IDirect3D9
        {
        public:
            CVoodoo3D9(UINT sdkVersion, IDirect3D9 * pD3D);
            ~CVoodoo3D9();

            /**
            * IUnknown methods
            */
            STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObj);
            STDMETHOD_(ULONG, AddRef)();
            STDMETHOD_(ULONG, Release)();

            /**
             * IDirect3D9 methods
             */
            STDMETHOD(RegisterSoftwareDevice)(void * pInitializeFunction);
            STDMETHOD_(UINT, GetAdapterCount)();
            STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier);
            STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter, D3DFORMAT Format);
            STDMETHOD(EnumAdapterModes)(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE *pMode);
            STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, D3DDISPLAYMODE *pMode);
            STDMETHOD(CheckDeviceType)
            (
                UINT Adapter,
                D3DDEVTYPE DevType,
                D3DFORMAT AdapterFormat,
                D3DFORMAT BackBufferFormat,
                BOOL bWindowed
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
            STDMETHOD(CheckDeviceMultiSampleType)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                D3DFORMAT SurfaceFormat,
                BOOL Windowed,
                D3DMULTISAMPLE_TYPE MultiSampleType,
                DWORD *pQualityLevels
            );
            STDMETHOD(CheckDepthStencilMatch)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                D3DFORMAT AdapterFormat,
                D3DFORMAT RenderTargetFormat,
                D3DFORMAT DepthStencilFormat
            );
            STDMETHOD(CheckDeviceFormatConversion)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                D3DFORMAT SourceFormat,
                D3DFORMAT TargetFormat
            );
            STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps);
            STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter);
            STDMETHOD(CreateDevice)
            (
                UINT Adapter,
                D3DDEVTYPE DeviceType,
                HWND hFocusWindow,
                DWORD BehaviorFlags,
                D3DPRESENT_PARAMETERS *pPresentationParameters,
                IDirect3DDevice9 **ppReturnedDeviceInterface
            );

            STDMETHOD_(IDirect3D9 *, VSGetRealObject)();
            STDMETHOD(VSSetRealObject)(IDirect3D9 * pObj);

        private:
            UINT m_Refs;
            IDirect3D9 * m_RealObject;
            UINT m_SdkVersion;
        };
    }
}
