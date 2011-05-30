/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

#include "Common.hpp"

using namespace ATL;

/**
 * @struct IVoodooCore
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooCore
 * @par GUID
 *    {1d3d7f86-6f32-11e0-8ac0-005056c00000}
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
 *    {1d3d7fa3-6f32-11e0-8ac0-005056c00000}
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
    virtual HRESULT STDMETHODCALLTYPE Initialize(const InitParams Params);
    virtual HRESULT STDMETHODCALLTYPE get_Parser(IVoodooParser **ppParser);
    virtual HRESULT STDMETHODCALLTYPE get_HookSystem(IVoodooHookSystem **ppHookSystem);
    virtual HRESULT STDMETHODCALLTYPE get_FileSystem(IVoodooFileSystem **ppFileSystem);
    virtual HRESULT STDMETHODCALLTYPE get_Adapter(IVoodooAdapter **ppAdapter);
    virtual HRESULT STDMETHODCALLTYPE get_Logger(IVoodooLogger **ppLogger);
    virtual HRESULT STDMETHODCALLTYPE get_Config(IUnknown **ppConfig);
    virtual HRESULT STDMETHODCALLTYPE get_CgContext(VARIANT * pCgContext);
    virtual HRESULT STDMETHODCALLTYPE put_CgContext(VARIANT pCgContext);
    virtual HRESULT STDMETHODCALLTYPE CreateShader(IVoodooFile *pFile, SAFEARRAY * pArgs, IVoodooShader **ppShader);
    virtual HRESULT STDMETHODCALLTYPE CreateParameter(BSTR pName, DWORD Type, IVoodooParameter **ppParameter);
    virtual HRESULT STDMETHODCALLTYPE CreateTexture(BSTR pName, TextureDesc Desc, VARIANT Data, IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE GetParameter(BSTR pName, IVoodooParameter **ppParameter);
    virtual HRESULT STDMETHODCALLTYPE GetTexture(BSTR pName, IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE RemoveParameter(BSTR pName);
    virtual HRESULT STDMETHODCALLTYPE RemoveTexture(BSTR pName);
    virtual HRESULT STDMETHODCALLTYPE GetStageTexture(DWORD Stage, IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE SetStageTexture(DWORD Stage, IVoodooTexture *pTexture);

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
     * Config file.
     */
    CComPtr<IXMLDOMDocument> m_Config;

    /**
     * Cg context used by void core.
     */
    CGcontext m_Context;

    /**
     * The currently bound (active) IAdapter implementation.
     */
    CComPtr<IVoodooAdapter> m_Adapter;

    /**
     * The current ILogger implementation.
     */
    CComPtr<IVoodooLogger> m_Logger;

    /**
     * The current IHookManager implementation.
     */
    CComPtr<IVoodooHookSystem> m_HookSystem;

    /**
     * The current IFileSystem implementation.
     */
    CComPtr<IVoodooFileSystem> m_FileSystem;

    /**
     * The current variable parser.
     */
    CComPtr<IVoodooParser> m_Parser;

    CMap<BSTR, BSTR, CComPtr<IVoodooParameter>, CComPtr<IVoodooParameter>> m_Parameters;
    CMap<BSTR, BSTR, CComPtr<IVoodooTexture>, CComPtr<IVoodooTexture>> m_Textures;
    CMap<TextureStage, TextureStage, CComPtr<IVoodooTexture>, CComPtr<IVoodooTexture>> m_StageTextures;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooCore), CVoodooCore)
