// XmlLogger.cpp : Implementation of DLL Exports.

//
// Note: COM+ 1.0 Information:
//      Please remember to run Microsoft Transaction Explorer to install the component(s).
//      Registration is not done by default. 

#include "stdafx.h"
#include "resource.h"
#include "XmlLogger_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

#include "Logger_Version.hpp"

// Used to determine whether the DLL can be unloaded by OLE.
STDAPI DllCanUnloadNow(void)
{
	#ifdef _MERGE_PROXYSTUB
	HRESULT hr = PrxDllCanUnloadNow();
	if (hr != S_OK)
		return hr;
#endif
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
	}

// Returns a class factory to create an object of the requested type.
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	#ifdef _MERGE_PROXYSTUB
	if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
		return S_OK;
#endif
		return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry.
STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	HRESULT hr = _AtlModule.DllRegisterServer();
	#ifdef _MERGE_PROXYSTUB
	if (FAILED(hr))
		return hr;
	hr = PrxDllRegisterServer();
#endif
		return hr;
}

// DllUnregisterServer - Removes entries from the system registry.
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	#ifdef _MERGE_PROXYSTUB
	if (FAILED(hr))
		return hr;
	hr = PrxDllRegisterServer();
	if (FAILED(hr))
		return hr;
	hr = PrxDllUnregisterServer();
#endif
		return hr;
}

// DllInstall - Adds/Removes entries to the system registry per user per machine.
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}


// XmlLogger.cpp : Implementation of CXmlLogger

#include "stdafx.h"
#include "XmlLogger.h"


// CXmlLogger
CXmlLogger::CXmlLogger()
{
    m_Refrs = 0;
    m_LogLevel = LL_Initial;
    m_Core = NULL;
}

STDMETHODIMP CXmlLogger::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooPlugin || iid == IID_IVoodooLogger ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CXmlLogger::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CXmlLogger::Release()
{
    --m_Refrs;
    if ( m_Refrs == 0 )
    {
        delete this;
        return 0;
    } else {
        return m_Refrs;
    }
}

STDMETHODIMP CXmlLogger::Initialize(IVoodooCore * pCore)
{
    if ( pCore == NULL )
    {
        return VSFERR_INVALID_ARG;
    }

    m_Core = pCore;
    return S_OK;
}

STDMETHODIMP CXmlLogger::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        (*ppCore)->AddRef();
        return S_OK;
    }
}

HRESULT CXmlLogger::Open(BSTR pFilename, VARIANT_BOOL Append)
{
    if ( pFilename == NULL ) return VSFERR_INVALID_ARG;

    UINT create = CFile::modeWrite;
    if ( Append == VARIANT_TRUE )
    {
        create |= CFile::modeNoTruncate;
    } else {
        create |= CFile::modeCreate;
    }

    if ( m_LogFile.Open(pFilename, create) == 0 )
    {
        return VSFERR_FILE_ERROR;
    } else {
        CStringW logMsg(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
        logMsg.Append(L"\n<VoodooLog ");
        logMsg.Append(this->LogDate());
        logMsg.Append(this->LogTime());
        logMsg.Append(this->LogTicks());
        logMsg.Append(L" >\n");

        this->DebugLog(logMsg);
        this->m_LogFile.WriteString(logMsg);

        this->Log(LL_Internal, VOODOO_LOGGER_NAME, L"Log file opened by XmlLogger::Open.");

        VersionStruct loggerVersion = VOODOO_META_VERSION_STRUCT(LOGGER);
        this->LogModule(loggerVersion);
    }
    return VSF_OK;
}

HRESULT CXmlLogger::Close()
{
    if ( m_LogFile.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;

    m_LogFile.WriteString(L"</VoodooLog>");
    m_LogFile.Close();

    return VSF_OK;
}

HRESULT CXmlLogger::Dump()
{
    if ( m_LogFile.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;

    m_LogFile.Flush();
    return VSF_OK;
}

CStringW CXmlLogger::LogTime()
{
    CTime time = CTime::GetCurrentTime();

    return time.Format(L" time=\"%H%M%S\" ");
}

CStringW CXmlLogger::LogDate()
{
    CTime date = CTime::GetCurrentTime();

    return date.Format(L" time=\"%Y%m%d\" ");
}

CStringW CXmlLogger::LogTicks()
{
    CStringW ticks;
    ticks.Format(L" ticks=\"%u\" ", GetTickCount());
    return ticks;
}

HRESULT CXmlLogger::LogModule
(
    VersionStruct version
)
{
    if ( m_LogFile.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;

    CStringW logMsg;

    logMsg.Format
    (
        L"    <Module name=\"%s\" major=\"%i\" minor=\"%i\" patch=\"%i\" rev=\"%i\" debug=\"%i\" />\n", 
        version.Name, version.Major, version.Minor, version.Patch, version.Rev, (version.Debug == VARIANT_TRUE)
    );

    DebugLog(logMsg);
    m_LogFile.WriteString(logMsg);

    return VSF_OK;
}

HRESULT CXmlLogger::Log(DWORD Level, BSTR pModule, BSTR pMsg)
{
    if ( m_LogFile.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;

    try
    {
        DWORD mask = (LogLevel)(Level & m_LogLevel);
        if ( !(mask & 0x0F) || !(mask & 0xF0) ) return VSFOK_LOG_LEVEL;

        CStringW logMsg;
        logMsg.Format(L"    <Message severity=\"%X\" %s %s source=\"%s\">%s</Message>\n", Level, this->LogTime(), this->LogTicks(), pModule, pMsg);

        DebugLog(logMsg);
        m_LogFile.WriteString(logMsg);

        return VSF_OK;
    } catch ( CException & exc ) {
        UNREFERENCED_PARAMETER(exc);

        return VSFERR_LOG_ERROR;
    }
}

HRESULT CXmlLogger::get_BufferSize(int * pSize)
{
    if ( pSize == NULL ) return VSFERR_INVALID_ARG;
    if ( m_LogFile.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;

    *pSize = -1;
    return S_OK;
}

HRESULT CXmlLogger::put_BufferSize(int Size)
{
    if ( m_LogFile.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;

    return S_OK;
}

HRESULT CXmlLogger::get_LogLevel(DWORD *pLevel)
{
    if ( pLevel == NULL ) return VSFERR_INVALID_ARG;

    *pLevel = m_LogLevel;
    return VSF_OK;
}

HRESULT CXmlLogger::put_LogLevel(DWORD Level)
{
    m_LogLevel = (LogLevel)Level;
    return VSF_OK;
}