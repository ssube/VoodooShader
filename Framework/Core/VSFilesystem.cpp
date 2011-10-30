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

#include "VSFilesystem.hpp"

namespace VoodooShader
{
    VSFileSystem::VSFileSystem(_In_ ICore * pCore) : 
        m_Core(pCore)
    { }

    VSFileSystem::~VSFileSystem(void)
    { }

    int32_t VSFileSystem::AddRef() const
    {
        return ++m_Refs;
    }

    int32_t VSFileSystem::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
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

    void VSFileSystem::AddPath(_In_ const String & dir)
    {
        UNREFERENCED_PARAMETER(dir);
    }

    void VSFileSystem::RemovePath(_In_ const String & dir)
    {
        UNREFERENCED_PARAMETER(dir);
    }

    IFile * VSFileSystem::FindFile(_In_ const String & name) const
    {
        UNREFERENCED_PARAMETER(name);

        return nullptr;
    }
}
