#ifndef VOODOO_GEM_ADAPTER_HPP
#define VOODOO_GEM_ADAPTER_HPP

#include "Gem_Module.hpp"

namespace VoodooShader
{
	namespace Gem
	{
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

		struct StandardQuadVert
		{
			FLOAT x, y, z, rhw;
			FLOAT tu, tv;
		};

		class VOODOO_API_GEM Adapter
			: VoodooShader::Adapter
		{

		public:
			Adapter(Core * core, IDirect3DDevice9 * device);
			~Adapter();

			bool LoadPass(Pass * pass);
			void BindPass(PassRef shader);
			void UnbindPass();

			void DrawQuad(Vertex * vertexData);

			void DrawShader(ShaderRef shader);

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
			
			LPDIRECT3DVERTEXBUFFER9 mQuadVerts;
		};
	};
}

#endif /*VOODOO_GEM_ADAPTER_HPP*/
