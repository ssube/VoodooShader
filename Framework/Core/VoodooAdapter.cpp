// VoodooAdapter.cpp : Implementation of CVoodooAdapter

#include "stdafx.h"
#include "VoodooAdapter.h"

#include "Common.hpp"


// CVoodooAdapter
CVoodooAdapter::CVoodooAdapter()
{
    m_Refrs = 0;
    m_Name = L"Null Adapter";
    m_Core = NULL;
}

CVoodooAdapter::~CVoodooAdapter()
{
    m_Refrs = 0;
    m_Name.Empty();
    m_Core = NULL;
}

IVoodooAdapter * CVoodooAdapter::Create(IVoodooCore * pCore)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooAdapter> ipAdapter = NULL;

    CComObject<CVoodooAdapter> * pAdapter = NULL;
    HRESULT hr = CComObject<CVoodooAdapter>::CreateInstance(&pAdapter);
    if ( SUCCEEDED(hr) )
    {
        pAdapter->AddRef();
        pAdapter->m_Core = pCore;
        hr = pAdapter->QueryInterface(IID_IVoodooTechnique, (void**)&ipAdapter);
        pAdapter->Release();
    }

    return ipAdapter.Detach();
}

STDMETHODIMP CVoodooAdapter::QueryInterface(REFIID iid, void ** pp) throw()
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

STDMETHODIMP_(ULONG) CVoodooAdapter::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooAdapter::Release()
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

STDMETHODIMP CVoodooAdapter::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooAdapter::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooAdapter::LoadPass(IVoodooPass *pPass)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooAdapter::UnloadPass(IVoodooPass *pPass)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooAdapter::get_Pass(IVoodooPass **ppPass)
{
    if ( ppPass == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppPass = NULL;
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooAdapter::put_Pass(IVoodooPass *pPass)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooAdapter::get_Target(IVoodooTexture **ppTarget)
{
    if ( ppTarget == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTarget = NULL;
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooAdapter::put_Target(IVoodooTexture *pTarget)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooAdapter::DrawGeometry(int Vertexes, VertexStruct *pVertexData)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooAdapter::ApplyParameter(IVoodooParameter *pParameter)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooAdapter::DrawShader(IVoodooShader *pShader)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooAdapter::CreateTexture(BSTR pName, TextureDesc Description, IVoodooTexture **ppTexture)
{
    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTexture = NULL;
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooAdapter::LoadTexture(IVoodooImage *pImage, IVoodooTexture **ppTexture)
{
    if ( ppTexture == NULL || pImage == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTexture = NULL;
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooAdapter::ConnectTexture(IVoodooParameter *pParameter, IVoodooTexture *pTexture)
{
    if ( pParameter == NULL )
    {
        return E_INVALIDARG;
    } else {
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooAdapter::HandleError(VARIANT pContext, DWORD Error, IVoodooCore *pCore)
{
    return E_NULLIMPL;
}