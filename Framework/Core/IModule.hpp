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

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_framework_interfaces
     * @{
     */

    /**
     * Provides a framework for loading and unloading modules in core, as well as retrieving symbols and modules 
     * from memory.
     */
    class IModuleManager : 
        public IObject
    {
    public:
        virtual ~IModuleManager(void) throw() {};

        virtual uint32_t AddRef(void) const throw() = 0;
        virtual uint32_t Release(void) const throw() = 0;
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw() = 0;
        virtual String ToString(void) const throw() = 0;
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Loads a set of modules from a given path. 
         * 
         * @param path The path to load from (may be any Windows path type). Sent through the parser before use.
         * @param filter A regex to filter filenames by. 
         * 
         * @note Only loads files whose filename matches the filter (standard regex match).
         */
        virtual bool LoadPath(_In_ const String & path, _In_ const String & filter) = 0;

        /**
         * Loads a single module, using an absolute or relative filename. 
         * 
         * @param pFile The file to load, a file system reference.
         * 
         * @note This always uses the module's directory in the search path for required DLLs. 
         */
        virtual bool LoadFile(_In_ const IFile * pFile) = 0;
        
        /**
         * Loads a single module, using an absolute or relative filename. 
         * 
         * @param filename The file to load, sent through the parser before use.
         * 
         * @note When the module is loaded, if an absolute path is provided, the module's directory is used in the search
         *     path for required DLLs. 
         */
        virtual bool LoadFile(_In_ const String & filename) = 0;
        
        /**
         * Tests to see if a class exists in the list provided by all loaded modules. 
         * 
         * @param clsid The class UUID.
         */
        virtual bool ClassExists(_In_ const Uuid & clsid) const = 0;

        /**
         * Tests to see if a class exists in the list provided by all loaded modules.
         * 
         * This first attempts to convert the name string to a UUID (String::ToUuid()), and failing that, looks up the name
         * in the internal map and checks for a corresponding UUID. The name lookup is imprecise, only the first-loaded
         * class with a given name will be registered.
         * 
         * @param name The class name.
         */
        virtual bool ClassExists(_In_ const String & name) const = 0;

        /**
        * Create a new instance of the given class, using specific class UUID. This requires a precise match and will fail
        * if no class is available.
        * 
        * @param name The class UUID to create. 
        * @return New object or nullptr if the class wasn't found or couldn't be created.
         */
        _Check_return_ virtual IObject * CreateObject(_In_ const Uuid & clsid) const = 0;

        /**
         * Create a new instance of the given class, using a class name or UUID in string form. 
         * 
         * This first attempts to convert the name string to a UUID (String::ToUuid()), and failing that, looks up the name
         * in the internal map and checks for a corresponding UUID. The name lookup is imprecise, only the first-loaded
         * class with a given name will be registered.
         * 
         * @param name The class name or uuid to create. 
         * @return New object or nullptr if the class wasn't found or couldn't be created.
         * 
         * @sa @ref String::String(const Uuid &) for String to Uuid conversion rules.
         */
        _Check_return_ virtual IObject * CreateObject(_In_ const String & name) const = 0;

        /**
         * Finds the address of a function in a module. The module must be loaded into the process and export the symbol, 
         * otherwise this will return nullptr. If the module name does not have an extension, ".dll" will be appended during 
         * the lookup. It is recommended you <em>do not</em> use an absolute path here. 
         * 
         * @param module The module name to look up.
         * @param name The function name to find.
         * @return The function's address if found, nullptr otherwise.
         */
        _Check_return_ virtual void * FindFunction(_In_ const String & module, _In_ const String & name) const = 0;
    };

    /**
     * Contains the handle to a loaded library and function pointers for creation and destruction.
     */
    class IModule : 
        public IObject
    {
    public:
        virtual ~IModule(void) throw() {};
        
        virtual uint32_t AddRef(void) const throw() = 0;
        virtual uint32_t Release(void) const throw() = 0;
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw() = 0;
        virtual String ToString(void) const throw() = 0;
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Get the current version of this module. @return The version, including name and
         * debug attribute.
         */
        virtual const Version * ModuleVersion(void) const throw() = 0;

        /** Get the class count from this module. */
        virtual const uint32_t ClassCount(void) const = 0;

        virtual const Uuid * ClassInfo(_In_ const uint32_t number, _Deref_out_ const wchar_t ** name) const = 0;

        virtual IObject * CreateClass(_In_ const uint32_t number, _In_ ICore * pCore) = 0;
    };
    /**
     * @}
     */
}
