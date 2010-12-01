
#include "DX9_Module.hpp"
#include "DX9_Converter.hpp"

namespace VoodooShader
{
	namespace DirectX9
	{
		VOODOO_API_DX9 Adapter::Adapter(Core * core, IDirect3DDevice9 * device)
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

#ifdef _DEBUG
			cgD3D9EnableDebugTracing(CG_TRUE);
#endif
			cgD3D9RegisterStates(core->GetCGContext());

			HRESULT errors = cgD3D9GetLastError();
			if ( !SUCCEEDED(errors) )
			{
				core->GetLog()->Log("Voodoo DX9: Errors setting Cg states.\n");
			} else {
				core->GetLog()->Log("Voodoo DX9: Cg states set succesfully.\n");
			}

			this->mDevice->CreateVertexBuffer(4 * sizeof(FSVert), 0, D3DFVF_CUSTOMVERTEX,
				D3DPOOL_DEFAULT, &FSQuadVerts, NULL);

			FSVert g_Vertices[4];
			g_Vertices[0].x = g_Vertices[0].y = 0.0f;
			g_Vertices[2].x = g_Vertices[2].y = 1.5f;
			g_Vertices[1].x = g_Vertices[3].y = 0.0f;
			g_Vertices[3].x = g_Vertices[1].y = 1.5f;

			g_Vertices[0].tu = g_Vertices[1].tu = 0.0f;
			g_Vertices[0].tv = g_Vertices[3].tv = 0.0f;
			g_Vertices[2].tu = g_Vertices[3].tu = 1.0f;
			g_Vertices[2].tv = g_Vertices[1].tv = 1.0f;

			g_Vertices[0].color = g_Vertices[2].color = 0xFF00F8FF;
			g_Vertices[1].color = g_Vertices[3].color = 0xFF8F00FF;

			VOID * pVertices;
			FSQuadVerts->Lock(0, sizeof(g_Vertices), &pVertices, 0);
			memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
			FSQuadVerts->Unlock();
		}

		VOODOO_API_DX9 bool Adapter::PreparePass(PassRef pass)
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

		VOODOO_API_DX9 void Adapter::BindPass(PassRef pass)
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
					}
					return;
				} else {
					mBoundFP = fragProg;
				}
			}
		}

		VOODOO_API_DX9 void Adapter::UnbindPass()
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

		VOODOO_API_DX9 void Adapter::DrawQuad(bool fullscreen, float * coords)
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
					this->mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); 
					this->mDevice->EndScene();
				}

				this->mDevice->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
				this->mDevice->SetFVF(sourceFVF);
			}
		}

		VOODOO_API_DX9 void Adapter::ApplyParameter(ParameterRef param)
		{
			switch ( param->Category() )
			{
			case PC_Float:
				cgD3D9SetUniform(param->Param(), param->GetFloat());
				break;
			case PC_Sampler:
				cgD3D9SetTextureParameter(param->Param(), (IDirect3DTexture9 *)param->GetTexture()->Get());
				break;
			}
		}

		VOODOO_API_DX9 bool Adapter::ConnectTexture(ParameterRef param, TextureRef texture)
		{
			if ( param->Category() == PC_Sampler )
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

		VOODOO_API_DX9 TextureRef Adapter::CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
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
				mCore->GetLog()->Format("Voodoo DX9: Error creating texture: %i.\n").With(hr).Done();
				return TextureRef();
			}
		}

		VOODOO_API_DX9 void Adapter::HandleError(CGcontext context, CGerror error, void * core)
		{
			Core * actualCore = reinterpret_cast<Core*>(core);
			actualCore->GetLog()->Format("Voodoo DX9: Cg error: %s\n")
				.With(cgD3D9TranslateCGerror(error)).Done();
		}
	}
}
