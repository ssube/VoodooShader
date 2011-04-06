/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
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

#include "Meta.hpp"
#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * Defines the interface for handling function-level hooks, redirecting existing functions 
     * and calls into new locations. Each Core owns a HookManager implementation.
     */
    class IHookManager
        : public IObject
    {
    public:
        virtual ~IHookManager() throw()
        { }

        /**
         * Install a single hook at the specified point. This will only affect the
         * process(es) the HookManager is bound to.
         *
         * @param name The name for the hook.
         * @param src The point to install the hook at.
         * @param dest The function to redirect execution into.
         * @return The success of the hook installation.
         * @throws Exception if a hook with the same name already exists.
         *
         * @note The name is often the name of the function in src (&func) for
         *        simplicities sake. 
         * @warning The calling convention of src and dest must be identical, or bad
         *        things might happen. This is only a bother with member functions, but
         *        can be worked around relatively easily.
         */
        virtual bool CreateHook
        (
            _In_ std::string name, 
            _In_ void * src, 
            _In_ void * dest
        ) = 0;
        
        /**
         * Removes a single hook.
         *
         * @param name The name of the hook to remove.
         * @return The success of the removal operation.
         * @throws Exception of the hook is not found.
         * 
         * @warning <em>Do not</em>, under any circumstances, remove a hook while
         *        execution is passing through the trampoline function. This can cause
         *        the process to crash in rare cases. I'm not sure the reason, but it's
         *        not good. Until I replace EasyHook, be careful!
         */
        virtual bool RemoveHook
        (
            _In_ std::string name
        ) = 0;
        
        /**
         * Removes all hooks created with this HookManager.
         */
        virtual void RemoveAllHooks() = 0;
    };
}