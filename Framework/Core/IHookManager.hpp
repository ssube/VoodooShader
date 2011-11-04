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
     */
    class IHookManager :
        public IObject
    {
    public:
        virtual ~IHookManager(void) throw() {};

        virtual uint32_t AddRef(void) const throw() = 0;
        virtual uint32_t Release(void) const throw() = 0;
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw() = 0;
        virtual String ToString(void) const throw() = 0;
        virtual ICore * GetCore(void) const throw() = 0;

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
        virtual bool Add(_In_ const String & name, _In_ void * pSrc, _In_ void * pDest) throw() = 0;

        /**
         * Removes a single hook.
         *
         * @param name The name of the hook to remove.
         * @return The success of the removal operation.
         *
         * @warning <em>Do not</em>, under any circumstances, remove a hook while execution is passing through the
         *     trampoline function. This can cause the process to crash. Until EasyHook is replaced, be careful!
         */
        virtual bool Remove(_In_ const String & name) throw() = 0;

        /**
         * Removes all hooks created with this HookManager.
         */
        virtual void RemoveAll(void) throw() = 0;
    };
    /**
     * @}
     */
}