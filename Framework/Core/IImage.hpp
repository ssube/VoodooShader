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
#pragma once

#include "VoodooFramework.hpp"
#include "IFile.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_interfaces
     * @{
     */

    /**
     * @class IImage
     *
     * Provides abstraction to an image for specialized access.
     *
     * @iid e6f3128d-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IImage, IFile, ({0x8d, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         */

        VOODOO_METHOD_(String, GetPath)() CONST PURE;
        VOODOO_METHOD(Open)(_In_ FileOpenMode mode) PURE;
        VOODOO_METHOD(Close)() PURE;
        VOODOO_METHOD(Seek)(_In_ StreamType stream, _In_ SeekMode mode, _In_ int32_t offset) PURE;
        VOODOO_METHOD_(int32_t, Tell)(_In_ StreamType stream) PURE;
        VOODOO_METHOD_(int32_t, Read)(_In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) PURE;
        VOODOO_METHOD_(int32_t, Write)(_In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) PURE;
        VOODOO_METHOD_(IImage *, OpenImage)() CONST PURE;

        /**
         * Get the texture description.
         *
         * @return The texture description.
         */
        VOODOO_METHOD_(TextureDesc, GetDesc)() CONST PURE;
        /**
         * Get texture data for the given texture region. This must perform an internal size check and verifies that the
         * buffer size is large enough before copying any data, so it is not possible to fill the buffer with part of a
         * larger region.
         *
         * @param desc The texture region to copy.
         * @param size The buffer size in bytes.
         * @param pBuffer The buffer to copy into, must be allocated.
         * @return The number of bytes copied, or -1 on error.
         */
        VOODOO_METHOD_(int32_t, GetData)(_In_ const TextureRegion desc, _In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) CONST PURE;
    };
    /**
     * @}
     */
}
