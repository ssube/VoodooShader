// VoodooPass.h : Declaration of the CVoodooPass

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include "Common.hpp"

using namespace ATL;



// CVoodooPass

class ATL_NO_VTABLE CVoodooPass :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooPass, &CLSID_VoodooPass>,
	public IDispatchImpl<IVoodooPass, &IID_IVoodooPass, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooPass();
    virtual ~CVoodooPass();
    static IVoodooPass * Create(IVoodooTechnique * pTechnique, CGpass Pass);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPASS)

DECLARE_NOT_AGGREGATABLE(CVoodooPass)

BEGIN_COM_MAP(CVoodooPass)
	COM_INTERFACE_ENTRY(IVoodooPass)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooPass
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Target( 
        /* [retval][out] */ IVoodooTexture **ppTexture);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Shader( 
        /* [retval][out] */ IVoodooShader **ppShader);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Technique( 
        /* [retval][out] */ IVoodooTechnique **ppTechnique);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCgProgram( 
        /* [in] */ ProgramStage Stage,
        /* [retval][out] */ VARIANT *ppProgram);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE get_CgPass( 
        /* [retval][out] */ VARIANT *ppPass);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    IVoodooTexture * m_Target;
    IVoodooShader * m_Shader;
    IVoodooTechnique * m_Technique;
    CGpass m_Pass;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooPass), CVoodooPass)
