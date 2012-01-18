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

#include "HookManager.hpp"
#include "Hook_Version.hpp"

#include "Format.hpp"

#pragma warning(push)
#pragma warning(disable: 4005)
#include <ntstatus.h>
#pragma warning(pop)

#include <algorithm>

namespace VoodooShader
{
    namespace EasyHook
    {
        static const Version moduleVersion = VOODOO_VERSION_STRUCT(HOOK);
        static const wchar_t * name_VSEHHookManager = L"VSEHHookManager";
        static const Uuid clsid_VSEHHookManager = CLSID_VSEHHookManager;

        const Version * VOODOO_CALLTYPE API_ModuleVersion()
        {

            return &moduleVersion;
        }

        const uint32_t VOODOO_CALLTYPE API_ClassCount()
        {
            return 1;
        }

        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid)
        {
            if (!pUuid)
            {
                return nullptr;
            }

            if (index == 0)
            {
                *pUuid = clsid_VSEHHookManager;
                return name_VSEHHookManager;
            }

            return nullptr;
        }

        IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t number, _In_ ICore *core)
        {
            if (number == 0)
            {
                return new VSEHHookManager(core);
            }

            return nullptr;
        }

        VSEHHookManager::VSEHHookManager(ICore * pCore) :
            m_Core(pCore)
        {
        }

        VSEHHookManager::~VSEHHookManager()
        {
        }

        uint32_t VSEHHookManager::AddRef() const
        {
            return SAFE_INCREMENT(m_Refs);
        }


        uint32_t VSEHHookManager::Release() const
        {
            if (SAFE_DECREMENT(m_Refs) == 0)
            {
                delete this;
                return 0;
            }
            else
            {
                return m_Refs;
            }
        }

        VoodooResult VSEHHookManager::QueryInterface(_In_ Uuid clsid, _Deref_out_opt_ const IObject ** ppOut) const
        {
            if (!ppOut)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else
            {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                }
                else if (clsid == IID_IHookManager)
                {
                    *ppOut = static_cast<const IHookManager*>(this);
                }
                else if (clsid == CLSID_VSEHHookManager)
                {
                    *ppOut = static_cast<const VSEHHookManager*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return VSFERR_INVALIDPARAMS;
                }

                (*ppOut)->AddRef();
                return VSF_OK;
            }
        }

        String VSEHHookManager::ToString() const
        {
            return VSTR("VSEHHookManager");
        }

        ICore * VSEHHookManager::GetCore() const
        {
            return m_Core;
        }

        VoodooResult VSEHHookManager::Add(const String & name, void * pSrc, void * pDest)
        {
        }

        VoodooResult VSEHHookManager::Remove(const String & name)
        {
        }

        VoodooResult VSEHHookManager::RemoveAll()
        {
        }
    }
}
