// VoodooLogger.h : Declaration of the CVoodooLogger

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CVoodooLogger

class ATL_NO_VTABLE CVoodooLogger :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooLogger, &CLSID_VoodooLogger>,
	public IDispatchImpl<IVoodooLogger, &IID_IVoodooLogger, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooLogger()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOLOGGER)

DECLARE_NOT_AGGREGATABLE(CVoodooLogger)

BEGIN_COM_MAP(CVoodooLogger)
	COM_INTERFACE_ENTRY(IVoodooLogger)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooLogger
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Open( 
        /* [in] */ BSTR pFilename,
        /* [in] */ BOOL Append);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Dump( void);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LogLevel( 
        /* [retval][out] */ LogLevel *pLevel);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LogLevel( 
        /* [in] */ LogLevel Level);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE LogModule( 
        /* [in] */ Version Module);

    virtual /* [vararg][id] */ HRESULT STDMETHODCALLTYPE Log( 
        /* [in] */ LogLevel Level,
        /* [in] */ BSTR pModule,
        /* [in] */ BSTR pFormat,
        /* [in] */ SAFEARRAY * *pArgs);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BufferSize( 
        /* [retval][out] */ UINT *pSize);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BufferSize( 
        /* [in] */ UINT Size);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    LogLevel m_Level;
    CFile m_File;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooLogger), CVoodooLogger)
