
#pragma once

#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include <Cg/Cg.h>

#include "VoodooVersion.hpp"

extern const HRESULT E_BADTHING;
extern const HRESULT E_INVALIDCFG;
extern const HRESULT E_BADCLSID;
extern const HRESULT E_DUPNAME;
extern const HRESULT E_NOT_FOUND;
extern const HRESULT S_NOT_FOUND;

// IVoodooCore
extern const HRESULT E_NOTINIT;

// IVoodooLogger
extern const HRESULT E_FILEERROR;

// IVoodooPass
extern const HRESULT E_NOPROGRAM;

// IVoodooParser
extern const HRESULT E_ISSYSVAR;

extern const HRESULT E_NULLIMPL;

// Creates an interface to a string-format class ID. The ID may be in registry form or a ProgID.
HRESULT WINAPI InstanceFromString(_In_ BSTR lpStr, _In_ REFIID iid, _In_ void ** pp);
CGparameter WINAPI CreateVirtualParameter(IVoodooCore * pCore, ParameterType Type);

inline HRESULT WINAPI LogMsg(IVoodooLogger * pLogger, DWORD Level, LPWSTR pModule, LPWSTR pMsg, ...)
{
    VARIANT v;
    VariantInit(&v);
    V_VT(&v) = VT_BYREF;

    va_list args;
    va_start(args, pModule);

    V_BYREF(&v) = args;
    HRESULT hr = pLogger->LogList(Level, pModule, pMsg, v);

    va_end(args);
    return hr;
}

CGtype WINAPI ToCgType(ParameterType Type);
ParameterType WINAPI ToParameterType(CGtype Type);
ParameterCategory WINAPI ToParameterCategory(ParameterType Type);
