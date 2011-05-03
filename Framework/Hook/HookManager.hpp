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

#include <string>

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

namespace VoodooShader
{
    namespace EasyHook
    {
        /**
         * Classes and functions related to the EasyHook-based implementation of IHookManager.
         * 
         * @addtogroup VoodooEHHook Voodoo/EHHook
         * @{
         */
        typedef std::map<std::string, void*> HookMap;

        Version API_ModuleVersion();

        int API_ClassCount();

        const char * API_ClassInfo
        (
            _In_ int number
        );

        IObject * API_ClassCreate
        (
            _In_ int number, 
            _In_ Core * core
        );

        /**
         * Handles function-level hooks, redirecting function calls into new locations. 
         */
        class HookManager
            : public VoodooShader::IHookManager
        {
        public:
            /**
             * Creates a new HookManager bound to the current process.
             */
            HookManager
            (
                _In_ Core * core
            );
        
            /**
             * Removes all hooks and cleans up the HookManager.
             */
            ~HookManager();

            const char * GetObjectClass();
        
            bool CreateHook
            (
                _In_ std::string name, 
                _In_ void * src, 
                _In_ void * dest
            );
        
            
            bool RemoveHook
            (
                _In_ std::string name
            );
        
            /**
             * Removes all hooks created with this HookManager.
             */
            void RemoveAllHooks();

        private:
            Core * mCore;
            HookMap mHooks;
            unsigned long * mThreadIDs;
            unsigned long mThreadCount;
        };
        /**
         * @}
         */
    };
}
