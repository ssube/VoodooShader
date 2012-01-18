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

#include "VoodooFramework.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_interfaces
     * @{
     */

    /**
     * @class IModuleManager
     *
     * Provides a framework for loading and unloading modules in core, as well as retrieving symbols and modules
     * from memory.
     *
     * @iid e6f31290-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IModuleManager, IObject, ({0x90, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut) PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name Module Methods
         * @{
         */
        /**
         * Tests if the module with the given name is currently loaded.
         * 
         * @param name The module name to check.
         */
        VOODOO_METHOD_(bool, IsLoaded)(_In_ const String & name) CONST PURE;
        /**
         * Tests if the module with the given Uuid is currently loaded.
         * 
         * @param libid The module Uuid to check.
         */
        VOODOO_METHOD_(bool, IsLoaded)(_In_ const Uuid & libid) CONST PURE;
        /**
         * Loads a set of modules from a given path.
         *
         * @param path The path to load from (may be any Windows path type). Sent through the parser before use.
         * @param filter A regex to filter filenames by.
         *
         * @note Only loads files whose filename matches the filter (standard regex match).
         */
        VOODOO_METHOD(LoadPath)(_In_ const String & path, _In_ const String & filter) PURE;
        /**
         * Loads a single module, from an already-existing file.
         *
         * @param pFile The file to load, a file system reference.
         *
         * @note This always uses the module's directory in the search path for required DLLs.
         */
        VOODOO_METHOD(LoadFile)(_In_ const IFile * pFile) PURE;
        /**
         * Loads a single module, using an absolute or relative filename.
         *
         * @param filename The file to load, sent through the parser before use.
         *
         * @note If a relative path is provided, this will canonicalize that path relative to Voodoo's global root. For
         *      example, the call will be loaded as:
         *      @code
         *      Call: IModuleManager->LoadFile("../Library.dll");
         *      Absolute: Canonicalize("$(path)\\bin\\../Library.dll");
         *      Parsed: Canonicalize("C:\\Voodoo\\bin\\../Library.dll");
         *      Final: "C:\\Voodoo\\Library.dll";
         *      @endcode
         *      Among other things, this guarantees an absolute path for use when searching for dependencies.
         *
         * @note The module's directory is used in the search path for required DLLs. The search path, through this method,
         *      is:
         *      @li The directory given with the filename, or Voodoo's global root if a relative path was given.
         *      @li The target application's directory.
         *      @li The current directory.
         *      @li The system directory.
         *      @li The Windows directory.
         *      @li All directories listed in the PATH environment variable, in order.
         *
         */
        VOODOO_METHOD(LoadFile)(_In_ const String & filename) PURE;
        /**
         * @}
         * @name Class Methods
         * @{
         */
        /**
         * Tests to see if a class exists in the list provided by all loaded modules.
         *
         * @param clsid The class Uuid.
         * @return Existence of the class.
         */
        VOODOO_METHOD_(bool, ClassExists)(_In_ const Uuid refid) CONST PURE;
        /**
         * Tests to see if a class exists in the list provided by all loaded modules.
         *
         * This first attempts to convert the name string to a Uuid (String::ToUuid()), and failing that, looks up the name
         * in the internal map and checks for a corresponding Uuid. The name lookup is imprecise, only the last-loaded
         * class with a given name will be registered.
         *
         * @param name The class name.
         * @return Existence of the class.
         */
        VOODOO_METHOD_(bool, ClassExists)(_In_ const String & name) CONST PURE;
        /**
         * Create a new instance of the given class, using specific class Uuid. This requires a precise match and will fail
         * if no class is available.
         *
         * @param clsid The class Uuid to create.
         * @return New object or nullptr if the class wasn't found or couldn't be created.
         */
        _Check_return_ VOODOO_METHOD_(IObject *, CreateObject)(_In_ const Uuid refid) CONST PURE;
        /**
         * Create a new instance of the given class, using a class name or Uuid in string form.
         *
         * This first attempts to convert the name string to a Uuid (String::ToUuid()), and failing that, looks up the name
         * in the internal map and checks for a corresponding Uuid. The name lookup is imprecise, only the first-loaded
         * class with a given name will be registered.
         *
         * @param name The class name or uuid to create.
         * @return New object or nullptr if the class wasn't found or couldn't be created.
         *
         * @sa @ref String::String(const Uuid &) for String to Uuid conversion rules.
         */
        _Check_return_ VOODOO_METHOD_(IObject *, CreateObject)(_In_ const String & name) CONST PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
