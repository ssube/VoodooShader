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
        case PT_Matrix:
            return CG_FLOAT4x4;
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
            return PT_Float1;
        case CG_FLOAT2:
            return PT_Float2;
        case CG_FLOAT3:
            return PT_Float3;
        case CG_FLOAT4:
            return PT_Float4;
        case CG_FLOAT4x4:
            return PT_Matrix;
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
        case PT_Float1:
        case PT_Float2:
        case PT_Float3:
        case PT_Float4:
            return PC_Float;
        case PT_Sampler1D:
        case PT_Sampler2D:
        case PT_Sampler3D:
            return PC_Sampler;
        case PT_Matrix:
            return PC_Matrix;
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
        if ( strncmp(format, "TF_", 3) != 0 )
        {
            return TF_Unknown;
        }

        const char * formatBody = format + 3;

        if ( formatBody[0] == 'D' )
        {
            // Depth format
            if ( strcmp(formatBody, "D16") == 0 ) return TF_D16;
            if ( strcmp(formatBody, "D32") == 0 ) return TF_D32;
        } else {
            if ( strcmp(formatBody, "RGB5"   ) == 0 ) return TF_RGB5;
            if ( strcmp(formatBody, "RGB5A1" ) == 0 ) return TF_RGB5A1;
            if ( strcmp(formatBody, "RGB8"   ) == 0 ) return TF_RGB8;
            if ( strcmp(formatBody, "RGBA8"  ) == 0 ) return TF_RGBA8;
            if ( strcmp(formatBody, "RGB10A2") == 0 ) return TF_RGB10A2;
            if ( strcmp(formatBody, "RGBA16F") == 0 ) return TF_RGBA16F;
            if ( strcmp(formatBody, "RGBA32F") == 0 ) return TF_RGBA32F;
        }

        return TF_Unknown;
    }

    const char * Converter::ToString
    (
        _In_ TextureFormat tf
    )
    {
        switch ( tf )
        {
        case TF_RGB5:
            return "TF_RGB5";
        case TF_RGB5A1:
            return "TF_RGB5A1";
        case TF_RGB8:
            return "TF_RGB8";
        case TF_RGBA8:
            return "TF_RGBA8";
        case TF_RGB10A2:
            return "TF_RGB10A2";
        case TF_RGBA16F:
            return "TF_RGBA16f";
        case TF_RGBA32F:
            return "TF_RGBA32f";
        case TF_D16:
            return "TF_D16";
        case TF_D32:
            return "TF_D32";
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
        case PT_Sampler1D:
            return "PT_Sampler1D";
        case PT_Sampler2D:
            return "PT_Sampler2D";
        case PT_Sampler3D:
            return "PT_Sampler3D";
        case PT_Float1:
            return "PT_Float1";
        case PT_Float2:
            return "PT_Float2";
        case PT_Float3:
            return "PT_Float3";
        case PT_Float4:
            return "PT_Float4";
        case PT_Matrix:
            return "PT_Matrix";
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
        case PC_Float:
            return "PC_Float";
        case PC_Sampler:
            return "PC_Sampler";
        case PC_Matrix:
            return "PC_Matrix";
        case PC_Unknown:
        default:
            return "PC_Unknown";
        }
    }}