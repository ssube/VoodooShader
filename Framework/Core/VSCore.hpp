/**
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
#pragma once

#include "ICore.hpp"

namespace VoodooShader
{
    /* VSCore: e6f3129b-05af-11e1-9e05-005056c00008 */
    DEFINE_UUID_IMPL(VSCore, 0x9B, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);

    /**
     * Non-member function provided to Cg as an error handling callback.
     *
     * @param pContext The Cg context throwing the error.
     * @param error    The error code.
     * @param pCore    If non-nullptr, the core associated with the error
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
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
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
