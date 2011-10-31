/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 **/
#pragma once

#include "ICore.hpp"

namespace VoodooShader
{
    /**
     * Non-member function provided to Cg as an error handling callback. 
     * 
     * @param context The Cg context throwing the error. 
     * @param error   The error code. 
     * @param core    If non-nullptr, the core associated with the error
     */
    void Voodoo_CgErrorHandler_Func(_In_ CGcontext pContext, _In_ CGerror error, _In_opt_ void * pCore);

    /**
     * ICore engine class for the Voodoo Shader Framework. Provides centralized management and handling for
     * shaders, textures, plugins and variable/configuration mechanics.
     */
    class VSCore : 
        public ICore
    {
        friend void Voodoo_CgErrorHandler_Func(CGcontext, CGerror, void *);

    public:
        VSCore(_In_ const InitParams * const pInitParams);
        virtual ~VSCore(void);
        
        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual IParser * GetParser(void) const;
        virtual IHookManager * GetHookManager(void) const;
        virtual IFileSystem * GetFileSystem(void) const;
        virtual IAdapter * GetAdapter(void) const;
        virtual ILogger * GetLogger(void) const;
        virtual XmlDocument GetConfig(void) const;

        virtual IShader * CreateShader(_In_ const IFile * const pFile, _In_opt_ const char ** ppArgs);
        virtual IParameter * CreateParameter(_In_ const String & name, _In_ const ParameterType type);
        virtual ITexture * CreateTexture(_In_ const String & name, _In_ const TextureDesc * const pDesc);
        virtual IParameter * GetParameter(_In_ const String & name, _In_ const ParameterType type) const;
        virtual ITexture * GetTexture(_In_ const String & name) const;
        virtual bool RemoveParameter(_In_ const String & name);
        virtual bool RemoveTexture(_In_ const String & name);

        virtual ITexture * GetStageTexture(_In_ const TextureStage stage) const;
        virtual void SetStageTexture(_In_ const TextureStage stage, _In_ ITexture * const pTexture);

        _Check_return_ virtual CGcontext GetCgContext(void) const;
        virtual bool SetCgContext(_In_opt_ CGcontext const pContext);

    private:    
        void CgErrorHandler(_In_ CGcontext pContext, _In_ int error) const;

    private:
        mutable uint32_t m_Refs;

        /** Config file. */
        XmlDocument m_ConfigFile;

        /** Cg context used by this pCore. */
        CGcontext m_CgContext;

        /** The current IAdapter implementation. */
        IAdapterRef m_Adapter;

        /** The current ILogger implementation. */
        ILoggerRef m_Logger;

        /** The current IHookManager implementation. */
        IHookManagerRef m_HookManager;

        /** The current IFileSystem implementation. */
        IFileSystemRef m_FileSystem;

        /** The current module manager. */
        IModuleManagerRef m_ModuleManager;

        /** The current variable parser. */
        IParserRef m_Parser;

        /** Collection of all shaders created by this pCore. */
        ShaderVector m_Shaders;

        /** Collection of all usable textures. */
        TextureMap m_Textures;

        /** Collection of all virtual parameters created by this pCore and used by the Cg context. */
        ParameterMap m_Parameters;

        /** Default pass target texture for shader linker. */
        ITextureRef m_LastPass;

        /** Default technique target for shader linker. */
        ITextureRef m_LastShader;
    };
}
