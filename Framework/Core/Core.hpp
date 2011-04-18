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

#ifndef VOODOO_CORE_HPP
#define VOODOO_CORE_HPP

#include "Meta.hpp"

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
        /**
         * Create a new Voodoo Core and initializes appropriate paths.
         *
         * @note You can not call this function externally, you must call CoCreate instead.
         */
        Core();

        /**
         * Releases all references to modules and objects held by the core, including shaders,
         * textures, parameters, etc. This may cause the destruction of objects and unloading
         * of modules. It should not invalidate loaded resources that are held in other locations.
         */
        ~Core();

        // IUnknown Methods
        STDMETHOD(QueryInterface)(REFIID iid, void ** pp);
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();

        STDMETHOD(Initialize)(BSTR pConfig);
        STDMETHOD(GetParser)(IVoodooParser ** pParser);
        STDMETHOD(GetHookManager)(IVoodooHookSystem ** ppHookManager);
        STDMETHOD(GetFileSystem)(IVoodooFileSystem ** ppFileSystem);
        STDMETHOD(GetAdapter)(IVoodooAdapter ** ppAdapter);
        STDMETHOD(GetLogger)(IVoodooLogger ** ppLogger);

        /**
         * Retrieve the Xml config document for this Core.
         * 
         * @note This is actually a <code>pugi::xml_document *</code>, but stored and provided
         *    as a <code>void *</code> so linking against the Core doesn't require the pugixml
         *    headers. To use this, simply cast it into the actual type.
         */
        STDMETHOD(GetConfig)(void ** ppConfig);

        STDMETHOD(SetCgContext)(void * pContext);
        STDMETHOD(GetCgContext)(void ** ppContext);

        STDMETHOD(CreateShader)(IVoodooFile * pFile, BSTR * ppArgs, IVoodooShader ** ppShader);
        STDMETHOD(CreateParameter)(BSTR pName, ParameterType Type, IVoodooParameter ** ppParameter);
        STDMETHOD(GetParameter)(BSTR pName, IVoodooParameter ** ppParameter);
        STDMETHOD(AddTexture)(BSTR pName, void * pData);
        STDMETHOD(GetTexture)(BSTR pName, IVoodooTexture ** ppTexture);
        STDMETHOD(RemoveTexture)(BSTR pName);
        STDMETHOD(GetStageTexture)(TextureType Stage, IVoodooTexture ** ppTexture);
        STDMETHOD(SetStageTexture)(TextureType Stage, IVoodooTexture * pTexture);

    private:
        /**
         * Error handling callback for the Cg context. If an internal Cg error occurs, this function 
         * will be called with as much information as possible. While error recovery may not be 
         * possible, this does log the error in detail (if a core is provided).
         * 
         * @param context The Cg context the error occurred in.
         * @param error The error code.
         * @param core The core to use for error logging, if one was provided the Cg context
         *    during creation. This may be NULL.
         */
        static void CgErrorHandler
        (
            _In_ CGcontext context, 
            _In_ CGerror error, 
            _In_opt_ void * core
        );

    private:
        UINT mRefrs;

        /**
         * Base path this core was created with.
         */
        CComBSTR mGlobalRoot;
        CComBSTR mLocalRoot;
        CComBSTR mRunRoot;
        CComBSTR mTarget;

        /**
         * Config file (actually a <code>pugi::xml_document *</code>, stored as void).
         */
        void * mConfig;

        /**
         * Cg context used by this core.
         */
        CGcontext mCgContext;

        /**
         * The currently bound (active) IAdapter implementation.
         */
        IVoodooAdapter * mAdapter;

        /**
         * The current ILogger implementation.
         */
        IVoodooLogger * mLogger;

        /**
         * The current IHookManager implementation.
         */
        IVoodooHookSystem * mHookSystem;

        /**
         * The current IFileSystem implementation.
         */
        IVoodooFileSystem * mFileSystem;

        /**
         * The current variable parser.
         */
        IVoodooParser * mParser;

        std::map<std::string, IVoodooParameter*> mParameters;
        std::map<std::string, IVoodooTexture*> mTextures;
        std::map<TextureType, IVoodooTexture*> mStageTextures;
    };
    /**
     * @}
     */
}

#endif
