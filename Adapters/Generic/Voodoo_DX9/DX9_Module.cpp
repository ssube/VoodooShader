

#include "DX9_Module.hpp"
#include "DX9_Converter.hpp"
#include "DX9_Version.hpp"

#include "IVoodoo3D9.hpp"
#include "IVoodoo3DDevice9.hpp"

VoodooShader::ICore * gpVoodooCore = nullptr;
VoodooShader::Adapter * VoodooDX9 = nullptr;

IVoodoo3D9 *VoodooObject = nullptr;
IVoodoo3DDevice9 *VoodooDevice = nullptr;

IDirect3DSurface9 *backbufferSurf = nullptr;

IDirect3DSurface9 *surface_ThisFrame = nullptr;
VoodooShader::ITexture* texture_ThisFrame;

VoodooShader::IShader* testShader;

namespace VoodooShader
{
 namespace DirectX9
{

  hr = this->mDevice->CreateVertexBuffer
   (
    6 * sizeof(FSVert),
    0,
    D3DFVF_CUSTOMVERTEX,
    D3DPOOL_DEFAULT,
    &FSQuadVerts,
    nullptr
   );

  if (FAILED(hr))
  {
   mCore->Log("Voodoo DX9: Failed to create vertex buffer.\n");
  }

  FSVert g_Vertices[4];

  memset(g_Vertices, 0, sizeof(FSVert) * 4);

  g_Vertices[0].x = -0.5f;
  g_Vertices[0].y = -0.5f;
  g_Vertices[0].z = 0.5f;
  g_Vertices[1].x = fx;
  g_Vertices[1].y = -0.5f;
  g_Vertices[1].z = 0.5f;
  g_Vertices[2].x = -0.5f;
  g_Vertices[2].y = fy;
  g_Vertices[2].z = 0.5f;
  g_Vertices[3].x = fx;
  g_Vertices[3].y = fy;
  g_Vertices[3].z = 0.5f;

  g_Vertices[0].rhw = g_Vertices[1].rhw = g_Vertices[2].rhw = g_Vertices[3].rhw = 1.0f;

  g_Vertices[0].tu = 0.0f;
  g_Vertices[0].tv = 0.0f;
  g_Vertices[1].tu = 1.0f;
  g_Vertices[1].tv = 0.0f;
  g_Vertices[2].tu = 0.0f;
  g_Vertices[2].tv = 1.0f;
  g_Vertices[3].tu = 1.0f;
  g_Vertices[3].tv = 1.0f;

  void *pVertices;

  FSQuadVerts->Lock(0, sizeof(FSVert) * 4, &pVertices, 0);

  memcpy(pVertices, g_Vertices, sizeof(FSVert) * 4);

  FSQuadVerts->Unlock();
 }

/**
 *
 */
VOODOO_API_DX9 void *__stdcall Voodoo3DCreate9(UINT version)
{

 // Voodoo DX9 Init function
 gpVoodooCore = VoodooShader::ICore::Create("Voodoo_DX9.log");

 VoodooLogger->Log("Voodoo DX9: Direct3DCreate9 called, SDK version: %d.\n", version);


 // Load the real d3d8 dll and get device caps
 char Path[MAX_PATH];

 GetSystemDirectoryA(Path, MAX_PATH);
 strcat_s(Path, MAX_PATH, "\\d3d9.dll");

 HMODULE d3ddll = LoadLibraryA(Path);
 D3DFunc9 d3d9func = (D3DFunc9) GetProcAddress(d3ddll, "Direct3DCreate9");

 if (!d3d9func)
 {
  VoodooLogger->Log("Voodoo DX9: Could not find D3D9 create true func.\n");
  return 0;
 }


 // Call DX9 to create a real device with the latest version
 IDirect3D9 *object = (d3d9func) (D3D_SDK_VERSION);

 if (!object)
 {
  VoodooLogger->Log("Voodoo DX9: Direct3DCreate9 returned nullptr.\n");
 }


 // Turn it into a FakeObject and return it.
 IVoodoo3D9 *vObj = new IVoodoo3D9(object);

 VoodooObject = vObj;
 return vObj;
}

#pragma comment(linker, "/export:Direct3DCreate9=?Voodoo3DCreate9@@YGPAXI@Z")
