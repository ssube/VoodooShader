// VoodooTexture.cpp : Implementation of CVoodooTexture

#include "stdafx.h"
#include "VoodooTexture.h"

#include "Common.hpp"

// CVoodooTexture
CVoodooTexture::CVoodooTexture()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Name.Empty();
    m_Data = NULL;
}

CVoodooTexture::~CVoodooTexture()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Name.Empty();
    m_Data = NULL;
}

IVoodooTexture * CVoodooTexture::Create(IVoodooCore * pCore, BSTR pName, void * pData)
{
    if ( pCore == NULL || pName == NULL ) return NULL;

    CComPtr<IVoodooTexture> ipTexture = NULL;

    CComObject<CVoodooTexture> * pTexture = NULL;
    HRESULT hr = CComObject<CVoodooTexture>::CreateInstance(&pTexture);
    if ( SUCCEEDED(hr) )
    {
        pTexture->AddRef();
        pTexture->m_Core = pCore;
        pTexture->m_Name = pName;
        pTexture->m_Data = pData;
        hr = pTexture->QueryInterface(IID_IVoodooTexture, (void**)&ipTexture);
        pTexture->Release();
    }

    return ipTexture.Detach();
}

STDMETHODIMP CVoodooTexture::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooTexture ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooTexture::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooTexture::Release()
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

STDMETHODIMP CVoodooTexture::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooTexture::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooTexture::get_Data(VARIANT *ppData)
{
    if ( ppData == NULL )
    {
        return E_INVALIDARG;
    } else {
        V_VT(ppData) = VT_BYREF;
        V_BYREF(ppData) = m_Data;
        return S_OK;
    }
}

