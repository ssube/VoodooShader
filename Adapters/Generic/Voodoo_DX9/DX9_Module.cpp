

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

 /**
  *
  */
 Adapter::Adapter(ICore *core, IDirect3DDevice9 *device) :
  mCore(core),
  mDevice(device)
 {
  assert(device);

  if (!core)
  {
   core = VoodooShader::ICore::Create();
  }

  core->Log("Voodoo DX9: Starting adapter...\n");

  try
  {
   core->SetAdapter(reinterpret_cast < VoodooShader::Adapter * > (this));
   core->Log("Voodoo DX9: ICore adapter set to this.\n");
  }
  catch(std::exception & exc)
  {
   core->Log("Voodoo DX9: Error setting adapter on core: %s.\n", exc.what());
  }

  HRESULT hr = cgD3D9SetDevice(device);

  if (!SUCCEEDED(hr))
  {
   Throw("Voodoo DX9: Could not set Cg device.", core);
  }
  else
  {
   core->Log("Voodoo DX9: Set Cg device.\n");
  }

  // ifdef _DEBUG



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
 bool Adapter::LoadPass(IPass *pass)
 {
 }

 /**
  *
  */
 void Adapter::BindPass(IPass* pass)
 {


 }

 /**
  *
  */
 void Adapter::UnbindPass(void)
 {
  if (cgIsProgram(mBoundVP))
  {
   cgD3D9UnbindProgram(mBoundVP);
  }

  if (cgIsProgram(mBoundFP))
  {
   cgD3D9UnbindProgram(mBoundFP);
  }

  mDevice->SetVertexShader(nullptr);
  mDevice->SetPixelShader(nullptr);
 }

 /**
  *
  */
 void Adapter::DrawQuad(Vertex *vertexData)
 {

  IDirect3DVertexBuffer9 *sourceBuffer;
  UINT sourceOffset, sourceStride;
  DWORD sourceFVF, zEnabled, aEnabled, cullMode;

  this->mDevice->GetStreamSource(0, &sourceBuffer, &sourceOffset, &sourceStride);
  this->mDevice->GetFVF(&sourceFVF);
  this->mDevice->GetRenderState(D3DRS_ZENABLE, &zEnabled);
  this->mDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &aEnabled);
  this->mDevice->GetRenderState(D3DRS_CULLMODE, &cullMode);

  this->mDevice->SetStreamSource(0, FSQuadVerts, 0, sizeof(FSVert));
  this->mDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
  this->mDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  this->mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  this->mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  if (!vertexData)
  {

   HRESULT hr = this->mDevice->BeginScene();

   if (SUCCEEDED(hr))
   {
    this->mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    this->mDevice->EndScene();
   }
   else
   {
    mCore->Log("Voodoo DX9: Failed to draw quad.\n");
   }
  }
  else
  {


   // Draw a quad from user vertexes
   HRESULT hr = this->mDevice->BeginScene();

   if (SUCCEEDED(hr))
   {
    hr = this->mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexData, sizeof(FSVert));
    if (!SUCCEEDED(hr))
    {
     this->mCore->Log("Voodoo DX9: Error drawing user quad.");
    }

    this->mDevice->EndScene();
   }
  }

  this->mDevice->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
  this->mDevice->SetFVF(sourceFVF);
  this->mDevice->SetRenderState(D3DRS_ZENABLE, zEnabled);
  this->mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, aEnabled);
  this->mDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
 }

 /**
  *
  */
 void Adapter::DrawShader(IShader* shader)
 { }

 /**
  *
  */
 void Adapter::ApplyParameter(IParameter* param)
 {
 }

 /**
  *
  */
 bool Adapter::ConnectTexture(IParameter* param, ITexture* texture)
 {
  if (Converter::ToParameterCategory(param->GetType()) == PC_Sampler)
  {
   param->Set(texture);

   IDirect3DTexture9 *texObj = (IDirect3DTexture9 *) texture->GetTexture();
   CGparameter texParam = param->GetParameter();

   cgD3D9SetTextureParameter(texParam, texObj);
   mCore->Log("Voodoo DX9: Bound texture %s to parameter %s.\n", texture->Name().GetData(), param->Name());
   return true;
  }
  else
  {
   Throw("Voodoo DX9: Invalid binding attempt, parameter is not a sampler.\n", this->mCore);
   return false;
  }
 }

 /**
  *
  */
 ITexture* Adapter::CreateTexture
(
 std::string name,
 size_t width,
 size_t height,
 size_t depth,
 bool mipmaps,
 TextureFormat format
)
 {

  IDirect3DTexture9 *tex = nullptr;
  D3DFORMAT fmt = DX9_Converter::ToD3DFormat(format);
  HRESULT hr = mDevice->CreateTexture
   (
    width,
    height,
    depth,
    D3DUSAGE_RENDERTARGET,
    fmt,
    D3DPOOL_DEFAULT,
    &tex,
    nullptr
   );

  if (SUCCEEDED(hr))
  {

   ITexture* texRef = mCore->CreateTexture(name, reinterpret_cast < void * > (tex));

   return texRef;
  }
  else
  {

   const char *error = cgD3D9TranslateHRESULT(hr);

   mCore->Log("Voodoo DX9: Error creating texture %s: %s\n", name, error);
   return ITexture*();
  }
 }

 /**
  *
  */
 void Adapter::HandleError(CGcontext context, CGerror error, void *core)
 {

  ICore *actualCore = reinterpret_cast < ICore * > (core);

  actualCore->Log("Voodoo DX9: Cg error: %s\n", cgD3D9TranslateCGerror(error));
 }
}
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
