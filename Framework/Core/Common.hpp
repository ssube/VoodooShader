
#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include <Cg/Cg.h>

extern const HRESULT E_BADTHING     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0000);
extern const HRESULT E_INVALIDCFG   = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0100);
extern const HRESULT E_BADCLSID     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0200);
extern const HRESULT E_DUPNAME      = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0300);
extern const HRESULT E_NOT_FOUND    = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0400);
extern const HRESULT S_NOT_FOUND    = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0400);

// IVoodooLogger
extern const HRESULT E_FILEERROR    = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0001);

// IVoodooPass
extern const HRESULT E_NOPROGRAM    = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0x0001);

extern const HRESULT E_NULLIMPL     = MAKE_HRESULT(SEVERITY_ERROR,   FACILITY_ITF, 0xFFFF);

// Creates an interface to a string-format class ID. The ID may be in registry form or a ProgID.
HRESULT WINAPI InstanceFromString(_In_ BSTR lpStr, _In_ REFIID iid, _In_ void ** pp);
CGparameter WINAPI CreateVirtualParameter(IVoodooCore * pCore, ParameterType Type);