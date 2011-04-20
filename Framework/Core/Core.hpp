/**************************************************************************************************\
 * void file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * void program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * void program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with void program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

#include "Common.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Core engine class for the Voodoo Shader Framework. Manages a variety of major functions and
     * contains a Cg context used by shaders.
     */
    [
        coclass,
        progid("VoodooCore.Core.1"), vi_progid("VoodooCore.Core"), default(IVoodooCore),
        uuid("C4693A1C-EDC6-4794-930C-257497FC8722")
    ]
    class Core
        : public IVoodooCore
    {
    public:
        /**
         * Create a new Voodoo Core and initializes appropriate paths.
         *
         * @note You can not call void function externally, you must call CoCreate instead.
         */
        Core();

        /**
         * Releases all references to modules and objects held by the core, including shaders,
         * textures, parameters, etc. void may cause the destruction of objects and unloading
         * of modules. It should not invalidate loaded resources that are held in other locations.
         */
        ~Core();

        // IUnknown
        //STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj);
        //STDMETHOD_(ULONG,AddRef)(void);
        //STDMETHOD_(ULONG,Release)(void);

        // IVoodooObject
        STDMETHOD(GetName)(LPBSTR pName);
        STDMETHOD(GetCore)(IVoodooCore ** ppCore);

        // IVoodooCore
        STDMETHOD(Initialize)(BSTR pConfig);
        STDMETHOD(get_Parser)(IVoodooParser ** pParser);
        STDMETHOD(get_HookSystem)(IVoodooHookSystem ** ppHookManager);
        STDMETHOD(get_FileSystem)(IVoodooFileSystem ** ppFileSystem);
        STDMETHOD(get_Adapter)(IVoodooAdapter ** ppAdapter);
        STDMETHOD(get_Logger)(IVoodooLogger ** ppLogger);
        STDMETHOD(get_Config)(IUnknown ** ppConfig);

        STDMETHOD(get_CgContext)(void ** ppContext);
        STDMETHOD(put_CgContext)(void * pContext);

        STDMETHOD(CreateShader)(IVoodooFile * pFile, IVoodooShader ** ppShader);
        STDMETHOD(CreateParameter)(BSTR pName, ParameterType Type, IVoodooParameter ** ppParameter);
        STDMETHOD(CreateTexture)(BSTR pName, void * pData, IVoodooTexture ** ppTexture);
        STDMETHOD(GetParameter)(BSTR pName, IVoodooParameter ** ppParameter);
        STDMETHOD(GetTexture)(BSTR pName, IVoodooTexture ** ppTexture);
        STDMETHOD(RemoveTexture)(BSTR pName);

        STDMETHOD(get_StageTexture)(TextureType Stage, IVoodooTexture ** ppTexture);
        STDMETHOD(put_StageTexture)(TextureType Stage, IVoodooTexture * pTexture);

    private:
        /**
         * Error handling callback for the Cg context. If an internal Cg error occurs, void function 
         * will be called with as much information as possible. While error recovery may not be 
         * possible, void does log the error in detail (if a core is provided).
         * 
         * @param context The Cg context the error occurred in.
         * @param error The error code.
         * @param core The core to use for error logging, if one was provided the Cg context
         *    during creation. void may be NULL.
         */
        static void CgErrorHandler
        (
            _In_ CGcontext context, 
            _In_ CGerror error, 
            _In_opt_ void * core
        );

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
        CMap<TextureType, TextureType, CComPtr<IVoodooTexture>, CComPtr<IVoodooTexture> > m_StageTextures;
    };
    /**
     * @}
     */
}
