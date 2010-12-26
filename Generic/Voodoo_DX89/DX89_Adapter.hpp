#ifndef VOODOO_DX89_ADAPTER_HPP
#define VOODOO_DX89_ADAPTER_HPP

#include "DX89_Module.hpp"

namespace VoodooShader
{
	namespace DirectX89
	{
		struct FSVert
		{
			FLOAT x, y, z, rhw;
			FLOAT tu, tv;
		};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

		extern LPDIRECT3DVERTEXBUFFER9 FSQuadVerts;

		class VOODOO_API_DX89 Adapter
			: VoodooShader::Adapter
		{

		public:
			Adapter(Core * core, IDirect3DDevice9 * device);

			bool LoadPass(Pass * pass);
			void BindPass(PassRef shader);
			void UnbindPass();

			void DrawQuad(bool fullscreen = true, void * vertexData = NULL);

			void ApplyParameter(ParameterRef param);

			bool ConnectTexture(ParameterRef param, TextureRef texture);
			TextureRef CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
				bool mipmaps, TextureFormat format);

			void HandleError(CGcontext context, CGerror error, void * core);

		private:
			Core * mCore;
			IDirect3DDevice9 * mDevice;

			CGprogram mBoundVP;
			CGprogram mBoundFP;

			IDirect3DSurface9 * mRenderTarget;
		};
	};
}

#endif /*VOODOO_DX9_ADAPTER_HPP*/
