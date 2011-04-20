// VoodooAdapter.h : Declaration of the CVoodooAdapter

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooAdapter

class ATL_NO_VTABLE CVoodooAdapter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooAdapter, &CLSID_VoodooAdapter>,
	public IDispatchImpl<IVoodooAdapter, &IID_IVoodooAdapter, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooAdapter()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOADAPTER)

DECLARE_NOT_AGGREGATABLE(CVoodooAdapter)

BEGIN_COM_MAP(CVoodooAdapter)
	COM_INTERFACE_ENTRY(IVoodooAdapter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooAdapter
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooAdapter), CVoodooAdapter)
