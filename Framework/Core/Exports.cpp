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

#include "Exports.hpp"

#include "Null.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    Version VOODOO_CALL API_ModuleVersion(void)
    {
        Version coreVersion = VOODOO_META_VERSION_STRUCT(CORE);

        return coreVersion;
    }

    int32_t VOODOO_CALL API_ClassCount(void)
    {
        return 4;
    }

    String VOODOO_CALL API_ClassInfo(_In_ int number)
    {
        switch (number)
        {
        case 0:
            return "NullAdapter";
        case 1:
            return "NullLogger";
        case 2:
            return "NullFileSystem";
        case 3:
            return "NullHookManager";
        default:
            return String();
        }
    }

    IObject * VOODOO_CALL API_ClassCreate(_In_ int number, _In_ ICore * core)
    {
        switch (number)
        {
        case 0:
            return new VoodooNull::NullAdapter(core);
        case 1:
            return new VoodooNull::NullLogger(core);
        case 2:
            return new VoodooNull::NullFileSystem(core);
        case 3:
            return new VoodooNull::NullHookManager(core);
        default:
            return nullptr;
        }
    }

    // Boost intptr funcs
    void intrusive_ptr_add_ref(IObject * obj)
    {
        obj->AddRef();
    }

    void intrusive_ptr_release(IObject * obj)
    {
        obj->Release();
    }
}
