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

#include "VSModule.hpp"

#include "ICore.hpp"
#include "IFile.hpp"
#include "IFilesystem.hpp"
#include "ILogger.hpp"
#include "IObject.hpp"
#include "IParser.hpp"

#include "Exception.hpp"
#include "Regex.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    VSModule * VSModule::Load(_In_ ICore * const pCore, _In_ const String & path)
    {
        // Load the module
        HMODULE hmodule = LoadLibraryEx(path.GetData(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if (hmodule != nullptr)
        {
            VSModule * module = new VSModule(pCore, hmodule);

            module->m_ModuleVersion = reinterpret_cast<Functions::ModuleVersionFunc>(GetProcAddress(hmodule, "ModuleVersion"));
            module->m_ClassCount = reinterpret_cast<Functions::ModuleCountFunc>(GetProcAddress(hmodule, "ClassCount"));
            module->m_ClassInfo = reinterpret_cast<Functions::ModuleInfoFunc>(GetProcAddress(hmodule, "ClassInfo"));
            module->m_ClassCreate = reinterpret_cast<Functions::ModuleCreateFunc>(GetProcAddress(hmodule, "ClassCreate"));

            if
            (
                module->m_ModuleVersion == nullptr ||
                module->m_ClassCount == nullptr ||
                module->m_ClassInfo == nullptr ||
                module->m_ClassCreate == nullptr
            )
            {
                delete module;
                return nullptr;
            } else {
                return module;
            }
        } else {
            return nullptr;
        }
    }

    VOODOO_METHODTYPE VSModule::VSModule(_In_ ICore * pCore, HMODULE hmodule) :
        m_Refs(0), m_Core(pCore), m_Handle(hmodule)
    { }

    VOODOO_METHODTYPE VSModule::~VSModule()
    {
        if (m_Handle != INVALID_HANDLE_VALUE)
        {
            FreeLibrary(m_Handle);
            m_Handle = nullptr;
        }
    }

    uint32_t VOODOO_METHODTYPE VSModule::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSModule::Release() CONST
    {
        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    bool VOODOO_METHODTYPE VSModule::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSModule;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IModule) {
                *ppOut = static_cast<const IModule*>(this);
            } else if (clsid == CLSID_VSModule) {
                *ppOut = static_cast<const VSModule*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSModule::ToString() CONST
    {
        return VSTR("VSModule()");
    }

    ICore * VOODOO_METHODTYPE VSModule::GetCore() CONST
    {
        return m_Core;
    }

    const Version * VOODOO_METHODTYPE VSModule::ModuleVersion() CONST
    {
        return m_ModuleVersion();
    }

    uint32_t VOODOO_METHODTYPE VSModule::ClassCount() CONST
    {
        return m_ClassCount();
    }

    const vchar_t * VOODOO_METHODTYPE VSModule::ClassInfo(_In_ const uint32_t number, _Out_ Uuid * pUuid) CONST
    {
        return m_ClassInfo(number, pUuid);
    }

    IObject * VOODOO_METHODTYPE VSModule::CreateClass(_In_ const uint32_t number, _In_ ICore * const pCore) CONST
    {
        return m_ClassCreate(number, pCore);
    }
}
