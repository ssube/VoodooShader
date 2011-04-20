// VoodooParameter.h : Declaration of the CVoodooParameter

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooParameter

class ATL_NO_VTABLE CVoodooParameter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooParameter, &CLSID_VoodooParameter>,
	public IDispatchImpl<IVoodooParameter, &IID_IVoodooParameter, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooParameter()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPARAMETER)

DECLARE_NOT_AGGREGATABLE(CVoodooParameter)

BEGIN_COM_MAP(CVoodooParameter)
	COM_INTERFACE_ENTRY(IVoodooParameter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooParameter
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooParameter), CVoodooParameter)
