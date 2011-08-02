/**************************************************************************************************\
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
\**************************************************************************************************/

#include "IVoodoo3D8.hpp"

IVoodoo3D8::IVoodoo3D8(IDirect3D9 * realObject)
    : mRealObject(realObject)
{

}

// IUnknown methods
STDMETHODIMP IVoodoo3D8::QueryInterface(REFIID riid, void ** ppvObj)
{
    return mRealObject->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG) IVoodoo3D8::AddRef()
{
    return mRealObject->AddRef();
}

STDMETHODIMP_(ULONG) IVoodoo3D8::Release()
{
    ULONG refCount = mRealObject->Release();

    if ( refCount == 0 )
    {
        delete this;
        return 0;
    } else {
        return refCount;
    }
}

STDMETHODIMP IVoodoo3D8::CheckDepthStencilMatch
(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat,
    D3DFORMAT RenderTargetFormat,
    D3DFORMAT DepthStencilFormat
)
{
    HRESULT hr = mRealObject->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::CheckDepthStencilMatch(%d, %d, %d, %d, %d) == %d\n",
        Adapter,DeviceType, AdapterFormat, RenderTargetFormat, 
        DepthStencilFormat, hr);
#endif

    return hr;
}

STDMETHODIMP IVoodoo3D8::CheckDeviceFormat
(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat,
    DWORD Usage,
    D3DRESOURCETYPE RType,
    D3DFORMAT CheckFormat
)
{
    HRESULT hr = mRealObject->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::CheckDeviceFormat(%d, %d, %d, %d, %d, %d) == %d\n",
        Adapter, DeviceType, AdapterFormat, Usage, RType, 
        CheckFormat, hr);
#endif

    return hr;
}

STDMETHODIMP IVoodoo3D8::CheckDeviceMultiSampleType
(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT SurfaceFormat,
    BOOL Windowed,
    D3DMULTISAMPLE_TYPE MultiSampleType
)
{
    HRESULT hr = mRealObject->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, NULL);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::CheckDeviceMultiSampleType(%d, %d, %d, %d, %d) == %d\n",
        Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, 
        hr);
#endif    

    return hr;
}

STDMETHODIMP IVoodoo3D8::CheckDeviceType
(
    UINT Adapter,
    D3DDEVTYPE CheckType,
    D3DFORMAT DisplayFormat,
    D3DFORMAT BackBufferFormat,
    BOOL Windowed
)
{
    HRESULT hr = mRealObject->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);

#ifdef _DEBUG 
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::CheckDeviceType(%d, %d, %d, %d, %d) == %d\n",
        Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed, 
        hr);
#endif

    return hr;
}

STDMETHODIMP IVoodoo3D8::CreateDevice
(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS8 * pPresentationParameters,
    IDirect3DDevice8 ** ppReturnedDeviceInterface
)
{
    D3DPRESENT_PARAMETERS9 mpPresentationParameters;

    mpPresentationParameters.Flags = pPresentationParameters->Flags;

    if( mpPresentationParameters.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER )
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
    const char * textureType = 
        VoodooShader::Converter::ToString(
        VoodooShader::DirectX89::DX89_Converter::ToTextureFormat(mpPresentationParameters.BackBufferFormat)
        );

    VoodooLogger->Log("Voodoo DX8.9: Backbuffer parameters for new device: %d by %d (%d buffers), %s.\n",
        mpPresentationParameters.BackBufferWidth, mpPresentationParameters.BackBufferHeight, 
        mpPresentationParameters.BackBufferCount, textureType);
#endif

    IDirect3DDevice9 * mRealDevice;
    HRESULT hr = mRealObject->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &mpPresentationParameters, &mRealDevice);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::CreateDevice(%d, %d, %d, %d, %d, %d) == %d\n",
        Adapter, DeviceType, hFocusWindow, BehaviorFlags, 
        &mpPresentationParameters, mRealDevice, hr);
#endif

    if ( !SUCCEEDED(hr) )
    {
        VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::CreateDevice failed with error %d on adapter %d.\n",
            hr, Adapter);
        return hr;
    } else {
        // Succeeded, create a fake device and go from there
        IVoodoo3DDevice8 * mFakeDevice = new IVoodoo3DDevice8(mRealDevice);
        VoodooDevice = mFakeDevice;

        VoodooDX89 = (VoodooShader::Adapter*)new VoodooShader::DirectX89::Adapter(VoodooCore, mRealDevice);

        (*ppReturnedDeviceInterface) = (IDirect3DDevice8*)mFakeDevice;

        HRESULT hrt = mRealDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_LEFT, &backbufferSurf);

        if ( SUCCEEDED(hrt) )
        {
            VoodooLogger->Log("Voodoo DX8.9: Cached backbuffer surface.\n");
        } else {
            VoodooLogger->Log("Voodoo DX8.9: Failed to retrieve backbuffer surface.\n");
        }

        texture_ThisFrame = VoodooDX89->CreateTexture(":thisframe", 
            pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, 1,
            true, VoodooShader::TF_RGB8);
        if ( texture_ThisFrame.get() )
        {
            IDirect3DTexture9 * texture = (IDirect3DTexture9*)texture_ThisFrame->GetTexture();
            hrt = texture->GetSurfaceLevel(0, &surface_ThisFrame);
            if ( SUCCEEDED(hrt) )
            {
                VoodooLogger->Log("Voodoo DX8.9: Cached :thisframe surface.\n");
            } else {
                VoodooLogger->Log("Voodoo DX8.9: Failed to :thisframe scratch surface.\n");
            }
        }

        try
        {
            testShader = VoodooCore->CreateShader("test.cgfx", NULL);
            testShader->Link();
        } catch ( std::exception & exc ) {
            VoodooLogger->Log("Voodoo DX8.9: Error loading shader: %s", exc.what());
        }

        return hr;
    }
}

