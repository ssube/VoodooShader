// VoodooFile.h : Declaration of the CVoodooFile

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooFile

class ATL_NO_VTABLE CVoodooFile :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooFile, &CLSID_VoodooFile>,
	public IDispatchImpl<IVoodooFile, &IID_IVoodooFile, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooFile()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOFILE)

DECLARE_NOT_AGGREGATABLE(CVoodooFile)

BEGIN_COM_MAP(CVoodooFile)
	COM_INTERFACE_ENTRY(IVoodooFile)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooFile
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooFile), CVoodooFile)
