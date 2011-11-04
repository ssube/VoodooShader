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

#include "VSFilesystem.hpp"

namespace VoodooShader
{
    VSFileSystem::VSFileSystem(_In_ ICore * pCore) :
        m_Core(pCore)
    { }

    VSFileSystem::~VSFileSystem(void)
    { }

    uint32_t VSFileSystem::AddRef() const
    {
        return ++m_Refs;
    }

    uint32_t VSFileSystem::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VSFileSystem::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = IID_IFileSystem;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IFileSystem) {
                *ppOut = static_cast<const IFileSystem*>(this);
            } else if (clsid == CLSID_VSFileSystem) {
                *ppOut = static_cast<const VSFileSystem*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
        }
    }

    String VSFileSystem::ToString(void) const
    {
        return L"VSFileSystem";
    }

    ICore * VSFileSystem::GetCore(void) const
    {
        return m_Core;
    }

    bool VSFileSystem::AddPath(_In_ const String & dir)
    {
        UNREFERENCED_PARAMETER(dir);

        return true;
    }

    bool VSFileSystem::RemovePath(_In_ const String & dir)
    {
        UNREFERENCED_PARAMETER(dir);

        return true;
    }

    IFile * VSFileSystem::FindFile(_In_ const String & name) const
    {
        UNREFERENCED_PARAMETER(name);

        return nullptr;
    }
}
