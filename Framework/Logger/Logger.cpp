#include "Logger.hpp"

#include "Logger_Version.hpp"

XmlLogger::XmlLogger()
    : m_Core(NULL), m_LogLevel(LL_Initial)
{
}

XmlLogger::~XmlLogger()
{
    m_LogFile.Close();
}

STDMETHODIMP XmlLogger::QueryInterface(REFIID iid, void ** pp) throw()
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

STDMETHODIMP_(ULONG) XmlLogger::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) XmlLogger::Release()
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

STDMETHODIMP XmlLogger::Initialize(IVoodooCore * pCore)
{
    if ( pCore == NULL )
    {
        return VSFERR_INVALID_ARG;
    }

    m_Core = pCore;
    return S_OK;
}

STDMETHODIMP XmlLogger::get_Core(IVoodooCore **ppCore)
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

HRESULT XmlLogger::Open(BSTR pFilename, VARIANT_BOOL Append)
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

        this->Log(LL_Internal, VOODOO_LOGGER_NAME, L"Log file opened by XmlLogger::Open.", NULL);

        VersionStruct loggerVersion = VOODOO_META_VERSION_STRUCT(LOGGER);
        this->LogModule(loggerVersion);
    }
    return VSF_OK;
}

HRESULT XmlLogger::Close()
{
    m_LogFile.WriteString(L"</VoodooLog>");
    m_LogFile.Close();

    return VSF_OK;
}

HRESULT XmlLogger::Dump()
{
    m_LogFile.Flush();
    return VSF_OK;
}

CStringW XmlLogger::LogTime()
{
    CTime time = CTime::GetCurrentTime();

    return time.Format(L" time=\"%H%M%S\" ");
}

CStringW XmlLogger::LogDate()
{
    CTime date = CTime::GetCurrentTime();

    return date.Format(L" time=\"%Y%m%d\" ");
}

CStringW XmlLogger::LogTicks()
{
    CStringW ticks;
    ticks.Format(L" ticks=\"%u\" ", GetTickCount());
    return ticks;
}

HRESULT XmlLogger::LogModule
(
    VersionStruct version
)
{
    //if ( !this->m_LogFile.GetSt() ) return VSFERR_FILE_ERROR;

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

HRESULT XmlLogger::put_BufferSize(int Size)
{
    //if ( !this->mLogFile.is_open() ) return;

    //this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
}

HRESULT XmlLogger::get_LogLevel(DWORD *pLevel)
{
    if ( pLevel == NULL ) return VSFERR_INVALID_ARG;

    *pLevel = m_LogLevel;
    return VSF_OK;
}

HRESULT XmlLogger::put_LogLevel(DWORD Level)
{
    m_LogLevel = (LogLevel)Level;
    return VSF_OK;
}

HRESULT XmlLogger::Log(DWORD Level, BSTR pModule, BSTR pMsg, SAFEARRAY * pArgs)
{
    try
    {
        //if ( !this->mLogFile.is_open() ) return;
        DWORD mask = (LogLevel)(Level & m_LogLevel);
        if ( !(mask & 0x0F) || !(mask & 0xF0) ) return VSFOK_LOG_LEVEL;

        CStringW logMsg;
        logMsg.Format(L"    <Message severity=\"%X\" %s %s source=\"%s\">", Level, this->LogTime(), this->LogTicks(), pModule);
        logMsg.Append(pMsg);
        logMsg.Append(L"</Message>\n");

        DebugLog(logMsg);
        m_LogFile.WriteString(logMsg);

        return VSF_OK;
    } catch ( CException & exc ) {
        return VSFERR_LOG_ERROR;
    }
}
