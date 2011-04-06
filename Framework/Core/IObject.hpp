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

#ifndef VOODOO_OBJECT_HPP
#define VOODOO_OBJECT_HPP

#include "Meta.hpp"
#include "Module.hpp"

namespace VoodooShader
{
    /**
     * Defines a simple interface all Voodoo objects from dynamic modules must inherit. This
     * interface handles destruction and identification of these objects.
     * 
     * @warning All classes derived from IObject <b><em>must</em></b> have unique names.
     */
    class IObject
    {
        friend IObject * ModuleManager::CreateClass( _In_ String name );

    public:
        /**
         * Base virtual destructor for all object classes. This throws when called, and so
         * must be overridden.
         */
        virtual ~IObject() throw()
        { };

        /**
         * Get the name of this object's class. May be used for identification or validation
         * purposes.
         * 
         * @note The returned string may be compared, but must not be deleted or modified.
         */
        virtual const char * GetObjectClass() = 0;

    private:
        /**
         * A shared pointer to the module this class is defined in. Keeping 
         * this in the class forces the module to remain in memory until the last 
         * class from it is destroyed. This prevents class methods from becoming invalid.
         */
        ModuleRef mSourceModule;
    };
}

#endif /*VOODOO_OBJECT_HPP*/