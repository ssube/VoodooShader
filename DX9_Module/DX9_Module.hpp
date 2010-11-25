
#include "../Framework/VoodooFramework.hpp"
#pragma comment(lib, "../Debug/VoodooFramework.lib")

#include "D3D9.h"
#include "Cg/cgD3D9.h"
#pragma comment(lib, "cgD3D9.lib")

namespace VoodooShader
{
	namespace DirectX9
	{
		struct FSVert
		{
			FLOAT x, y, z, rhw;
			DWORD color;
			FLOAT tu, tv;
		};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

		LPDIRECT3DVERTEXBUFFER9 FSQuadVerts;

		class VOODOO_API Adapter
			: VoodooShader::Adapter
		{

		public:
			Adapter(Core * core, IDirect3DDevice9 * device);

			void Bind(PassRef shader);
			void Unbind();
			void DrawFSQuad();
			void Apply(ParameterRef param);
			TextureRef CreateTexture(size_t width, size_t height, size_t depth, bool mipmaps,
				TextureFormat format);
			void HandleError(CGcontext context, CGerror error, void * core);


		private:
			Core * mCore;
			IDirect3DDevice9 * mDevice;
		};
	};
}