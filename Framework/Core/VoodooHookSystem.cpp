// VoodooHookSystem.cpp : Implementation of CVoodooHookSystem

#include "stdafx.h"
#include "VoodooHookSystem.h"

#include "Common.hpp"


// CVoodooHookSystem
CVoodooHookSystem::CVoodooHookSystem()
{
    m_Refrs = 0;
    m_Core = NULL;
}

CVoodooHookSystem::~CVoodooHookSystem()
{
    m_Refrs = 0;
    m_Core = NULL;
}

IVoodooHookSystem * CVoodooHookSystem::Create(IVoodooCore * pCore)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooHookSystem> ipHookSystem = NULL;

    CComObject<CVoodooHookSystem> * pHookSystem = NULL;
    HRESULT hr = CComObject<CVoodooHookSystem>::CreateInstance(&pHookSystem);
    if ( SUCCEEDED(hr) )
    {
        pHookSystem->AddRef();
        pHookSystem->m_Core = pCore;
        hr = pHookSystem->QueryInterface(IID_IVoodooTechnique, (void**)&ipHookSystem);
        pHookSystem->Release();
    }

    return ipHookSystem.Detach();
}

STDMETHODIMP CVoodooHookSystem::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooHookSystem ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooHookSystem::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooHookSystem::Release()
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

STDMETHODIMP CVoodooHookSystem::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooHookSystem::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooHookSystem::Add(BSTR pName, FunctionPtr pSource, FunctionPtr pDest)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooHookSystem::Remove(BSTR pName)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooHookSystem::RemoveAll(void)
{
    return E_NULLIMPL;
}
