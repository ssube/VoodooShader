
#include "Gem_Adapter.hpp"

#include "IVoodoo3D8.hpp"
#include "IVoodoo3DDevice8.hpp"
#include "IVoodoo3DSurface8.hpp"
#include "IVoodoo3DTexture8.hpp"

namespace VoodooShader
{
    namespace Gem
    {
        Adapter::Adapter(Core * core)
            : mCore(core), mDevice(NULL), mQuadVerts(NULL), mBoundFP(NULL), mBoundVP(NULL)
        {
            mCore->Log(LL_Info, VOODOO_GEM_NAME, "Starting adapter...");

            VoodooCore = mCore;
            VoodooGem = this;

            // Get the handles to the needed hook modules
            HMODULE procmodule = GetModuleHandle(NULL);
            if ( procmodule )
            {
                char procpath[MAX_PATH];
                GetModuleFileName(procmodule, procpath, MAX_PATH);
                mCore->Log(LL_Info, VOODOO_GEM_NAME, "Gem loaded into process \"%s\".", procpath);
            }

            void * d3d8hookpoint = mCore->GetModuleManager()->FindFunction("d3d8.dll", "Direct3DCreate8");
            if ( d3d8hookpoint )
            {
                IHookManager * hooker = mCore->GetHookManager();
                hooker->CreateHook("d3d8create", d3d8hookpoint, &Gem_D3D8Create);
                hooker->CreateHook("createfile", &CreateFileA, &Gem_CreateFileA);
            } else {
                mCore->Log(LL_Error, VOODOO_GEM_NAME, "Unable to find D3D8 hook point; adapter will most likely not run.");
            }
        }

        Adapter::~Adapter()
        {
            //this->SetDevice(NULL);
        }

        const char * Adapter::GetObjectClass()
        {
            return "Gem_Adapter";
        }

        void Adapter::SetDevice( _In_opt_ LPDIRECT3DDEVICE9 device )
        {
            if ( mQuadVerts )
            {
                mQuadVerts->Release();
            }

            if ( mDevice )
            {
                if ( mBoundFP )
                {
                    cgD3D9UnbindProgram(mBoundFP);
                }

                if ( mBoundVP )
                {
                    cgD3D9UnbindProgram(mBoundVP);
                }

                cgD3D9UnloadAllPrograms();

                mDevice->Release();
            }

            if ( device == NULL )
            {
                return;
            }

            mDevice = device;

            mDevice->AddRef(); // Make sure it won't be released while we're using it

            HRESULT hr = cgD3D9SetDevice(mDevice);
            if ( !SUCCEEDED(hr) )
            {
                Throw(VOODOO_GEM_NAME, "Could not set Cg device.", mCore);
            } else {
                mCore->Log(LL_Debug, VOODOO_GEM_NAME, "Set Cg device.");
            }

#ifdef _DEBUG
            cgD3D9EnableDebugTracing(CG_TRUE);
#endif

            CGcontext context = mCore->GetCgContext();
            cgD3D9SetManageTextureParameters(context, CG_TRUE);

            cgD3D9RegisterStates(context);

            HRESULT errors = cgD3D9GetLastError();
            if ( !SUCCEEDED(errors) )
            {
                mCore->Log(LL_Error, VOODOO_GEM_NAME, "Errors setting Cg states.");
            } else {
                mCore->Log(LL_Debug, VOODOO_GEM_NAME, "Cg states set successfully.");
            }

            // Setup profiles
            CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
            CGprofile bestVert = cgD3D9GetLatestVertexProfile();

            mCore->Log
            (
                LL_Info,
                VOODOO_GEM_NAME,
                "The following profiles have been detected as the latest available: %s (vertex), %s (fragment)",
                cgGetProfileString(bestVert), cgGetProfileString(bestFrag)
            );

            // Get params
            float fx = (float)gParams.BackBufferWidth;
            float fy = (float)gParams.BackBufferHeight;

            mCore->Log(LL_Debug, VOODOO_GEM_NAME, "Prepping for %f by %f target.", fx, fy);

            hr = this->mDevice->CreateVertexBuffer(4 * sizeof(StandardQuadVert), 0, D3DFVF_CUSTOMVERTEX,
                D3DPOOL_DEFAULT, &mQuadVerts, NULL);

            mQuadVerts->AddRef();

            if ( FAILED(hr) )
            {
                mCore->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create vertex buffer.");
            }

            StandardQuadVert g_Vertices[4];
            memset(g_Vertices, 0, sizeof(StandardQuadVert) * 4);

            g_Vertices[0].x =    - 0.5f;    g_Vertices[0].y =    - 0.5f;    g_Vertices[0].z = 1.0f;
            g_Vertices[1].x = fx + 0.5f;    g_Vertices[1].y =    - 0.5f;    g_Vertices[1].z = 1.0f;
            g_Vertices[2].x =    - 0.5f;    g_Vertices[2].y = fy + 0.5f;    g_Vertices[2].z = 1.0f;
            g_Vertices[3].x = fx + 0.5f;    g_Vertices[3].y = fy + 0.5f;    g_Vertices[3].z = 1.0f;

            g_Vertices[0].rhw = g_Vertices[1].rhw = g_Vertices[2].rhw = g_Vertices[3].rhw = 1.0f;

            g_Vertices[0].tu = 0.0f;    g_Vertices[0].tv = 0.0f;
            g_Vertices[1].tu = 1.0f;    g_Vertices[1].tv = 0.0f;
            g_Vertices[2].tu = 0.0f;    g_Vertices[2].tv = 1.0f;
            g_Vertices[3].tu = 1.0f;    g_Vertices[3].tv = 1.0f;

            void * pVertices;
            mQuadVerts->Lock(0, sizeof(StandardQuadVert) * 4, &pVertices, 0);

            memcpy(pVertices, g_Vertices, sizeof(StandardQuadVert) * 4);

            mQuadVerts->Unlock();

            // Cache various data bits
            HRESULT hrt = mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_LEFT, &(gBackbuffer.RawSurface));

