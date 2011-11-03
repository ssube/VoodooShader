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
    static const wchar_t * name_VSAdapter = L"VSAdapter";
    static const wchar_t * name_VSFileSystem = L"VSFileSystem";
    static const wchar_t * name_VSHookManager = L"VSHookManager";
    static const wchar_t * name_VSLogger = L"VSLogger";
    static const Uuid clsid_VSAdapter = CLSID_VSAdapter;
    static const Uuid clsid_VSFileSystem = CLSID_VSFileSystem;
    static const Uuid clsid_VSHookManager = CLSID_VSHookManager;
    static const Uuid clsid_VSLogger = CLSID_VSLogger;

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
            if (ppName) *ppName = name_VSAdapter;
            return &clsid_VSAdapter;
        case 1:
            if (ppName) *ppName = name_VSFileSystem;
            return &clsid_VSFileSystem;
        case 2:
            if (ppName) *ppName = name_VSHookManager;
            return &clsid_VSHookManager;
        case 3:
            if (ppName) *ppName = name_VSLogger;
            return &clsid_VSLogger;
        default:
            if (ppName) *ppName = nullptr;
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