STDMETHODIMP IVoodoo3D8::EnumAdapterModes
(
    UINT Adapter,
    UINT Mode,
    D3DDISPLAYMODE * pMode
)
{
    HRESULT hr = mRealObject->EnumAdapterModes(Adapter, D3DFMT_X8R8G8B8, Mode, pMode);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::EnumAdapterModes(%d, %d, %d) == %d\n",
        Adapter, Mode, pMode, hr);
#endif

    return hr;
}

STDMETHODIMP_(UINT) IVoodoo3D8::GetAdapterCount()
{
    UINT r = mRealObject->GetAdapterCount();

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::GetAdapterCount() == %d\n",
        r);
#endif

    return r;
}

STDMETHODIMP IVoodoo3D8::GetAdapterDisplayMode
(
    UINT Adapter,
    D3DDISPLAYMODE * pMode
)
{
    HRESULT hr = mRealObject->GetAdapterDisplayMode(Adapter, pMode);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::GetAdapterDisplayMode(%d, %d) == %d\n",
        Adapter, pMode, hr);
#endif

    pMode->Format = D3DFMT_X8R8G8B8;

    return hr;
}

STDMETHODIMP IVoodoo3D8::GetAdapterIdentifier
(
    UINT Adapter,
    DWORD Flags,
    D3DADAPTER_IDENTIFIER8 * pIdentifier
)
{
    D3DADAPTER_IDENTIFIER9 realIdentifier;

    HRESULT hr = mRealObject->GetAdapterIdentifier(Adapter, 0, &realIdentifier);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::GetAdapterIdentifier(%d, %d, %d) == %d\n",
        Adapter, Flags, pIdentifier, hr);
#endif

    if ( SUCCEEDED(hr) )
    {
        // Strings, so use copymemory instead of assignment
        ::CopyMemory (pIdentifier, &realIdentifier, 1024);
        pIdentifier->DeviceId            = realIdentifier.DeviceId;
        pIdentifier->DeviceIdentifier    = realIdentifier.DeviceIdentifier;
        pIdentifier->DriverVersion        = realIdentifier.DriverVersion;
        pIdentifier->Revision            = realIdentifier.Revision;
        pIdentifier->SubSysId            = realIdentifier.SubSysId;
        pIdentifier->VendorId            = realIdentifier.VendorId;
        pIdentifier->WHQLLevel            = realIdentifier.WHQLLevel;
    }

    return hr;
}

STDMETHODIMP_(UINT) IVoodoo3D8::GetAdapterModeCount
(
    UINT Adapter
)
{
    UINT r = mRealObject->GetAdapterModeCount(Adapter, D3DFMT_X8R8G8B8);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::GetAdapterModeCount(%d) == %d\n",
        Adapter, r);
#endif

    return r;
}

STDMETHODIMP_(HMONITOR) IVoodoo3D8::GetAdapterMonitor
(
    UINT Adapter
)
{
    HMONITOR hm = mRealObject->GetAdapterMonitor(Adapter);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::GetAdapterMonitor(%d) == %d\n",
        Adapter, hm);
#endif

    return hm;
}

STDMETHODIMP IVoodoo3D8::GetDeviceCaps
(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DCAPS8 * pCaps
)
{
    *pCaps = d3d8Caps;

    D3DCAPS9 realCaps;
    HRESULT hr = mRealObject->GetDeviceCaps(Adapter, DeviceType, &realCaps);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::GetDeviceCaps(%d, %d, %d) == %d\n",
        Adapter, DeviceType, pCaps, hr);
#endif

    int same = memcmp(pCaps, &realCaps, sizeof(D3DCAPS8));

    if ( same == 0 )
    {
        VoodooLogger->Log("Voodoo DX8.9: D3D8 and 9 caps are identical.\n"); 
    } else {
        VoodooLogger->Log("Voodoo DX8.9: Caps differ.\n");
    }

    return D3D_OK;
}
   
STDMETHODIMP IVoodoo3D8::RegisterSoftwareDevice
(
    void * pInitializeFunction
)
{
    HRESULT hr = mRealObject->RegisterSoftwareDevice(pInitializeFunction);

#ifdef _DEBUG
    VoodooLogger->Log("Voodoo DX8.9: IVoodoo3D8::RegisterSoftwareDevice(%d) == %d\n",
        pInitializeFunction, hr);
#else
    VoodooLogger->Log("Voodoo DX8.9: The application has registered a software device.\n");
#endif

    return hr;
}
