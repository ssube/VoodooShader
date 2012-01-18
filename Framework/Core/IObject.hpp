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
     * @class IObject
     *
     * Defines a simple interface all Voodoo objects from dynamic modules must inherit. This interface handles
     * destruction and identification of these objects.
     *
     * @iid e6f31287-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE_(IObject, ({0x87, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * Add a reference to this object.
         *
         * @return          The new reference count.
         *
         * @note This function is equivalent to COM's IUnknown::AddRef.
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        /**
         * Release a reference from this object.
         *
         * @return          The new reference count.
         *
         * @note This function is equivalent to COM's IUnknown::Release.
         */
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        /**
         * Performs type checking internally, then provides a pointer to this object with the desired interface if possible.
         *
         * @param   refid   IID of the interface desired.
         * @param   ppOut   A pointer to the new interface, passed as <code>void**</code>.
         */
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Deref_out_ IObject ** ppOut) PURE;
        /**
         * Get a string identifying this object. This must follow the form given in @ref voodoo_spec_iobject_tostring.
         *
         * @return The identifier.
         */
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        /**
         * Get the core this object was associated with.
         *
         * @return The core.
         */
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
    };
    /**
     * @}
     */
}
