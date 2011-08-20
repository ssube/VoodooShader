#include "Converter.hpp"

namespace VoodooShader
{
    CGtype Converter::ToCGType
    (
        _In_ ParameterType pt
    )
    {
        switch ( pt )
        {
        // Vectors
        case PT_Float1x1:
            return CG_FLOAT;
        case PT_Float1x2:
            return CG_FLOAT2;
        case PT_Float1x3:
            return CG_FLOAT3;
        case PT_Float1x4:
            return CG_FLOAT4;
        case PT_Float2x1:
            return CG_FLOAT2x1;
        case PT_Float2x2:
            return CG_FLOAT2x2;
        case PT_Float2x3:
            return CG_FLOAT2x3;
        case PT_Float2x4:
            return CG_FLOAT2x4;
        case PT_Float3x1:
            return CG_FLOAT3x1;
        case PT_Float3x2:
            return CG_FLOAT3x2;
        case PT_Float3x3:
            return CG_FLOAT3x3;
        case PT_Float3x4:
            return CG_FLOAT3x4;
        case PT_Float4x1:
            return CG_FLOAT4x1;
        case PT_Float4x2:
            return CG_FLOAT4x2;
        case PT_Float4x3:
            return CG_FLOAT4x3;
        case PT_Float4x4:
            return CG_FLOAT4x4;
        // Samplers
        case PT_Sampler1D:
            return CG_SAMPLER1D;
        case PT_Sampler2D:
            return CG_SAMPLER2D;
        case PT_Sampler3D:
            return CG_SAMPLER3D;
        // Structs
        case PT_Struct:
            return CG_STRUCT;
        // Other
        case PT_Unknown:
        default:
            return CG_UNKNOWN_TYPE;
        }
    }

    ParameterType Converter::ToParameterType
    (
        _In_ CGtype pt
    )
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
        case CG_FLOAT1:
        case CG_FLOAT1x1:
            return PT_Float1x1;
        case CG_FLOAT2:
        case CG_FLOAT1x2:
            return PT_Float1x2;
        case CG_FLOAT3:
        case CG_FLOAT1x3:
            return PT_Float1x3;
        case CG_FLOAT4:
        case CG_FLOAT1x4:
            return PT_Float1x4;
        case CG_FLOAT2x1:
            return PT_Float2x1;
        case CG_FLOAT2x2:
            return PT_Float2x2;
        case CG_FLOAT2x3:
            return PT_Float2x3;
        case CG_FLOAT2x4:
            return PT_Float2x4;
        case CG_FLOAT3x1:
            return PT_Float3x1;
        case CG_FLOAT3x2:
            return PT_Float3x2;
        case CG_FLOAT3x3:
            return PT_Float3x3;
        case CG_FLOAT3x4:
            return PT_Float3x4;
        case CG_FLOAT4x1:
            return PT_Float4x1;
        case CG_FLOAT4x2:
            return PT_Float4x2;
        case CG_FLOAT4x3:
            return PT_Float4x3;
        case CG_FLOAT4x4:
            return PT_Float4x4;
        case CG_UNKNOWN_TYPE:
        default:
            return PT_Unknown;
        }
    }

    ParameterCategory Converter::ToParameterCategory
    (
        _In_ ParameterType type
    )
    {
        switch ( type )
        {
        case PT_Float1x1:
        case PT_Float1x2:
        case PT_Float1x3:
        case PT_Float1x4:
        case PT_Float2x1:
        case PT_Float2x2:
        case PT_Float2x3:
        case PT_Float2x4:
        case PT_Float3x1:
        case PT_Float3x2:
        case PT_Float3x3:
        case PT_Float3x4:
        case PT_Float4x1:
        case PT_Float4x2:
        case PT_Float4x3:
        case PT_Float4x4:
            return PC_Float;
        case PT_Sampler1D:
        case PT_Sampler2D:
        case PT_Sampler3D:
            return PC_Sampler;
        case PT_Unknown:
        default:
            return PC_Unknown;
        }
    }

    TextureFormat Converter::ToTextureFormat
    (
        _In_ const char * format
    )
    {
        if ( _strnicmp(format, "TF_", 3) != 0 )
        {
            return TF_Unknown;
        }

        const char * formatBody = format + 3;

        if ( formatBody[0] == 'D' )
        {
            // Depth format
            if ( _stricmp(formatBody, "D16") == 0 ) return TF_D16;
            if ( _stricmp(formatBody, "D32") == 0 ) return TF_D32;
        } else {
            if ( _stricmp(formatBody, "RGB5"   ) == 0 ) return TF_RGB5;
            if ( _stricmp(formatBody, "RGB5A1" ) == 0 ) return TF_RGB5A1;
            if ( _stricmp(formatBody, "RGB8"   ) == 0 ) return TF_RGB8;
            if ( _stricmp(formatBody, "RGBA8"  ) == 0 ) return TF_RGBA8;
            if ( _stricmp(formatBody, "RGB10A2") == 0 ) return TF_RGB10A2;
            if ( _stricmp(formatBody, "RGBA16F") == 0 ) return TF_RGBA16F;
            if ( _stricmp(formatBody, "RGBA32F") == 0 ) return TF_RGBA32F;
        }

        return TF_Unknown;
    }

    int Converter::ToComponents( _In_ ParameterType type)
    {
        switch ( type )
        {
        case PT_Float1x1:
            return 1;
        case PT_Float1x2:
        case PT_Float2x1:
            return 2;
        case PT_Float1x3:
        case PT_Float3x1:
            return 3;
        case PT_Float1x4:
        case PT_Float2x2:
        case PT_Float4x1:
            return 4;
        case PT_Float2x3:
        case PT_Float3x2:
            return 6;
        case PT_Float2x4:
        case PT_Float4x2:
            return 8;
        case PT_Float3x3:
            return 9;
        case PT_Float3x4:
        case PT_Float4x3:
            return 12;
        case PT_Float4x4:
            return 16;
        case PT_Sampler1D:
        case PT_Sampler2D:
        case PT_Sampler3D:
        case PT_Struct:
        case PT_Unknown:
        default:
            return -1;
        }
    }

