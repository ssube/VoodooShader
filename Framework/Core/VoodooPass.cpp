// VoodooPass.cpp : Implementation of CVoodooPass

#include "stdafx.h"
#include "VoodooPass.h"

#include "Common.hpp"


// CVoodooPass
ULONG m_Refrs;
CComBSTR m_Name;
IVoodooCore * m_Core;
IVoodooTexture * m_Target;
IVoodooShader * m_Shader;
IVoodooTechnique * m_Technique;
CGpass m_Pass;

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

        hr = pPass->QueryInterface(IID_IVoodooTechnique, (void**)&ipPass);
        pPass->Release();
    }

    return ipPass.Detach();
}

STDMETHODIMP CVoodooPass::QueryInterface(REFIID iid, void ** pp) throw()
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
        return S_OK;
    }
}

STDMETHODIMP CVoodooPass::GetCgProgram( 
    /* [in] */ ProgramStage Stage,
    /* [retval][out] */ VARIANT *ppProgram)
{
    CGprogram program = NULL;

    switch ( Stage )
    {
    case Domain:
        program = cgGetPassProgram(m_Pass, CG_TESSELLATION_CONTROL_DOMAIN);
        break;
    case Hull:
        program = cgGetPassProgram(m_Pass, CG_TESSELLATION_EVALUATION_DOMAIN);
        break;
    case Vertex:
        program = cgGetPassProgram(m_Pass, CG_VERTEX_DOMAIN);
        break;
    case Geometry:
        program = cgGetPassProgram(m_Pass, CG_GEOMETRY_DOMAIN);
        break;
    case Fragment:
        program = cgGetPassProgram(m_Pass, CG_FRAGMENT_DOMAIN);
        break;
    default:
        return E_INVALIDARG;
    }

    if ( !cgIsProgram(program) )
    {
        return E_NOPROGRAM;
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