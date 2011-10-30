/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
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
     * @addtogroup VoodooCore 
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

        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

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

        /**
         * Install a single hook at the specified point. This will only affect the process(es) the HookManager is bound to. 
         * 
         * @param Name The name for the hook.
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