            if ( SUCCEEDED(hrt) )
            {
                mCore->Log(LL_Debug, VOODOO_GEM_NAME, "Cached backbuffer surface.");
            } else {
                mCore->Log(LL_Error, VOODOO_GEM_NAME, "Failed to retrieve backbuffer surface.");
            }

            TextureDesc stdtex = { gParams.BackBufferWidth, gParams.BackBufferHeight, 1, true, VoodooShader::TF_RGB8 };

            TextureRef thisframeTex = this->CreateTexture(":thisframe", stdtex);

            if ( thisframeTex.get() )
            {
                LPDIRECT3DTEXTURE9 texture = thisframeTex->GetData<IDirect3DTexture9>();
                LPDIRECT3DSURFACE9 surface;
                hrt = texture->GetSurfaceLevel(0, &surface);
                if ( SUCCEEDED(hrt) )
                {
                    mCore->Log(LL_Debug, VOODOO_GEM_NAME, "Cached :thisframe surface.");
                } else {
                    mCore->Log(LL_Error, VOODOO_GEM_NAME, "Failed to :thisframe scratch surface.");
                }

                gThisFrame.Texture = thisframeTex;
                gThisFrame.RawTexture = texture;
                gThisFrame.RawSurface = surface;
            } else {
                mCore->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create :scratch texture.");
            }

            TextureRef scratchTex = this->CreateTexture(":scratch", stdtex);

            if ( scratchTex.get() )
            {
                LPDIRECT3DTEXTURE9 texture = scratchTex->GetData<IDirect3DTexture9>();
                LPDIRECT3DSURFACE9 surface;
                hrt = texture->GetSurfaceLevel(0, &surface);
                if ( SUCCEEDED(hrt) )
                {
                    mCore->Log(LL_Debug, VOODOO_GEM_NAME, "Cached :scratch surface.");
                } else {
                    mCore->Log(LL_Error, VOODOO_GEM_NAME, "Failed to cache :scratch surface.");
                }

                gScratch.Texture = scratchTex;
                gScratch.RawTexture = texture;
                gScratch.RawSurface = surface;
            } else {
                mCore->Log(LL_Error, VOODOO_GEM_NAME, "Failed to create :scratch texture.");
            }

            // Create the global matrix parameters
            gMatrixView  = VoodooCore->CreateParameter("matrix_view",  PT_Matrix);
            gMatrixProj  = VoodooCore->CreateParameter("matrix_proj",  PT_Matrix);
            gMatrixWorld = VoodooCore->CreateParameter("matrix_world", PT_Matrix);

