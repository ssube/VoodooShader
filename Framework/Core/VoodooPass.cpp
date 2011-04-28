// VoodooPass.cpp : Implementation of CVoodooPass

#include "stdafx.h"
#include "VoodooPass.h"

// CVoodooPass
CVoodooPass::CVoodooPass()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Target = NULL;
    m_Shader = NULL;
    m_Technique = NULL;
    m_Pass = NULL;
}

CVoodooPass::~CVoodooPass()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Target = NULL;
    m_Shader = NULL;
    m_Technique = NULL;
    m_Pass = NULL;
}

IVoodooPass * CVoodooPass::Create(IVoodooTechnique * pTechnique, CGpass Pass)
{
    if ( pTechnique == NULL || Pass == NULL || !cgIsPass(Pass) ) return NULL;

    CComPtr<CVoodooPass> ipPass = NULL;

    CComObject<CVoodooPass> * pPass = NULL;
    HRESULT hr = CComObject<CVoodooPass>::CreateInstance(&pPass);
    if ( SUCCEEDED(hr) )
    {
        pPass->AddRef();

        pPass->m_Pass = Pass;
        pPass->m_Technique = pTechnique;
        pTechnique->get_Shader(&pPass->m_Shader);
        pTechnique->get_Core(&pPass->m_Core);

        pPass->LinkPass();

        hr = pPass->QueryInterface(IID_IVoodooPass, (void**)&ipPass);
        pPass->Release();
    }

    return ipPass.Detach();
}

STDMETHODIMP CVoodooPass::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooPass ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooPass::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooPass::Release()
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

STDMETHODIMP CVoodooPass::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooPass::get_Core(IVoodooCore **ppCore)
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

STDMETHODIMP CVoodooPass::get_Target(IVoodooTexture **ppTexture)
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

STDMETHODIMP CVoodooPass::get_Shader(IVoodooShader **ppShader)
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

STDMETHODIMP CVoodooPass::get_Technique( 
    /* [retval][out] */ IVoodooTechnique **ppTechnique)
{
    if ( ppTechnique == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTechnique = m_Technique;
        (*ppTechnique)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooPass::GetCgProgram( 
    /* [in] */ DWORD Stage,
    /* [retval][out] */ VARIANT *ppProgram)
{
    CGprogram program = NULL;

    switch ( Stage )
    {
    case PS_Domain:
        program = cgGetPassProgram(m_Pass, CG_TESSELLATION_CONTROL_DOMAIN);
        break;
    case PS_Hull:
        program = cgGetPassProgram(m_Pass, CG_TESSELLATION_EVALUATION_DOMAIN);
        break;
    case PS_Vertex:
        program = cgGetPassProgram(m_Pass, CG_VERTEX_DOMAIN);
        break;
    case PS_Geometry:
        program = cgGetPassProgram(m_Pass, CG_GEOMETRY_DOMAIN);
        break;
    case PS_Fragment:
        program = cgGetPassProgram(m_Pass, CG_FRAGMENT_DOMAIN);
        break;
    default:
        return E_INVALIDARG;
    }

    if ( !cgIsProgram(program) )
    {
        return VSFERR_NO_PROGRAM;
    }

    V_VT(ppProgram) = VT_BYREF;
    V_BYREF(ppProgram) = (void*)program;
    return S_OK;
}

STDMETHODIMP CVoodooPass::get_CgPass( 
    /* [retval][out] */ VARIANT *ppPass)
{
    if ( ppPass == NULL )
    {
        return E_INVALIDARG;
    } else {
        V_VT(ppPass) = VT_BYREF;
        V_BYREF(ppPass) = (void*)m_Pass;
        return S_OK;
    }
}

STDMETHODIMP CVoodooPass::LinkPass()
{
    if ( !cgIsPass(m_Pass) )
    {
        return VSFERR_INVALID_CG;
    }

    CGannotation targetAnnotation = cgGetNamedPassAnnotation(m_Pass, "target");
    if ( cgIsAnnotation(targetAnnotation) && cgGetAnnotationType(targetAnnotation) == CG_STRING )
    {
        const char * targetName = cgGetStringAnnotationValue(targetAnnotation);
        CComBSTR targetbstr(targetName);

        m_Core->GetTexture(targetbstr, &m_Target);

        if ( m_Target == NULL )
        {
            m_Core->GetStageTexture(TS_Pass, &m_Target);
        }
    } else {
        /*mCore->GetLogger()->Log
        (
            LL_Debug,
            VOODOO_CORE_NAME,
            "Pass %s has no target annotation.", 
            this->GetName().c_str()
        );*/

        m_Core->GetStageTexture(TS_Pass, &m_Target);
    }

    // Load the programs
    IVoodooAdapter * adapter = NULL;
    m_Core->get_Adapter(&adapter);

    if ( adapter != NULL )
    {
        if ( FAILED(adapter->LoadPass(this)) )
        {
            //mCore->GetLogger()->Log(LL_Error, VOODOO_CORE_NAME, "Failed to load pass %s.", this->GetName().c_str());
            return VSFERR_LINKER_ERROR;
        } else {
            //mCore->GetLogger()->Log(LL_Info, VOODOO_CORE_NAME, "Successfully loaded pass %s.", this->GetName().c_str());
        }
    } else {
        /*mCore->GetLogger()->Log
        (
            LL_Warning,
            VOODOO_CORE_NAME,
            "No adapter found, pass %s must be explicitly loaded later.", 
            this->GetName().c_str()
        )*/;
        return VSFERR_NO_ADAPTER;
    }

    return VSF_OK;
}