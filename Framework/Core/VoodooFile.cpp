// VoodooFile.cpp : Implementation of CVoodooFile

#include "stdafx.h"
#include "VoodooFile.h"



// CVoodooFile
CVoodooFile::CVoodooFile()
{
    m_Refrs = 0;
}

CVoodooFile::~CVoodooFile()
{
    m_Refrs = 0;
}

IVoodooFile * CVoodooFile::Create(BSTR pPath)
{
    CComPtr<CVoodooFile> ipFile = NULL;

    CComObject<CVoodooFile> * pFile = NULL;
    HRESULT hr = CComObject<CVoodooFile>::CreateInstance(&pFile);
    if ( SUCCEEDED(hr) )
    {
        pFile->AddRef();

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

STDMETHODIMP CVoodooFile::get_Path(LPBSTR pPath)
{
    if ( pPath == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Path.CopyTo(pPath);
    }
}

STDMETHODIMP CVoodooFile::Open(DWORD Mode)
{
    UNREFERENCED_PARAMETER(Mode);

    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooFile::Close()
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooFile::Read(int *Length, byte Buffer[ ])
{
    if ( Length == NULL || Buffer == NULL )
    {
        return E_INVALIDARG;
    } else {
        *Length = 0;
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooFile::Write(int Length, byte Buffer[ ])
{
    UNREFERENCED_PARAMETER(Length);

    if ( Buffer == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}