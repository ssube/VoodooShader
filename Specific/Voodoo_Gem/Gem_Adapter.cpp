
#include "Gem_Adapter.hpp"
#include "Gem_Converter.hpp"

#include "Gem_Version.hpp"

IDirect3DSurface9 * scratchSurface = NULL;
IDirect3DTexture9 * scratchTexture = NULL;

namespace VoodooShader
{
	namespace Gem
	{
		LPDIRECT3DVERTEXBUFFER9 FSQuadVerts = NULL;

		Adapter::Adapter(Core * core, IDirect3DDevice9 * device)
			: mCore(core), mDevice(device)
		{
			assert(device);

			device->AddRef(); // Make sure it won't be released while we're using it

			if ( !core )
			{
				core = VoodooShader::Core::Create("Voodoo_Gem.log");
			}

			core->Log("Voodoo Gem: Starting adapter...\n");

			try
			{
				core->SetAdapter(reinterpret_cast<VoodooShader::Adapter*>(this));
				core->Log("Voodoo Gem: Core adapter set to this.\n");
			} catch ( VoodooShader::Exception & exc ) {
				core->Log("Voodoo Gem: Error setting adapter on core: %s.\n",
					exc.Message());
				//exit(1);
			}

			core->Log("Voodoo Gem: Assembly ID: "VOODOO_META_STRING_VERSION_FULL(GEM)"\n");

			// Core version check
			Version coreVersion = core->GetVersion();

			if ( coreVersion.Rev != VOODOO_GLOBAL_VERSION_GITREV )
			{
				core->Log("Voodoo Gem: Warning: The core module appears to be from a different revision.\n");
			}

			HRESULT hr = cgD3D9SetDevice(device);
			if ( !SUCCEEDED(hr) )
			{
				Throw("Voodoo Gem: Could not set Cg device.", core);
			} else {
				core->Log("Voodoo Gem: Set Cg device.\n");
			}

#ifdef _DEBUG
			cgD3D9EnableDebugTracing(CG_TRUE);
#endif

			cgD3D9SetManageTextureParameters(core->GetCGContext(), CG_TRUE);

			cgD3D9RegisterStates(core->GetCGContext());

			HRESULT errors = cgD3D9GetLastError();
			if ( !SUCCEEDED(errors) )
			{
				core->Log("Voodoo Gem: Errors setting Cg states.\n");
			} else {
				core->Log("Voodoo Gem: Cg states set successfully.\n");
			}

			// Setup profiles
			CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
			CGprofile bestVert = cgD3D9GetLatestVertexProfile();

			core->Log("Voodoo Gem: The following profiles have been detected as the latest available:\n\tVertex: %s\n\tFragment: %s\n",
				cgGetProfileString(bestVert), cgGetProfileString(bestFrag));

			// Get params
			D3DVIEWPORT9 viewport;
			device->GetViewport(&viewport);

			float fx = ( (float)viewport.Width   ) + 0.5f	;/// 2;
			float fy = ( (float)viewport.Height  ) + 0.5f	;/// 2;

			mCore->Log("Voodoo Gem: Prepping for %f by %f target.\n",
				fx, fy);

			hr = this->mDevice->CreateVertexBuffer(6 * sizeof(FSVert), 0, D3DFVF_CUSTOMVERTEX,
				D3DPOOL_DEFAULT, &FSQuadVerts, NULL);

			FSQuadVerts->AddRef();

			if ( FAILED(hr) )
			{
				mCore->Log("Voodoo Gem: Failed to create vertex buffer.\n");
			}

			FSVert g_Vertices[4];
			memset(g_Vertices, 0, sizeof(FSVert) * 4);

			g_Vertices[0].x = -0.5f; g_Vertices[0].y = -0.5f; g_Vertices[0].z = 0.5f;
			g_Vertices[1].x =    fx; g_Vertices[1].y = -0.5f; g_Vertices[1].z = 0.5f;
			g_Vertices[2].x = -0.5f; g_Vertices[2].y =    fy; g_Vertices[2].z = 0.5f;
			g_Vertices[3].x =    fx; g_Vertices[3].y =    fy; g_Vertices[3].z = 0.5f;

			g_Vertices[0].rhw = g_Vertices[1].rhw = g_Vertices[2].rhw = g_Vertices[3].rhw = 1.0f;

			g_Vertices[0].tu = 0.0f; g_Vertices[0].tv = 0.0f;
			g_Vertices[1].tu = 1.0f; g_Vertices[1].tv = 0.0f;
			g_Vertices[2].tu = 0.0f; g_Vertices[2].tv = 1.0f;
			g_Vertices[3].tu = 1.0f; g_Vertices[3].tv = 1.0f;

			void * pVertices;
			FSQuadVerts->Lock(0, sizeof(FSVert) * 4, &pVertices, 0);

			memcpy(pVertices, g_Vertices, sizeof(FSVert) * 4);

			FSQuadVerts->Unlock();
		}