            try
            {
                testShader = mCore->CreateShader("test.cgfx", NULL);
                testShader->Link();
            } catch ( VoodooShader::Exception & exc ) {
                UNREFERENCED_PARAMETER(exc);

                mCore->Log(LL_Error, VOODOO_GEM_NAME, "Error loading test shader.");
            }
        }

        bool Adapter::LoadPass(Pass * pass)
        {
            if ( pass == NULL )
            {
                this->mCore->Log
                (
                    LL_Error,
                    VOODOO_GEM_NAME,
                    "Attempted to load null pass."
                );

                return false;
            }

            CGprogram vertProg = pass->GetProgram(PS_Vertex);
            CGprogram fragProg = pass->GetProgram(PS_Fragment);

            this->mCore->Log
            (
                LL_Debug, 
                VOODOO_GEM_NAME, 
                "Loading pass %s", 
                pass->GetName().c_str()
            );

            HRESULT hr = S_OK;

            if ( cgIsProgram(vertProg) )
            {
                hr = cgD3D9LoadProgram(vertProg, CG_TRUE, 0);
                if ( !SUCCEEDED(hr) )
                {
                    this->mCore->Log
                    (
                        LL_Warning,
                        VOODOO_GEM_NAME, 
                        "Error loading vertex program from '%s': %s.",
                        pass->GetName().c_str(), cgD3D9TranslateHRESULT(hr)
                    );

                    return false;
                }
            }

            if ( cgIsProgram(fragProg) )
            {
                hr = cgD3D9LoadProgram(fragProg, CG_TRUE, 0);
                if ( !SUCCEEDED(hr) )
                {
                    this->mCore->Log
                    (
                        LL_Warning,
                        VOODOO_GEM_NAME, 
                        "Error loading fragment program from '%s': %s.",
                        pass->GetName().c_str(), cgD3D9TranslateHRESULT(hr)
                    );

                    return false;
                }
            }

            this->mCore->Log
            (
                LL_Debug,
                VOODOO_GEM_NAME,
                "Successfully loaded programs from '%s'.",
                pass->GetName().c_str()
            );

            return true;
        }

        bool Adapter::UnloadPass(Pass * pass)
        {
            if ( pass == NULL )
            {
                this->mCore->Log
                (
                    LL_Error,
                    VOODOO_GEM_NAME,
                    "Attempted to unload null pass."
                );

                return false;
            }

            CGprogram vertProg = pass->GetProgram(PS_Vertex);
            CGprogram fragProg = pass->GetProgram(PS_Fragment);

            this->mCore->Log
            (
                LL_Debug, 
                VOODOO_GEM_NAME, 
                "Unloading pass %s", 
                pass->GetName().c_str()
            );

            HRESULT hr = cgD3D9UnloadProgram(vertProg);
            if ( FAILED(hr) )
            {
                this->mCore->Log
                (
                    LL_Warning, 
                    VOODOO_GEM_NAME, 
                    "Error unloading vertex program from pass %s.", 
                    pass->GetName().c_str()
                );
            }
            
            hr = cgD3D9UnloadProgram(fragProg);
            if ( FAILED(hr) )
            {
                this->mCore->Log
                (
                    LL_Warning, 
                    VOODOO_GEM_NAME, 
                    "Error unloading fragment program from pass %s.", 
                    pass->GetName().c_str()
                );
            }

            return true;
        }

        void Adapter::BindPass(PassRef pass)
        {
            // Both should be loaded and valid (if they exist and prepare was called)
            CGprogram vertProg = pass->GetProgram(PS_Vertex);
            CGprogram fragProg = pass->GetProgram(PS_Fragment);

            if ( cgIsProgram(vertProg) )
            {
                HRESULT hr = cgD3D9BindProgram(vertProg);

                if ( !SUCCEEDED(hr) )
                {
                    this->mCore->Log
                    (
                        LL_Error,
                        VOODOO_GEM_NAME,
                        "Error binding vertex program from '%s': %s.",
                        pass->GetName().c_str(), cgD3D9TranslateHRESULT(hr)
                    );

                    return;
                } else {
                    mBoundVP = vertProg;
                }
            } else {
                mDevice->SetVertexShader(NULL);
            }

            if ( cgIsProgram(fragProg) )
            {
                HRESULT hr = cgD3D9BindProgram(fragProg);

                if ( !SUCCEEDED(hr) )
                {
                    this->mCore->Log
                    (
                        LL_Error,
                        VOODOO_GEM_NAME,
                        "Error binding fragment program from '%s': %s.",
                        pass->GetName().c_str(), cgD3D9TranslateHRESULT(hr)
                    );

                    if ( cgIsProgram(vertProg) )
                    {
                        cgD3D9UnbindProgram(vertProg);
                        mBoundVP = NULL;
                    }
                    return;
                } else {
                    mBoundFP = fragProg;
                }
            } else {
                mDevice->SetPixelShader(NULL);
            }
        }

