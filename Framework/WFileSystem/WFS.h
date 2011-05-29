// WFS.h : Declaration of the CWFS

#pragma once
#include "WFileSystem_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CWFS

class ATL_NO_VTABLE CWFS :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWFS, &CLSID_WFS>,
	public IDispatchImpl<IVoodooFileSystem, &IID_IVoodooFileSystem, &LIBID_WFileSystemLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CWFS();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WFS)

DECLARE_NOT_AGGREGATABLE(CWFS)

BEGIN_COM_MAP(CWFS)
	COM_INTERFACE_ENTRY(IVoodooFileSystem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooFileSystem
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE AddDirectory(BSTR pPath);
    virtual HRESULT STDMETHODCALLTYPE RemoveDirectory(BSTR pPath);
    virtual HRESULT STDMETHODCALLTYPE FindFile(BSTR pPath, IVoodooFile **ppFile);
    virtual HRESULT STDMETHODCALLTYPE FindImage(BSTR pPath, IVoodooImage **ppImage);

private:
	BOOL m_Init;
    ULONG m_Refrs;
    IVoodooCore * m_Core;
	CStringList m_Directories;
};

OBJECT_ENTRY_AUTO(__uuidof(WFS), CWFS)
