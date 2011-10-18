#pragma once
#include "IVoodoo3DDevice9.hpp"

class IVoodoo3D9 :
 public IDirect3D9
{

/**
 *
 */
private:
 IDirect3D9 *m_d3d;

/**
 *
 */
public:

 /**
  *
  */
 IVoodoo3D9(IDirect3D9 *d3d) :
 m_d3d(d3d)
 { }

 /**
  * IUnknown methods
  */
 HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj)   { return m_d3d->QueryInterface(riid, ppvObj); }

 /**
  *
  */
 ULONG STDMETHODCALLTYPE AddRef(void) { return m_d3d->AddRef(); }

 /**
  *
  */
 ULONG STDMETHODCALLTYPE Release(void)
 {

  ULONG count = m_d3d->Release();

  if (0 == count) delete this;
  return count;
 }


 /**
  * IDirect3D9 methods
  */
 HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void *pInitializeFunction)
 {
  return m_d3d->RegisterSoftwareDevice(pInitializeFunction);
 }

 /**
  *
  */
 UINT STDMETHODCALLTYPE GetAdapterCount(void) { return m_d3d->GetAdapterCount(); }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier)
 {
  return m_d3d->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
 }

 /**
  *
  */
 UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
 {
  return m_d3d->GetAdapterModeCount(Adapter, Format);
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE *pMode)
 {
  return m_d3d->EnumAdapterModes(Adapter, Format, Mode, pMode);
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode)
 {
  return m_d3d->GetAdapterDisplayMode(Adapter, pMode);
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE CheckDeviceType
 (
  UINT Adapter,
  D3DDEVTYPE DevType,
  D3DFORMAT AdapterFormat,
  D3DFORMAT BackBufferFormat,
  BOOL bWindowed
 )
 {
  return m_d3d->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE CheckDeviceFormat
 (
  UINT Adapter,
  D3DDEVTYPE DeviceType,
  D3DFORMAT AdapterFormat,
  DWORD Usage,
  D3DRESOURCETYPE RType,
  D3DFORMAT CheckFormat
 )
 {
  return m_d3d->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType
 (
  UINT Adapter,
  D3DDEVTYPE DeviceType,
  D3DFORMAT SurfaceFormat,
  BOOL Windowed,
  D3DMULTISAMPLE_TYPE MultiSampleType,
  DWORD *pQualityLevels
 )
 {
  return m_d3d->CheckDeviceMultiSampleType
   (
    Adapter,
    DeviceType,
    SurfaceFormat,
    Windowed,
    MultiSampleType,
    pQualityLevels
   );
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch
 (
  UINT Adapter,
  D3DDEVTYPE DeviceType,
  D3DFORMAT AdapterFormat,
  D3DFORMAT RenderTargetFormat,
  D3DFORMAT DepthStencilFormat
 )
 {
  return m_d3d->CheckDepthStencilMatch
   (
    Adapter,
    DeviceType,
    AdapterFormat,
    RenderTargetFormat,
    DepthStencilFormat
   );
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE CheckDeviceFormatConversion
 (
  UINT Adapter,
  D3DDEVTYPE DeviceType,
  D3DFORMAT SourceFormat,
  D3DFORMAT TargetFormat
 )
 {
  return m_d3d->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
 }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps)
 {
  return m_d3d->GetDeviceCaps(Adapter, DeviceType, pCaps);
 }

 /**
  *
  */
 HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter)  { return m_d3d->GetAdapterMonitor(Adapter); }

 /**
  *
  */
 HRESULT STDMETHODCALLTYPE CreateDevice
 (
  UINT Adapter,
  D3DDEVTYPE DeviceType,
  HWND hFocusWindow,
  DWORD BehaviorFlags,
  D3DPRESENT_PARAMETERS *pPresentationParameters,
  IDirect3DDevice9 **ppReturnedDeviceInterface
 )
 {

  IDirect3DDevice9 *realDevice;
  HRESULT hr = m_d3d->CreateDevice
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
   VoodooDX9 = (VoodooShader::Adapter *) new VoodooShader::DirectX9::Adapter(VoodooCore, realDevice);

   HRESULT hrt = realDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_LEFT, &backbufferSurf);

   if (SUCCEEDED(hrt))
   {
    VoodooLogger->Log("Voodoo DX9: Cached backbuffer surface.\n");
   }
   else
   {
    VoodooLogger->Log("Voodoo DX9: Failed to retrieve backbuffer surface.\n");
   }

   texture_ThisFrame = VoodooDX9->CreateTexture
    (
     ":thisframe",
     pPresentationParameters->BackBufferWidth,
     pPresentationParameters->BackBufferHeight,
     1,
     true,
     VoodooShader::TF_RGB8
    );
   if (texture_ThisFrame.get())
   {

    IDirect3DTexture9 *texture = (IDirect3DTexture9 *) texture_ThisFrame->GetTexture();

    hrt = texture->GetSurfaceLevel(0, &surface_ThisFrame);
    if (SUCCEEDED(hrt))
    {
     VoodooLogger->Log("Voodoo DX9: Cached :thisframe surface.\n");
    }
    else
    {
     VoodooLogger->Log("Voodoo DX9: Failed to :thisframe scratch surface.\n");
    }
   }

   try
   {
    testShader = VoodooCore->CreateShader("test.cgfx", nullptr);
    testShader->Link();
   }
   catch(std::exception & exc)
   {
    VoodooLogger->Log("Voodoo DX9: Error loading shader: %s.", exc.what());
   }
  }

  return hr;
 }
};
