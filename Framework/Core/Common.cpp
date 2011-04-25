
#include "stdafx.h"

#include "Common.hpp"

// 0b1nnnnnnn is a library-level code. 0b0nnnnnnn is interface-level. This works around
// the facility restriction.
const HRESULT E_BADTHING     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x8000);
const HRESULT E_INVALIDCFG   = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x8001);
const HRESULT E_BADCLSID     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x8002);
const HRESULT E_DUPNAME      = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x8003);
const HRESULT E_NOT_FOUND    = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x8004);
const HRESULT S_NOT_FOUND    = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x8004);

// IVoodooCore
const HRESULT E_NOTINIT      = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0001);

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