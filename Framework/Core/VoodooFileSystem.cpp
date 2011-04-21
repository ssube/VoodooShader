// VoodooFileSystem.cpp : Implementation of CVoodooFileSystem

#include "stdafx.h"
#include "VoodooFileSystem.h"

#include "Common.hpp"


// CVoodooFileSystem
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
        return S_OK;
    }
}

STDMETHODIMP CVoodooFileSystem::AddDirectory(BSTR pPath)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooFileSystem::RemoveDirectory(BSTR pPath)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooFileSystem::FindFile(BSTR pPath, IVoodooFile **ppFile)
{
    if ( ppFile == NULL )
    {
        return E_INVALIDARG;
    } else {
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooFileSystem::FindImage(BSTR pPath, IVoodooImage **ppImage)
{
    if ( ppImage == NULL )
    {
        return E_INVALIDARG;
    } else {
        return E_NULLIMPL;
    }
}