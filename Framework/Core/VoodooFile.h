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
	public IDispatchImpl<IVoodooFile, &IID_IVoodooFile, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooFile();
    virtual ~CVoodooFile();
    static IVoodooFile * Create(IVoodooCore * pCore, BSTR pPath);

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
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
        /* [retval][out] */ LPBSTR pPath);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Open( 
        /* [in] */ DWORD Mode);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close(void);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Read( 
        /* [out][in] */ int *Length,
        /* [size_is][out] */ byte Buffer[  ]);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Write( 
        /* [in] */ int Length,
        /* [size_is][in] */ byte Buffer[  ]);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    CComBSTR m_Path;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooFile), CVoodooFile)