		Adapter::~Adapter()
		{
			// Clean up any active shaders
			if ( mBoundFP )
			{
				cgD3D9UnbindProgram(mBoundFP);
			}

			if ( mBoundVP )
			{
				cgD3D9UnbindProgram(mBoundVP);
			}

			cgD3D9UnloadAllPrograms();

			FSQuadVerts->Release();

			mDevice->Release();

			mCore->SetAdapter(NULL);
		}

		bool Adapter::LoadPass(Pass * pass)
		{
			assert(pass);

			CGprogram vertProg = pass->GetProgram(PS_Vertex);
			CGprogram fragProg = pass->GetProgram(PS_Fragment);

			this->mCore->Debug("Voodoo Gem: Loading pass %s\n", pass->Name().c_str());

			HRESULT hr = S_OK;

			if ( cgIsProgram(vertProg) )
			{
				hr = cgD3D9LoadProgram(vertProg, CG_TRUE, 0);
				if ( !SUCCEEDED(hr) )
				{
					this->mCore->Log("Voodoo Gem: Error loading vertex program from '%s': %s.\n",
						pass->Name().c_str(), cgD3D9TranslateHRESULT(hr));
					return false;
				}
			}

			if ( cgIsProgram(fragProg) )
			{
				hr = cgD3D9LoadProgram(fragProg, CG_TRUE, 0);
				if ( !SUCCEEDED(hr) )
				{
					this->mCore->Log("Voodoo Gem: Error loading fragment program from '%s': %s.\n",
						pass->Name().c_str(), cgD3D9TranslateHRESULT(hr));
					return false;
				}
			}

			this->mCore->Log("Voodoo Gem: Successfully loaded programs from '%s'.\n",
				pass->Name().c_str());
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
					this->mCore->Log("Voodoo Gem: Error binding vertex program from '%s': %s.\n",
						pass->Name().c_str(), cgD3D9TranslateHRESULT(hr));
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
					this->mCore->Log("Voodoo Gem: Error binding fragment program from '%s': %s.\n",
						pass->Name().c_str(), cgD3D9TranslateHRESULT(hr));

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
			// Set up textures and set scratch surface as render target
			IDirect3DSurface9 * rt = NULL;

			HRESULT hr = mDevice->GetRenderTarget(0, &rt);
			if ( FAILED(hr) )
			{
				mCore->Log("Voodoo Gem: Failed to retrieve render target for shader %s.\n", shader->Name().c_str());
				return;
			}
			
			hr = mDevice->SetRenderTarget(0, scratchSurface);
			if ( FAILED(hr) )
			{
				mCore->Log("Voodoo Gem: Failed to bind render target for shader %s.\n", shader->Name().c_str());
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
				IDirect3DTexture9 * passTargetD3D = (IDirect3DTexture9 *)passTarget->GetTexture();
				IDirect3DSurface9 * passSurface = NULL;

				HRESULT hr = passTargetD3D->GetSurfaceLevel(0, &passSurface);
				if ( FAILED(hr) || !passSurface )
				{	
					mCore->Log("Voodoo Gem: Failed to get target surface for pass %s (targeting texture %s).\n",
						pass->Name().c_str(), passTarget->Name().c_str());

					hr = mDevice->StretchRect(scratchSurface, NULL, passSurface, NULL, D3DTEXF_NONE);
					if ( FAILED(hr) )
					{
						mCore->Log("Voodoo Gem: Failed to copy results to target for pass %s.\n",
							pass->Name().c_str());
					}
				} 
				*/
				
				hr = mDevice->StretchRect(scratchSurface, NULL, backbufferSurf, NULL, D3DTEXF_NONE);

				if ( FAILED(hr) )
				{
					mCore->Log("Voodoo Gem: Failed to copy results to target for pass %s (result %d).\n",
						pass->Name().c_str(), hr);
				}
			}

			/*
			TextureRef techTarget = tech->GetTarget();
			IDirect3DTexture9 * techTargetD3D = (IDirect3DTexture9 *)techTarget->GetTexture();
			IDirect3DSurface9 * techSurface = NULL;

			HRESULT hr = techTargetD3D->GetSurfaceLevel(0, &techSurface);
			if ( FAILED(hr) || !techSurface )
			{
				mCore->Log("Voodoo Gem: Failed to get target surface for technique %s (targeting texture %s).\n",
					tech->Name().c_str(), techTarget->Name().c_str());
			} else {
				hr = mDevice->StretchRect(scratchSurface, NULL, techSurface, NULL, D3DTEXF_NONE);
				if ( FAILED(hr) )
				{
					mCore->Log("Voodoo Gem: Failed to copy results to target for technique %s.\n",
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

			this->mDevice->SetStreamSource(0, FSQuadVerts, 0, sizeof(FSVert));
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
					mCore->Log("Voodoo Gem: Failed to draw quad.\n");
				}
			} else {
				// Draw a quad from user vertexes
				HRESULT hr = this->mDevice->BeginScene();
				if ( SUCCEEDED(hr) )
				{
					hr = this->mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexData, sizeof(FSVert));
					if ( !SUCCEEDED(hr) )
					{
						this->mCore->Log("Voodoo Gem: Error drawing user quad.");
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
			switch ( Converter::ToParameterCategory(param->GetType()) )
			{
			case PC_Float:
				cgD3D9SetUniform(param->GetParameter(), param->GetFloat());
				break;
			case PC_Sampler:
				cgD3D9SetTextureParameter(param->GetParameter(), (IDirect3DTexture9 *)param->GetTexture()->GetTexture());
				break;
			case PC_Unknown:
			default:
				this->mCore->Log("Voodoo Gem: Unable to bind parameter %s of unknown type.", param->Name().c_str());
			}
		}

		bool Adapter::ConnectTexture(ParameterRef param, TextureRef texture)
		{
			if ( Converter::ToParameterCategory(param->GetType()) == PC_Sampler )
			{
				param->Set(texture);

				IDirect3DTexture9 * texObj = (IDirect3DTexture9 *)texture->GetTexture();
				CGparameter texParam = param->GetParameter();
				cgD3D9SetTextureParameter(texParam, texObj);
				mCore->Log("Voodoo Gem: Bound texture %s to parameter %s.\n", texture->Name().c_str(), param->Name().c_str());
				return true;
			} else {
				Throw("Voodoo Gem: Invalid binding attempt, parameter is not a sampler.\n", this->mCore);
				return false;
			}
		}

		TextureRef Adapter::CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
			bool mipmaps, TextureFormat format)
		{
			IDirect3DTexture9 * tex = NULL;
			D3DFORMAT fmt = Gem_Converter::ToD3DFormat(format);

			HRESULT hr = mDevice->CreateTexture(width, height, depth, 
				D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex, NULL);
			if ( SUCCEEDED(hr) )
			{
				TextureRef texRef = mCore->CreateTexture(name, reinterpret_cast<void*>(tex));
				return texRef;
			} else {
				const char * error = cgD3D9TranslateHRESULT(hr);
				mCore->Log("Voodoo Gem: Error creating texture %s: %s\n", name.c_str(), error);
				return TextureRef();
			}
		}

		void Adapter::HandleError(CGcontext context, CGerror error, void * core)
		{
			Core * actualCore = reinterpret_cast<Core*>(core);
			actualCore->Log("Voodoo Gem: Cg error: %s\n", cgD3D9TranslateCGerror(error));
		}
	}
}