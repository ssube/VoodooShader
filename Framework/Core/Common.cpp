
#include "stdafx.h"

#include "Common.hpp"

// 0b1nnnnnnn is a library-level code. 0b0nnnnnnn is interface-level. This works around
// the facility restriction.
const HRESULT VSF_OK                  MAKE_VSF_LOK(0x0000);
const HRESULT VSF_FAIL                MAKE_VSF_LERR(0x0000);

const HRESULT VSFOK_NOT_FOUND         MAKE_VSF_LOK(0x0006);
const HRESULT VSFOK_NULL_IMPL         MAKE_VSF_LERR(0x0003);

const HRESULT VSFERR_BAD_CLSID        MAKE_VSF_LERR(0x0001);
const HRESULT VSFERR_DUP_NAME         MAKE_VSF_LERR(0x0002);
const HRESULT VSFERR_LINKER_ERROR     MAKE_VSF_LERR(0x0004);

const HRESULT VSFERR_NOT_LINKED       MAKE_VSF_LERR(0x0005);
const HRESULT VSFERR_NOT_FOUND        MAKE_VSF_LERR(0x0006);

const HRESULT VSFERR_INVALID_CFG      MAKE_VSF_LERR(0x0007);
const HRESULT VSFERR_INVALID_CG       MAKE_VSF_LERR(0x0008);
const HRESULT VSFERR_INVALID_TECH     MAKE_VSF_LERR(0x0009);
const HRESULT VSFERR_INVALID_ARG      MAKE_VSF_LERR(0x0010);

const HRESULT VSFERR_NO_CORE          MAKE_VSF_LERR(0x0020);
const HRESULT VSFERR_NO_PARSER        MAKE_VSF_LERR(0x0021);
const HRESULT VSFERR_NO_SHADER        MAKE_VSF_LERR(0x0022);
const HRESULT VSFERR_NO_TECHNIQUE     MAKE_VSF_LERR(0x0023);
const HRESULT VSFERR_NO_PASS          MAKE_VSF_LERR(0x0024);
const HRESULT VSFERR_NO_TEXTURE       MAKE_VSF_LERR(0x0025);
const HRESULT VSFERR_NO_PARAMETER     MAKE_VSF_LERR(0x0026);
const HRESULT VSFERR_NO_ADAPTER       MAKE_VSF_LERR(0x0027);
const HRESULT VSFERR_NO_HOOKSYSTEM    MAKE_VSF_LERR(0x0028);
const HRESULT VSFERR_NO_LOGGER        MAKE_VSF_LERR(0x0029);
const HRESULT VSFERR_NO_FILESYSTEM    MAKE_VSF_LERR(0x002A);
const HRESULT VSFERR_NO_FILE          MAKE_VSF_LERR(0x002B);
const HRESULT VSFERR_NO_IMAGE         MAKE_VSF_LERR(0x002C);
const HRESULT VSFERR_NO_PROGRAM       MAKE_VSF_LERR(0x002D);

// IVoodooCore
const HRESULT VSFERR_NOT_INIT         MAKE_VSF_IERR(0x0001);

// IVoodooLogger
const HRESULT VSFERR_FILE_ERROR       MAKE_VSF_IERR(0x0001);

// IVoodooParser
const HRESULT VSFERR_IS_SYSVAR        MAKE_VSF_IERR(0x0001);

// Creates an interface to a string-format class ID. The ID may be in registry form or a ProgID.
HRESULT WINAPI InstanceFromString(_In_ BSTR lpStr, _In_ REFIID iid, _In_ void ** pp)
{
    // Error check
    if ( lpStr == NULL || pp == NULL )
    {
        return E_INVALIDARG;
    }

    CLSID clsid;
    // Check string type
    if ( lpStr[0] == '{' )
    {
        // Registry-format string.
        HRESULT getHR = CLSIDFromString(lpStr, &clsid);
        if ( FAILED(getHR) )
        {
            return getHR;
        }
    } else {
        // Assume ProgID
        HRESULT getHR = CLSIDFromProgID(lpStr, &clsid);
        if ( FAILED(getHR) )
        {
            return getHR;
        }
    }

    return CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, iid, pp);
}

CGparameter WINAPI CreateVirtualParameter(IVoodooCore * pCore, ParameterType Type)
{
    if ( pCore == NULL ) return NULL;

    CGtype cgtype = ToCgType(Type);

    CGcontext cgcontext = NULL;
    VARIANT vc;
    pCore->get_CgContext(&vc);
    cgcontext = (CGcontext)V_BYREF(&vc);

    return cgCreateParameter(cgcontext, cgtype);
}

CGtype WINAPI ToCgType(ParameterType Type)
{
    switch ( Type )
    {
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
    case PT_Sampler1D:
        return CG_SAMPLER1D;
    case PT_Sampler2D:
        return CG_SAMPLER2D;
    case PT_Sampler3D:
        return CG_SAMPLER3D;
    case PT_Struct:
        return CG_STRUCT;
    case PT_Unknown:
    default:
        return CG_UNKNOWN_TYPE;
    }
}

ParameterType WINAPI ToParameterType(CGtype Type)
{
    switch ( Type )
    {
    case CG_FLOAT1x1:
        return PT_Float1;
    case CG_FLOAT1x2:
        return PT_Float2;
    case CG_FLOAT1x3:
        return PT_Float3;
    case CG_FLOAT1x4:
        return PT_Float4;
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
    case CG_SAMPLER1D:
        return PT_Sampler1D;
    case CG_SAMPLER2D:
        return PT_Sampler2D;
    case CG_SAMPLER3D:
        return PT_Sampler3D;
    case CG_STRUCT:
        return PT_Struct;
    case CG_UNKNOWN_TYPE:
    default:
        return PT_Unknown;
    }
}

ParameterCategory WINAPI ToParameterCategory(ParameterType Type)
{
    switch ( Type )
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
    case PT_Struct:
        return PC_Struct;
    default:
        return PC_Unknown;
    }
}

TextureFormat WINAPI ToTextureFormat(BSTR pString)
{
    CStringW str(pString);

    if ( str.Left(3).CompareNoCase(L"TF_") != 0 )
    {
        return TF_Unknown;
    }

    if ( str[3] == 'D' )
    {
        // Depth format
        if ( str.CompareNoCase(L"TF_D16") == 0 )     return TF_D16;
        if ( str.CompareNoCase(L"TF_D32") == 0 )     return TF_D32;
    } else {
        if ( str.CompareNoCase(L"TF_RGB5") == 0 )    return TF_RGB5;
        if ( str.CompareNoCase(L"TF_RGB5A1") == 0 )  return TF_RGB5A1;
        if ( str.CompareNoCase(L"TF_RGB8") == 0 )    return TF_RGB8;
        if ( str.CompareNoCase(L"TF_RGBA8") == 0 )   return TF_RGBA8;
        if ( str.CompareNoCase(L"TF_RGB10A2") == 0 ) return TF_RGB10A2;
        if ( str.CompareNoCase(L"TF_RGBA16F") == 0 ) return TF_RGBA16F;
        if ( str.CompareNoCase(L"TF_RGBA32F") == 0 ) return TF_RGBA32F;
    }

    return TF_Unknown;
}