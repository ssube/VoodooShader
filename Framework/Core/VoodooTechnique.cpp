// VoodooTechnique.cpp : Implementation of CVoodooTechnique

#include "stdafx.h"
#include "VoodooTechnique.h"

#include "Common.hpp"


// CVoodooTechnique
CVoodooTechnique::CVoodooTechnique()
{
    m_Refrs = 0;
    m_Name.Empty();
    m_Core = NULL;
    m_Target = NULL;
    m_Passes.RemoveAll();
    m_Shader = NULL;
    m_Technique = NULL;
}

CVoodooTechnique::~CVoodooTechnique()
{
    m_Refrs = 0;
    m_Name.Empty();
    m_Core = NULL;
    m_Target = NULL;
    m_Passes.RemoveAll();
    m_Shader = NULL;
    m_Technique = NULL;
}

IVoodooTechnique * CVoodooTechnique::Create(IVoodooShader * pShader, CGtechnique pTechnique)
{
    if ( pShader == NULL || pTechnique == NULL || !cgIsTechnique(pTechnique) ) return NULL;

    CComPtr<CVoodooTechnique> ipcTechnique = NULL;

    CComObject<CVoodooTechnique> * pcTechnique = NULL;
    HRESULT hr = CComObject<CVoodooTechnique>::CreateInstance(&pcTechnique);
    if ( SUCCEEDED(hr) )
    {
        pcTechnique->AddRef();

        CComBSTR name;
        pShader->get_Name(&name);
        name.Append(L":");
        name.Append(cgGetTechniqueName(pTechnique));
        pcTechnique->m_Name = name;

        pShader->get_Core(&pcTechnique->m_Core);
        pcTechnique->m_Shader = pShader;
        pcTechnique->m_Technique = pTechnique;

        hr = pcTechnique->QueryInterface(IID_IVoodooTechnique, (void**)&ipcTechnique);
        pcTechnique->Release();
    }

    return ipcTechnique.Detach();
}

STDMETHODIMP CVoodooTechnique::QueryInterface(REFIID iid, void ** pp) throw()
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

STDMETHODIMP_(ULONG) CVoodooTechnique::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooTechnique::Release()
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

STDMETHODIMP CVoodooTechnique::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooTechnique::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooTechnique::get_Target(IVoodooTexture ** ppTexture)
{
    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTexture = m_Target;
        return S_OK;
    }
}

STDMETHODIMP CVoodooTechnique::get_PassCount(INT * ppCount)
{
    if ( ppCount == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCount = m_Passes.GetCount();
        return S_OK;
    }
}

STDMETHODIMP CVoodooTechnique::GetPass(INT Number, IVoodooPass **ppPass)
{
    if ( ppPass == NULL )
    {
        return E_INVALIDARG;
    } else if ( Number < 0 || Number >= m_Passes.GetCount() ) {
        return E_INVALIDARG;
    } else {
        CComPtr<IVoodooPass> pass = m_Passes.GetAt(Number);
        *ppPass = pass;
        return S_OK;
    }
}

STDMETHODIMP CVoodooTechnique::get_Shader(IVoodooShader **ppShader)
{
    if ( ppShader == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppShader = m_Shader;
        return S_OK;
    }
}

STDMETHODIMP CVoodooTechnique::GetCgTechnique(VARIANT *ppPass)
{
    if ( ppPass == NULL )
    {
        return E_INVALIDARG;
    } else {
        V_VT(ppPass) = VT_BYREF;
        V_BYREF(ppPass) = (void*)m_Technique;
        return S_OK;
    }
}