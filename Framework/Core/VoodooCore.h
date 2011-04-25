// VoodooCore.h : Declaration of the CVoodooCore

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <msxml2.h>

#include "Common.hpp"

using namespace ATL;

/**
 * @struct IVoodooCore
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooCore
 * @par GUID
 *    {6D183074-9127-4615-8366-02587BAFE38C}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Primary core interface for Voodoo, acts as a kind of meta-factory and provides all core
 *    functionality.
 *    
 * The Core handles most Voodoo tasks that are not better suited to a plugin, and provides most
 * major implementation details. It handles the general compilation and loading of shaders, and
 * provides a list of active textures and parameters for use in the shader linker.
 * 
 * @note Requires a Cg context be provided before shader-related functions will work.
 **/

/**
 * @class CVoodooCore
 * 
 * @par Interface
 *    @ref IVoodooCore
 * @par IID/CLSID
 *    @ref CLSID_VoodooCore
 * @par GUID
 *    {C50C9C7D-7E66-40ED-B5FD-BE7135DA40BF}
 * @par Attributes
 *    none
 * @par Version
 *    0.3
 *    
 * @brief 
 *    Core implementation of Voodoo (full implementation).
 *    
 * 
 */
class ATL_NO_VTABLE CVoodooCore :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooCore, &CLSID_VoodooCore>,
	public IDispatchImpl<IVoodooCore, &IID_IVoodooCore, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
	CVoodooCore();
    virtual ~CVoodooCore();

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
        /* [optional][in] */ const InitParams Params);

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
        /* [in] */ DWORD Type,
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
        /* [in] */ DWORD Stage,
        /* [retval][out] */ IVoodooTexture **ppTexture) ;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE SetStageTexture( 
        /* [in] */ DWORD Stage,
        /* [in] */ IVoodooTexture *pTexture) ;

    private:
        HRESULT STDMETHODCALLTYPE LoadConfig(BSTR pFile);

        bool m_Init;
        ULONG m_Refrs;

        /**
         * Base path void core was created with.
         */
        CComBSTR m_GlobalRoot;
        CComBSTR m_LocalRoot;
        CComBSTR m_RunRoot;
        CComBSTR m_Target;
        CComBSTR m_Loader;
        CComBSTR m_ConfigFile;

        /**
         * Config file (actually a <code>pugi::xml_document *</code>, stored as void).
         */
        IXMLDOMDocument * m_Config;

        /**
         * Cg context used by void core.
         */
        CGcontext m_Context;

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
