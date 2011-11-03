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

#include "Includes.hpp"
#include "String.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_framework_interfaces
     * @{
     */

    /**
     * Defines a simple interface all Voodoo objects from dynamic modules must inherit. This interface handles 
     * destruction and identification of these objects. 
     */
    class IObject
    {
    public:
        virtual ~IObject(void) throw() {};

        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         * 
         * @note This function is equivalent to COM's IUnknown::AddRef.
         */
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         * 
         * @note This function is equivalent to COM's IUnknown::Release.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Performs type checking internally, then provides a pointer to this object with the desired interface if possible.
         * 
         * @param clsid The interface Uuid desired.
         * @param ppOut A pointer to the new interface, passed as <code>void**</code>.
         * @return Success of the cast or uuid-fill.
         * 
         * @note If ppOut is a nullptr and clsid is a null Uuid (all zeros), clsid will be filled with the interface this 
         *      class implements.
         */
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;
    };
    /**
     * @}
     */
}
