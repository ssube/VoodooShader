
#include "DX9_Module.hpp"
#include "DX9_Converter.hpp"

namespace VoodooShader
{
	namespace DirectX9
	{
		Adapter::Adapter(Core * core, IDirect3DDevice9 * device)
			: mCore(core), mDevice(device)
		{
			if ( !core )
			{
				core = new VoodooShader::Core();
			}
			
			core->GetLog()->Log("Voodoo DX9: Starting adapter...\n");

			try
			{
				core->SetAdapter(reinterpret_cast<VoodooShader::Adapter*>(this));
				core->GetLog()->Log("Voodoo DX9: Core adapter set to this.\n");
			} catch ( VoodooShader::Exception & exc ) {
				core->GetLog()->Format("Voodoo DX9: Error setting adapter on core: %s.\n")
					.With(exc.Message()).Done();
				//exit(1);
			}

			HRESULT hr = cgD3D9SetDevice(device);
			if ( !SUCCEEDED(hr) )
			{
				Throw("Voodoo DX9: Could not set Cg device.", core);
			} else {
				core->GetLog()->Log("Voodoo DX9: Set Cg device.\n");
			}

//#ifdef _DEBUG
			cgD3D9EnableDebugTracing(CG_TRUE);
//#endif

			cgD3D9SetManageTextureParameters(core->GetCGContext(), CG_TRUE);

			cgD3D9RegisterStates(core->GetCGContext());

			HRESULT errors = cgD3D9GetLastError();
			if ( !SUCCEEDED(errors) )
			{
				core->GetLog()->Log("Voodoo DX9: Errors setting Cg states.\n");
			} else {
				core->GetLog()->Log("Voodoo DX9: Cg states set successfully.\n");
			}

			// Setup profiles
			CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
			CGprofile bestVert = cgD3D9GetLatestVertexProfile();

			core->GetLog()->Format("Voodoo DX9: Detected the following profiles:\n\tVertex: %s\n\tFragment: %s\n")
				.With(cgGetProfileString(bestVert)).With(cgGetProfileString(bestFrag)).Done();

			//cgD3D9Se

			// Get params
			D3DVIEWPORT9 viewport;
			device->GetViewport(&viewport);

			float fx = viewport.Width;
			float fy = viewport.Height;

			this->mDevice->CreateVertexBuffer(6 * sizeof(FSVert), 0, D3DFVF_CUSTOMVERTEX,
				D3DPOOL_DEFAULT, &FSQuadVerts, NULL);

			FSVert g_Vertices[6];
			g_Vertices[0].x = 0.0f; g_Vertices[0].y = 0.0f;
			g_Vertices[1].x = 0.0f; g_Vertices[1].y =   fy;
			g_Vertices[2].x =   fx; g_Vertices[2].y =   fy;
			g_Vertices[3].x =   fx; g_Vertices[3].y =   fy;
			g_Vertices[4].x =   fx; g_Vertices[4].y = 0.0f;
			g_Vertices[5].x = 0.0f; g_Vertices[5].y = 0.0f;

			g_Vertices[0].z = g_Vertices[1].z = g_Vertices[2].z =
			g_Vertices[3].z = g_Vertices[4].z = g_Vertices[5].z = 2.0f;

			g_Vertices[0].tu = 0.0f; g_Vertices[0].tv = 0.0f;
			g_Vertices[1].tu = 0.0f; g_Vertices[1].tv = 1.0f;
			g_Vertices[2].tu = 1.0f; g_Vertices[2].tv = 1.0f;
			g_Vertices[3].tu = 1.0f; g_Vertices[3].tv = 1.0f;
			g_Vertices[4].tu = 1.0f; g_Vertices[4].tv = 0.0f;
			g_Vertices[5].tu = 0.0f; g_Vertices[5].tv = 0.0f;

			//g_Vertices[0].color = g_Vertices[5].color = 0xFFFFFFFF; //0xFF00F8FF;
			//g_Vertices[2].color = g_Vertices[3].color = 0xFFFFFFFF; //0xFF8F00FF;
			//g_Vertices[4].color = g_Vertices[1].color = 0xFFFFFFFF; //0xFFCFCFFF;

			VOID * pVertices;
			FSQuadVerts->Lock(0, sizeof(g_Vertices), &pVertices, 0);
			memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
			FSQuadVerts->Unlock();
		}

		bool Adapter::LoadPass(Pass * pass)
		{
			CGprogram vertProg = pass->VertexProgram();
			CGprogram fragProg = pass->FragmentProgram();

			HRESULT hr = S_OK;

			if ( cgIsProgram(vertProg) )
			{
				hr = cgD3D9LoadProgram(vertProg, CG_TRUE, 0);
				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo DX9: Error loading vertex program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
					return false;
				}
			}

			if ( cgIsProgram(fragProg) )
			{
				hr = cgD3D9LoadProgram(fragProg, CG_TRUE, 0);
				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo DX9: Error loading fragment program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
					return false;
				}
			}

