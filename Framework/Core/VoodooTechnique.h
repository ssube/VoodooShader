// VoodooTechnique.h : Declaration of the CVoodooTechnique

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include "Common.hpp"

using namespace ATL;



// CVoodooTechnique

class ATL_NO_VTABLE CVoodooTechnique :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooTechnique, &CLSID_VoodooTechnique>,
	public IDispatchImpl<IVoodooTechnique, &IID_IVoodooTechnique, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooTechnique()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOTECHNIQUE)

DECLARE_NOT_AGGREGATABLE(CVoodooTechnique)

BEGIN_COM_MAP(CVoodooTechnique)
	COM_INTERFACE_ENTRY(IVoodooTechnique)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooTechnique
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Target( 
        /* [retval][out] */ IVoodooTexture **ppTexture);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PassCount( 
        /* [retval][out] */ INT *ppCount);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPass( 
        /* [in] */ INT Number,
        /* [retval][out] */ IVoodooPass **ppPass);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Shader( 
        /* [retval][out] */ IVoodooShader **ppShader);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCgTechnique( 
        /* [retval][out] */ VARIANT *ppPass);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    IVoodooTexture * m_Target;
    CArray<CComPtr<IVoodooPass>> m_Passes;
    IVoodooShader * m_Shader;
    CGtechnique m_Technique;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooTechnique), CVoodooTechnique)
