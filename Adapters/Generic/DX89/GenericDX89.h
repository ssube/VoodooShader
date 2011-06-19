// GenericDX89.h : Declaration of the CGenericDX89

#pragma once
#include "DX89_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;

// CGenericDX89

class ATL_NO_VTABLE CGenericDX89 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGenericDX89, &CLSID_GenericDX89>,
	public IDispatchImpl<IVoodooAdapter, &IID_IVoodooAdapter, &LIBID_DX89Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CGenericDX89();
    ~CGenericDX89();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GENERICDX89)

DECLARE_NOT_AGGREGATABLE(CGenericDX89)

BEGIN_COM_MAP(CGenericDX89)
	COM_INTERFACE_ENTRY(IVoodooAdapter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooAdapter
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE LoadPass(IVoodooPass *pPass);
    virtual HRESULT STDMETHODCALLTYPE UnloadPass(IVoodooPass *pPass);
    virtual HRESULT STDMETHODCALLTYPE get_Pass(IVoodooPass **ppPass);
    virtual HRESULT STDMETHODCALLTYPE put_Pass(IVoodooPass *pPass);
    virtual HRESULT STDMETHODCALLTYPE get_Target(IVoodooTexture **ppTarget);
    virtual HRESULT STDMETHODCALLTYPE put_Target(IVoodooTexture *pTarget);
    virtual HRESULT STDMETHODCALLTYPE get_Light(int Index, LightStruct *pLight);
    virtual HRESULT STDMETHODCALLTYPE put_Light(int Index, LightStruct Light);
    virtual HRESULT STDMETHODCALLTYPE SetProperty(BSTR pName, VARIANT *pData);
    virtual HRESULT STDMETHODCALLTYPE DrawGeometry(int Vertexes, VertexStruct pVertexData[]);
    virtual HRESULT STDMETHODCALLTYPE ApplyParameter(IVoodooParameter *pParameter);
    virtual HRESULT STDMETHODCALLTYPE DrawShader(IVoodooShader *pShader);
    virtual HRESULT STDMETHODCALLTYPE CreateTexture(BSTR pName, TextureDesc Description, IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE LoadTexture(IVoodooImage *pImage, TextureRegion Region, IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE ConnectTexture(IVoodooParameter *pParameter, IVoodooTexture *pTexture);
    virtual HRESULT STDMETHODCALLTYPE HandleError(VARIANT pContext, DWORD Error, IVoodooCore *pCore);

private:
    ULONG m_Refrs;
    BOOL m_Init;
    CComPtr<IVoodooCore> m_Core;
    CComPtr<IVoodooLogger> m_Logger;
    CGcontext m_Context;

    // D3D and V3D objects/devices
    CComPtr<IVoodoo3D8> m_FakeObject;
    CComPtr<IDirect3D8> m_RealObject;
    CComPtr<IVoodoo3DDevice8> m_FakeDevice;
    CComPtr<IDirect3DDevice8> m_RealDevice;

    CComPtr<IVoodooPass> m_CurrentPass;
    CComPtr<IVoodooTexture> m_CurrentTarget;

    LightStruct m_Lights[8];
};

OBJECT_ENTRY_AUTO(__uuidof(GenericDX89), CGenericDX89)
