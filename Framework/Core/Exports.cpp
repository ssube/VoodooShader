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

    const wchar_t * VOODOO_CALL API_ClassInfo(_In_ const uint32_t number, _Out_ Uuid * pUuid)
    {
        if (!pUuid)
        {
            return nullptr;
        }

        switch (number)
        {
        case 0:
            *pUuid = clsid_VSAdapter;
            return name_VSAdapter;
        case 1:
            *pUuid = clsid_VSFileSystem;
            return name_VSFileSystem;
        case 2:
            *pUuid = clsid_VSHookManager;
            return name_VSHookManager;
        case 3:
            *pUuid = clsid_VSLogger;
            return name_VSLogger;
        default:
            return nullptr;
        }
    }

    IObject * VOODOO_CALL API_ClassCreate(_In_ const uint32_t number, _In_ ICore * pCore)
    {
        switch (number)
        {
        case 0:
            return new VSAdapter(pCore);
        case 1:
            return new VSFileSystem(pCore);
        case 2:
            return new VSHookManager(pCore);
        case 3:
            return new VSLogger(pCore);
        default:
            return nullptr;
        }
    }

    // Boost intrusive_ptr functions
    void VOODOO_PUBLIC_FUNC intrusive_ptr_add_ref(IObject * obj)
    {
#ifdef VOODOO_DEBUG_MEMORY
        uint32_t refs = obj->AddRef();
        if (obj && obj->GetCore() && obj->GetCore()->GetLogger())
        {
            obj->GetCore()->GetLogger()->Log(LL_CoreError, L"VOODOO_DEBUG_MEMORY", "intrusive_ptr_add_ref(%X) = %d", obj, refs);
        }
#else
        obj->AddRef();
#endif
    }

    void VOODOO_PUBLIC_FUNC intrusive_ptr_release(IObject * obj)
    {
#ifdef VOODOO_DEBUG_MEMORY
        if (obj && obj->GetCore() && obj->GetCore()->GetLogger())
        {
            obj->AddRef();
            uint32_t refs = obj->Release() - 1;
            obj->GetCore()->GetLogger()->Log(LL_CoreError, L"VOODOO_DEBUG_MEMORY", "intrusive_ptr_release(%X) = %d", obj, refs);
        }
#else
        obj->Release();
#endif
    }
}
