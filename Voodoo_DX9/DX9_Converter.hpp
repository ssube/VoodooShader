
#ifndef VOODOO_DX9_CONVERTER_HPP
#define VOODOO_DX9_CONVERTER_HPP

#include "../Framework/Meta.hpp"

namespace VoodooShader
{
	namespace DirectX9
	{
		class DX9_Converter
		{
		public:
			static inline D3DFORMAT ToD3DFMT(TextureFormat tf)
			{
				switch ( tf )
				{
				case TF_RGB5:
					return D3DFMT_X1R5G5B5;
				case TF_RGB5A1:
					return D3DFMT_A1R5G5B5;
				case TF_RGB8:
					return D3DFMT_X8R8G8B8;
				case TF_RGBA8:
					return D3DFMT_A8R8G8B8;
				case TF_RGB10A2:
					return D3DFMT_A2B10G10R10;
				case TF_RGBA16F:
					return D3DFMT_A16B16G16R16F;
				case TF_RGBA32F:
					return D3DFMT_A32B32G32R32F;
				case TF_D16:
					return D3DFMT_D16;
				case TF_D32:
					return D3DFMT_D32;
				case TF_Unknown:
				default:
					return D3DFMT_UNKNOWN;
				}
			}

			static inline TextureFormat FromD3DFMT(D3DFORMAT tf)
			{
				switch ( tf )
				{
				case D3DFMT_X1R5G5B5:
					return TF_RGB5;
				case D3DFMT_A1R5G5B5:
					return TF_RGB5A1;
				case D3DFMT_X8R8G8B8:
					return TF_RGB8;
				case D3DFMT_A8R8G8B8:
					return TF_RGBA8;
				case D3DFMT_A2B10G10R10:
					return TF_RGB10A2;
				case D3DFMT_A16B16G16R16F:
					return TF_RGBA16F;
				case D3DFMT_A32B32G32R32F:
					return TF_RGBA32F;
				case D3DFMT_D16:
					return TF_D16;
				case D3DFMT_D32:
					return TF_D32;
				case D3DFMT_UNKNOWN:
				default:
					return TF_Unknown;
				}
			}
		};
	}
}

#endif /*VOODOO_DX9_CONVERTER_HPP*/