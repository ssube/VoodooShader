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

#include "IVoodoo3DDevice9.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        IVoodoo3D9::IVoodoo3D9(IDirect3D9 * pD3D) :
            m_RealObj(pD3D)
        { }

        IVoodoo3D9::~IVoodoo3D9()
        {
            m_RealObj = nullptr;
        }

        /**
         * IUnknown methods
         */
        HRESULT STDMETHODCALLTYPE IVoodoo3D9::QueryInterface(REFIID riid, void **ppvObj)
        { 
            return m_RealObj->QueryInterface(riid, ppvObj);
        }

        ULONG STDMETHODCALLTYPE IVoodoo3D9::AddRef(void) 
        { 
            return m_RealObj->AddRef(); 
        }

        ULONG STDMETHODCALLTYPE IVoodoo3D9::Release(void)
        {
            ULONG count = m_RealObj->Release();

            if (count == 0)
            {
                delete this;
            }

            return count;
        }

        /**
         * IDirect3D9 methods
         */
        HRESULT STDMETHODCALLTYPE IVoodoo3D9::RegisterSoftwareDevice(void * pInitializeFunction)
        {
            return m_RealObj->RegisterSoftwareDevice(pInitializeFunction);
        }

        UINT STDMETHODCALLTYPE IVoodoo3D9::GetAdapterCount(void) 
        { 
            return m_RealObj->GetAdapterCount(); 
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier)
        {
            return m_RealObj->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
        }

        UINT STDMETHODCALLTYPE IVoodoo3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
        {
            return m_RealObj->GetAdapterModeCount(Adapter, Format);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE *pMode)
        {
            return m_RealObj->EnumAdapterModes(Adapter, Format, Mode, pMode);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode)
        {
            return m_RealObj->GetAdapterDisplayMode(Adapter, pMode);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::CheckDeviceType
        (
            UINT Adapter,
            D3DDEVTYPE DevType,
            D3DFORMAT AdapterFormat,
            D3DFORMAT BackBufferFormat,
            bool bWindowed
        )
        {
            return m_RealObj->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::CheckDeviceFormat
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT AdapterFormat,
            DWORD Usage,
            D3DRESOURCETYPE RType,
            D3DFORMAT CheckFormat
        )
        {
            return m_RealObj->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::CheckDeviceMultiSampleType
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT SurfaceFormat,
            bool Windowed,
            D3DMULTISAMPLE_TYPE MultiSampleType,
            DWORD *pQualityLevels
        )
        {
            return m_RealObj->CheckDeviceMultiSampleType
            (
                Adapter,
                DeviceType,
                SurfaceFormat,
                Windowed,
                MultiSampleType,
                pQualityLevels
            );
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::CheckDepthStencilMatch
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT AdapterFormat,
            D3DFORMAT RenderTargetFormat,
            D3DFORMAT DepthStencilFormat
        )
        {
            return m_RealObj->CheckDepthStencilMatch
            (
                Adapter,
                DeviceType,
                AdapterFormat,
                RenderTargetFormat,
                DepthStencilFormat
            );
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::CheckDeviceFormatConversion
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT SourceFormat,
            D3DFORMAT TargetFormat
        )
        {
            return m_RealObj->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps)
        {
            return m_RealObj->GetDeviceCaps(Adapter, DeviceType, pCaps);
        }

        HMONITOR STDMETHODCALLTYPE IVoodoo3D9::GetAdapterMonitor(UINT Adapter)  
        { 
            return m_RealObj->GetAdapterMonitor(Adapter); 
        }

        HRESULT STDMETHODCALLTYPE IVoodoo3D9::CreateDevice
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            HWND hFocusWindow,
            DWORD BehaviorFlags,
            D3DPRESENT_PARAMETERS *pPresentationParameters,
            IDirect3DDevice9 **ppReturnedDeviceInterface
        )
        {
            IDirect3DDevice9 * realDevice = nullptr;

            HRESULT hr = m_RealObj->CreateDevice
            (
                Adapter,
                DeviceType,
                hFocusWindow,
                BehaviorFlags,
                pPresentationParameters,
                &realDevice
            );

            if (SUCCEEDED(hr))
            {
                // Return our device
                *ppReturnedDeviceInterface = new IVoodoo3DDevice9(this, realDevice);

                DX9Adapter * pDXAdapter = nullptr;
                if (pDXAdapter->QueryInterface(CLSID_DX9Adapter, &pDXAdapter) && pDXAdapter)
                {
                    pDXAdapter->SetDXDevice(realDevice);
                }

                ILoggerRef logger = gpVoodooCore->GetLogger();

                HRESULT hrt = realDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_LEFT, &backbufferSurf);

                if (SUCCEEDED(hrt))
                {
                    logger->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Cached backbuffer surface.");
                }
                else
                {
                    logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to retrieve backbuffer surface.");
                }

                TextureDesc bufferTextureDesc;
                bufferTextureDesc.Size.X = pPresentationParameters->BackBufferWidth;
                bufferTextureDesc.Size.Y = pPresentationParameters->BackBufferHeight;
                bufferTextureDesc.Size.Z = 1;
                bufferTextureDesc.Format = TF_RGBA8;
                bufferTextureDesc.Mipmaps = true;
                bufferTextureDesc.RenderTarget = true;

                texture_ThisFrame = gpVoodooCore->CreateTexture(L":thisframe", &bufferTextureDesc);
                if (texture_ThisFrame)
                {
                    IDirect3DTexture9 * texture = reinterpret_cast<IDirect3DTexture9 *>(texture_ThisFrame->GetData());

                    hrt = texture->GetSurfaceLevel(0, &surface_ThisFrame);
                    if (SUCCEEDED(hrt))
                    {
                        logger->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Cached :thisframe surface.");
                    }
                    else
                    {
                        logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to :thisframe scratch surface.");
                    }
                }

                try
                {
                    testShader = gpVoodooCore->CreateShader("test.cgfx", nullptr);
                }
                catch(std::exception & exc)
                {
                    logger->Log(LL_ModError, VOODOO_DX9_NAME, L"Error loading shader: %S", exc.what());
                }
            }

            return hr;
        }
    }
}
