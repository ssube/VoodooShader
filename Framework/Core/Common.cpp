
#include "stdafx.h"

#include "Common.hpp"

const HRESULT E_BADTHING     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0000);
const HRESULT E_INVALIDCFG   = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0100);
const HRESULT E_BADCLSID     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0200);
const HRESULT E_DUPNAME      = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0300);
const HRESULT E_NOT_FOUND    = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0400);
const HRESULT S_NOT_FOUND    = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0400);

// IVoodooLogger
const HRESULT E_FILEERROR    = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0001);

// IVoodooPass
const HRESULT E_NOPROGRAM    = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0001);

// IVoodooParser
const HRESULT E_ISSYSVAR     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0001);

const HRESULT E_NULLIMPL     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0xFFFF);

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
    case Float1x1:
        return CG_FLOAT;
    case Float1x2:
        return CG_FLOAT2;
    case Float1x3:
        return CG_FLOAT3;
    case Float1x4:
        return CG_FLOAT4;
    case Float2x1:
        return CG_FLOAT2x1;
    case Float2x2:
        return CG_FLOAT2x2;
    case Float2x3:
        return CG_FLOAT2x3;
    case Float2x4:
        return CG_FLOAT2x4;
    case Float3x1:
        return CG_FLOAT3x1;
    case Float3x2:
        return CG_FLOAT3x2;
    case Float3x3:
        return CG_FLOAT3x3;
    case Float3x4:
        return CG_FLOAT3x4;
    case Float4x1:
        return CG_FLOAT4x1;
    case Float4x2:
        return CG_FLOAT4x2;
    case Float4x3:
        return CG_FLOAT4x3;
    case Float4x4:
        return CG_FLOAT4x4;
    case Sampler1D:
        return CG_SAMPLER1D;
    case Sampler2D:
        return CG_SAMPLER2D;
    case Sampler3D:
        return CG_SAMPLER3D;
    case UserStruct:
        return CG_STRUCT;
    case UnknownType:
    default:
        return CG_UNKNOWN_TYPE;
    }
}

ParameterType WINAPI ToParameterType(CGtype Type)
{
    switch ( Type )
    {
    case CG_FLOAT1x1:
        return Float1;
    case CG_FLOAT1x2:
        return Float2;
    case CG_FLOAT1x3:
        return Float3;
    case CG_FLOAT1x4:
        return Float4;
    case CG_FLOAT2x1:
        return Float2x1;
    case CG_FLOAT2x2:
        return Float2x2;
    case CG_FLOAT2x3:
        return Float2x3;
    case CG_FLOAT2x4:
        return Float2x4;
    case CG_FLOAT3x1:
        return Float3x1;
    case CG_FLOAT3x2:
        return Float3x2;
    case CG_FLOAT3x3:
        return Float3x3;
    case CG_FLOAT3x4:
        return Float3x4;
    case CG_FLOAT4x1:
        return Float4x1;
    case CG_FLOAT4x2:
        return Float4x2;
    case CG_FLOAT4x3:
        return Float4x3;
    case CG_FLOAT4x4:
        return Float4x4;
    case CG_SAMPLER1D:
        return Sampler1D;
    case CG_SAMPLER2D:
        return Sampler2D;
    case CG_SAMPLER3D:
        return Sampler3D;
    case CG_STRUCT:
        return UserStruct;
    case CG_UNKNOWN_TYPE:
    default:
        return UnknownType;
    }
}

ParameterCategory WINAPI ToParameterCategory(ParameterType Type)
{
    switch ( Type )
    {
    case Float1x1:
    case Float1x2:
    case Float1x3:
    case Float1x4:
    case Float2x1:
    case Float2x2:
    case Float2x3:
    case Float2x4:
    case Float3x1:
    case Float3x2:
    case Float3x3:
    case Float3x4:
    case Float4x1:
    case Float4x2:
    case Float4x3:
    case Float4x4:
        return Scalar;
    case Sampler1D:
    case Sampler2D:
    case Sampler3D:
        return Sampler;
    case UserStruct:
        return Struct;
    default:
        return UnknownCategory;
    }
}