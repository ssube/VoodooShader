// VoodooParameter.cpp : Implementation of CVoodooParameter

#include "stdafx.h"
#include "VoodooParameter.h"

#include "Common.hpp"


// CVoodooParameter
CVoodooParameter::CVoodooParameter()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Shader = NULL;
    m_Data.Create(16);
    m_Texture = NULL;
}

CVoodooParameter::~CVoodooParameter()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Shader = NULL;
    m_Data.Destroy();
    m_Texture = NULL;
}

IVoodooParameter * CVoodooParameter::Create(IVoodooCore * pCore, BSTR pName, ParameterType Type)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooParameter> ipParameter = NULL;

    CComObject<CVoodooParameter> * pParameter = NULL;
    HRESULT hr = CComObject<CVoodooParameter>::CreateInstance(&pParameter);
    if ( SUCCEEDED(hr) )
    {
        pParameter->AddRef();

        pParameter->m_Parameter = CreateVirtualParameter(pCore, Type);
        pParameter->m_Type = Type;
        pParameter->m_Virtual = TRUE;
        pParameter->m_Core = pCore;
        pParameter->m_Name = pName;

        hr = pParameter->QueryInterface(IID_IVoodooTechnique, (void**)&ipParameter);
        pParameter->Release();
    }

    return ipParameter.Detach();
}

IVoodooParameter * CVoodooParameter::Create(IVoodooShader * pShader, CGparameter Parameter)
{
    if ( pShader == NULL ) return NULL;

    CComPtr<CVoodooParameter> ipParameter = NULL;

    CComObject<CVoodooParameter> * pParameter = NULL;
    HRESULT hr = CComObject<CVoodooParameter>::CreateInstance(&pParameter);
    if ( SUCCEEDED(hr) )
    {
        pParameter->AddRef();

        pParameter->m_Parameter = Parameter;
        pParameter->m_Type = ToParameterType(cgGetParameterType(Parameter));
        pParameter->m_Shader = pShader;
        pParameter->m_Virtual = FALSE;
        pShader->get_Core(&pParameter->m_Core);
        pParameter->m_Name = CA2W(cgGetParameterName(Parameter));

        hr = pParameter->QueryInterface(IID_IVoodooTechnique, (void**)&ipParameter);
        pParameter->Release();
    }

    return ipParameter.Detach();
}

STDMETHODIMP CVoodooParameter::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooParameter ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooParameter::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooParameter::Release()
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

STDMETHODIMP CVoodooParameter::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooParameter::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::get_Type(ParameterType *pType)
{
    if ( pType == NULL )
    {
        return E_INVALIDARG;
    } else {
        *pType = m_Type;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::get_Virtual(boolean *pVirtual)
{
    if ( pVirtual == NULL )
    {
        return E_INVALIDARG;
    } else {
        *pVirtual = m_Virtual;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::AttachParameter(IVoodooParameter *pParameter)
{
    if ( pParameter == NULL )
    {
        return E_INVALIDARG;
    }

    // Check types
    ParameterType othertype;
    pParameter->get_Type(&othertype);

    if ( othertype != m_Type )
    {
        return E_INVALIDARG;
    }

    CGparameter otherparam;
    VARIANT othervar;
    pParameter->get_CgParameter(&othervar);
    otherparam = (CGparameter)V_BYREF(&othervar);

    cgConnectParameter(m_Parameter, otherparam);

    return S_OK;
}

STDMETHODIMP CVoodooParameter::get_Components(int * Components)
{
    switch ( m_Type )
    {
    case Float1:
        return 1;
    case Float2:
        return 2;
    case Float3:
        return 3;
    case Float4:
        return 4;
    case Float4x4:
        return 16;
    case Sampler1D:
        return 1;
    case Sampler2D:
        return 2;
    case Sampler3D:
        return 3;
    default:
        return -1;
    }
} 

STDMETHODIMP CVoodooParameter::get_SamplerValue( 
    /* [retval][out] */ IVoodooTexture **ppTexture)
{
    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTexture = m_Texture;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::put_SamplerValue( 
    /* [in] */ IVoodooTexture *pTexture)
{
    pTexture = m_Texture;
    return S_OK;
}

STDMETHODIMP CVoodooParameter::get_ScalarValue( 
    /* [retval][out] */ SAFEARRAY * *ppData)
{
    return m_Data.CopyTo(ppData);
}

STDMETHODIMP CVoodooParameter::put_ScalarValue( 
    /* [in] */ SAFEARRAY * pData)
{
    return m_Data.CopyFrom(pData);
}

STDMETHODIMP CVoodooParameter::get_Shader( 
    /* [retval][out] */ IVoodooShader **ppShader)
{
    if ( ppShader == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppShader = m_Shader;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::get_CgParameter( 
    /* [retval][out] */ VARIANT *ppCgParameter)
{
    if ( ppCgParameter == NULL )
    {
        return E_INVALIDARG;
    } else {
        V_VT(ppCgParameter) = VT_BYREF;
        V_BYREF(ppCgParameter) = m_Parameter;
        return S_OK;
    }
}