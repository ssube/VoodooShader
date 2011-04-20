// VoodooTechnique.h : Declaration of the CVoodooTechnique

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooTechnique

class ATL_NO_VTABLE CVoodooTechnique :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooTechnique, &CLSID_VoodooTechnique>,
	public IDispatchImpl<IVoodooTechnique, &IID_IVoodooTechnique, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooTechnique()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOTECHNIQUE)

DECLARE_NOT_AGGREGATABLE(CVoodooTechnique)

BEGIN_COM_MAP(CVoodooTechnique)
	COM_INTERFACE_ENTRY(IVoodooTechnique)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooTechnique
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooTechnique), CVoodooTechnique)
