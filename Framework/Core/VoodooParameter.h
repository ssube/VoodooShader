// VoodooParameter.h : Declaration of the CVoodooParameter

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <atlsafe.h>

#include "Common.hpp"

using namespace ATL;



// CVoodooParameter

class ATL_NO_VTABLE CVoodooParameter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooParameter, &CLSID_VoodooParameter>,
	public IDispatchImpl<IVoodooParameter, &IID_IVoodooParameter, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooParameter();
    virtual ~CVoodooParameter();
    static IVoodooParameter * Create(IVoodooCore * pCore, BSTR pName, ParameterType Type);
    static IVoodooParameter * Create(IVoodooShader * pShader, CGparameter Parameter);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPARAMETER)

DECLARE_NOT_AGGREGATABLE(CVoodooParameter)

BEGIN_COM_MAP(CVoodooParameter)
	COM_INTERFACE_ENTRY(IVoodooParameter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooParameter
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
        /* [retval][out] */ ParameterType *pType);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Virtual( 
        /* [retval][out] */ BOOL *pVirtual);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE AttachParameter( 
        /* [in] */ IVoodooParameter *pParameter);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Components( 
        /* [retval][out] */ int *pComponents);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SamplerValue( 
        /* [retval][out] */ IVoodooTexture **ppTexture);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_SamplerValue( 
        /* [in] */ IVoodooTexture *pTexture);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ScalarValue( 
        /* [retval][out] */ SAFEARRAY * *ppData);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ScalarValue( 
        /* [in] */ SAFEARRAY * pData);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE get_Shader( 
        /* [retval][out] */ IVoodooShader **ppShader);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE get_CgParameter( 
        /* [retval][out] */ VARIANT *ppCgParameter);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    IVoodooShader * m_Shader;
    ParameterType m_Type;
    BOOL m_Virtual;
    CComPtr<IVoodooTexture> m_Texture;
    CComSafeArray<float> m_Data;
    CGparameter m_Parameter;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooParameter), CVoodooParameter)