#define CASESTRING(x) case x: return #x

    const char * Converter::ToString
    (
        _In_ TextureFormat tf
    )
    {
        switch ( tf )
        {
            CASESTRING(TF_RGB5);
            CASESTRING(TF_RGB5A1);
            CASESTRING(TF_RGB8);
            CASESTRING(TF_RGBA8);
            CASESTRING(TF_RGB10A2);
            CASESTRING(TF_RGBA16F);
            CASESTRING(TF_RGBA32F);
            CASESTRING(TF_D16);
            CASESTRING(TF_D32);
        case TF_Unknown:
        default:
            return "TF_Unknown";
        }
    }

    const char * Converter::ToString
    (
        _In_ ParameterType pt
    )
    {
        switch ( pt )
        {
            CASESTRING(PT_Sampler1D);
            CASESTRING(PT_Sampler2D);
            CASESTRING(PT_Sampler3D);
            CASESTRING(PT_Float1x1);
            CASESTRING(PT_Float1x2);
            CASESTRING(PT_Float1x3);
            CASESTRING(PT_Float1x4);
            CASESTRING(PT_Float2x1);
            CASESTRING(PT_Float2x2);
            CASESTRING(PT_Float2x3);
            CASESTRING(PT_Float2x4);
            CASESTRING(PT_Float3x1);
            CASESTRING(PT_Float3x2);
            CASESTRING(PT_Float3x3);
            CASESTRING(PT_Float3x4);
            CASESTRING(PT_Float4x1);
            CASESTRING(PT_Float4x2);
            CASESTRING(PT_Float4x3);
            CASESTRING(PT_Float4x4);
            CASESTRING(PT_Struct);
        case PT_Unknown:
        default:
            return "PT_Unknown";
        }
    }

    const char * Converter::ToString
    (
        _In_ ParameterCategory cat
    )
    {
        switch ( cat )
        {
            CASESTRING(PC_Float);
            CASESTRING(PC_Sampler);
            CASESTRING(PC_Struct);
        case PC_Unknown:
        default:
            return "PC_Unknown";
        }
    }

#undef CASESTRING
}
