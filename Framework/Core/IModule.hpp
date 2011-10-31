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
 */
#pragma once

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore 
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
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw();

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw();

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw();

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw();

        /**
         * Loads a set of modules from a given path. 
         * 
         * @param Path The path to load from (may be any Windows path type). Sent through the parser before use.
         * @param Filter A regex to filter filenames by. 
         * 
         * @note Only loads files whose filename matches the filter (standard regex match).
         */
        virtual bool LoadPath(_In_ const String & path, _In_ const String & filter);

        /**
         * Loads a single module, using an absolute or relative filename. 
         * 
         * @param Filename The file to load, sent through the parser before use.
         * 
         * @note When the module is loaded, if an absolute path is provided, the module's directory is used in the search
         *     path for required DLLs. 
         */
        virtual bool LoadFile(_In_ const IFile * pFile);

        virtual bool LoadFile(_In_ const String & filename);

        /**
         * Tests to see if a class exists in the list provided by all loaded modules. 
         */
        virtual bool ClassExists(_In_ const String & name) const;

        /**
         * Create a new instance of the given class. This object will be created in a dynamic module and a pointer given.
         * 
         * @param name The class name to create. 
         * @return New object or nullptr if the class wasn't found or couldn't be created.
         */
        _Check_return_ virtual IObject * CreateObject(_In_ const String & name) const;

        /**
         * Finds the address of a function in a module. The module must be loaded into the process and export the symbol, 
         * otherwise this will return nullptr. If the module name does not have an extension, ".dll" will be appended during 
         * the lookup. It is recommended you <em>do not</em> use an absolute path here. 
         * 
         * @param module The module name to look up.
         * @param name The function name to find.
         * @return The function's address if found, nullptr otherwise.
         */
        _Check_return_ virtual void * FindFunction(_In_ const String & module, _In_ const String & name) const;
    };

    /**
     * Contains the handle to a loaded library and function pointers for creation and destruction.
     */
    class IModule : 
        public IObject
    {
    public:
        virtual ~IModule(void) throw() {};
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw();

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw();

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw();

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw();

        /**
         * Get the current version of this module. @return The version, including name and
         * debug attribute.
         */
        virtual const Version * ModuleVersion(void) const throw();

        /** Get the class count from this module. */
        virtual const uint32_t ClassCount(void) const;

        virtual const char * ClassInfo(_In_ const uint32_t number) const;

        virtual IObject * CreateClass(_In_ const uint32_t number, _In_ ICore * pCore);
    };
    /* @} */
}
