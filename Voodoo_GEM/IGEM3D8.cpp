/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (C) 2010 by Sean Sube
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

/**
 * The core GEM wrapper class for D3D8 objects. This class is responsible for the primary abstraction
 * from D3D8 to D3D9. This class is responsible for creating devices, giving it control over what is
 * returned to the engine.   
 * 
 * @note The various IGEM3D8 interfaces provide an application-opaque wrapper that actually
 * 		 implements a Direct3D 8.9 layer (8 to 9 translation). For use with D3D9 applications, the
 * 		 IGEM3D9 interface set should be used.         
 */
class IGEM3D8 
	: public IDirect3D8
{
	/**
	 * A pointer to the true underlying IDirect3D9 object wrapped by this IGEM interface.            
	 */
	IDirect3D9 * mRealObject;

public:
	/**
	 * The default, public constructor for IGEM3D objects.            
	 */
	IGEM3D8(IDirect3D9 * realObject)
		: mRealObject(realObject)
	{
		
	}

	// IUnknown methods
	GEMCALL(HRESULT) QueryInterface(REFIID riid, void ** ppvObj)
	{
		return mRealObject->QueryInterface(riid, ppvObj);
	}

	GEMCALL(ULONG) AddRef()
	{
		return mRealObject->AddRef();
	}

	GEMCALL(ULONG) Release()
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

	// IDirect3D8 methods

	/**
	 * Determines whether or not a depth-stencil format is compatible with a render target format in 
	 * a particular display mode.
	 *
	 * @param Adapter Ordinal number denoting the display adapter to query. D3DADAPTER_DEFAULT is 
	 * 				  always the primary display adapter. 
	 * @param DeviceType Member of the D3DDEVTYPE enumerated type, identifying the device type. 
	 * @param AdapterFormat Member of the D3DFORMAT enumerated type, identifying the format of the 
	 * 						display mode into which the adapter will be placed. 
	 * @param RenderTargetFormat Member of the D3DFORMAT enumerated type, identifying the format of 
	 * 							 the render target surface to be tested.
	 * @param DepthStencilFormat Member of the D3DFORMAT enumerated type, identifying the format of 
	 * 							 the depth-stencil surface to be tested. 
	 * @return D3D_OK if the format combination is valid, other values if invalid.
	 * 		    
	 * @note Will log heavily in debug builds.
	 */
	GEMCALL(HRESULT) CheckDepthStencilMatch
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
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::CheckDepthStencilMatch(%d, %d, %d, %d, %d) == %d\n")
			.With(Adapter).With(DeviceType).With(AdapterFormat).With(RenderTargetFormat)
			.With(DepthStencilFormat).With(cgD3D9TranslateHRESULT(hr)).Done();
#endif

		return hr;
	}

	GEMCALL(HRESULT) CheckDeviceFormat
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
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::CheckDeviceFormat(%d, %d, %d, %d, %d, %d) == %d\n")
			.With(Adapter).With(DeviceType).With(AdapterFormat).With(Usage).With(RType)
			.With(CheckFormat).With(cgD3D9TranslateHRESULT(hr)).Done();
#endif

		return hr;
	}

	/**
	 * @note Direct3D8 doesn't seem to support the concept of multisampling quality levels, or won't
	 * 		 recognize them in this function. They are not passed back because of this.            
	 */
	GEMCALL(HRESULT) CheckDeviceMultiSampleType
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
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::CheckDeviceMultiSampleType(%d, %d, %d, %d, %d) == %d\n")
			.With(Adapter).With(DeviceType).With(SurfaceFormat).With(Windowed).With(MultiSampleType)
			.With(cgD3D9TranslateHRESULT(hr)).Done();
#endif	

		return hr;
	}

	GEMCALL(HRESULT) CheckDeviceType
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
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::CheckDeviceType(%d, %d, %d, %d, %d) == %d\n")
			.With(Adapter).With(CheckType).With(DisplayFormat).With(BackBufferFormat).With(Windowed)
			.With(cgD3D9TranslateHRESULT(hr)).Done();
#endif

		return hr;
	}

	GEMCALL(HRESULT) CreateDevice
	(
		UINT Adapter,
		D3DDEVTYPE DeviceType,
		HWND hFocusWindow,
		DWORD BehaviorFlags,
		D3DPRESENT_PARAMETERS pPresentationParameters,
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
			VoodooShader::DirectX9::DX9_Converter::ToTextureFormat(mpPresentationParameters.BackBufferFormat)
			);

		VoodooCore->GetLog()->Format("Voodoo GEM: Backbuffer parameters for new device: %d by %d (%d buffers), %s.\n")
			.With(mpPresentationParameters.BackBufferWidth).With(mpPresentationParameters.BackBufferHeight)
			.With(mpPresentationParameters.BackBufferCount).With(textureType).Done();
#endif

		IDirect3DDevice9 * mRealDevice;
		HRESULT hr = mRealObject->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, mpPresentationParameters, &mRealDevice);

#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::CreateDevice(%d, %d, %d, %d, %d, %d) == %d\n")
			.With(Adapter).With(DeviceType).With(hFocusWindow).With(BehaviorFlags)
			.With(&mpPresentationParameters).With(mRealDevice).With(hr).Done();
#endif

		if ( !SUCCEEDED(hr) )
		{
			VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::CreateDevice failed with error %d on adapter %d.\n")
				.With(cgD3D9TranslateHRESULT(hr)).With(Adapter).Done();
			return hr;
		} else {
			// Succeeded, create a fake device and go from there
			IGEM3DDevice8 * mFakeDevice = new IGEM3DDevice8(mRealDevice);

			(*ppReturnedDeviceInterface) = mFakeDevice;

			return hr;
		}
	}

	GEMCALL(HRESULT) EnumAdapterModes
	(
		UINT Adapter,
		UINT Mode,
		D3DDISPLAYMODE * pMode
	)
	{
		HRESULT hr = mRealObject->EnumAdapterModes(Adapter, D3DFMT_X8R8G8B8, Mode, pMode);

#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::EnumAdapterModes(%d, %d, %d) == %d\n")
			.With(Adapter).With(Mode).With(pMode).With(cgD3D9TranslateHRESULT(hr)).Done();
#endif

		return hr;
	}

	GEMCALL(UINT) GetAdapterCount()
	{
		UINT r = mRealObject->GetAdapterCount();

#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::GetAdapterCount() == %d\n")
			.With(r).Done();
#endif

		return r;
	}

	GEMCALL(HRESULT) GetAdapterDisplayMode
	(
		UINT Adapter,
		D3DDISPLAYMODE * pMode
	)
	{
		HRESULT hr = mRealObject->GetAdapterDisplayMode(Adapter, pMode);

#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: IGEM3D8::GetAdapterDisplayMode(%d, %d) == %d\n")
			.With(Adapter).With(pMode).With(hr).Done();
#endif

		return hr;
	}
};
