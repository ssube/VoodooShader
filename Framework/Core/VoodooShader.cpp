// VoodooShader.cpp : Implementation of CVoodooShader

#include "stdafx.h"
#include "VoodooShader.h"
#include "VoodooParameter.h"
#include "VoodooTechnique.h"

// CVoodooShader
CVoodooShader::CVoodooShader()
{
    m_Linked = 0;
    m_Refrs = 0;
    m_Core = NULL;
}

CVoodooShader::~CVoodooShader()
{
    m_Linked = 0;
    m_Refrs = 0;
    m_Core = NULL;
}

IVoodooShader * CVoodooShader::Create(IVoodooCore * pCore, IVoodooFile * pFile, SAFEARRAY * pArgs)
{
    UNREFERENCED_PARAMETER(pArgs);

    if ( pCore == NULL || pFile == NULL ) return NULL;

    CComPtr<CVoodooShader> ipShader = NULL;

    CComObject<CVoodooShader> * pShader = NULL;
    HRESULT hr = CComObject<CVoodooShader>::CreateInstance(&pShader);
    if ( SUCCEEDED(hr) )
    {
        CGcontext context = NULL;
        VARIANT vc;
        pCore->get_CgContext(&vc);
        context = (CGcontext)V_BYREF(&vc);

        if ( context == NULL || !cgIsContext(context) )
        {
            return NULL;
        }

        BSTR path;
        pFile->get_Path(&path);
        CW2A apath(path);

        //! @todo Shader compiler args are not passed to the compiler. They need converted from SAFEARRAY<BSTR> to char[]*.
        CGeffect effect = cgCreateEffectFromFile(context, (LPSTR)apath, NULL);

        if ( effect == NULL || !cgIsEffect(effect) )
        {
            return NULL;
        }

        pShader->AddRef();

        pShader->m_Core = pCore;
        pShader->m_Effect = effect;

        pShader->LinkShader();

        hr = pShader->QueryInterface(IID_IVoodooShader, (void**)&ipShader);
        pShader->Release();
    }

    return ipShader.Detach();
}

STDMETHODIMP CVoodooShader::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooShader ) {
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
        (*ppCore)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::get_TechniqueCount(INT * pCount)
{
    if ( !m_Linked ) return VSFERR_NOT_LINKED;
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
    if ( !m_Linked ) return VSFERR_NOT_LINKED;
    if ( ppTechnique == NULL )
    {
        return E_INVALIDARG;
    } else if ( Number < 0 || Number >= m_Techniques.GetCount() ) {
        return E_INVALIDARG;
    } else {
        *ppTechnique = m_Techniques.GetAt(Number);
        (*ppTechnique)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::get_DefaultTechnique(IVoodooTechnique ** ppTechnique)
{
    if ( !m_Linked ) return VSFERR_NOT_LINKED;
    if ( ppTechnique == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTechnique = m_Default;
        (*ppTechnique)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::put_DefaultTechnique(IVoodooTechnique * pTechnique)
{
    if ( !m_Linked ) return VSFERR_NOT_LINKED;
    if ( pTechnique == NULL )
    {
        return E_INVALIDARG;
    } else {
        // Prevent cross-linking
        CComPtr<IVoodooShader> shader;
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
    if ( !m_Linked ) return VSFERR_NOT_LINKED;
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
    if ( !m_Linked ) return VSFERR_NOT_LINKED;
    if ( ppParameter == NULL )
    {
        return E_INVALIDARG;
    } else if ( Number < 0 || Number >= m_Parameters.GetCount() ) {
        return E_INVALIDARG;
    } else {
        *ppParameter = m_Parameters.GetAt(Number);
        (*ppParameter)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooShader::get_CgShader(VARIANT *ppCgShader)
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

STDMETHODIMP CVoodooShader::LinkShader()
{
    // Make sure it's a valid effect
    if ( !cgIsEffect(m_Effect) )
    {
        return VSFERR_INVALID_CG;
    }

    // Link parameters first
    CGparameter cgParameter = cgGetFirstEffectParameter(m_Effect);
    while ( cgIsParameter(cgParameter) )
    {
        IVoodooParameter * iParameter = CVoodooParameter::Create(this, cgParameter);

        if ( iParameter != NULL )
        {
            m_Parameters.Add(iParameter);
        }

        cgParameter = cgGetNextParameter(cgParameter);
    }

    // Validate and link techniques next
    CGtechnique cgTechnique = cgGetFirstTechnique(m_Effect);
    while ( cgIsTechnique(cgTechnique) )
    {
        IVoodooTechnique * iTechnique = CVoodooTechnique::Create(this, cgTechnique);

        if ( iTechnique != NULL )
        {
            if ( m_Default == NULL )
            {
                m_Default = iTechnique;
            }

            m_Techniques.Add(iTechnique);
        }

        cgTechnique = cgGetNextTechnique(cgTechnique);
    }

    return VSF_OK;
}