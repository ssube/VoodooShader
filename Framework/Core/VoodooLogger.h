// VoodooLogger.h : Declaration of the CVoodooLogger

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooLogger

class ATL_NO_VTABLE CVoodooLogger :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooLogger, &CLSID_VoodooLogger>,
	public IDispatchImpl<IVoodooLogger, &IID_IVoodooLogger, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooLogger()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOLOGGER)

DECLARE_NOT_AGGREGATABLE(CVoodooLogger)

BEGIN_COM_MAP(CVoodooLogger)
	COM_INTERFACE_ENTRY(IVoodooLogger)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooLogger
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooLogger), CVoodooLogger)
