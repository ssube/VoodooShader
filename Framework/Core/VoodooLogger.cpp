// VoodooLogger.cpp : Implementation of CVoodooLogger

#include "stdafx.h"
#include "VoodooLogger.h"

#include "Common.hpp"


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

STDMETHODIMP CVoodooLogger::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooLogger::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooLogger::Open( 
    /* [in] */ BSTR pFilename,
    /* [in] */ boolean Append)
{
    if ( pFilename == NULL ) return E_INVALIDARG;

    UINT create = CFile::modeWrite;
    if ( Append )
    {
        create |= CFile::modeNoTruncate;
    } else {
        create |= CFile::modeCreate;
    }

    if ( m_File.Open(pFilename, create) == 0 )
    {
        return E_FILEERROR;
    } else {
        this->Log(Internal, L"Voodoo/Logger", L"Log file opened.", NULL);
    }
    return S_OK;
}

STDMETHODIMP CVoodooLogger::Close( void)
{
    m_File.Close();
    return S_OK;
}

STDMETHODIMP CVoodooLogger::Dump( void)
{
    m_File.Flush();
    return S_OK;
}

STDMETHODIMP CVoodooLogger::get_LogLevel( 
    /* [retval][out] */ LogLevel *pLevel)
{
    if ( pLevel == NULL ) return E_INVALIDARG;

    *pLevel = m_Level;
    return S_OK;
}

STDMETHODIMP CVoodooLogger::put_LogLevel( 
    /* [in] */ LogLevel Level)
{
    m_Level = Level;
    return S_OK;
}

STDMETHODIMP CVoodooLogger::LogModule( 
    /* [in] */ VersionStruct Module)
{
    return E_NOTIMPL;
}

STDMETHODIMP CVoodooLogger::Log( 
    /* [in] */ LogLevel Level,
    /* [in] */ BSTR pModule,
    /* [in] */ BSTR pFormat,
    /* [in] */ SAFEARRAY * ppArgs)
{
    return E_NOTIMPL;
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