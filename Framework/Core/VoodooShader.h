// VoodooShader.h : Declaration of the CVoodooShader

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>



using namespace ATL;



// CVoodooShader

class ATL_NO_VTABLE CVoodooShader :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooShader, &CLSID_VoodooShader>,
	public IDispatchImpl<IVoodooShader, &IID_IVoodooShader, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooShader();
    virtual ~CVoodooShader();
    static IVoodooShader * Create(IVoodooCore * pCore, IVoodooFile * pFile);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOSHADER)

DECLARE_NOT_AGGREGATABLE(CVoodooShader)

BEGIN_COM_MAP(CVoodooShader)
	COM_INTERFACE_ENTRY(IVoodooShader)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooShader
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TechniqueCount( 
        /* [retval][out] */ INT * pCount);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTechnique( 
        /* [in] */ INT Number,
        /* [retval][out] */ IVoodooTechnique **ppTechnique);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DefaultTechnique( 
        /* [retval][out] */ IVoodooTechnique **ppTechnique);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DefaultTechnique( 
        /* [in] */ IVoodooTechnique *pTechnique);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ParameterCount( 
        /* [retval][out] */ INT *ppCount);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetParameter( 
        /* [in] */ INT Number,
        /* [retval][out] */ IVoodooParameter **ppParameter);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE get_CgShader( 
        /* [retval][out] */ VARIANT *ppCgShader);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    CArray<CComPtr<IVoodooTechnique>> m_Techniques;
    CComPtr<IVoodooTechnique> m_Default;
    CArray<CComPtr<IVoodooParameter>> m_Parameters;
    CGeffect m_Effect;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooShader), CVoodooShader)
