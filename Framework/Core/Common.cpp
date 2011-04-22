
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

    return CoCreateInstance(clsid, NULL, NULL, iid, pp);
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
    case (Sampler1D):
        return CG_SAMPLER1D;
    case (Sampler2D):
        return CG_SAMPLER2D;
    case (Sampler3D):
        return CG_SAMPLER3D;
    case (Float1):
        return CG_FLOAT;
    case (Float2):
        return CG_FLOAT2;
    case (Float3):
        return CG_FLOAT3;
    case (Float4):
        return CG_FLOAT4;
    case (Float4x4):
        return CG_FLOAT4x4;
    case (PT_Unknown):
    default:
        return CG_UNKNOWN_TYPE;
    }
}

ParameterType WINAPI ToParameterType(CGtype Type)
{
    switch ( Type )
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

ParameterCategory WINAPI ToParameterCategory(ParameterType Type)
{
    switch ( Type )
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