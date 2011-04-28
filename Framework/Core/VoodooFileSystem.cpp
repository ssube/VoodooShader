// VoodooFileSystem.cpp : Implementation of CVoodooFileSystem

#include "stdafx.h"
#include "VoodooFileSystem.h"




// CVoodooFileSystem
CVoodooFileSystem::CVoodooFileSystem()
{
    m_Refrs = 0;
    m_Core = NULL;
}

CVoodooFileSystem::~CVoodooFileSystem()
{
    m_Refrs = 0;
    m_Core = NULL;
}

IVoodooFileSystem * CVoodooFileSystem::Create(IVoodooCore * pCore)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooFileSystem> ipFileSystem = NULL;

    CComObject<CVoodooFileSystem> * pFileSystem = NULL;
    HRESULT hr = CComObject<CVoodooFileSystem>::CreateInstance(&pFileSystem);
    if ( SUCCEEDED(hr) )
    {
        pFileSystem->AddRef();
        pFileSystem->m_Core = pCore;
        pFileSystem->m_Name = L"Null FileSystem";

        hr = pFileSystem->QueryInterface(IID_IVoodooFileSystem, (void**)&ipFileSystem);
        pFileSystem->Release();
    }

    return ipFileSystem.Detach();
}

STDMETHODIMP CVoodooFileSystem::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooFileSystem ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooFileSystem::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooFileSystem::Release()
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

STDMETHODIMP CVoodooFileSystem::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooFileSystem::get_Core(IVoodooCore **ppCore)
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

STDMETHODIMP CVoodooFileSystem::AddDirectory(BSTR pPath)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooFileSystem::RemoveDirectory(BSTR pPath)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooFileSystem::FindFile(BSTR pPath, IVoodooFile **ppFile)
{
    if ( ppFile == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooFileSystem::FindImage(BSTR pPath, IVoodooImage **ppImage)
{
    if ( ppImage == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}