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
        this->Log(LL_Internal, VOODOO_CORE_NAME, VSTR("Log file opened."));
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
    /* [in] */ BSTR pMsg
)
{
    UNREFERENCED_PARAMETER(Level);
    UNREFERENCED_PARAMETER(pModule);

    if ( pMsg == NULL ) return VSFERR_INVALID_ARG;
    if ( m_File.m_hFile == CFile::hFileNull ) return VSFERR_NO_FILE;
    
    CComBSTR msg(pMsg);

    m_File.WriteString(msg);
    return S_OK;
}

STDMETHODIMP CVoodooLogger::get_BufferSize( 
    /* [retval][out] */ int *pSize)
{
    UNREFERENCED_PARAMETER(pSize);

    return E_NOTIMPL;
}

STDMETHODIMP CVoodooLogger::put_BufferSize( 
    /* [in] */ int Size)
{
    UNREFERENCED_PARAMETER(Size);

    return E_NOTIMPL;
}