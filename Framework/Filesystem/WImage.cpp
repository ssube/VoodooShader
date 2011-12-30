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

#include "FileSystem.hpp"

#include "Filesystem_Version.hpp"

#include "Format.hpp"

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

        VSWImage::VSWImage(ICore * pCore, const String & path, ILuint image) :
            m_Core(pCore), m_Path(path), m_Image(image)
        {
            m_Desc.Size.X = ilGetInteger(IL_IMAGE_WIDTH);
            m_Desc.Size.Y = ilGetInteger(IL_IMAGE_HEIGHT);
            m_Desc.Size.Z = ilGetInteger(IL_IMAGE_DEPTH);
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
                m_Core->GetLogger()->LogMessage
                (
                    LL_ModWarning, VOODOO_FILESYSTEM_NAME,
                    Format(VSTR("Unable to resolve format for image '%1%' %2%.")) << m_Path << image
                );
            }
        }

        VSWImage::~VSWImage()
        {
            ilDeleteImage(m_Image);
        }

        uint32_t VSWImage::AddRef() const
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VSWImage::Release() const
        {
            if (SAFE_DECREMENT(m_Refs) == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSWImage::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
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
                return true;
            }
        }

        String VSWImage::ToString() const
        {
            return VSTR("VSWImage()");
        }

        ICore * VSWImage::GetCore() const
        {
            return m_Core;
        }

        String VSWImage::GetPath() const
        {
            return m_Path;
        }

        bool VSWImage::Open(FileOpenMode mode)
        {
            return VSWFileImpl::Open(m_Core, m_Path, m_File, mode);
        }

        IImage * VSWImage::OpenImage() const
        {
            return const_cast<VSWImage*>(this);
        }

        bool VSWImage::Close()
        {
            return VSWFileImpl::Close(m_File);
        }

        bool VSWImage::Seek(_In_ StreamType stream, _In_ SeekMode mode, _In_ int32_t offset)
        {
            return VSWFileImpl::Seek(m_File, stream, mode, offset);
        }

        int32_t VSWImage::Tell(_In_ StreamType stream)
        {
            return VSWFileImpl::Tell(m_File, stream);
        }

        int VSWImage::Read(_In_ int count, _In_opt_bytecount_(count) void *buffer)
        {
            return VSWFileImpl::Read(m_Core, m_File, count, buffer);
        }

        int32_t VSWImage::Write(_In_ const int32_t count, _In_opt_bytecount_(count) void * buffer)
        {
            return VSWFileImpl::Write(m_Core, m_File, count, buffer);
        }

        TextureDesc VSWImage::GetDesc() const
        {
            return m_Desc;
        }

        int32_t VSWImage::GetData(_In_ const TextureRegion pDesc, _In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) const
        {
            //! @todo Add a buffer size check.
            UNREFERENCED_PARAMETER(size);

            ILint ilFmt = 0, ilType = 0;

            // Convert TextureFormat to DevIL format
            switch (pDesc.Format)
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
                m_Core->GetLogger()->LogMessage
                (
                    LL_ModWarning, VOODOO_FILESYSTEM_NAME,
                    Format(VSTR("Invalid texture format for getting image data (%1%).")) << pDesc.Format
                );
                return 0;
            }

            return ilCopyPixels
            (
                pDesc.Origin.X, pDesc.Origin.Y, pDesc.Origin.Z,
                pDesc.Size.X, pDesc.Size.Y, pDesc.Size.Z,
                ilFmt, ilType, pBuffer
            );
        }
    }
}
