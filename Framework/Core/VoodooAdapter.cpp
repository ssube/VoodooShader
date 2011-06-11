// VoodooAdapter.cpp : Implementation of CVoodooAdapter

#include "stdafx.h"
#include "VoodooAdapter.h"




// CVoodooAdapter
CVoodooAdapter::CVoodooAdapter()
{
    m_Refrs = 0;
    m_Name = VSTR("Null Adapter");
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

STDMETHODIMP CVoodooAdapter::Initialize(IVoodooCore * pCore)
{
    if ( pCore == NULL )
    {
        return VSFERR_INVALID_ARG;
    }

    m_Core = pCore;
    return S_OK;
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
    UNREFERENCED_PARAMETER(pPass);

    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::UnloadPass(IVoodooPass *pPass)
{
    UNREFERENCED_PARAMETER(pPass);

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
    UNREFERENCED_PARAMETER(pPass);

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
    UNREFERENCED_PARAMETER(pTarget);

    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::get_Light(int Index, LightStruct *pLight)
{
    UNREFERENCED_PARAMETER(Index);

    if ( pLight == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::put_Light(int Index, LightStruct pLight)
{
    UNREFERENCED_PARAMETER(Index);
    UNREFERENCED_PARAMETER(pLight);

    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::SetProperty(BSTR pName, VARIANT *pData)
{
    UNREFERENCED_PARAMETER(pName);

    if ( pData == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::DrawGeometry(int Vertexes, SAFEARRAY *pVertexData)
{
    UNREFERENCED_PARAMETER(Vertexes);
    UNREFERENCED_PARAMETER(pVertexData);

    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::ApplyParameter(IVoodooParameter *pParameter)
{
    UNREFERENCED_PARAMETER(pParameter);

    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::DrawShader(IVoodooShader *pShader)
{
    UNREFERENCED_PARAMETER(pShader);

    return VSFOK_NULL_IMPL;
}

STDMETHODIMP CVoodooAdapter::CreateTexture(BSTR pName, TextureDesc Description, IVoodooTexture **ppTexture)
{
    UNREFERENCED_PARAMETER(pName);
    UNREFERENCED_PARAMETER(Description);

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
    UNREFERENCED_PARAMETER(Region);

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
    UNREFERENCED_PARAMETER(pTexture);

    if ( pParameter == NULL )
    {
        return E_INVALIDARG;
    } else {
        return VSFOK_NULL_IMPL;
    }
}

STDMETHODIMP CVoodooAdapter::HandleError(VARIANT pContext, DWORD Error, IVoodooCore *pCore)
{
    UNREFERENCED_PARAMETER(pContext);
    UNREFERENCED_PARAMETER(Error);
    UNREFERENCED_PARAMETER(pCore);

    return VSFOK_NULL_IMPL;
}