// VoodooParser.h : Declaration of the CVoodooParser

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooParser

class ATL_NO_VTABLE CVoodooParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooParser, &CLSID_VoodooParser>,
	public IDispatchImpl<IVoodooParser, &IID_IVoodooParser, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooParser()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPARSER)

DECLARE_NOT_AGGREGATABLE(CVoodooParser)

BEGIN_COM_MAP(CVoodooParser)
	COM_INTERFACE_ENTRY(IVoodooParser)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooParser
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooParser), CVoodooParser)
