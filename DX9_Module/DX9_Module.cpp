
#include "DX9_Module.hpp"

namespace VoodooShader
{
	namespace DirectX9
	{
		Adapter::Adapter(Core * core, IDirect3DDevice9 * device)
			: mCore(core), mDevice(device)
		{
			HRESULT hr = cgD3D9SetDevice(device);
			if ( !SUCCEEDED(hr) )
			{
				Throw("Voodoo DX9: Could not set Cg device.", core);
			}

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
				core->GetLog()->Log("Voodoo DX9: Error setting adapter on core.\n");
				//exit(1);
			}

			cgD3D9EnableDebugTracing(CG_TRUE);

			this->mDevice->CreateVertexBuffer(4 * sizeof(FSVert), 0, D3DFVF_CUSTOMVERTEX,
				D3DPOOL_DEFAULT, &FSQuadVerts, NULL);

			FSVert g_Vertices[4];
			g_Vertices[0].x = g_Vertices[0].y = -1.0f;
			g_Vertices[2].x = g_Vertices[2].y = 0.5f;
			g_Vertices[1].x = g_Vertices[3].y = -1.0f;
			g_Vertices[3].x = g_Vertices[1].y = 0.5f;

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

		void Adapter::Bind(PassRef pass)
		{
			//! @todo Bind the shader here, need to apply params and set up textures
			CGprogram vertProg = pass->VertexProgram();
			CGprogram fragProg = pass->FragmentProgram();

			HRESULT hr = cgD3D9LoadProgram(vertProg, CG_TRUE, 0);
			if ( !SUCCEEDED(hr) )
			{
				this->mCore->GetLog()->Format("Voodoo DX9: Error loading vertex program from '%s': %s")
					.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
			}
			hr = cgD3D9LoadProgram(fragProg, CG_TRUE, 0);
			if ( !SUCCEEDED(hr) )
			{
				this->mCore->GetLog()->Format("Voodoo DX9: Error loading fragment program from '%s': %s")
					.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
			}
		}

		void Adapter::Unbind()
		{

		}

		void Adapter::DrawFSQuad()
		{
			IDirect3DVertexBuffer9 * sourceBuffer;
			UINT sourceOffset, sourceStride;
			DWORD sourceFVF;

			this->mDevice->GetStreamSource(0, &sourceBuffer, &sourceOffset, &sourceStride);
			this->mDevice->GetFVF(&sourceFVF);

			this->mDevice->SetStreamSource(0, FSQuadVerts, 0, sizeof(FSVert));
			this->mDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

			this->mDevice->BeginScene();
			this->mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); 
			this->mDevice->EndScene();
			
			this->mDevice->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
			this->mDevice->SetFVF(sourceFVF);
		}
		void Adapter::Apply(ParameterRef param)
		{
			switch ( param->Type() )
			{
			case PT_Float4:
			case PT_Float3:
			case PT_Float2:
			case PT_Float1:
				//cgD3D9SetUniform(param->mParam, param->mValueFloat);
				break;
			case PT_Sampler3D:
			case PT_Sampler2D:
			case PT_Sampler1D:
				//cgD3D9SetTextureParameter(param->mParam, param->mValueTexture->mTextureObject);
				break;
			}
		}

		TextureRef Adapter::CreateTexture(size_t width, size_t height, size_t depth, bool mipmaps,
			TextureFormat format)
		{
			IDirect3DTexture9 * tex = NULL;
			HRESULT hr = mDevice->CreateTexture(width, height, depth, 
				D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex, NULL);
			if ( SUCCEEDED(hr) )
			{
				TextureRef texRef = mCore->CreateTexture("texA", tex);
				return texRef;
			} else {
				mCore->GetLog()->Format("Voodoo DX9: Error creating texture: %i").With(hr).Done();
				return TextureRef();
			}
		}

		void Adapter::HandleError(CGcontext context, CGerror error, void * core)
		{
			Core * actualCore = reinterpret_cast<Core*>(core);
			actualCore->GetLog()->Format("Voodoo DX9: Cg error: %s\n").With(cgD3D9TranslateCGerror(error)).Done();
		}
	}
}