// VoodooShader.cpp : Implementation of CVoodooShader

#include "stdafx.h"
#include "VoodooShader.h"

#include "Common.hpp"


// CVoodooShader
STDMETHODIMP CVoodooShader::QueryInterface(REFIID iid, void ** pp) throw()
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

STDMETHODIMP_(ULONG) CVoodooShader::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooShader::Release()
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

STDMETHODIMP CVoodooShader::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooShader::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::get_TechniqueCount(INT * pCount)
{
    if ( pCount == NULL )
    {
        return E_INVALIDARG;
    } else {
        *pCount = m_Techniques.GetCount();
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::GetTechnique(INT Number, IVoodooTechnique **ppTechnique)
{
    if ( ppTechnique == NULL )
    {
        return E_INVALIDARG;
    } else if ( Number < 0 || Number >= m_Techniques.GetCount() ) {
        return E_INVALIDARG;
    } else {
        *ppTechnique = m_Techniques.GetAt(Number);
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::get_DefaultTechnique(IVoodooTechnique ** ppTechnique)
{
    if ( ppTechnique == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTechnique = m_Default;
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::put_DefaultTechnique(IVoodooTechnique * pTechnique)
{
    if ( pTechnique == NULL )
    {
        return E_INVALIDARG;
    } else {
        // Prevent cross-linking
        IVoodooShader * shader = NULL;
        pTechnique->get_Shader(&shader);
        if ( shader != this )
        {
            return E_INVALIDARG;
        } else {
            m_Default = pTechnique;
            return S_OK;
        }
    }
}

STDMETHODIMP CVoodooShader::get_ParameterCount(INT * pCount)
{
    if ( pCount == NULL )
    {
        return E_INVALIDARG;
    } else {
        *pCount = m_Parameters.GetCount();
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::GetParameter(INT Number, IVoodooParameter **ppParameter)
{
    if ( ppParameter == NULL )
    {
        return E_INVALIDARG;
    } else if ( Number < 0 || Number >= m_Parameters.GetCount() ) {
        return E_INVALIDARG;
    } else {
        *ppParameter = m_Parameters.GetAt(Number);
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::GetCgShader(VARIANT *ppCgShader)
{
    if ( ppCgShader == NULL )
    {
        return E_INVALIDARG;
    } else {
        V_VT(ppCgShader) = VT_BYREF;
        V_BYREF(ppCgShader) = (void*)m_Effect;
        return S_OK;
    }
}