// VoodooTexture.h : Declaration of the CVoodooTexture

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooTexture

class ATL_NO_VTABLE CVoodooTexture :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooTexture, &CLSID_VoodooTexture>,
	public IDispatchImpl<IVoodooTexture, &IID_IVoodooTexture, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooTexture()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOTEXTURE)

DECLARE_NOT_AGGREGATABLE(CVoodooTexture)

BEGIN_COM_MAP(CVoodooTexture)
	COM_INTERFACE_ENTRY(IVoodooTexture)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooTexture
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
        /* [retval][out] */ VARIANT *ppData);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    void * m_Data;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooTexture), CVoodooTexture)
