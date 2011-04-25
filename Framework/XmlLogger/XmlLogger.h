// XmlLogger.h : Declaration of the CXmlLogger

#pragma once
#include "resource.h"       // main symbols



#include "XmlLogger_i.h"


#ifdef _WIN32_WCE
#error "Neutral-threaded COM objects are not supported on Windows CE."
#endif

using namespace ATL;


// CXmlLogger

class ATL_NO_VTABLE CXmlLogger :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CXmlLogger, &CLSID_XmlLogger>,
	public IDispatchImpl<IVoodooLogger, &IID_IVoodooLogger, &LIBID_XmlLoggerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CXmlLogger()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_XMLLOGGER1)

DECLARE_NOT_AGGREGATABLE(CXmlLogger)

BEGIN_COM_MAP(CXmlLogger)
	COM_INTERFACE_ENTRY(IVoodooLogger)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



};

OBJECT_ENTRY_AUTO(__uuidof(XmlLogger), CXmlLogger)
