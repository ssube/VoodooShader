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

#include "VoodooFramework.hpp"
#include "VoodooInternal.hpp"
// Voodoo Core
#include "VSCore.hpp"
#include "VSFileSystem.hpp"
#include "VSHookManager.hpp"
#include "VSLogger.hpp"

EXTERN_C BOOL WINAPI DllMain(HINSTANCE hinstDLL,  DWORD fdwReason, LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        VoodooShader::gCoreHandle = hinstDLL;
    }

    return TRUE;
}    

namespace VoodooShader
{
    CONST Version * VOODOO_CALLTYPE API_PluginInit(_In_ ICore * pCore)
    {
        UNREFERENCED_PARAMETER(pCore);

        static CONST Version coreVersion = VOODOO_VERSION_STRUCT(CORE);

        return &coreVersion;
    }

    void VOODOO_CALLTYPE API_PluginReset(_In_ ICore * pCore)
    {
        UNREFERENCED_PARAMETER(pCore);
    }

    CONST uint32_t VOODOO_CALLTYPE API_ClassCount()
    {
        return 4;
    }

	CONST wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ CONST uint32_t index, _Out_ Uuid * pUuid )
	{
        static CONST wchar_t * name_VSCore = VSTR("VSCore");
        static CONST wchar_t * name_VSFileSystem = VSTR("VSFileSystem");
        static CONST wchar_t * name_VSHookManager = VSTR("VSHookManager");
        static CONST wchar_t * name_VSLogger = VSTR("VSLogger");

        if (!pUuid)
        {
            return nullptr;
        }

        switch (index)
        {
        case 0:
            *pUuid = CLSID_VSCore;
            return name_VSCore;
        case 1:
            *pUuid = CLSID_VSFileSystem;
            return name_VSFileSystem;
        case 2:
            *pUuid = CLSID_VSHookManager;
            return name_VSHookManager;
        case 3:
            *pUuid = CLSID_VSLogger;
            return name_VSLogger;
        default:
			*pUuid = VoodooShader::NilUuid;
            return nullptr;
        }
    }

    IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ CONST uint32_t number, _In_ ICore * pCore)
    {
        switch (number)
        {
        case 0:
            return nullptr; //new VSCore(nullptr);
        case 1:
            return new VSFileSystem(pCore);
        case 2:
            return new VSHookManager(pCore);
        default:
            return nullptr;
        }
    }

    // Boost intrusive_ptr functions
    VOODOO_FUNCTION(void, intrusive_ptr_add_ref)(IObject * obj)
    {
#if defined(VOODOO_DEBUG_MEMORY)
        uint32_t refs = obj->AddRef();
        if (obj && obj->GetCore() && obj->GetCore()->GetLogger())
        {
            obj->GetCore()->GetLogger()->Log(VSLog_CoreError, VSTR("VOODOO_DEBUG_MEMORY"), VSTR("intrusive_ptr_add_ref(%p) = %d"), obj, refs);
        }
#else
        obj->AddRef();
#endif
    }

    VOODOO_FUNCTION(void, intrusive_ptr_release)(IObject * obj)
    {
#if defined(VOODOO_DEBUG_MEMORY)
        if (obj && obj->GetCore() && obj->GetCore()->GetLogger())
        {
            obj->AddRef();
            uint32_t refs = obj->Release() - 1;
            obj->GetCore()->GetLogger()->Log(VSLog_CoreError, VSTR("VOODOO_DEBUG_MEMORY"), VSTR("intrusive_ptr_release(%p) = %d"), obj, refs);
        }
#else
        obj->Release();
#endif
    }
}
