// VoodooAdapter.cpp : Implementation of CVoodooAdapter

#include "stdafx.h"
#include "VoodooAdapter.h"




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
        hr = pAdapter->QueryInterface(IID_IVoodooAdapter, (void**)&ipAdapter);
        pAdapter->Release();
    }

    return ipAdapter.Detach();
}

STDMETHODIMP CVoodooAdapter::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooAdapter ) {
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
        return VSFERR_INVALID_ARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooAdapter::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return VSFERR_INVALID_ARG;
    } else {
        *ppCore = m_Core;
        (*ppCore)->AddRef();
        return VSF_OK;
    }
}

STDMETHODIMP CVoodooAdapter::LoadPass(IVoodooPass *pPass)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::UnloadPass(IVoodooPass *pPass)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::get_Pass(IVoodooPass **ppPass)
{
    if ( ppPass == NULL )
    {
        return VSFERR_INVALID_ARG;
    } else {
        *ppPass = NULL;
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::put_Pass(IVoodooPass *pPass)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::get_Target(IVoodooTexture **ppTarget)
{
    if ( ppTarget == NULL )
    {
        return VSFERR_INVALID_ARG;
    } else {
        *ppTarget = NULL;
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::put_Target(IVoodooTexture *pTarget)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::get_Light(int Index, LightStruct *pLight)
{
    if ( pLight == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::put_Light(int Index, LightStruct pLight)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::SetProperty(BSTR pName, VARIANT *pData)
{
    if ( pData == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::DrawGeometry(int Vertexes, SAFEARRAY *pVertexData)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::ApplyParameter(IVoodooParameter *pParameter)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::DrawShader(IVoodooShader *pShader)
{
    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::CreateTexture(BSTR pName, TextureDesc Description, IVoodooTexture **ppTexture)
{
    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTexture = NULL;
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::LoadTexture(IVoodooImage *pImage, TextureRegion Region, IVoodooTexture **ppTexture)
{
    if ( ppTexture == NULL || pImage == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTexture = NULL;
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::ConnectTexture(IVoodooParameter *pParameter, IVoodooTexture *pTexture)
{
    if ( pParameter == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::HandleError(VARIANT pContext, DWORD Error, IVoodooCore *pCore)
{
    return VSFOK_NULL_IMPL;
}