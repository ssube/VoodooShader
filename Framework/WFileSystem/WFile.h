// WFile.h : Declaration of the CWFile

#pragma once
#include "WFileSystem_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CWFile

class ATL_NO_VTABLE CWFile :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CWFile, &CLSID_WFile>,
    public IDispatchImpl<IVoodooFile, &IID_IVoodooFile, &LIBID_WFileSystemLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    static IVoodooFile * Create(CStringW Filename);

    CWFile();
    virtual ~CWFile();

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_WFILE)

DECLARE_NOT_AGGREGATABLE(CWFile)

BEGIN_COM_MAP(CWFile)
    COM_INTERFACE_ENTRY(IVoodooFile)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooFile
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

// IVoodooFile
public:
    ULONG m_Refrs;
    CComBSTR m_Path;
    CFile m_File;
};

OBJECT_ENTRY_AUTO(__uuidof(WFile), CWFile)
