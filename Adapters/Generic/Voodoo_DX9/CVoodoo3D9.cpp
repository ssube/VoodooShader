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

#include "CVoodoo3D9.hpp"

#include "CVoodoo3DDevice9.hpp"

#include "DX9_Adapter.hpp"
#include "DX9_Converter.hpp"
#include "DX9_Version.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        CVoodoo3D9::CVoodoo3D9(UINT sdkVersion, IDirect3D9 * pD3D) :
            m_SdkVersion(sdkVersion), m_RealObject(pD3D)
        { }

        CVoodoo3D9::~CVoodoo3D9()
        {
            m_RealObject = nullptr;
        }

        /**
         * IUnknown methods
         */
        HRESULT STDMETHODCALLTYPE CVoodoo3D9::QueryInterface(REFIID riid, void **ppvObj)
        {
            return m_RealObject->QueryInterface(riid, ppvObj);
        }

        ULONG STDMETHODCALLTYPE CVoodoo3D9::AddRef()
        {
            return m_RealObject->AddRef();
        }

        ULONG STDMETHODCALLTYPE CVoodoo3D9::Release()
        {
            ULONG count = m_RealObject->Release();

            if (count == 0)
            {
                delete this;
            }

            return count;
        }

        /**
         * IDirect3D9 methods
         */
        HRESULT STDMETHODCALLTYPE CVoodoo3D9::RegisterSoftwareDevice(void * pInitializeFunction)
        {
            return m_RealObject->RegisterSoftwareDevice(pInitializeFunction);
        }

        UINT STDMETHODCALLTYPE CVoodoo3D9::GetAdapterCount()
        {
            return m_RealObject->GetAdapterCount();
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier)
        {
            return m_RealObject->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
        }

        UINT STDMETHODCALLTYPE CVoodoo3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
        {
            return m_RealObject->GetAdapterModeCount(Adapter, Format);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE *pMode)
        {
            return m_RealObject->EnumAdapterModes(Adapter, Format, Mode, pMode);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode)
        {
            return m_RealObject->GetAdapterDisplayMode(Adapter, pMode);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::CheckDeviceType
        (
            UINT Adapter,
            D3DDEVTYPE DevType,
            D3DFORMAT AdapterFormat,
            D3DFORMAT BackBufferFormat,
            BOOL bWindowed
        )
        {
            return m_RealObject->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::CheckDeviceFormat
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT AdapterFormat,
            DWORD Usage,
            D3DRESOURCETYPE RType,
            D3DFORMAT CheckFormat
        )
        {
            return m_RealObject->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::CheckDeviceMultiSampleType
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT SurfaceFormat,
            BOOL Windowed,
            D3DMULTISAMPLE_TYPE MultiSampleType,
            DWORD *pQualityLevels
        )
        {
            return m_RealObject->CheckDeviceMultiSampleType
            (
                Adapter,
                DeviceType,
                SurfaceFormat,
                Windowed,
                MultiSampleType,
                pQualityLevels
            );
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::CheckDepthStencilMatch
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT AdapterFormat,
            D3DFORMAT RenderTargetFormat,
            D3DFORMAT DepthStencilFormat
        )
        {
            return m_RealObject->CheckDepthStencilMatch
            (
                Adapter,
                DeviceType,
                AdapterFormat,
                RenderTargetFormat,
                DepthStencilFormat
            );
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::CheckDeviceFormatConversion
        (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT SourceFormat,
            D3DFORMAT TargetFormat
        )
        {
            return m_RealObject->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps)
        {
            return m_RealObject->GetDeviceCaps(Adapter, DeviceType, pCaps);
        }

        HMONITOR STDMETHODCALLTYPE CVoodoo3D9::GetAdapterMonitor(UINT Adapter)
        {
            return m_RealObject->GetAdapterMonitor(Adapter);
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D9::CreateDevice
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

            HRESULT hr = m_RealObject->CreateDevice
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
                *ppReturnedDeviceInterface = new CVoodoo3DDevice9(this, realDevice);

                Uuid adapterID = CLSID_DX9Adapter;
                DX9Adapter * pDXAdapter = nullptr;
                if (gpVoodooCore->GetAdapter()->QueryInterface(adapterID, (const void**)&pDXAdapter) && pDXAdapter)
                {
                    pDXAdapter->SetDXDevice(realDevice);
                }

                ILoggerRef logger = gpVoodooCore->GetLogger();

                HRESULT hrt = realDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbufferSurf);

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
                bufferTextureDesc.Size.Z = 0;
                bufferTextureDesc.Format = VoodooShader::VoodooDX9::DX9_Converter::ToTextureFormat(pPresentationParameters->BackBufferFormat);
                bufferTextureDesc.Mipmaps = true;
                bufferTextureDesc.RenderTarget = true;

                texture_ThisFrame = gpVoodooCore->CreateTexture(L":thisframe", bufferTextureDesc);
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
                    IFile * shaderFile = gpVoodooCore->GetFileSystem()->FindFile(L"test.cgfx");
                    if (shaderFile)
                    {
                        testShader = gpVoodooCore->CreateShader(shaderFile, nullptr);
                    }
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
