
#include "Meta.hpp"

namespace VoodooShader
{
	class Converter
	{
	public:
		static inline CGtype ToCGType(ParameterType pt)
		{
			switch ( pt )
			{
			case PT_Sampler1D:
				return CG_SAMPLER1D;
			case PT_Sampler2D:
				return CG_SAMPLER2D;
			case PT_Sampler3D:
				return CG_SAMPLER3D;
			case PT_Float1:
				return CG_FLOAT;
			case PT_Float2:
				return CG_FLOAT2;
			case PT_Float3:
				return CG_FLOAT3;
			case PT_Float4:
				return CG_FLOAT4;
			case PT_Unknown:
			default:
				return CG_UNKNOWN_TYPE;
			}
		}

		static inline ParameterType FromCGType(CGtype pt)
		{
			switch ( pt )
			{
			case CG_SAMPLER1D:
				return PT_Sampler1D;
			case CG_SAMPLER2D:
				return PT_Sampler2D;
			case CG_SAMPLER3D:
				return PT_Sampler3D;
			case CG_FLOAT:
				return PT_Float1;
			case CG_FLOAT2:
				return PT_Float2;
			case CG_FLOAT3:
				return PT_Float3;
			case CG_FLOAT4:
				return PT_Float4;
			case CG_UNKNOWN_TYPE:
			default:
				return PT_Unknown;
			}
		}
	};
}