// VoodooFile.cpp : Implementation of CVoodooFile

#include "stdafx.h"
#include "VoodooFile.h"

#include "Common.hpp"

// CVoodooFile
CVoodooFile::CVoodooFile()
{
    m_Refrs = 0;
    m_Core = NULL;
}

CVoodooFile::~CVoodooFile()
{
    m_Refrs = 0;
    m_Core = NULL;
}

IVoodooFile * CVoodooFile::Create(IVoodooCore * pCore, BSTR pPath)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooFile> ipFile = NULL;

    CComObject<CVoodooFile> * pFile = NULL;
    HRESULT hr = CComObject<CVoodooFile>::CreateInstance(&pFile);
    if ( SUCCEEDED(hr) )
    {
        pFile->AddRef();

        pFile->m_Core = pCore;
        pFile->m_Name = pPath; //! @todo Change this to just the filename, no path
        pFile->m_Path = pPath;

        hr = pFile->QueryInterface(IID_IVoodooFile, (void**)&ipFile);
        pFile->Release();
    }

    return ipFile.Detach();
}

STDMETHODIMP CVoodooFile::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooFile ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooFile::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooFile::Release()
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

STDMETHODIMP CVoodooFile::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooFile::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooFile::get_Path(LPBSTR pPath)
{
    if ( pPath == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Path.CopyTo(pPath);
    }
}

STDMETHODIMP CVoodooFile::Open(FileOpenMode Mode)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooFile::Close()
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooFile::Read(int *Length, byte Buffer[ ])
{
    if ( Length == NULL || Buffer == NULL )
    {
        return E_INVALIDARG;
    } else {
        *Length = 0;
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooFile::Write(int Length, byte Buffer[ ])
{
    if ( Buffer == NULL )
    {
        return E_INVALIDARG;
    } else {
        return E_NULLIMPL;
    }
}