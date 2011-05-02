// VoodooLogger.cpp : Implementation of CVoodooLogger

#include "stdafx.h"
#include "VoodooLogger.h"

// CVoodooLogger
CVoodooLogger::CVoodooLogger()
{
    m_Refrs = 0;
    m_Core = NULL;
}

CVoodooLogger::~CVoodooLogger()
{
    m_Refrs = 0;
    m_Core = NULL;
}

IVoodooLogger * CVoodooLogger::Create(IVoodooCore * pCore)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooLogger> ipLogger = NULL;

    CComObject<CVoodooLogger> * pLogger = NULL;
    HRESULT hr = CComObject<CVoodooLogger>::CreateInstance(&pLogger);
    if ( SUCCEEDED(hr) )
    {
        pLogger->AddRef();
        pLogger->m_Core = pCore;
        hr = pLogger->QueryInterface(IID_IVoodooLogger, (void**)&ipLogger);
        pLogger->Release();
    }

    return ipLogger.Detach();
}

STDMETHODIMP CVoodooLogger::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooLogger ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooLogger::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooLogger::Release()
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

STDMETHODIMP CVoodooLogger::Initialize(IVoodooCore * pCore)
{
    if ( pCore == NULL )
    {
        return VSFERR_INVALID_ARG;
    }

    m_Core = pCore;
    return S_OK;
}

STDMETHODIMP CVoodooLogger::get_Core(IVoodooCore **ppCore)
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

STDMETHODIMP CVoodooLogger::Open( 
    /* [in] */ BSTR pFilename,
    /* [in] */ VARIANT_BOOL Append)
{
    if ( pFilename == NULL ) return VSFERR_INVALID_ARG;

    UINT create = CFile::modeWrite;
    if ( Append == VARIANT_TRUE )
    {
        create |= CFile::modeNoTruncate;
    } else {
        create |= CFile::modeCreate;
    }

    if ( m_File.Open(pFilename, create) == 0 )
    {
        return VSFERR_FILE_ERROR;
    } else {
        this->Log(LL_Internal, VOODOO_CORE_NAME, L"Log file opened.", NULL);
    }
    return VSF_OK;
}

STDMETHODIMP CVoodooLogger::Close( void)
{
    m_File.Close();
    return VSF_OK;
}

STDMETHODIMP CVoodooLogger::Dump( void)
{
    m_File.Flush();
    return VSF_OK;
}

STDMETHODIMP CVoodooLogger::get_LogLevel
( 
    DWORD * pLevel
)
{
    if ( pLevel == NULL ) return E_INVALIDARG;

    *pLevel = m_Level;
    return VSF_OK;
}

STDMETHODIMP CVoodooLogger::put_LogLevel( 
    /* [in] */ DWORD Level)
{
    m_Level = (LogLevel)Level;
    return S_OK;
}

STDMETHODIMP CVoodooLogger::LogModule(VersionStruct Module)
{
    UNREFERENCED_PARAMETER(Module);

    return E_NOTIMPL;
}

STDMETHODIMP CVoodooLogger::Log
( 
    /* [in] */ DWORD Level,
    /* [in] */ BSTR pModule,
    /* [in] */ BSTR pFormat,
    /* [in] */ SAFEARRAY * ppArgs
)
{
    if ( pFormat == NULL ) return VSFERR_INVALID_ARG;
    if ( m_File.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;
    
    //! @todo Implement proper formatting here that isn't so ugly.
    CComSafeArray<VARIANT> args;
    args.Attach(ppArgs);
    CStringW msg(pFormat);

    for ( ULONG carg = 0; carg < args.GetCount(); ++carg )
    {
        CStringW search;
        search.Format(L"{%u}", carg);
        // Format the arg alone
        VARIANT varg = args.GetAt(carg);
        CStringW farg;
        switch ( V_VT(&varg)  )
        {
        case VT_UINT:
            farg.Format(L"%u", V_UINT(&varg));
            break;
        default:
            farg = L"-argerror-";
        }

        msg.Replace(search.GetString(), farg.GetString());
    }

    m_File.WriteString(msg);
    return S_OK;
}

STDMETHODIMP CVoodooLogger::LogList( 
    /* [in] */ DWORD Level,
    /* [in] */ BSTR pModule,
    /* [in] */ BSTR pFormat,
    /* [in] */ VARIANT pList)
{
    if ( pFormat == NULL ) return VSFERR_INVALID_ARG;
    if ( m_File.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;

    CStringW msg;

    msg.Format(L"Level: %u\tModule: %s\tMessage:\n\t", Level, pModule);

    va_list vargs = (va_list)V_BYREF(&pList);
    //va_start(vargs, pFormat);
    msg.AppendFormatV(pFormat, vargs);
    //va_end(vargs);

    msg.AppendChar(L'\n');

    m_File.WriteString(msg);

    return S_OK;
}

STDMETHODIMP CVoodooLogger::get_BufferSize( 
    /* [retval][out] */ int *pSize)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVoodooLogger::put_BufferSize( 
    /* [in] */ int Size)
{
    return E_NOTIMPL;
}