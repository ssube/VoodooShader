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
     * @class IModule
     *
     * Contains the handle to a loaded library and function pointers for creation and destruction.
     *
     * @iid e6f3128f-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IModule, IObject, ({0x8f, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
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
         * @name Module Interface Methods
         * @{
         */
        /**
         * Get the current version of this module.
         */
        VOODOO_METHOD_(const Version *, ModuleVersion)() CONST PURE;
        /**
         * Get the number of classes this module publicly provides. 
         */
        VOODOO_METHOD_(uint32_t, ClassCount)() CONST PURE;
        /**
         * Get the name and ClsId of a class, by index.
         * 
         * @param number The class index in this module.
         * @param pUuid The destination Uuid buffer.
         */
        VOODOO_METHOD_(const wchar_t *, ClassInfo)(_In_ const uint32_t number, _Out_ Uuid * pUuid) CONST PURE;
        VOODOO_METHOD_(IObject *, CreateClass)(_In_ const uint32_t number, _In_ ICore * pCore) CONST PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
