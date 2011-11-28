/*
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
    /**
     * @addtogroup voodoo_uuids 
     */
    DEFINE_CLSID(VSCore) = {0x9B, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

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
     * 
     * @par CLSID
     *      e6f3129b-05af-11e1-9e05-005056c00008
     */
    class VSCore :
        public ICore
    {
        friend void Voodoo_CgErrorHandler_Func(CGcontext, CGerror, void *);

    public:
        VSCore();
        virtual ~VSCore();

        // IObject
        VOODOO_METHODCALL_(uint32_t, AddRef)() CONST;
        VOODOO_METHODCALL_(uint32_t, Release)() CONST;
        VOODOO_METHODCALL(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHODCALL_(String, ToString)() CONST;
        VOODOO_METHODCALL_(ICore *, GetCore)() CONST;
        
        // ICore
        VOODOO_METHODCALL(Initialize)(_In_ const InitParams * const pInitParams);
        VOODOO_METHODCALL(Reset)();
        VOODOO_METHODCALL_(IParser *, GetParser)() CONST;
        VOODOO_METHODCALL_(IHookManager *, GetHookManager)() CONST;
        VOODOO_METHODCALL_(IFileSystem *, GetFileSystem)() CONST;
        VOODOO_METHODCALL_(IAdapter *, GetAdapter)() CONST;
        VOODOO_METHODCALL_(ILogger *, GetLogger)() CONST;
        VOODOO_METHODCALL_(XmlDocument, GetConfig)() CONST;
        VOODOO_METHODCALL_(IShader *, CreateShader)(_In_ const IFile * const pFile, _In_opt_ const char ** ppArgs);
        VOODOO_METHODCALL_(IParameter *, CreateParameter)(_In_ const String & name, _In_ const ParameterType type);
        VOODOO_METHODCALL_(ITexture *, CreateTexture)(_In_ const String & name, _In_ const TextureDesc * const pDesc);
        VOODOO_METHODCALL_(IParameter *, GetParameter)(_In_ const String & name, _In_ const ParameterType type) CONST;
        VOODOO_METHODCALL_(ITexture *, GetTexture)(_In_ const String & name) CONST;
        VOODOO_METHODCALL(RemoveParameter)(_In_ const String & name);
        VOODOO_METHODCALL(RemoveTexture)(_In_ const String & name);
        VOODOO_METHODCALL_(ITexture *, GetStageTexture)(_In_ const TextureStage stage) CONST;
        VOODOO_METHODCALL_(void, SetStageTexture)(_In_ const TextureStage stage, _In_opt_ ITexture * const pTexture);
        _Check_return_ VOODOO_METHODCALL_(CGcontext, GetCgContext)() CONST;
        VOODOO_METHODCALL(SetCgContext)(_In_opt_ CGcontext const pContext);

    private:
        void CgErrorHandler(_In_ CGcontext pContext, _In_ CGerror error) const;

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
