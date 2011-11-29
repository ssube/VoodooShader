/*
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
     * Defines the interface for handling function-level hooks, redirecting existing functions and calls into new
     * locations. Each Core owns a HookManager implementation.
     * 
     * @par IID
     *      e6f3128c-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IHookManager, IObject, {0x8c, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;

        /**
         * Install a single hook at the specified point. This will only affect the process(es) the HookManager is bound to.
         *
         * @param name The name for the hook.
         * @param pSrc The point to install the hook at.
         * @param pDest The function to redirect execution into.
         * @return The success of the hook installation.
         *
         * @warning The calling convention of pSrc and pDest must be identical, or bad things might happen. This is only a
         *     bother with member functions but should be noted for all.
         */
        VOODOO_METHOD(Add)(_In_ const String & name, _In_ void * pSrc, _In_ void * pDest) PURE;
        /**
         * Removes a single hook.
         *
         * @param name The name of the hook to remove.
         * @return The success of the removal operation.
         *
         * @warning <em>Do not</em>, under any circumstances, remove a hook while execution is passing through the
         *     trampoline function. This can cause the process to crash. Until EasyHook is replaced, be careful!
         */
        VOODOO_METHOD(Remove)(_In_ const String & name) PURE;
        /**
         * Removes all hooks created with this HookManager.
         */
        VOODOO_METHOD(RemoveAll)() PURE;
    };
    /**
     * @}
     */
}
