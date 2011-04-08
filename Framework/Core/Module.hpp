/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube *
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

#ifndef VOODOO_MODULEMANAGER_HPP
#define VOODOO_MODULEMANAGER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Provides a clean framework for loading and unloading modules in core, as
     * well as retrieving symbols and modules from memory. 
     */
    class VOODOO_API ModuleManager
    {
    public:
        ModuleManager
        (
            _In_ Core * core
        );

        ~ModuleManager();

        /**
         * Loads a module into process memory if it is not present. The module must meet the 
         * @ref modulespec "module specifications" and export all needed symbols. If the module 
         * does not export these, it will fail to load (they are checked during the load process).
         * 
         * @note Modules are loaded with a call to @p LoadModuleEx, and the search path is altered 
         *    so that the module's directory will be searched for any required modules (if a full 
         *    path is provided).
         *       
         * @note If the module name begins with the characters ".\", the Voodoo binary path will 
         *    replace that ("$(globalroot)\\bin\\") and the module loaded from that location. If an 
         *    absolute path is provided, that will be used. Otherwise, the module will be loaded by 
         *    filename using the Windows default search path.
         *       
         * @note After the module is loaded, it will not be unloaded until it is unloaded from this 
         *    ModuleManager @em and the last class created from it is destroyed. Both this object 
         *    and all IObject-derived classes contain shared pointers to the module.
         *       
         * @param name The filename of the module to load.
         * @return A shared pointer to the Module, or an empty shared pointer if loading failed.
         */
        ModuleRef LoadModule
        (
            _In_ String name
        );

        /**
         * Attempts to unload the given module. This removes the reference from the module list,
         * then checks if any outstanding handles remain (typically help by objects created from
         * the module). If no handles remain, the module is fully unloaded. Otherwise, you must
         * assume the module is still loaded. 
         * 
         * @param name The module to unload (this may have ".\" prepended, as with LoadModule()).
         * @return Whether the module has been unloaded or not.
         * @warning If this does not return true, the module is likely still in memory and the
         *    file locked.
         */
        bool UnloadModule
        (
            _In_ String name
        );

        /**
         * Tests to see if a class exists in the list provided by all loaded modules.
         */
        bool ClassExists
        (
            _In_ String name
        );

        /**
         * Wraps ModuleManager::CreateClass to return a shared pointer of a
         * specific type. The type <em>must</em> be derived from IObject.
         */
        template<typename T>
        std::shared_ptr<T> SharedCreateClass
        (
            _In_ String name
        )
        {
            IObject * object = this->CreateClass(name);

            return std::shared_ptr<T>((T*)object);
        }

        /**
         * Create a new instance of the given class. This object will be created in a
         * dynamic module and a pointer given. It should be cast to the actual type
         * before being used.
         * 
         * @param name The class name to create.
         * @return New object or null if the class wasn't found or couldn't be created.
         */
        _Check_return_
        IObject * CreateClass
        (
            _In_ String name
        );

        /**
         * Finds the address of a function in a module. The module must be loaded into
         * the process and export the symbol, otherwise this will return null. If the
         * module name does not have an extension, ".dll" will be appended during the
         * lookup. It is recommended you <em>do not</em> use an absolute path here.
         * 
         * @param module The module name to look up.
         * @param name The function name to find.
         * @return The function's address if found, null otherwise.
         */
        _Check_return_
        void * FindFunction
        (
            _In_ String module,
            _In_ String name
        );

    private:
        Core * mCore;
        ModuleMap mModules;
        ClassMap mClasses;
    };

    /**
     * Contains the handle to a loaded library and function pointers for
     * creation and destruction.
     */
     class VOODOO_API Module
     {
     public:
         /**
          * Attempt to load a module and locate all necessary functions. This will only succeed for
          * modules meeting the @ref modulespec, otherwise the module will fail to load.
          * 
          * @param path The path to the module (may be relative or absolute). 
          * @return A new Module object if the module loads successfully, null otherwise.
          * @note If an absolute path is used, the directory portion will be searched for
          *    dependencies. The underlying load mechanism is 
          *    <code>LoadLibraryEx(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)</code>.
          */
        static Module * Load
        (
            _In_ String path
        );

        Module
        (
            _In_ HMODULE hmodule
        );

        ~Module();

        /**
         * Get the current version of this module.
         * 
         * @return The version, including name and debug attribute.
         */
        Version ModuleVersion();
        
        /**
         * Get the class count from this module.
         */
        int ClassCount();

        const char * ClassInfo
        (
            _In_ int number
        );

        IObject * CreateClass
        (
            _In_ int number,
            _In_ Core * core
        );

     private:
        bool mOwned;
        HMODULE mHandle;
        Functions::VersionFunc mModuleVersion;
        Functions::CountFunc   mClassCount;
        Functions::InfoFunc    mClassInfo;  
        Functions::CreateFunc  mClassCreate;
     };
    /**
     * @}
     */
}

#endif /*VOODOO_MODULEMANAGER_HPP*/