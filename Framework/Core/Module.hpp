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
            _In_ String name,
            _In_ bool prepend = false
        );

        _Check_return_
        IObject * CreateClass
        (
            _In_ String name
        );

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
        Functions::CountFunc   mClassCount;
        Functions::InfoFunc    mClassInfo;  
        Functions::CreateFunc  mClassCreate;
     };
}

#endif /*VOODOO_MODULEMANAGER_HPP*/