			this->mCore->GetLog()->Format("Voodoo DX9: Successfully loaded programs from '%s'.\n")
				.With(pass->Name()).Done();
			return true;
		}

		void Adapter::BindPass(Pass * pass)
		{
			// Both should be loaded and valid (if they exist and prepare was called)
			CGprogram vertProg = pass->VertexProgram();
			CGprogram fragProg = pass->FragmentProgram();

			HRESULT hr;

			if ( cgIsProgram(vertProg) )
			{
				hr = cgD3D9BindProgram(pass->VertexProgram());

				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo DX9: Error binding vertex program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
					return;
				} else {
					mBoundVP = vertProg;
				}
			}

			if ( cgIsProgram(fragProg) )
			{
				hr = cgD3D9BindProgram(pass->FragmentProgram());

				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo DX9: Error binding fragment program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();

					if ( cgIsProgram(vertProg) )
					{
						cgD3D9UnbindProgram(pass->VertexProgram());
						mBoundVP = NULL;
					}
					return;
				} else {
					mBoundFP = fragProg;
				}
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
		}

		void Adapter::DrawQuad(bool fullscreen, void * vertexData)
		{
			if ( fullscreen )
			{
				IDirect3DVertexBuffer9 * sourceBuffer;
				UINT sourceOffset, sourceStride;
				DWORD sourceFVF;

				this->mDevice->GetStreamSource(0, &sourceBuffer, &sourceOffset, &sourceStride);
				this->mDevice->GetFVF(&sourceFVF);

				this->mDevice->SetStreamSource(0, FSQuadVerts, 0, sizeof(FSVert));
				this->mDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

				HRESULT hr = this->mDevice->BeginScene();
				if ( SUCCEEDED(hr) )
				{
					this->mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2); 
					this->mDevice->EndScene();
				}

				this->mDevice->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
				this->mDevice->SetFVF(sourceFVF);
			} else {
				if ( !vertexData )
				{
					Throw("Voodoo DX9: Draw Quad called with no vertices.", mCore);
				} else {
					// Draw a quad from user vertices
					HRESULT hr = this->mDevice->BeginScene();
					if ( SUCCEEDED(hr) )
					{
						hr = this->mDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertexData, sizeof(FSVert));
						if ( !SUCCEEDED(hr) )
						{
							this->mCore->GetLog()->Log("Voodoo DX9: Error drawing user quad.");
						}
						this->mDevice->EndScene();
					}
				}
			}
		}

		void Adapter::ApplyParameter(ParameterRef param)
		{
			switch ( Converter::ToParameterCategory(param->Type()) )
			{
			case PC_Float:
				cgD3D9SetUniform(param->Param(), param->GetFloat());
				break;
			case PC_Sampler:
				cgD3D9SetTextureParameter(param->Param(), (IDirect3DTexture9 *)param->GetTexture()->Get());
				break;
			case PC_Unknown:
			default:
				this->mCore->GetLog()->Format("Voodoo DX9: Unable to bind parameter %s of unknown type.")
					.With(param->Name()).Done();
			}
		}

		bool Adapter::ConnectTexture(ParameterRef param, TextureRef texture)
		{
			if ( Converter::ToParameterCategory(param->Type()) == PC_Sampler )
			{
				param->Set(texture);

				IDirect3DTexture9 * texObj = (IDirect3DTexture9 *)texture->Get();
				CGparameter texParam = param->Param();
				cgD3D9SetTextureParameter(texParam, texObj);
				mCore->GetLog()->Format("Voodoo DX9: Bound texture %s to parameter %s.\n")
					.With(texture->Name()).With(param->Name()).Done();
				return true;
			} else {
				Throw("Voodoo DX9: Invalid binding attempt, parameter is not a sampler.\n", this->mCore);
				return false;
			}
		}

		TextureRef Adapter::CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
			bool mipmaps, TextureFormat format)
		{
			IDirect3DTexture9 * tex = NULL;
			D3DFORMAT fmt = DX9_Converter::ToD3DFormat(format);

			HRESULT hr = mDevice->CreateTexture(width, height, depth, 
				D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex, NULL);
			if ( SUCCEEDED(hr) )
			{
				TextureRef texRef = mCore->CreateTexture(name, reinterpret_cast<void*>(tex));
				return texRef;
			} else {
				const char * error = cgD3D9TranslateHRESULT(hr);
				mCore->GetLog()->Format("Voodoo DX9: Error creating texture %s: %s\n")
					.With(name).With(error).Done();
				return TextureRef();
			}
		}

		void Adapter::HandleError(CGcontext context, CGerror error, void * core)
		{
			Core * actualCore = reinterpret_cast<Core*>(core);
			actualCore->GetLog()->Format("Voodoo DX9: Cg error: %s\n")
				.With(cgD3D9TranslateCGerror(error)).Done();
		}
	}
}
