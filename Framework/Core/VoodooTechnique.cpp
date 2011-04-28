// VoodooTechnique.cpp : Implementation of CVoodooTechnique

#include "stdafx.h"
#include "VoodooTechnique.h"
#include "VoodooPass.h"

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

        const char * cgname = cgGetTechniqueName(pTechnique);
        if ( cgname )
        {
            name.Append(cgname);
        } else {
            CStringW pname;
            pname.Format(L"%p", pTechnique);
            name.Append(pname);
        }

        pcTechnique->m_Name = name;

        pShader->get_Core(&pcTechnique->m_Core);
        pcTechnique->m_Shader = pShader;
        pcTechnique->m_Technique = pTechnique;

        pcTechnique->LinkTechnique();

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
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooTechnique ) {
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
        (*ppCore)->AddRef();
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
        (*ppTexture)->AddRef();
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
        (*ppPass)->AddRef();
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
        (*ppShader)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooTechnique::get_CgTechnique(VARIANT *ppPass)
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

STDMETHODIMP CVoodooTechnique::LinkTechnique()
{
    if ( !cgIsTechnique(m_Technique) )
    {
        return VSFERR_INVALID_CG;
    }

    CGbool valid = cgValidateTechnique(m_Technique);

    if ( valid == CG_FALSE )
    {
        /*m_Core->GetLogger()->Log
        (
            LL_Warning,
            VOODOO_CORE_NAME,
            "Technique failed to validate: %s.",
            cgGetTechniqueName(cTech)
        );*/
        return VSFERR_INVALID_TECH;
    }

    /*m_Core->GetLogger()->Log
    (
        LL_Debug,
        VOODOO_CORE_NAME,
        "Validated technique %s.",
        cgGetTechniqueName(cTech)
    );*/

    // Process the technique's target annotation
    CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(m_Technique, "target");

    if ( cgIsAnnotation(targetAnnotation) && cgGetAnnotationType(targetAnnotation) == CG_STRING )
    {
        const char * targetName = cgGetStringAnnotationValue(targetAnnotation);
        CComBSTR targetbstr(targetName);

        m_Core->GetTexture(targetbstr, &m_Target);

        if ( m_Target == NULL )
        {
            /*mCore->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Pass %s cannot find target %s.",
                this->GetName().c_str(), targetName
            );*/

            m_Core->GetStageTexture(TS_Shader, &m_Target);
        }
    } else {
        /*mCore->GetLogger()->Log
        (
            LL_Debug,
            VOODOO_CORE_NAME,
            "Pass %s has no target annotation.",
            this->GetName().c_str()
        );*/

        m_Core->GetStageTexture(TS_Shader, &m_Target);
    }

    CGpass cgPass = cgGetFirstPass(m_Technique);

    while ( cgIsPass(cgPass) )
    {
        // Insert the pass into the vector
        IVoodooPass * iPass = CVoodooPass::Create(this, cgPass);

        if ( iPass != NULL )
        {
            m_Passes.Add(iPass);
        }

        cgPass = cgGetNextPass(cgPass);
    }

    return VSF_OK;
}