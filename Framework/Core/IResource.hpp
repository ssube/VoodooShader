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
     * @class IResource
     *
     * Describes a graphics API-backed resource, managed by the core or adapter.
     *
     * @iid     e6f31297-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IResource, IObject, ({0x97, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         */
        /**
         * Returns the name of the object, as defined by the object and resource type. This may be texture
         * name or filename, shader filename, technique name, pass name, etc.
         */
        VOODOO_METHOD_(String, GetName)() CONST PURE;
        /**
         * Returns a resource property, an API-defined variant stored in the resource.
         *
         * @param name      The name of the tag to get.
         * @param pValue    The to be filled
         */
        VOODOO_METHOD(GetProperty)(const String & name, _In_ Variant * pValue) CONST PURE;
        /**
         * Sets a resource property.
         * 
         * @param value     The value to store in the tag.
         */
        VOODOO_METHOD(SetProperty)(const String & name, _In_ const Variant & value) CONST PURE;
    };
    /**
     * @}
     */
}
