

#include "Gem_Adapter.hpp"

#include "IVoodoo3D8.hpp"
#include "IVoodoo3DDevice8.hpp"
#include "IVoodoo3DSurface8.hpp"
#include "IVoodoo3DTexture8.hpp"

namespace VoodooShader
{
 namespace Gem
{

 /**
  *
  */
 Adapter::Adapter(ICore *core) :
  mCore(core),
  mDevice(nullptr),
  mQuadVerts(nullptr)
 {
  VoodooCore = mCore;
  VoodooLogger = mCore->GetLogger();
  VoodooGem = this;


  // Get the handles to the needed hook modules
  HMODULE procmodule = GetModuleHandle(nullptr);

  if (procmodule)
  {

   char procpath[MAX_PATH];

   GetModuleFileName(procmodule, procpath, MAX_PATH);
   mCore->GetLogger()->Log(LL_Info, VOODOO_GEM_NAME, "Gem loaded into process \"%s\".", procpath);
  }

  void *d3d8hookpoint = mCore->GetModuleManager()->FindFunction("d3d8.dll", "Direct3DCreate8");

  if (d3d8hookpoint)
  {

   IHookManager* hooker = mCore->GetHookManager();

   hooker->CreateHook("d3d8create", d3d8hookpoint, &Gem_D3D8Create);
   hooker->CreateHook("createfile", &CreateFileA, &Gem_CreateFileA);
  }
  else
  {
   mCore->GetLogger()->Log
    (
     LL_Error,
     VOODOO_GEM_NAME,
     "Unable to find D3D8 hook point; adapter will most likely not run."
    );
  }
 }

 /**
  *
  */
 Adapter::~Adapter(void)
 {

  // this->SetDevice(nullptr);
 }

 /**
  *
  */
 const char *Adapter::GetObjectClass(void)
 {
  return "Gem_Adapter";
 }

 /**
  *
  */
 void Adapter::SetDevice(_In_opt_ LPDIRECT3DDEVICE9 device)
 {
  if (mQuadVerts)
  {
   mQuadVerts->Release();
  }

  if (mDevice)
  {
   if (cgIsPass(mBoundPass))
   {
    cgResetPassState(mBoundPass);
   }

   mDevice->Release();
  }

  if (device == nullptr)
  {
   return;
  }

  mDevice = device;

  mDevice->AddRef(); // Make sure it won't be released while we're using it

  // Init Cg
  this->mCgContext = cgCreateContext();

  if (!cgIsContext(this->mCgContext))
  {
   throw std::exception("Unable to create Cg context.");
  }

  mCore->SetCgContext(mCgContext);

  cgSetContextBehavior(mCgContext, CG_BEHAVIOR_LATEST);
  cgSetLockingPolicy(CG_NO_LOCKS_POLICY);

  cgSetAutoCompile(mCgContext, CG_COMPILE_IMMEDIATE);
  cgSetParameterSettingMode(mCgContext, CG_IMMEDIATE_PARAMETER_SETTING);


  // Init CgD3D9
  HRESULT hr = cgD3D9SetDevice(mDevice);

  if (!SUCCEEDED(hr))
  {
   Throw(VOODOO_GEM_NAME, "Could not set Cg device.", mCore);
  }
  else
  {
   mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Set Cg device.");
  }

#ifdef _DEBUG
  cgD3D9EnableDebugTracing(CG_TRUE);
#endif
  cgD3D9RegisterStates(mCgContext);

  cgD3D9SetManageTextureParameters(mCgContext, CG_TRUE);

  HRESULT errors = cgD3D9GetLastError();

  if (!SUCCEEDED(errors))
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Errors setting Cg states.");
  }
  else
  {
   mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Cg states set successfully.");
  }


  // Setup profiles
  CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
  CGprofile bestVert = cgD3D9GetLatestVertexProfile();

  mCore->GetLogger()->Log
   (
    LL_Info,
    VOODOO_GEM_NAME,
    "The following profiles have been detected as the latest available: %s (vertex), %s (fragment)",
    cgGetProfileString(bestVert),
    cgGetProfileString(bestFrag)
   );


  // Setup resources Get params
  float fx = (float) gParams.BackBufferWidth;
  float fy = (float) gParams.BackBufferHeight;

  mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Prepping for %f by %f target.", fx, fy);

  hr = this->mDevice->CreateVertexBuffer
   (
    4 * sizeof(StandardQuadVert),
    0,
    D3DFVF_CUSTOMVERTEX,
    D3DPOOL_DEFAULT,
    &mQuadVerts,
    nullptr
   );

  mQuadVerts->AddRef();

  if (FAILED(hr))
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create vertex buffer.");
  }

  StandardQuadVert g_Vertices[4];

  memset(g_Vertices, 0, sizeof(StandardQuadVert) * 4);

  g_Vertices[0].x = -0.5f;
  g_Vertices[0].y = -0.5f;
  g_Vertices[0].z = 1.0f;
  g_Vertices[1].x = fx + 0.5f;
  g_Vertices[1].y = -0.5f;
  g_Vertices[1].z = 1.0f;
  g_Vertices[2].x = -0.5f;
  g_Vertices[2].y = fy + 0.5f;
  g_Vertices[2].z = 1.0f;
  g_Vertices[3].x = fx + 0.5f;
  g_Vertices[3].y = fy + 0.5f;
  g_Vertices[3].z = 1.0f;

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

  mQuadVerts->Lock(0, sizeof(StandardQuadVert) * 4, &pVertices, 0);

  memcpy(pVertices, g_Vertices, sizeof(StandardQuadVert) * 4);

  mQuadVerts->Unlock();


