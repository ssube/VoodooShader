
#include "DX9_Module.hpp"
#include "DX9_Converter.hpp"

#include "IVoodoo3D9.hpp"
#include "IVoodoo3DDevice9.hpp"

VoodooShader::Core * VoodooCore = NULL;
VoodooShader::Adapter * VoodooDX9 = NULL;

IVoodoo3D9 * VoodooObject = NULL;
IVoodoo3DDevice9 * VoodooDevice = NULL;

IDirect3DSurface9 * backbufferSurf = NULL;

IDirect3DSurface9 * surface_ThisFrame = NULL;
VoodooShader::TextureRef texture_ThisFrame;

VoodooShader::ShaderRef testShader;

namespace VoodooShader
{
	namespace DirectX9
	{
		Adapter::Adapter(Core * core, IDirect3DDevice9 * device)
			: mCore(core), mDevice(device)
		{
			if ( !core )
			{
				core = VoodooShader::Core::Create();
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

			float fx = ( viewport.Width  / 2 ) + 0.5	;/// 2;
			float fy = ( viewport.Height / 2 ) + 0.5	;/// 2;

			mCore->GetLog()->Format("Voodoo DX9: Prepping for %d by %d target.\n")
				.With(fx).With(fy).Done();

			hr = this->mDevice->CreateVertexBuffer(6 * sizeof(FSVert), 0, D3DFVF_CUSTOMVERTEX,
				D3DPOOL_DEFAULT, &FSQuadVerts, NULL);

			if ( FAILED(hr) )
			{
				mCore->GetLog()->Log("Voodoo DX9: Failed to create vertex buffer.\n");
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

		void Adapter::BindPass(PassRef pass)
		{
			// Both should be loaded and valid (if they exist and prepare was called)
			CGprogram vertProg = pass->VertexProgram();
			CGprogram fragProg = pass->FragmentProgram();

			if ( cgIsProgram(vertProg) )
			{
				HRESULT hr = cgD3D9BindProgram(pass->VertexProgram());

				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo DX9: Error binding vertex program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
					return;
				} else {
					mBoundVP = vertProg;
				}
			} else {
				mDevice->SetVertexShader(NULL);
			}

			if ( cgIsProgram(fragProg) )
			{
				HRESULT hr = cgD3D9BindProgram(pass->FragmentProgram());

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

		void Adapter::DrawQuad(Vertex * vertexData)
		{
			if ( !vertexData )
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

				HRESULT hr = this->mDevice->BeginScene();
				if ( SUCCEEDED(hr) )
				{
					this->mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); 
					this->mDevice->EndScene();
				} else {
					mCore->GetLog()->Log("Voodoo DX9: Failed to draw quad.\n");
				}

				this->mDevice->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
				this->mDevice->SetFVF(sourceFVF);
				this->mDevice->SetRenderState(D3DRS_ZENABLE, zEnabled);
				this->mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, aEnabled);
				this->mDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
			} else {
				// Draw a quad from user vertexes
				HRESULT hr = this->mDevice->BeginScene();
				if ( SUCCEEDED(hr) )
				{
					hr = this->mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexData, sizeof(FSVert));
					if ( !SUCCEEDED(hr) )
					{
						this->mCore->GetLog()->Log("Voodoo DX9: Error drawing user quad.");
					}
					this->mDevice->EndScene();
				}
			}
		}

		void Adapter::DrawShader(ShaderRef shader)
		{

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

VOODOO_API_DX9 void * __stdcall Voodoo3DCreate9(UINT version)
{
	// Voodoo DX9 Init function
	VoodooCore = VoodooShader::Core::Create("Voodoo_DX9.log");

#ifdef _DEBUG
	VoodooCore->GetLog()->SetBufferSize(0);
#endif

	VoodooCore->GetLog()->Format("Voodoo DX9: Direct3DCreate9 called, SDK version: %d.\n").With(version).Done();

	//Load the real d3d8 dll and get device caps
	char Path[MAX_PATH];
	GetSystemDirectoryA (Path, MAX_PATH);
	strcat_s (Path, MAX_PATH, "\\d3d9.dll");

	HMODULE d3ddll = LoadLibraryA(Path);
	D3DFunc9 d3d9func = (D3DFunc9)GetProcAddress (d3ddll, "Direct3DCreate9");

	if (!d3d9func) 
	{
		VoodooCore->GetLog()->Log("Voodoo DX9: Could not find D3D9 create true func.\n");
		return 0;
	}

	// Call DX9 to create a real device with the latest version
	IDirect3D9 * object = (d3d9func)(D3D_SDK_VERSION);

	if ( !object )
	{
		VoodooCore->GetLog()->Log("Voodoo DX9: Direct3DCreate9 returned null.\n");
	}

	// Turn it into a FakeObject and return it.
	IVoodoo3D9 * vObj = new IVoodoo3D9(object);
	VoodooObject = vObj;
	return vObj;
}

// Visual Studio-specific linker directive, forces the function to be exported with and
// without decoration. The actual symbol is undecorated, but I'd rather allow exception
// handling than use extern "C".
//#pragma comment(linker, "/export:Direct3DCreate9=?Voodoo3DCreate9@@YGPAXI@Z")
