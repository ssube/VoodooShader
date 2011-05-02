// XmlLogger.h : Declaration of the CXmlLogger

#pragma once
#include "XmlLogger_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CXmlLogger

class ATL_NO_VTABLE CXmlLogger :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CXmlLogger, &CLSID_XmlLogger>,
	public IDispatchImpl<IVoodooLogger, &IID_IVoodooLogger, &LIBID_XmlLoggerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CXmlLogger()
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

DECLARE_REGISTRY_RESOURCEID(IDR_XMLLOGGER1)

DECLARE_NOT_AGGREGATABLE(CXmlLogger)

BEGIN_COM_MAP(CXmlLogger)
	COM_INTERFACE_ENTRY(IVoodooLogger)
    COM_INTERFACE_ENTRY(IVoodooPlugin)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooLogger
public:

    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE Open(BSTR pFilename, VARIANT_BOOL Append);
    virtual HRESULT STDMETHODCALLTYPE Close(void);
    virtual HRESULT STDMETHODCALLTYPE Dump(void);
    virtual HRESULT STDMETHODCALLTYPE get_LogLevel(DWORD *pLevel);
    virtual HRESULT STDMETHODCALLTYPE put_LogLevel(DWORD Level);
    virtual HRESULT STDMETHODCALLTYPE LogModule(VersionStruct Module);
    virtual HRESULT STDMETHODCALLTYPE Log(DWORD Level, BSTR pModule, BSTR pFormat, SAFEARRAY * ppArgs);
    virtual HRESULT STDMETHODCALLTYPE LogList(DWORD Level, BSTR pModule, BSTR pFormat, VARIANT pList);
    virtual HRESULT STDMETHODCALLTYPE get_BufferSize(int *pSize);
    virtual HRESULT STDMETHODCALLTYPE put_BufferSize(int Size);

private:
    /**
     * Formats a timestamp for the log. The timestamp will have the 
     * form <code>HHMMSS</code>. Leading zeros are guaranteed to be
     * present, so the timestamp length is 6 chars.
     *
     * @note If the system time cannot be retrieved, an equal-length error 
     *       stamp of <code>000000</code> will be returned.
     */
    CStringW STDMETHODCALLTYPE LogTime();

    /**
     * Formats a date for the log. The date will have the form 
     * <code>YYYYMMDD</code>. Leading zeros are guaranteed to be present,
     * so the date length is 8 chars.
     *
     * @note If the system time cannot be retrieved, an equal-length error 
     *       stamp of <code>00000000</code> will be returned.
     */
    CStringW STDMETHODCALLTYPE LogDate();

    /**
     * Formats the system's current tick count. The stamp will have the
     * form <code>xxxxxxxxx</code>, with a potentially varying length. This
     * records ticks, usually ms since system start.
     */
    CStringW STDMETHODCALLTYPE LogTicks();

    inline void DebugLog(LPCWSTR pMsg)
    {
#ifdef _DEBUG
        OutputDebugString(pMsg);
#endif
    };

    ULONG m_Refrs;
    IVoodooCore * m_Core;
    LogLevel m_LogLevel;
    CStdioFile m_LogFile;
};

OBJECT_ENTRY_AUTO(__uuidof(XmlLogger), CXmlLogger)
