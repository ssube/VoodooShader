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

#include "Exports.hpp"

#include "Version.hpp"

#include "VSAdapter.hpp"
#include "VSFileSystem.hpp"
#include "VSHookManager.hpp"
#include "VSLogger.hpp"

namespace VoodooShader
{
    static const Version coreVersion = VOODOO_META_VERSION_STRUCT(CORE);
    static const Uuid clsid_vsa = CLSID_VSAdapter;
    static const Uuid clsid_vsf = CLSID_VSFileSystem;
    static const Uuid clsid_vsh = CLSID_VSHookManager;
    static const Uuid clsid_vsl = CLSID_VSLogger;

    const Version * VOODOO_CALL API_ModuleVersion(void)
    {
        return &coreVersion;
    }

    const uint32_t VOODOO_CALL API_ClassCount(void)
    {
        return 4;
    }

    const Uuid * VOODOO_CALL API_ClassInfo(_In_ const uint32_t number, _Deref_out_opt_ const wchar_t ** ppName)
    {
        switch (number)
        {
        case 0:
            *ppName = L"VSAdapter";
            return &clsid_vsa;
        case 1:
            *ppName = L"VSFileSystem";
            return &clsid_vsf;
        case 2:
            *ppName = L"VSHookManager";
            return &clsid_vsh;
        case 3:
            *ppName = L"VSLogger";
            return &clsid_vsl;
        default:
            *ppName = nullptr;
            return nullptr;
        }
    }

    IObject * VOODOO_CALL API_ClassCreate(_In_ const uint32_t number, _In_ ICore * core)
    {
        switch (number)
        {
        case 0:
            return new VSAdapter(core);
        case 1:
            return new VSFileSystem(core);
        case 2:
            return new VSHookManager(core);
        case 3:
            return new VSLogger(core);
        default:
            return nullptr;
        }
    }

    // Boost intrusive_ptr functions
    void VOODOO_PUBLIC_FUNC intrusive_ptr_add_ref(IObject * obj)
    {
        obj->AddRef();
    }

    void VOODOO_PUBLIC_FUNC intrusive_ptr_release(IObject * obj)
    {
        obj->Release();
    }
}
