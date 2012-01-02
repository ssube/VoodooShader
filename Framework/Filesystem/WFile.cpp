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

#include "FileSystem.hpp"

#include "Filesystem_Version.hpp"

// The MS shlobj header contains a few functions that cause errors in analysis under /W4 (and cause the build to fail
// under /WX). This disables the warning for only that header.
#pragma warning(push)
#pragma warning(disable : 6387)
#include <shlobj.h>
#pragma warning(pop)
#include <strsafe.h>

namespace VoodooShader
{
    namespace VoodooWFS
    {
        VSWFile::VSWFile(ICore * pCore, const String & path) :
            m_Path(path), m_Core(pCore)
        { };

        VSWFile::~VSWFile()
        {
            this->Close();
        }

        uint32_t VSWFile::AddRef() const
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VSWFile::Release() const
        {
            if (SAFE_DECREMENT(m_Refs) == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSWFile::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_VSWFile;
                    return true;
                } else {
                    return false;
                }
            } else {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                } else if (clsid == IID_IFile) {
                    *ppOut = static_cast<const IFile*>(this);
                } else if (clsid == CLSID_VSWFile) {
                    *ppOut = static_cast<const VSWFile*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String VSWFile::ToString() const
        {
            return VSTR("VSWFile()");
        }

        ICore * VSWFile::GetCore() const
        {
            return m_Core;
        }

        String VSWFile::GetPath() const
        {
            return m_Path;
        }

        bool VSWFile::Open(FileOpenMode mode)
        {
            return VSWFileImpl::Open(m_Core, m_Path, m_File, mode);
        }

        IImage * VSWFile::OpenImage() const
        {
            return VSWImage::Load(m_Core, m_Path);
        }

        bool VSWFile::Close()
        {
            return VSWFileImpl::Close(m_File);
        }

        bool VSWFile::Seek(_In_ StreamType stream, _In_ SeekMode mode, _In_ int32_t offset)
        {
            return VSWFileImpl::Seek(m_File, stream, mode, offset);
        }

        int32_t VSWFile::Tell(_In_ StreamType stream)
        {
            return VSWFileImpl::Tell(m_File, stream);
        }

        int VSWFile::Read(_In_ int count, _In_opt_bytecount_(count) void *buffer)
        {
            return VSWFileImpl::Read(m_Core, m_File, count, buffer);
        }

        int32_t VSWFile::Write(_In_ const int32_t count, _In_opt_bytecount_(count) void * buffer)
        {
            return VSWFileImpl::Write(m_Core, m_File, count, buffer);
        }
    }
}
