// VoodooCore.h : Declaration of the CVoodooCore

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooCore

class ATL_NO_VTABLE CVoodooCore :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooCore, &CLSID_VoodooCore>,
	public IDispatchImpl<IVoodooCore, &IID_IVoodooCore, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooCore()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOCORE)

DECLARE_NOT_AGGREGATABLE(CVoodooCore)

BEGIN_COM_MAP(CVoodooCore)
	COM_INTERFACE_ENTRY(IVoodooCore)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooCore
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooCore), CVoodooCore)
