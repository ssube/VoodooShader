// VoodooCore.h : Declaration of the CVoodooCore

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <msxml2.h>

#include "Common.hpp"

using namespace ATL;

// CVoodooCore
class ATL_NO_VTABLE CVoodooCore :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooCore, &CLSID_VoodooCore>,
	public IDispatchImpl<IVoodooCore, &IID_IVoodooCore, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooCore()
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

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOCORE)

DECLARE_NOT_AGGREGATABLE(CVoodooCore)

BEGIN_COM_MAP(CVoodooCore)
	COM_INTERFACE_ENTRY(IVoodooCore)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooCore
public:    
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Initialize( 
        /* [optional][in] */ VARIANT pConfig) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Parser( 
        /* [retval][out] */ IVoodooParser **ppParser) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HookSystem( 
        /* [retval][out] */ IVoodooHookSystem **ppHookSystem) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FileSystem( 
        /* [retval][out] */ IVoodooFileSystem **ppFileSystem) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Adapter( 
        /* [retval][out] */ IVoodooAdapter **ppAdapter) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Logger( 
        /* [retval][out] */ IVoodooLogger **ppLogger) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Config( 
        /* [retval][out] */ IXMLDOMDocument **ppConfig) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CgContext( 
        /* [retval][out] */ VARIANT * pCgContext) ;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_CgContext( 
        /* [in] */ VARIANT pCgContext) ;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateShader( 
        /* [in] */ IVoodooFile *pFile,
        SAFEARRAY * pArgs,
        /* [retval][out] */ IVoodooShader **ppShader) ;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateParameter( 
        /* [in] */ BSTR pName,
        /* [in] */ ParameterType Type,
        /* [retval][out] */ IVoodooParameter **ppParameter) ;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateTexture( 
        /* [in] */ BSTR pName,
        /* [in] */ VARIANT Data,
        /* [retval][out] */ IVoodooTexture **ppTexture) ;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetParameter( 
        /* [in] */ BSTR pName,
        /* [retval][out] */ IVoodooParameter **ppParameter) ;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTexture( 
        /* [in] */ BSTR pName,
        /* [retval][out] */ IVoodooTexture **ppTexture) ;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveTexture( 
        /* [in] */ BSTR pName) ;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE GetStageTexture( 
        /* [in] */ TextureStage Stage,
        /* [retval][out] */ IVoodooTexture **ppTexture) ;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE SetStageTexture( 
        /* [in] */ TextureStage Stage,
        /* [in] */ IVoodooTexture *pTexture) ;

    private:
        UINT m_Refrs;

        /**
         * Base path void core was created with.
         */
        CComBSTR m_GlobalRoot;
        CComBSTR m_LocalRoot;
        CComBSTR m_RunRoot;
        CComBSTR m_Target;

        /**
         * Config file (actually a <code>pugi::xml_document *</code>, stored as void).
         */
        IXMLDOMDocument * m_Config;

        /**
         * Cg context used by void core.
         */
        void * m_Context;

        /**
         * The currently bound (active) IAdapter implementation.
         */
        IVoodooAdapter * m_Adapter;

        /**
         * The current ILogger implementation.
         */
        IVoodooLogger * m_Logger;

        /**
         * The current IHookManager implementation.
         */
        IVoodooHookSystem * m_HookSystem;

        /**
         * The current IFileSystem implementation.
         */
        IVoodooFileSystem * m_FileSystem;

        /**
         * The current variable parser.
         */
        IVoodooParser * m_Parser;

        CMap<BSTR, BSTR, CComPtr<IVoodooParameter>, CComPtr<IVoodooParameter> > m_Parameters;
        CMap<BSTR, BSTR, CComPtr<IVoodooTexture>, CComPtr<IVoodooTexture> > m_Textures;
        CMap<TextureStage, TextureStage, CComPtr<IVoodooTexture>, CComPtr<IVoodooTexture> > m_StageTextures;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooCore), CVoodooCore)
