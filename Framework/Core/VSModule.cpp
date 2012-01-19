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

#include "VSModule.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSModule
    DeclareDebugCache();

    VSModule * VSModule::Load(_In_ ICore * const pCore, _In_ const String & path)
    {
        // Load the module
        HMODULE hmodule = LoadLibraryEx(path.GetData(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if (hmodule != nullptr)
        {
            VSModule * module = new VSModule(pCore, hmodule);

            module->m_ModuleInit = reinterpret_cast<Functions::ModuleInitFunc>(GetProcAddress(hmodule, "ModuleInit"));
            module->m_ClassCount = reinterpret_cast<Functions::ModuleCountFunc>(GetProcAddress(hmodule, "ClassCount"));
            module->m_ClassInfo = reinterpret_cast<Functions::ModuleInfoFunc>(GetProcAddress(hmodule, "ClassInfo"));
            module->m_ClassCreate = reinterpret_cast<Functions::ModuleCreateFunc>(GetProcAddress(hmodule, "ClassCreate"));

            if
            (
                module->m_ModuleInit == nullptr ||
                module->m_ClassCount == nullptr ||
                module->m_ClassInfo == nullptr ||
                module->m_ClassCreate == nullptr
            )
            {
                delete module;
                return nullptr;
            }
            else
            {
                return module;
            }
        }
        else
        {
            return nullptr;
        }
    }

    VOODOO_METHODTYPE VSModule::VSModule(_In_ ICore * pCore, HMODULE hmodule) :
        m_Refs(0), m_Core(pCore), m_Handle(hmodule)
    {
        AddThisToDebugCache();
    }

    VOODOO_METHODTYPE VSModule::~VSModule()
    {
        RemoveThisFromDebugCache();

        if (m_Handle != INVALID_HANDLE_VALUE)
        {
            FreeLibrary(m_Handle);
            m_Handle = nullptr;
        }
    }

    uint32_t VOODOO_METHODTYPE VSModule::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSModule::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    VoodooResult VOODOO_METHODTYPE VSModule::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ IObject ** ppOut)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            return VSFERR_INVALIDPARAMS;
        }
        else
        {
            if (refid == IID_IObject)
            {
                *ppOut = static_cast<IObject*>(this);
            }
            else if (refid == IID_IModule)
            {
                *ppOut = static_cast<IModule*>(this);
            }
            else if (refid == CLSID_VSModule)
            {
                *ppOut = static_cast<VSModule*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return VSFERR_INVALIDUUID;
            }

            (*ppOut)->AddRef();
            return VSF_OK;
        }
    }

    String VOODOO_METHODTYPE VSModule::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSTR("VSModule()");
    }

    ICore * VOODOO_METHODTYPE VSModule::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Core;
    }

    const Version * VOODOO_METHODTYPE VSModule::ModuleVersion() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_ModuleInit();
    }

    uint32_t VOODOO_METHODTYPE VSModule::ClassCount() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_ClassCount();
    }

    const wchar_t * VOODOO_METHODTYPE VSModule::ClassInfo(_In_ const uint32_t number, _Out_ Uuid * pUuid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_ClassInfo(number, pUuid);
    }

    IObject * VOODOO_METHODTYPE VSModule::CreateClass(_In_ const uint32_t number, _In_ ICore * const pCore) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_ClassCreate(number, pCore);
    }
}
