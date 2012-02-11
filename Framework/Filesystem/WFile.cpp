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
#pragma warning(push,3)
#include <shlobj.h>
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(push,3)
#include <strsafe.h>
#pragma warning(pop)

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
            }
            else
            {
                return m_Refs;
            }
        }

        VoodooResult VSWFile::QueryInterface(_In_ CONST Uuid clsid, _Deref_out_opt_ IObject ** ppOut)
        {
            if (!ppOut)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else
            {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<IObject*>(this);
                }
                else if (clsid == IID_IFile)
                {
                    *ppOut = static_cast<IFile*>(this);
                }
                else if (clsid == CLSID_VSWFile)
                {
                    *ppOut = static_cast<VSWFile*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return false;
                }

                (*ppOut)->AddRef();
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

        VoodooResult VSWFile::Open(FileOpenMode mode)
        {
            std::ios_base::openmode access = 0;

            if (mode & VSOpen_Read)
            {
                access |= std::ios_base::in;
            }

            if (mode & VSOpen_Write)
            {
                access |= std::ios_base::out;
            }

            if (mode & VSOpen_End)
            {
                access |= std::ios_base::ate;
            }

            if (mode & VSOpen_Append)
            {
                access |= std::ios_base::app;
            }

            if (mode & VSOpen_Truncate)
            {
                access |= std::ios_base::trunc;
            }

            if (access == 0)
            {
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_PlugWarning, VOODOO_FILESYSTEM_NAME,
                    StringFormat(VSTR("Attempted to open file '%1%' with unknown mode (%2%).")) << m_Path << mode
                );
                return VSFERR_INVALIDPARAMS;
            }

            access |= std::ios_base::binary;

            m_Core->GetLogger()->LogMessage
            (
                VSLog_PlugDebug, VOODOO_FILESYSTEM_NAME,
                StringFormat(VSTR("Opening file %1% with mode %2% (underlying %3%).")) << m_Path << mode << access
            );

            m_File.open(m_Path.GetData(), access);

            if (!m_File.is_open())
            {
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_PlugWarning, VOODOO_FILESYSTEM_NAME, 
                    StringFormat(VSTR("Unable to open file '%1%'.")) << m_Path
                );
                return VSFERR_FILENOTFOUND;
            }
            else
            {
                return VSF_OK;
            }
        }

        VoodooResult VSWFile::Close()
        {
            if (m_File.is_open())
            {
                m_File.close();
            }

            return VSF_OK;
        }

        VoodooResult VSWFile::Seek(_In_ StreamType stream, _In_ SeekMode mode, _In_ int32_t offset)
        {
            std::ios_base::seekdir dir;

            if (mode == VSSeek_Begin) {dir = std::ios_base::beg;}
            else if (mode == VSSeek_Current) {dir = std::ios_base::cur;}
            else if (mode == VSSeek_End) {dir = std::ios_base::end;}
            else { return false; }

            if (stream == VSStream_Get)
            {
                m_File.seekg(offset, dir);
            }
            else if (stream == VSStream_Put)
            {
                m_File.seekp(offset, dir);
            }

            if (m_File.fail())
            {
                return VSFERR_INVALIDCALL;
            }
            else
            {
                return VSF_OK;
            }
        }

        int32_t VSWFile::Tell(_In_ StreamType stream)
        {
            if (stream == VSStream_Get)
            {
                return (int32_t)m_File.tellg();
            }
            else if (stream == VSStream_Put)
            {
                return (int32_t)m_File.tellp();
            }
            else
            {
                return -1;
            }
        }

        int VSWFile::Read(_In_ int count, _In_opt_bytecount_(count) void * pBuffer)
        {
            UNREFERENCED_PARAMETER(m_Core);

            if (!m_File.is_open())
            {
                return -1;
            }

            m_File.read((char*)pBuffer, count);
            int32_t ret = (int32_t)m_File.gcount();

            if (m_File.bad())
            {
                return -1;
            }
            else
            {
                return ret;
            }
        }

        int32_t VSWFile::Write(_In_ const int32_t count, _In_opt_bytecount_(count) void * pBuffer)
        {
            UNREFERENCED_PARAMETER(m_Core);

            if (!m_File.is_open())
            {
                return -1;
            }

            m_File.write((char*)pBuffer, count);

            if (m_File.bad())
            {
                return -1;
            }
            else
            {
                return count;
            }
        }
    }
}
