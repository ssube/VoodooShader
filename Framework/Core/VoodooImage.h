// VoodooImage.h : Declaration of the CVoodooImage

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooImage

class ATL_NO_VTABLE CVoodooImage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooImage, &CLSID_VoodooImage>,
	public IDispatchImpl<IVoodooImage, &IID_IVoodooImage, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooImage()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOIMAGE)

DECLARE_NOT_AGGREGATABLE(CVoodooImage)

BEGIN_COM_MAP(CVoodooImage)
	COM_INTERFACE_ENTRY(IVoodooImage)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooImage
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooImage), CVoodooImage)