        void Adapter::UnbindPass()
        {
            if ( cgIsProgram(mBoundVP) )
            {
                cgD3D9UnbindProgram(mBoundVP);
            }

            if ( cgIsProgram(mBoundFP) )
            {
                cgD3D9UnbindProgram(mBoundFP);
            }

            mDevice->SetVertexShader(NULL);
            mDevice->SetPixelShader(NULL);
        }

        void Adapter::DrawShader(ShaderRef shader)
        {
            if ( !shader.get() )
            {
                this->mCore->Log
                (
                    LL_Error,
                    VOODOO_GEM_NAME,
                    "Attempting to draw null shader."
                );
            }
            // Set up textures and set scratch surface as render target
            IDirect3DSurface9 * rt = NULL;

            HRESULT hr = mDevice->GetRenderTarget(0, &rt);

            if ( FAILED(hr) )
            {
                mCore->Log
                (
                    LL_Error,
                    VOODOO_GEM_NAME,
                    "Failed to retrieve render target for shader %s.", 
                    shader->GetName().c_str()
                );

                return;
            }
            
            hr = mDevice->SetRenderTarget(0, gScratch.RawSurface);
            if ( FAILED(hr) )
            {
                mCore->Log
                (
                    LL_Error,
                    VOODOO_GEM_NAME,
                    "Failed to bind render target for shader %s.", 
                    shader->GetName().c_str()
                );

                return;
            }

            // Get technique
            TechniqueRef tech = shader->GetDefaultTechnique();

            size_t passCount = tech->GetPassCount();

            for ( size_t curPass = 0; curPass < passCount; ++curPass )
            {
                PassRef pass = tech->GetPass(curPass);

                this->BindPass(pass);
                
                this->DrawQuad(NULL);

                this->UnbindPass();

                /*
                TextureRef passTarget = pass->GetTarget();
                IDirect3DTexture9 * passTargetD3D = (IDirect3DTexture9 *)passTarget->GetData();
                IDirect3DSurface9 * passSurface = NULL;

                HRESULT hr = passTargetD3D->GetSurfaceLevel(0, &passSurface);
                if ( FAILED(hr) || !passSurface )
                {    
                    mCore->Log("Failed to get target surface for pass %s (targeting texture %s).",
                        pass->Name().c_str(), passTarget->Name().c_str());

                    hr = mDevice->StretchRect(scratchSurface, NULL, passSurface, NULL, D3DTEXF_NONE);
                    if ( FAILED(hr) )
                    {
                        mCore->Log("Failed to copy results to target for pass %s.",
                            pass->Name().c_str());
                    }
                } 
                */
                
                hr = mDevice->StretchRect(gScratch.RawSurface, NULL, gBackbuffer.RawSurface, NULL, D3DTEXF_NONE);

                if ( FAILED(hr) )
                {
                    mCore->Log
                    (
                        LL_Error,
                        VOODOO_GEM_NAME,
                        "Failed to copy results to target for pass %s (result %d).",
                        pass->GetName().c_str(), hr
                    );
                }
            }

            /*
            TextureRef techTarget = tech->GetTarget();
            IDirect3DTexture9 * techTargetD3D = (IDirect3DTexture9 *)techTarget->GetData();
            IDirect3DSurface9 * techSurface = NULL;

            HRESULT hr = techTargetD3D->GetSurfaceLevel(0, &techSurface);
            if ( FAILED(hr) || !techSurface )
            {
                mCore->Log("Failed to get target surface for technique %s (targeting texture %s).",
                    tech->Name().c_str(), techTarget->Name().c_str());
            } else {
                hr = mDevice->StretchRect(scratchSurface, NULL, techSurface, NULL, D3DTEXF_NONE);
                if ( FAILED(hr) )
                {
                    mCore->Log("Failed to copy results to target for technique %s.",
                        tech->Name().c_str());
                }
            }
            */

            mDevice->SetRenderTarget(0, rt);
        }

