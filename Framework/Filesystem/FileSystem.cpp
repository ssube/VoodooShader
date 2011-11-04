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
        static const Version version = VOODOO_META_VERSION_STRUCT(FILESYSTEM);
        static const wchar_t * name_VSWFileSystem = L"VSWFileSystem";
        static const Uuid clsid_VSWFileSystem = CLSID_VSWFileSystem;


        const Version * VOODOO_CALL API_ModuleVersion(void)
        {
            return &version;
        }

        const uint32_t VOODOO_CALL API_ClassCount(void)
        {
            return 1;
        }

        const wchar_t * VOODOO_CALL API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid)
        {
            if (!pUuid)
            {
                return nullptr;
            }

            if (index == 0)
            {
                *pUuid = clsid_VSWFileSystem;
                return name_VSWFileSystem;
            }

            return nullptr;
        }

        IObject * VOODOO_CALL API_ClassCreate(const uint32_t index, _In_ ICore * pCore)
        {
            if (index == 0)
            {
                return new VSWFileSystem(pCore);
            }
            else
            {
                return nullptr;
            }
        }

        VSWFileSystem::VSWFileSystem(_In_ ICore * pCore) :
            m_Core(pCore)
        {
            using namespace pugi;
            // Create builtin vars
            IParser* parser = m_Core->GetParser();
            wchar_t cvar[MAX_PATH];

            if (SHGetFolderPath(nullptr, CSIDL_COMMON_DOCUMENTS, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
            {
                StringCchCat(cvar, MAX_PATH, L"\\My Games\\");
                parser->Add(L"allgames", cvar, VT_System);
            }

            if (SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
            {
                StringCchCat(cvar, MAX_PATH, L"\\My Games\\");
                parser->Add(L"mygames", cvar, VT_System);
            }

            if (SHGetFolderPath(nullptr, CSIDL_SYSTEM, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
            {
                parser->Add(L"systemroot", cvar, VT_System);
            }

            // Init DevIL
            ilInit();

            // Load paths from the config
            xml_document *config = (xml_document *) m_Core->GetConfig();
            xpath_query pathsQuery(L"/VoodooConfig/WFileSystem/SearchPaths/Path");
            xpath_query pathValueQuery(L"./text()");
            xpath_node_set pathNodes = pathsQuery.evaluate_node_set(*config);
            xpath_node_set::const_iterator pathIter = pathNodes.begin();

            while (pathIter != pathNodes.end())
            {
                String path = pathValueQuery.evaluate_string(*pathIter).c_str();

                this->AddPath(path);

                ++pathIter;
            }
        }

        VSWFileSystem::~VSWFileSystem(void)
        {
            m_Directories.clear();
        }

        uint32_t VSWFileSystem::AddRef() const
        {
            return ++m_Refs;
        }

        uint32_t VSWFileSystem::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSWFileSystem::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_VSWFileSystem;
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
                } else if (clsid == CLSID_VSWFileSystem) {
                    *ppOut = static_cast<const VSWFileSystem*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            }
        }

        String VSWFileSystem::ToString(void) const
        {
            return L"VSWFileSystem";
        }

        ICore * VSWFileSystem::GetCore(void) const
        {
            return m_Core;
        }

        bool VSWFileSystem::AddPath(const String & name)
        {
            String realname = m_Core->GetParser()->Parse(name);
            size_t splitter = realname.Find(L';');

            while (splitter != String::Npos)
            {
                String partname = realname.Substr(0, splitter);

                this->m_Directories.push_front(partname);
                realname = realname.Substr(splitter + 1);
                splitter = realname.Find(L';');
            }

            this->m_Directories.push_front(realname);

            return true;
        }

        bool VSWFileSystem::RemovePath(const String & name)
        {
            String realname = m_Core->GetParser()->Parse(name) + L"\\";

            this->m_Directories.remove_if(
                [&](const String & current)
                {
                    return (current == realname);
                }
            );

            return true;
        }

        IFile * VSWFileSystem::FindFile(const String & name) const
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_FILESYSTEM_NAME, L"Searching for raw file \"%s\".", name.GetData());

            String filename = m_Core->GetParser()->Parse(name);

            m_Core->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_FILESYSTEM_NAME,
                L"Searching for parsed file \"%s\".",
                filename.GetData()
            );

            StringList::const_iterator curDir = m_Directories.begin();

            while (curDir != m_Directories.end())
            {
                // Try to find the file in each registered dir
                String fullname = (*curDir) + L"\\" + filename;

                HANDLE file = CreateFile(fullname.GetData(), 0, 0, nullptr, OPEN_EXISTING, 0, nullptr);

                if (file != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(file);

                    m_Core->GetLogger()->Log
                    (
                        LL_Debug, VOODOO_CORE_NAME,
                        L"File %s found in directory %s.",
                        name.GetData(), (*curDir).GetData()
                    );

                    return new VSWFile(m_Core, fullname);
                }

                ++curDir;
            }

            m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, "Unable to find file %s.", name.GetData());

            return nullptr;
        }

        VSWFile::VSWFile(ICore * pCore, const String & path) :
            m_Path(path), m_Core(pCore), m_Handle(nullptr)
        { };

        VSWFile::~VSWFile(void)
        {
            this->Close();
        }

        uint32_t VSWFile::AddRef() const
        {
            return ++m_Refs;
        }

        uint32_t VSWFile::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSWFile::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
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
            }
        }

        String VSWFile::ToString(void) const
        {
            return L"VSWFile";
        }

        ICore * VSWFile::GetCore(void) const
        {
            return m_Core;
        }

        String VSWFile::GetPath(void) const
        {
            return m_Path;
        }

        bool VSWFile::Open(FileOpenMode mode)
        {
            DWORD access = 0;

            if (mode & FM_Read)
            {
                access |= GENERIC_READ;
            }

            if (mode & FM_Write)
            {
                access |= GENERIC_WRITE;
            }

            if (access == 0)
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning, VOODOO_FILESYSTEM_NAME,
                    L"Attempted to open file '%s' with unknown mode (%X).",
                    m_Path.GetData(), mode
                );
                return false;
            }

            m_Core->GetLogger()->Log
            (
                LL_Debug, VOODOO_FILESYSTEM_NAME,
                L"Opening file %s with mode %u (underlying %u).",
                m_Path.GetData(), mode, access
            );

            m_Handle = CreateFile(m_Path.GetData(), access, 0, nullptr, OPEN_EXISTING, 0, nullptr);

            if (m_Handle == INVALID_HANDLE_VALUE)
            {
                m_Core->GetLogger()->Log(LL_Warning, VOODOO_FILESYSTEM_NAME, L"Unable to open file %s.", m_Path.GetData());
            }

            return (m_Handle != INVALID_HANDLE_VALUE);
        }

        IImage * VSWFile::OpenImage(void) const
        {
            return VSWImage::Load(m_Core, m_Path);
        }

        bool VSWFile::Close(void)
        {
            if (m_Handle && m_Handle != INVALID_HANDLE_VALUE)
            {
                return (CloseHandle(m_Handle) == TRUE);
            }
            else
            {
                return false;
            }
        }

        int VSWFile::Read(_In_ int count, _In_opt_bytecount_(count) void *buffer)
        {
            if (m_Handle)
            {

                DWORD readsize;

                if (count < 0)
                {
                    readsize = GetFileSize(m_Handle, nullptr);

                    if (buffer == nullptr)
                    {
                        return (int) readsize;
                    }
                }
                else
                {
                    readsize = (DWORD) count;
                }

                DWORD retsize = 0;
                BOOL success = ReadFile(m_Handle, buffer, readsize, &retsize, nullptr);

                if (success == 0)
                {
                    return 0;
                }
                else
                {
                    return retsize;
                }
            }

            return 0;
        }

        int32_t VSWFile::Write(_In_ const int32_t count, _In_opt_bytecount_(count) void * buffer)
        {
            if (m_Handle)
            {
                if (buffer == nullptr)
                {
                    DWORD size = 0;

                    if (count < 0)
                    {
                        Throw(VOODOO_FILESYSTEM_NAME, "Unable to write a negative number of nullptr bytes.", m_Core);
                    }
                    else
                    {
                        size = (DWORD) count;
                    }

                    buffer = malloc(size);

                    if (buffer == nullptr)
                    {
                        Throw(VOODOO_FILESYSTEM_NAME, "Error allocating memory for nullptr buffer.", m_Core);
                    }

                    memset(buffer, 0, size);

                    DWORD retsize = 0;
                    BOOL success = WriteFile(m_Handle, buffer, size, &retsize, nullptr);

                    free(buffer);

                    if (success == 0)
                    {
                        return false;
                    }
                    else if (retsize != size)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    if (count < 0)
                    {
                        Throw(VOODOO_FILESYSTEM_NAME, "Unable to write a negative nubber of bytes.", m_Core);
                    }

                    DWORD size = (DWORD) count;
                    DWORD retsize = 0;
                    BOOL success = WriteFile(m_Handle, buffer, size, &retsize, nullptr);

                    if (success == 0)
                    {
                        return false;
                    }
                    else if (retsize != size)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        VSWImage * VSWImage::Load(ICore * pCore, const String & name)
        {
            ILuint image = ilGenImage();

            ilBindImage(image);

            ILboolean loaded = ilLoadImage(name.GetData());

            if (loaded == IL_FALSE)
            {
                ilBindImage(0);
                ilDeleteImage(image);
                return nullptr;
            }
            else
            {
                return new VSWImage(pCore, name, image);
            }
        }

        VSWImage::VSWImage(ICore * pCore, const String & name, ILuint image) :
            m_Core(pCore), m_Name(name), m_Image(image)
        {
            m_Desc.Width = ilGetInteger(IL_IMAGE_WIDTH);
            m_Desc.Height = ilGetInteger(IL_IMAGE_HEIGHT);
            m_Desc.Depth = ilGetInteger(IL_IMAGE_DEPTH);
            m_Desc.Mipmaps = false;
            m_Desc.Format = TF_Unknown;

            ILint texFmt = ilGetInteger(IL_IMAGE_FORMAT);
            ILint texType = ilGetInteger(IL_IMAGE_TYPE);

            if (texFmt == IL_RGB)
            {
                if (texType == IL_BYTE || texType == IL_UNSIGNED_BYTE)
                {
                    m_Desc.Format = TF_RGB8;
                }
            }
            else if (texFmt == IL_RGBA)
            {
                if (texType == IL_BYTE || texType == IL_UNSIGNED_BYTE)
                {
                    m_Desc.Format = TF_RGBA8;
                }
                else if (texType == IL_FLOAT)
                {
                    m_Desc.Format = TF_RGBA16F;
                }
                else if (texType == IL_DOUBLE)
                {
                    m_Desc.Format = TF_RGBA32F;
                }
            }

            if (m_Desc.Format == TF_Unknown)
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning, VOODOO_FILESYSTEM_NAME,
                    "Unable to resolve format for image %s (%u).",
                    name.GetData(), image
                );
            }
        }

        VSWImage::~VSWImage(void)
        {
            ilDeleteImage(m_Image);
        }

        uint32_t VSWImage::AddRef() const
        {
            return ++m_Refs;
        }

        uint32_t VSWImage::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSWImage::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_VSWImage;
                    return true;
                } else {
                    return false;
                }
            } else {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                } else if (clsid == IID_IImage) {
                    *ppOut = static_cast<const IImage*>(this);
                } else if (clsid == CLSID_VSWImage) {
                    *ppOut = static_cast<const VSWImage*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            }
        }

        String VSWImage::ToString(void) const
        {
            return L"VSWImage";
        }

        ICore * VSWImage::GetCore(void) const
        {
            return m_Core;
        }

        String VSWImage::GetPath(void) const
        {
            return m_Name;
        }

        const TextureDesc * VSWImage::GetDesc(void) const
        {
            return &m_Desc;
        }

        uint32_t VSWImage::GetData(_In_ const TextureRegion * pDesc, _In_ const uint32_t size, _In_opt_count_(size) void * const pBuffer) const throw()
        {
            //! @todo Add a buffer size check.
            UNREFERENCED_PARAMETER(size);

            ILint ilFmt = 0, ilType = 0;

            // Convert TextureFormat to DevIL format
            switch (pDesc->Format)
            {
            case TF_RGB8:
                ilFmt = IL_RGB;
                ilType = IL_UNSIGNED_BYTE;
                break;
            case TF_RGBA8:
                ilFmt = IL_RGBA;
                ilType = IL_UNSIGNED_BYTE;
                break;
            case TF_RGBA16F:
                ilFmt = IL_RGBA;
                ilType = IL_FLOAT;
                break;
            case TF_RGBA32F:
                ilFmt = IL_RGBA;
                ilType = IL_DOUBLE;
                break;
            case TF_Unknown:
            default:
                m_Core->GetLogger()->Log
                (
                    LL_Warning, VOODOO_FILESYSTEM_NAME,
                    "Invalid texture format for getting image data (%X).",
                    pDesc->Format
                );
                return 0;
            }

            return ilCopyPixels
            (
                pDesc->OffX, pDesc->OffY, pDesc->OffZ,
                pDesc->Width, pDesc->Height, pDesc->Depth,
                ilFmt, ilType, pBuffer
            );
        }
    }
}