  // Cache various data bits
  HRESULT hrt = mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_LEFT, &(gBackbuffer.RawSurface));

  if (SUCCEEDED(hrt))
  {
   mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Cached backbuffer surface.");
  }
  else
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to retrieve backbuffer surface.");
  }

  TextureDesc stdtex = { gParams.BackBufferWidth, gParams.BackBufferHeight, 1, true, VoodooShader::TF_RGB8 };
  ITexture* thisframeTex = this->CreateTexture(":thisframe", stdtex);

  if (thisframeTex.get())
  {

   LPDIRECT3DTEXTURE9 texture = thisframeTex->GetData<IDirect3DTexture9> ();
   LPDIRECT3DSURFACE9 surface;

   hrt = texture->GetSurfaceLevel(0, &surface);
   if (SUCCEEDED(hrt))
   {
    mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Cached :thisframe surface.");
   }
   else
   {
    mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to :thisframe scratch surface.");
   }

   gThisFrame.Texture = thisframeTex;
   gThisFrame.RawTexture = texture;
   gThisFrame.RawSurface = surface;
  }
  else
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create :scratch texture.");
  }

  ITexture* scratchTex = this->CreateTexture(":scratch", stdtex);

  if (scratchTex.get())
  {

   LPDIRECT3DTEXTURE9 texture = scratchTex->GetData<IDirect3DTexture9> ();
   LPDIRECT3DSURFACE9 surface;

   hrt = texture->GetSurfaceLevel(0, &surface);
   if (SUCCEEDED(hrt))
   {
    mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Cached :scratch surface.");
   }
   else
   {
    mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to cache :scratch surface.");
   }

   gScratch.Texture = scratchTex;
   gScratch.RawTexture = texture;
   gScratch.RawSurface = surface;
  }
  else
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create :scratch texture.");
  }

  ITexture* lastframeTex = this->CreateTexture(":lastframe", stdtex);

  if (lastframeTex.get())
  {

   LPDIRECT3DTEXTURE9 texture = lastframeTex->GetData<IDirect3DTexture9> ();
   LPDIRECT3DSURFACE9 surface;

   hrt = texture->GetSurfaceLevel(0, &surface);
   if (SUCCEEDED(hrt))
   {
    mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Cached :lastframe surface.");
   }
   else
   {
    mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to cache :lastframe surface.");
   }

   gLastFrame.Texture = lastframeTex;
   gLastFrame.RawTexture = texture;
   gLastFrame.RawSurface = surface;
  }
  else
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create :lastframe texture.");
  }

  ITexture* lastshaderTex = this->CreateTexture(":lastshader", stdtex);

  if (lastshaderTex.get())
  {
   mCore->SetTexture(TT_ShaderTarget, lastshaderTex);

   LPDIRECT3DTEXTURE9 texture = lastshaderTex->GetData<IDirect3DTexture9> ();
   LPDIRECT3DSURFACE9 surface;

   hrt = texture->GetSurfaceLevel(0, &surface);
   if (SUCCEEDED(hrt))
   {
    mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Cached :lastshader surface.");
   }
   else
   {
    mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to cache :lastshader surface.");
   }

   gLastShader.Texture = lastshaderTex;
   gLastShader.RawTexture = texture;
   gLastShader.RawSurface = surface;
  }
  else
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create :lastshader texture.");
  }

  ITexture* lastpassTex = this->CreateTexture(":lastpass", stdtex);

  if (lastpassTex.get())
  {
   mCore->SetTexture(TT_PassTarget, lastpassTex);

   LPDIRECT3DTEXTURE9 texture = lastpassTex->GetData<IDirect3DTexture9> ();
   LPDIRECT3DSURFACE9 surface;

   hrt = texture->GetSurfaceLevel(0, &surface);
   if (SUCCEEDED(hrt))
   {
    mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Cached :lastpass surface.");
   }
   else
   {
    mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to cache :lastpass surface.");
   }

   gLastPass.Texture = lastpassTex;
   gLastPass.RawTexture = texture;
   gLastPass.RawSurface = surface;
  }
  else
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create :lastpass texture.");
  }

  // Create the global matrix parameters
  gMatrixView = VoodooCore->CreateParameter("matrix_view", PT_Matrix);
  gMatrixProj = VoodooCore->CreateParameter("matrix_proj", PT_Matrix);
  gMatrixWorld = VoodooCore->CreateParameter("matrix_world", PT_Matrix);

  try
  {
   testShader = mCore->CreateShader("test.cgfx", nullptr);
   testShader->Link();
  }
  catch(std::exception & exc)
  {
   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Error loading test shader: %s", exc.what());
  }
 }

 /**
  *
  */
 bool Adapter::LoadPass(IPass *pass)
 {

  ILogger* logger = this->mCore->GetLogger();

  if (pass == nullptr)
  {
   logger->Log(LL_Error, VOODOO_GEM_NAME, "Attempted to load nullptr pass.");

   return false;
  }

  logger->Log(LL_Debug, VOODOO_GEM_NAME, "Successfully loaded programs from '%s'.", pass->ToString().GetData());

  return true;
 }

 /**
  *
  */
 bool Adapter::UnloadPass(IPass *pass)
 {

  ILogger* logger = this->mCore->GetLogger();

  if (pass == nullptr)
  {
   logger->Log(LL_Error, VOODOO_GEM_NAME, "Attempted to unload nullptr pass.");

   return false;
  }

  logger->Log(LL_Debug, VOODOO_GEM_NAME, "Unloading pass %s", pass->ToString().GetData());

  return true;
 }

 /**
  *
  */
 void Adapter::BindPass(IPass* pass)
 {
  if (!pass.get())
  {
   this->mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Unable to bind nullptr pass.");
   return;
  }

  CGpass cgpass = pass->GetCgPass();

  if (!cgIsPass(cgpass))
  {
   this->mCore->GetLogger()->Log
    (
     LL_Error,
     VOODOO_GEM_NAME,
     "IPass %s contained invalid Cg pass (%p).",
     pass->ToString().GetData(),
     cgpass
    );
   return;
  }

  cgSetPassState(cgpass);

  CGerror error = cgGetError();

  while (error != CG_NO_ERROR)
  {
   this->mCore->GetLogger()->Log
    (
     LL_Error,
     VOODOO_GEM_NAME,
     "Cg error %u while binding pass \"%s\".",
     error,
     pass->ToString().GetData()
    );
   error = cgGetError();
  }

  mBoundPass = cgpass;
 }

 /**
  *
  */
 void Adapter::UnbindPass(void)
 {
  if (cgIsPass(mBoundPass))
  {
   cgResetPassState(mBoundPass);

   mBoundPass = nullptr;
  }
 }

 /**
  *
  */
 void Adapter::DrawShader(IShader* shader)
 {
  if (!shader.get())
  {
   this->mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Attempting to draw nullptr shader.");
  }


  // Set up textures and set scratch surface as render target
  IDirect3DSurface9 *rt = nullptr;
  HRESULT hr = mDevice->GetRenderTarget(0, &rt);

  if (FAILED(hr))
  {
   mCore->GetLogger()->Log
    (
     LL_Error,
     VOODOO_GEM_NAME,
     "Failed to retrieve render target for shader %s.",
     shader->ToString().GetData()
    );

   return;
  }

  hr = mDevice->SetRenderTarget(0, gScratch.RawSurface);
  if (FAILED(hr))
  {
   mCore->GetLogger()->Log
    (
     LL_Error,
     VOODOO_GEM_NAME,
     "Failed to bind render target for shader %s.",
     shader->ToString().GetData()
    );

   return;
  }


  // Get technique
  ITechnique* tech = shader->GetDefaultTechnique();
  size_t passCount = tech->GetPassCount();


  // Render loop
  for (size_t curPass = 0; curPass < passCount; ++curPass)
  {

   IPass* pass = tech->GetPass(curPass);

   this->BindPass(pass);

   this->DrawQuad(nullptr);

   this->UnbindPass();

   ITexture* passTarget = pass->GetTarget();

   if (passTarget.get())
   {

    IDirect3DTexture9 *passTargetD3D = passTarget->GetData<IDirect3DTexture9> ();
    IDirect3DSurface9 *passSurface = nullptr;

    hr = passTargetD3D->GetSurfaceLevel(0, &passSurface);
    if (FAILED(hr) || !passSurface)
    {
     mCore->GetLogger()->Log
      (
       LL_Warning,
       VOODOO_GEM_NAME,
       "Failed to get target surface for pass %s (targeting texture %s).",
       pass->ToString().GetData(),
       passTarget->ToString().GetData()
      );
    }
    else
    {
     hr = mDevice->StretchRect(gScratch.RawSurface, nullptr, passSurface, nullptr, D3DTEXF_NONE);

     if (FAILED(hr))
     {
      mCore->GetLogger()->Log
       (
        LL_Error,
        VOODOO_GEM_NAME,
        "Failed to copy results to target for pass %s.",
        pass->ToString().GetData()
       );
     }
    }
   }

   hr = mDevice->StretchRect(gScratch.RawSurface, nullptr, gBackbuffer.RawSurface, nullptr, D3DTEXF_NONE);

   if (FAILED(hr))
   {
    mCore->GetLogger()->Log
     (
      LL_Error,
      VOODOO_GEM_NAME,
      "Failed to copy results to target for pass %s (result %d).",
      pass->ToString().GetData(),
      hr
     );
   }
  }

  ITexture* techTarget = tech->GetTarget();

  if (techTarget.get())
  {

   IDirect3DTexture9 *techTargetD3D = techTarget->GetData<IDirect3DTexture9> ();
   IDirect3DSurface9 *techSurface = nullptr;

   hr = techTargetD3D->GetSurfaceLevel(0, &techSurface);
   if (FAILED(hr) || !techSurface)
   {
    mCore->GetLogger()->Log
     (
      LL_Warning,
      VOODOO_GEM_NAME,
      "Failed to get target surface for technique %s (targeting texture %s).",
      tech->ToString().GetData(),
      techTarget->ToString().GetData()
     );
   }
   else
   {
    hr = mDevice->StretchRect(gScratch.RawSurface, nullptr, techSurface, nullptr, D3DTEXF_NONE);

    if (FAILED(hr))
    {
     mCore->GetLogger()->Log
      (
       LL_Error,
       VOODOO_GEM_NAME,
       "Failed to copy results to target for technique %s.",
       tech->ToString().GetData()
      );
    }
   }
  }

  mDevice->SetRenderTarget(0, rt);
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

  this->mDevice->SetStreamSource(0, mQuadVerts, 0, sizeof(StandardQuadVert));
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
    mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Failed to draw quad.");
   }
  }
  else
  {


   // Draw a quad from user vertexes
   HRESULT hr = this->mDevice->BeginScene();

   if (SUCCEEDED(hr))
   {
    hr = this->mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexData, sizeof(StandardQuadVert));
    if (!SUCCEEDED(hr))
    {
     this->mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Error drawing user quad.");
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
 void Adapter::ApplyParameter(IParameter* param)
 {
  mCore->GetLogger()->Log(LL_Debug, VOODOO_GEM_NAME, "Applying parameter %s.", param->ToString().GetData());


  // ! @todo Find out if cgSetParameter or cgD3DSetUniform is better/faster and !
  // why cgD3DSetUniform returns errors on valid parameters.
  CGparameter cgparam = param->GetCgParameter();

  switch (param->GetType())
  {
  case PT_Float1:
   cgSetParameter1fv(cgparam, param->Getfloat());
   break;

  case PT_Float2:
   cgSetParameter2fv(cgparam, param->Getfloat());
   break;

  case PT_Float3:
   cgSetParameter3fv(cgparam, param->Getfloat());
   break;

  case PT_Float4:
   cgSetParameter4fv(cgparam, param->Getfloat());
   break;

  case PT_Matrix:
   cgSetMatrixParameterfc(cgparam, param->Getfloat());
   break;

  case PT_Sampler1D:
  case PT_Sampler2D:
  case PT_Sampler3D:
   cgD3D9SetTextureParameter(cgparam, param->GetTexture()->GetData<IDirect3DTexture9> ());
   break;

  case PT_Unknown:
  default:
   this->mCore->GetLogger()->Log
    (
     LL_Error,
     VOODOO_GEM_NAME,
     "Unable to bind parameters %s (unknown type).",
     param->ToString().GetData()
    );

   break;
  }
 }

 /**
  *
  */
 bool Adapter::ConnectTexture(IParameter* param, ITexture* texture)
 {
  if (Converter::ToParameterCategory(param->GetType()) == PC_Sampler)
  {
   param->Set(texture);

   IDirect3DTexture9 *texObj = texture->GetData<IDirect3DTexture9> ();
   CGparameter texParam = param->GetCgParameter();

   cgD3D9SetTextureParameter(texParam, texObj);
   mCore->GetLogger()->Log
    (
     LL_Debug,
     VOODOO_GEM_NAME,
     "Bound texture %s to parameter %s.",
     texture->ToString().GetData(),
     param->ToString().GetData()
    );

   return true;
  }
  else
  {
   Throw(VOODOO_GEM_NAME, "Invalid binding attempt, parameter is not a sampler.", this->mCore);
  }
 }

 /**
  *
  */
 ITexture* Adapter::CreateTexture(String name, TextureDesc desc)
 {

  IDirect3DTexture9 *tex = nullptr;
  D3DFORMAT fmt = Gem_Converter::ToD3DFormat(desc.Format);
  HRESULT hr = mDevice->CreateTexture
   (
    desc.Width,
    desc.Height,
    desc.Depth,
    D3DUSAGE_RENDERTARGET,
    fmt,
    D3DPOOL_DEFAULT,
    &tex,
    nullptr
   );

  if (SUCCEEDED(hr))
  {

   ITexture* texRef = mCore->AddTexture(name, reinterpret_cast < void * > (tex));

   return texRef;
  }
  else
  {

   const char *error = cgD3D9TranslateHRESULT(hr);

   mCore->GetLogger()->Log(LL_Error, VOODOO_GEM_NAME, "Error creating texture %s: %s", name.GetData(), error);

   return ITexture*();
  }
 }

 /**
  *
  */
 ITexture* Adapter::LoadTexture(_In_ IImage* image)
 {

  ITexture* tex = this->CreateTexture(image->GetPath(), image->GetImageDesc());
  LPDIRECT3DTEXTURE9 pTex = tex->GetData<IDirect3DTexture9> ();
  LPDIRECT3DSURFACE9 pSurf;

  pTex->GetSurfaceLevel(0, &pSurf);
  pSurf->LockRect(, nullptr, D3DLOCK_DISCARD);
 }

 /**
  *
  */
 void Adapter::HandleError(CGcontext context, CGerror error, void *core)
 {

  ICore *actualCore = reinterpret_cast < ICore * > (core);

  actualCore->GetLogger()->Log
   (
    LL_Error,
    VOODOO_GEM_NAME,
    "Cg error %u in context %p: %s",
    error,
    context,
    cgD3D9TranslateCGerror(error)
   );
 }
}
}
