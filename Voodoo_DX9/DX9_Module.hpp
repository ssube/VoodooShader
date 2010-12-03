
#ifndef VOODOO_DX9_MODULE_HPP
#define VOODOO_DX9_MODULE_HPP

#define VOODOO_IMPORT
#include "../Framework/VoodooFramework.hpp"
#pragma comment(lib, "../Debug/VoodooFramework.lib")

#include "D3D9.h"
#include "Cg/cgD3D9.h"
#pragma comment(lib, "cgD3D9.lib")

#ifndef VOODOO_IMPORT_DX9
#	define VOODOO_API_DX9 __declspec(dllexport)
#else
#	define VOODOO_API_DX9 __declspec(dllimport)
#endif

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

		class VOODOO_API_DX9 Adapter
			: VoodooShader::Adapter
		{

		public:
			Adapter(Core * core, IDirect3DDevice9 * device);

			bool LoadPass(Pass * pass);
			void BindPass(Pass * shader);
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
		};
	};
}

#endif /*VOODOO_DX9_MODULE_HPP*/
