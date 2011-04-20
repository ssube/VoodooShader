// VoodooFileSystem.h : Declaration of the CVoodooFileSystem

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooFileSystem

class ATL_NO_VTABLE CVoodooFileSystem :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooFileSystem, &CLSID_VoodooFileSystem>,
	public IDispatchImpl<IVoodooFileSystem, &IID_IVoodooFileSystem, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooFileSystem()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOFILESYSTEM)

DECLARE_NOT_AGGREGATABLE(CVoodooFileSystem)

BEGIN_COM_MAP(CVoodooFileSystem)
	COM_INTERFACE_ENTRY(IVoodooFileSystem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooFileSystem
public:
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooFileSystem), CVoodooFileSystem)
