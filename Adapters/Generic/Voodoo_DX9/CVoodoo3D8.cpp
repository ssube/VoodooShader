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

#include "CVoodoo3D8.hpp"

#include "CVoodoo3DDevice8.hpp"

#include "DX9_Converter.hpp"
#include "DX9_Version.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {  
        /**
        * The default, public constructor for CVoodoo3D objects.
        */
        CVoodoo3D8::CVoodoo3D8(IDirect3D9 *realObject) :
            m_RealObject(realObject)
        { }

        // IUnknown methods
        HRESULT STDMETHODCALLTYPE CVoodoo3D8::QueryInterface(REFIID riid, void **ppvObj)
        {
            return m_RealObject->QueryInterface(riid, ppvObj);
        }
        ULONG STDMETHODCALLTYPE CVoodoo3D8::AddRef()
        {
            return m_RealObject->AddRef();
        }
        ULONG STDMETHODCALLTYPE CVoodoo3D8::Release()
        {
            ULONG refCount = m_RealObject->Release();

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

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::CheckDepthStencilMatch
            (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT AdapterFormat,
            D3DFORMAT RenderTargetFormat,
            D3DFORMAT DepthStencilFormat
            )
        {
            HRESULT hr = m_RealObject->CheckDepthStencilMatch
                (Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::CheckDepthStencilMatch(%d, %d, %d, %d, %d) == %d", Adapter, DeviceType,
                AdapterFormat, RenderTargetFormat, DepthStencilFormat, hr
                );
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::CheckDeviceFormat
            (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT AdapterFormat,
            DWORD Usage,
            D3DRESOURCETYPE RType,
            D3DFORMAT CheckFormat
            )
        {
            HRESULT hr = m_RealObject->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::CheckDeviceFormat(%d, %d, %d, %d, %d, %d) == %d", Adapter, DeviceType,
                AdapterFormat, Usage, RType, CheckFormat, hr
                );
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::CheckDeviceMultiSampleType
            (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            D3DFORMAT SurfaceFormat,
            BOOL Windowed,
            D3DMULTISAMPLE_TYPE MultiSampleType
            )
        {
            HRESULT hr = m_RealObject->CheckDeviceMultiSampleType
                (Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, nullptr);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::CheckDeviceMultiSampleType(%d, %d, %d, %d, %d) == %d", Adapter, DeviceType,
                SurfaceFormat, Windowed, MultiSampleType, hr
                );
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::CheckDeviceType
            (
            UINT Adapter,
            D3DDEVTYPE CheckType,
            D3DFORMAT DisplayFormat,
            D3DFORMAT BackBufferFormat,
            BOOL Windowed
            )
        {
            HRESULT hr = m_RealObject->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::CheckDeviceType(%d, %d, %d, %d, %d) == %d", Adapter, CheckType, DisplayFormat,
                BackBufferFormat, Windowed, hr
                );
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::CreateDevice
            (
            UINT Adapter,
            D3DDEVTYPE DeviceType,
            HWND hFocusWindow,
            DWORD BehaviorFlags,
            D3DPRESENT_PARAMETERS8 * pPresentationParameters,
            IDirect3DDevice8 **ppReturnedDeviceInterface
            )
        {
            D3DPRESENT_PARAMETERS9 mpPresentationParameters;

            mpPresentationParameters.Flags = pPresentationParameters->Flags;

            if (mpPresentationParameters.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER)
            {
                mpPresentationParameters.Flags ^= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
            }
            mpPresentationParameters.Windowed = pPresentationParameters->Windowed;
            mpPresentationParameters.BackBufferCount = pPresentationParameters->BackBufferCount;
            mpPresentationParameters.BackBufferFormat = pPresentationParameters->BackBufferFormat;
            mpPresentationParameters.BackBufferHeight = pPresentationParameters->BackBufferHeight;
            mpPresentationParameters.BackBufferWidth = pPresentationParameters->BackBufferWidth;
            mpPresentationParameters.hDeviceWindow = pPresentationParameters->hDeviceWindow;
            mpPresentationParameters.EnableAutoDepthStencil = pPresentationParameters->EnableAutoDepthStencil;
            mpPresentationParameters.SwapEffect = pPresentationParameters->SwapEffect;
            mpPresentationParameters.MultiSampleQuality = 0;
            mpPresentationParameters.MultiSampleType = pPresentationParameters->MultiSampleType;
            mpPresentationParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
            mpPresentationParameters.FullScreen_RefreshRateInHz = 0;
            mpPresentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

#ifdef _DEBUG
            const char *textureType = VoodooShader::Converter::ToString
                (VoodooShader::DirectX89::DX89_Converter::ToTextureFormat(mpPresentationParameters.BackBufferFormat));
            gpVoodooLogger->Log
                (
                LL_ModInfo, VOODOO_DX9_NAME, L"Backbuffer parameters for new device: %d by %d (%d buffers), %s.",
                mpPresentationParameters.BackBufferWidth, mpPresentationParameters.BackBufferHeight,
                mpPresentationParameters.BackBufferCount, textureType
                );
#endif
            IDirect3DDevice9 *m_RealDevice;
            HRESULT hr = m_RealObject->CreateDevice
                (Adapter, DeviceType, hFocusWindow, BehaviorFlags, &mpPresentationParameters, &m_RealDevice);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModInfo, VOODOO_DX9_NAME, L"CVoodoo3D8::CreateDevice(%d, %d, %d, %d, %d, %d) == %d", Adapter, DeviceType,
                hFocusWindow, BehaviorFlags, &mpPresentationParameters, m_RealDevice, hr
                );
#endif
            if (!SUCCEEDED(hr))
            {
                gpVoodooLogger->Log(LL_ModError, VOODOO_DX9_NAME, L"CVoodoo3D8::CreateDevice failed with error %d on adapter %d.", hr, Adapter);
                return hr;
            }
            else
            {

                // Succeeded, create a fake device and go from there
                CVoodoo3DDevice8 * m_FakeDevice = new CVoodoo3DDevice8(m_RealDevice);
                VoodooDevice = m_FakeDevice;

                gpVoodooDX89 = (VoodooShader::Adapter *) new VoodooShader::DirectX89::Adapter(gpVoodooCore, m_RealDevice);
                (*ppReturnedDeviceInterface) = (IDirect3DDevice8 *) m_FakeDevice;

                HRESULT hrt = m_RealDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_LEFT, &backbufferSurf);

                if (SUCCEEDED(hrt))
                {
                    gpVoodooLogger->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Cached backbuffer surface.");
                }
                else
                {
                    gpVoodooLogger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to retrieve backbuffer surface.");
                }

                texture_ThisFrame = gpVoodooDX89->CreateTexture
                (
                    ":thisframe", pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, 1,
                    true, VoodooShader::TF_RGB8
                );

                if (texture_ThisFrame.get())
                {
                    IDirect3DTexture9 *texture = (IDirect3DTexture9 *) texture_ThisFrame->GetTexture();
                    hrt = texture->GetSurfaceLevel(0, &surface_ThisFrame);
                    if (SUCCEEDED(hrt))
                    {
                        gpVoodooLogger->Log(LL_ModInfo, VOODOO_DX9_NAME, L"Cached :thisframe surface.");
                    }
                    else
                    {
                        gpVoodooLogger->Log(LL_ModError, VOODOO_DX9_NAME, L"Failed to cache :thisframe surface.");
                    }
                }
                try
                {
                    testShader = gpVoodooCore->CreateShader("test.cgfx", nullptr);
                    testShader->Link();
                }
                catch(std::exception & exc)
                {
                    gpVoodooLogger->Log(LL_ModError, VOODOO_DX9_NAME, L"Error loading shader: %s", exc.what());
                }
                return hr;
            }
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::EnumAdapterModes(UINT Adapter, UINT Mode, D3DDISPLAYMODE * pMode)
        {
            HRESULT hr = m_RealObject->EnumAdapterModes(Adapter, D3DFMT_X8R8G8B8, Mode, pMode);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::EnumAdapterModes(%d, %d, %d) == %d", Adapter, Mode, pMode, hr);
#endif
            return hr;
        }

        UINT STDMETHODCALLTYPE CVoodoo3D8::GetAdapterCount()
        {
            UINT r = m_RealObject->GetAdapterCount();

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::GetAdapterCount() == %d", r);
#endif
            return r;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE * pMode)
        {
            HRESULT hr = m_RealObject->GetAdapterDisplayMode(Adapter, pMode);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::GetAdapterDisplayMode(%d, %d) == %d", Adapter, pMode, hr);
#endif
            pMode->Format = D3DFMT_X8R8G8B8;

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 * pIdentifier)
        {
            D3DADAPTER_IDENTIFIER9 realIdentifier;

            HRESULT hr = m_RealObject->GetAdapterIdentifier(Adapter, 0, &realIdentifier);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::GetAdapterIdentifier(%d, %d, %d) == %d", Adapter, Flags, pIdentifier, hr);
#endif
            if (SUCCEEDED(hr))
            {
                // Strings, so use copymemory instead of assignment
                ::CopyMemory(pIdentifier, &realIdentifier, 1024);
                pIdentifier->DeviceId = realIdentifier.DeviceId;
                pIdentifier->DeviceIdentifier = realIdentifier.DeviceIdentifier;
                pIdentifier->DriverVersion = realIdentifier.DriverVersion;
                pIdentifier->Revision = realIdentifier.Revision;
                pIdentifier->SubSysId = realIdentifier.SubSysId;
                pIdentifier->VendorId = realIdentifier.VendorId;
                pIdentifier->WHQLLevel = realIdentifier.WHQLLevel;
            }
            return hr;
        }

        UINT STDMETHODCALLTYPE CVoodoo3D8::GetAdapterModeCount(UINT Adapter)
        {
            UINT r = m_RealObject->GetAdapterModeCount(Adapter, D3DFMT_X8R8G8B8);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::GetAdapterModeCount(%d) == %d", Adapter, r);
#endif
            return r;
        }

        HMONITOR STDMETHODCALLTYPE CVoodoo3D8::GetAdapterMonitor(UINT Adapter)
        {
            HMONITOR hm = m_RealObject->GetAdapterMonitor(Adapter);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::GetAdapterMonitor(%d) == %d", Adapter, hm);
#endif
            return hm;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 * pCaps)
        {
            *pCaps = d3d8Caps;

            D3DCAPS9 realCaps;
            HRESULT hr = m_RealObject->GetDeviceCaps(Adapter, DeviceType, &realCaps);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::GetDeviceCaps(%d, %d, %d) == %d", Adapter, DeviceType, pCaps, hr);
#endif
            int same = memcmp(pCaps, &realCaps, sizeof(D3DCAPS8));

            if (same == 0)
            {
                gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"D3D8 and 9 caps are identical.");
            }
            else
            {
                gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"Caps differ.");
            }
            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3D8::RegisterSoftwareDevice(void *pInitializeFunction)
        {
            HRESULT hr = m_RealObject->RegisterSoftwareDevice(pInitializeFunction);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3D8::RegisterSoftwareDevice(%d) == %d", pInitializeFunction, hr);
#else
            gpVoodooLogger->Log(LL_ModWarning, VOODOO_DX9_NAME, L"The application has registered a software device with D3D8, illegal behavior.");
#endif

            return hr;
        }
    }
}
