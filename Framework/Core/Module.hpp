/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * While the Voodoo Shader Framework, as a whole, is licensed under the GNU General Public license,
 * adapters designed to work with the framework do not constitute derivative works. In the case an
 * adapter needs to use this file (Adapter.hpp) or Meta.hpp in order to work with the Framework, and
 * is not licensed under the GNU GPL or a compatible license, please contact the developer to figure
 * out alternate licensing arrangements.
 *
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

        ModuleRef LoadModule
        (
            _In_ String name
        );

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
        String mBasePath;
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
        static Module * Load
        (
            _In_ String path
        );

        Module
        (
            _In_ HMODULE hmodule
        );

        ~Module();

        Version ModuleVersion();
        
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