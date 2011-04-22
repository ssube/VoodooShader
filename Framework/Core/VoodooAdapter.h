// VoodooAdapter.h : Declaration of the CVoodooAdapter

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include "Common.hpp"

using namespace ATL;



// CVoodooAdapter

class ATL_NO_VTABLE CVoodooAdapter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooAdapter, &CLSID_VoodooAdapter>,
	public IDispatchImpl<IVoodooAdapter, &IID_IVoodooAdapter, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooAdapter();
    virtual ~CVoodooAdapter();
    static IVoodooAdapter * Create(IVoodooCore * pCore);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOADAPTER)

DECLARE_NOT_AGGREGATABLE(CVoodooAdapter)

BEGIN_COM_MAP(CVoodooAdapter)
	COM_INTERFACE_ENTRY(IVoodooAdapter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooAdapter
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE LoadPass( 
        /* [in] */ IVoodooPass *pPass);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnloadPass( 
        /* [in] */ IVoodooPass *pPass);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Pass( 
        /* [retval][out] */ IVoodooPass **ppPass);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Pass( 
        /* [in] */ IVoodooPass *pPass);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Target( 
        /* [retval][out] */ IVoodooTexture **ppTarget);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Target( 
        /* [in] */ IVoodooTexture *pTarget);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE DrawGeometry( 
        /* [in] */ int Vertexes,
        /* [in] */ SAFEARRAY *pVertexData);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE ApplyParameter( 
        /* [in] */ IVoodooParameter *pParameter);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE DrawShader( 
        /* [in] */ IVoodooShader *pShader);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateTexture( 
        /* [in] */ BSTR pName,
        /* [in] */ TextureDesc Description,
        /* [retval][out] */ IVoodooTexture **ppTexture);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE LoadTexture( 
        /* [in] */ IVoodooImage *pImage,
        TextureRegion Region,
        /* [retval][out] */ IVoodooTexture **ppTexture);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE ConnectTexture( 
        /* [in] */ IVoodooParameter *pParameter,
        /* [in] */ IVoodooTexture *pTexture);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE HandleError( 
        /* [in] */ VARIANT pContext,
        /* [in] */ DWORD Error,
        /* [in] */ IVoodooCore *pCore);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooAdapter), CVoodooAdapter)
