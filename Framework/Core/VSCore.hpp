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
     * Non-member function provided to Cg as an error handling callback.
     *
     * @param pContext The Cg context throwing the error.
     * @param error    The error code.
     * @param pCore    The core associated with the error, if any.
     */
    void Voodoo_CgErrorHandler_Func(_In_ CGcontext pContext, _In_ CGerror error, _In_opt_ void * pCore);

    /**
     * ICore engine class for the Voodoo Shader Framework. Provides centralized management and handling for
     * shaders, textures, plugins and variable/configuration mechanics.
     *
     * @clsid e6f3129b-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSCore, ICore, {0x9B, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
        friend void Voodoo_CgErrorHandler_Func(CGcontext, CGerror, void *);

    public:
        VSCore();
        ~VSCore();

        // IObject
        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        // ICore
        VOODOO_METHOD(Initialize)(_In_ const wchar_t * const config);
        VOODOO_METHOD(Reset)();
        VOODOO_METHOD_(IParser *, GetParser)() CONST;
        VOODOO_METHOD_(IHookManager *, GetHookManager)() CONST;
        VOODOO_METHOD_(IFileSystem *, GetFileSystem)() CONST;
        VOODOO_METHOD_(IAdapter *, GetAdapter)() CONST;
        VOODOO_METHOD_(ILogger *, GetLogger)() CONST;
        VOODOO_METHOD_(XmlDocument, GetConfig)() CONST;
        VOODOO_METHOD_(IShader *, CreateShader)(_In_ const IFile * const pFile, _In_opt_ const char ** ppArgs);
        VOODOO_METHOD_(IParameter *, CreateParameter)(_In_ const String & name, _In_ const ParameterType type);
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ const String & name, _In_ const TextureDesc Desc);
        VOODOO_METHOD_(IParameter *, GetParameter)(_In_ const String & name, _In_ const ParameterType type) CONST;
        VOODOO_METHOD_(ITexture *, GetTexture)(_In_ const String & name) CONST;
        VOODOO_METHOD(RemoveParameter)(_In_ const String & name);
        VOODOO_METHOD(RemoveTexture)(_In_ const String & name);
        VOODOO_METHOD_(ITexture *, GetStageTexture)(_In_ const TextureStage stage) CONST;
        VOODOO_METHOD_(void, SetStageTexture)(_In_ const TextureStage stage, _In_opt_ ITexture * const pTexture);
        _Check_return_ VOODOO_METHOD_(CGcontext, GetCgContext)() CONST;
        VOODOO_METHOD(SetCgContext)(_In_opt_ CGcontext const pContext);

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

#if defined(VOODOO_MEMORY_TRACK) || defined(VOODOO_MEMORY_DEBUG)
        ShaderList m_Dbg_Shaders;
        TextureList m_Dbg_Textures;
        ParameterList m_Dbg_Parameters;
#endif
    };
}
