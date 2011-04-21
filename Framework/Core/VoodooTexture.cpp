// VoodooTexture.cpp : Implementation of CVoodooTexture

#include "stdafx.h"
#include "VoodooTexture.h"

#include "Common.hpp"

// CVoodooTexture
CVoodooTexture::CVoodooTexture()
{
    m_Core = NULL;
    m_Name.Empty();
    m_Data = NULL;
}

CVoodooTexture::~CVoodooTexture()
{
    m_Core = NULL;
    m_Name.Empty();
    m_Data = NULL;
}

CVoodooTexture * CVoodooTexture::Create(IVoodooCore * pCore, BSTR pName, void * pData)
{
    if ( pCore == NULL || pName == NULL ) return NULL;

    CVoodooTexture * texture = new CVoodooTexture();
    texture->m_Core = pCore;
    texture->m_Name = pName;
    texture->m_Data = pData;
    return texture;
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

