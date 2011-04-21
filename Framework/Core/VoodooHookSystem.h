// VoodooHookSystem.h : Declaration of the CVoodooHookSystem

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooHookSystem

class ATL_NO_VTABLE CVoodooHookSystem :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooHookSystem, &CLSID_VoodooHookSystem>,
	public IDispatchImpl<IVoodooHookSystem, &IID_IVoodooHookSystem, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooHookSystem()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOHOOKSYSTEM)

DECLARE_NOT_AGGREGATABLE(CVoodooHookSystem)

BEGIN_COM_MAP(CVoodooHookSystem)
	COM_INTERFACE_ENTRY(IVoodooHookSystem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooHookSystem
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
        /* [in] */ BSTR pName,
        /* [in] */ FunctionPtr pSource,
        /* [in] */ FunctionPtr pDest);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
        /* [in] */ BSTR pName);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAll( void);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooHookSystem), CVoodooHookSystem)
