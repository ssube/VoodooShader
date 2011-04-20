// VoodooShader.h : Declaration of the CVoodooShader

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooShader

class ATL_NO_VTABLE CVoodooShader :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooShader, &CLSID_VoodooShader>,
	public IDispatchImpl<IVoodooShader, &IID_IVoodooShader, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooShader()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOSHADER)

DECLARE_NOT_AGGREGATABLE(CVoodooShader)

BEGIN_COM_MAP(CVoodooShader)
	COM_INTERFACE_ENTRY(IVoodooShader)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooShader
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooShader), CVoodooShader)
