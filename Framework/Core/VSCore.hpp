/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

#include "VoodooInternal.hpp"

#pragma warning(push,3)
#include <map>
#include <set>
#pragma warning(pop)

namespace VoodooShader
{
    /**
     * Core-internal handle to core module.
     */
    extern HMODULE gCoreHandle;

    /**
     * ICore engine class for the Voodoo Shader Framework. Provides centralized management and handling for
     * shaders, textures, plugins and variable/configuration mechanics.
     *
     * @clsid e6f3129b-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSCore, ICore, ({0x9B, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
        typedef std::set<Functions::CallbackFunc> EventCallbacks;
        typedef std::map<Uuid, EventCallbacks> EventRegistry;

    public:
        VSCore(uint32_t version);
        ~VSCore();

        // IObject
        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut);
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        // ICore
        VOODOO_METHOD(Init)(_In_ const wchar_t * config);
        VOODOO_METHOD(Bind)(CompilerProfile profile, uint32_t count, _In_count_(count) Variant * pParams);
        VOODOO_METHOD(Reset)();

        VOODOO_METHOD(AddEvent)(Uuid event, Functions::CallbackFunc func);
        VOODOO_METHOD(DropEvent)(Uuid event, Functions::CallbackFunc func);
        VOODOO_METHOD(CallEvent)(Uuid event, uint32_t count, _In_opt_count_(count) Variant * pArgs);

        VOODOO_METHOD_(XmlDocument, GetConfig)() CONST;
        VOODOO_METHOD_(IFileSystem *, GetFileSystem)() CONST;
        VOODOO_METHOD_(IHookManager *, GetHookManager)() CONST;
        VOODOO_METHOD_(ILogger *, GetLogger)() CONST;
        VOODOO_METHOD_(IParser *, GetParser)() CONST;

        VOODOO_METHOD_(IEffect *, CreateEffect)(_In_ IFile * pFile); 
        VOODOO_METHOD_(IParameter *, CreateParameter)(_In_ const String & name, _In_ ParameterDesc desc);
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ const String & name, _In_ TextureDesc desc);
        VOODOO_METHOD_(ITexture *, LoadTexture)(_In_ const String & name, _In_ IFile * pFile);
        VOODOO_METHOD_(IParameter *, GetParameter)(_In_ const String & name, _In_ ParameterDesc desc) CONST;
        VOODOO_METHOD_(ITexture *, GetTexture)(_In_ const String & name) CONST;
        VOODOO_METHOD(RemoveParameter)(_In_ const String & name);
        VOODOO_METHOD(RemoveTexture)(_In_ const String & name);

    private:
        // Private these to prevent copying internally (external libs never will).
        VSCore(const VSCore & other);
        VSCore & operator=(const VSCore & other);

        mutable uint32_t m_Refs;
        uint32_t m_Version;

        /** Config file. */
        XmlDocument m_ConfigFile;

        /** The current IAdapter implementation. */
        BindingRef m_Binding;

        /** The current IFileSystem implementation. */
        FileSystemRef m_FileSystem;

        /** The current IHookManager implementation. */
        HookManagerRef m_HookManager;

        /** The current ILogger implementation. */
        LoggerRef m_Logger;

        /** The current module manager. */
        ModuleManagerRef m_ModuleManager;

        /** The current variable parser. */
        ParserRef m_Parser;

        /** Collection of all usable shaders. */
        EffectList m_Effects;

        /** Collection of all usable textures. */
        TextureMap m_Textures;

        /** Collection of all virtual parameters created by this pCore. */ 
        ParameterMap m_Parameters;

        /** Default pass target texture for shader linker. */
        TextureRef m_LastPass;

        /** Default technique target for shader linker. */
        TextureRef m_LastShader;

        // Events
        EventRegistry m_Events;
    };
}
