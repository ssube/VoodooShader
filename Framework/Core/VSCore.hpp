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

        // IObject
        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut);
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        // ICore
        _Check_return_ VOODOO_METHOD(Init)(_In_z_ CONST wchar_t * config);
        _Check_return_ VOODOO_METHOD(Bind)(_In_ CONST CompilerProfile profile, _In_ CONST uint32_t count, _In_reads_(count) Variant * pParams);
        _Check_return_ VOODOO_METHOD(Reset)();

        VOODOO_METHOD(OnEvent)(_In_ CONST Uuid event, _In_ Functions::CallbackFunc func);
        VOODOO_METHOD(DropEvent)(_In_ CONST Uuid event, _In_ Functions::CallbackFunc func);
        VOODOO_METHOD(CallEvent)(_In_ CONST Uuid event, _In_ CONST uint32_t count, _In_reads_opt_(count) Variant * pArgs);

        VOODOO_METHOD_(XmlDocument, GetConfig)() CONST;
        VOODOO_METHOD_(IFileSystem *, GetFileSystem)() CONST;
        VOODOO_METHOD_(IHookManager *, GetHookManager)() CONST;
        VOODOO_METHOD_(ILogger *, GetLogger)() CONST;
        VOODOO_METHOD_(IParser *, GetParser)() CONST;

        VOODOO_METHOD_(IEffect *, CreateEffect)(_In_ IFile * pFile); 
        VOODOO_METHOD_(IParameter *, CreateParameter)(_In_ CONST String & name, _In_ CONST ParameterDesc desc);
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ CONST String & name, _In_ CONST TextureDesc desc);
        VOODOO_METHOD_(ITexture *, LoadTexture)(_In_ CONST String & name, _In_ IFile * pFile);
        VOODOO_METHOD_(IParameter *, GetParameter)(_In_ CONST String & name, _In_ CONST ParameterDesc desc) CONST;
        VOODOO_METHOD_(ITexture *, GetTexture)(_In_ CONST String & name) CONST;
        VOODOO_METHOD(RemoveParameter)(_In_ CONST String & name);
        VOODOO_METHOD(RemoveTexture)(_In_ CONST String & name);

    private:
        // Private these to prevent copying internally (external libs never will).
        VSCore(CONST VSCore & other);
        VSCore & operator=(CONST VSCore & other);
        ~VSCore();

        mutable uint32_t m_Refs;
        const uint32_t m_Version;

        /** The current variable parser. */
        ParserRef m_Parser;

        /** The current plugin server. */
        PluginServerRef m_Server;

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

        /** Collection of all usable shaders. */
        EffectList m_Effects;

        /** Collection of all usable textures. */
        TextureMap m_Textures;

        /** Collection of all virtual parameters created by this pCore. */ 
        ParameterMap m_Parameters;

        /** Event registry for core callbacks. */
        EventRegistry m_Events;
    };
}