        void Adapter::DrawQuad(Vertex * vertexData)
        {
            IDirect3DVertexBuffer9 * sourceBuffer;
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

            if ( !vertexData )
            {
                HRESULT hr = this->mDevice->BeginScene();
                if ( SUCCEEDED(hr) )
                {
                    this->mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); 
                    this->mDevice->EndScene();
                } else {
                    mCore->Log(LL_Error, VOODOO_GEM_NAME, "Failed to draw quad.");
                }
            } else {
                // Draw a quad from user vertexes
                HRESULT hr = this->mDevice->BeginScene();
                if ( SUCCEEDED(hr) )
                {
                    hr = this->mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexData, sizeof(StandardQuadVert));
                    if ( !SUCCEEDED(hr) )
                    {
                        this->mCore->Log(LL_Error, VOODOO_GEM_NAME, "Error drawing user quad.");
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

        void Adapter::ApplyParameter(ParameterRef param)
        {
            mCore->Log
            (
                LL_Debug,
                VOODOO_GEM_NAME,
                "Applying parameter %s.",
                param->GetName().c_str()
            );

            //! @todo Find out if cgSetParameter or cgD3DSetUniform is better/faster and
            //!        why cgD3DSetUniform returns errors on valid parameters.
            CGparameter cgparam = param->GetCgParameter();

            switch ( param->GetType() )
            {
            case PT_Float1:
                cgSetParameter1fv(cgparam, param->GetFloat());
                break;
            case PT_Float2:
                cgSetParameter2fv(cgparam, param->GetFloat());
                break;
            case PT_Float3:
                cgSetParameter3fv(cgparam, param->GetFloat());
                break;
            case PT_Float4:
                cgSetParameter4fv(cgparam, param->GetFloat());
                break;
            case PT_Matrix:
                cgSetMatrixParameterfc(cgparam, param->GetFloat());
                break;
            case PT_Sampler1D:
            case PT_Sampler2D:
            case PT_Sampler3D:
                cgD3D9SetTextureParameter(cgparam, param->GetTexture()->GetData<IDirect3DTexture9>());
                break;
            case PT_Unknown:
            default:
                this->mCore->Log
                (
                    LL_Error,
                    VOODOO_GEM_NAME,
                    "Unable to bind parameters %s (unknown type).", 
                    param->GetName().c_str()
                );

                break;
            }
        }

        bool Adapter::ConnectTexture(ParameterRef param, TextureRef texture)
        {
            if ( Converter::ToParameterCategory(param->GetType()) == PC_Sampler )
            {
                param->Set(texture);

                IDirect3DTexture9 * texObj = texture->GetData<IDirect3DTexture9>();
                CGparameter texParam = param->GetCgParameter();
                cgD3D9SetTextureParameter(texParam, texObj);
                mCore->Log
                (
                    LL_Debug,
                    VOODOO_GEM_NAME,
                    "Bound texture %s to parameter %s.", 
                    texture->GetName().c_str(), param->GetName().c_str()
                );

                return true;
            } else {
                Throw(VOODOO_GEM_NAME, "Invalid binding attempt, parameter is not a sampler.", this->mCore);
            }
        }

        TextureRef Adapter::CreateTexture(String name, TextureDesc desc)
        {
            IDirect3DTexture9 * tex = NULL;
            D3DFORMAT fmt = Gem_Converter::ToD3DFormat(desc.Format);

            HRESULT hr = mDevice->CreateTexture
            (
                desc.Width, desc.Height, desc.Depth, 
                D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex, NULL
            );

            if ( SUCCEEDED(hr) )
            {
                TextureRef texRef = mCore->AddTexture(name, reinterpret_cast<void*>(tex));
                return texRef;
            } else {
                const char * error = cgD3D9TranslateHRESULT(hr);
                mCore->Log
                (
                    LL_Error, 
                    VOODOO_GEM_NAME, 
                    "Error creating texture %s: %s", 
                    name.c_str(), error
                );

                return TextureRef();
            }
        }

        void Adapter::HandleError(CGcontext context, CGerror error, void * core)
        {
            UNREFERENCED_PARAMETER(context);

            Core * actualCore = reinterpret_cast<Core*>(core);
            actualCore->Log(LL_Error, VOODOO_GEM_NAME, "Cg error: %s", cgD3D9TranslateCGerror(error));
        }
    }
}
