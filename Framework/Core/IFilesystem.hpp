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
#pragma once

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_framework_interfaces
     * @{
     */

    /**
     * Provides abstraction of the local filesystem to allow for files, multiple search directories, archives, and even
     * network access. Currently provides a readonly filesystem.
     *
     * @par IID
     *      e6f31288-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IFileSystem, IObject, {0x8b, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;

        /**
         * Adds a directory to this file system's search path.
         *
         * @param path The path to add (may include variables, will be parsed, may be absolute or relative).
         *
         * @note This does not check for duplicates, so care should be taken not to add the same directory repeatedly.
         *     Directory search order is important for file priority.
         */
        VOODOO_METHOD(AddPath)(_In_ const String & path) PURE;
        /**
         * Removes a directory from the search path, following the same rules as adding (adding a string then removing the
         * same string will always work).
         *
         * @param path The path to remove.
         */
        VOODOO_METHOD(RemovePath)(_In_ const String & path) PURE;
        /**
         * Resolves a relative filename (usually just filename and extension, but may include directories) into an
         * absolute filename by searching the list of resource directories registered with this manager.
         *
         * @param name The file name to find.
         * @return The file, if found.
         */
        VOODOO_METHOD_(IFile *, FindFile)(_In_ const String & name) CONST PURE;
    };

    /**
     * Provides abstraction to a single file, for basic access.
     *
     * @par IID
     *      e6f3128a-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IFile, IObject, {0x8a, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;

        /**
         * Gets the absolute path of this file.
         */
        VOODOO_METHOD_(String, GetPath)() CONST PURE;
        VOODOO_METHOD(Open)(_In_ FileOpenMode mode) PURE;
        VOODOO_METHOD(Close)() PURE;
        VOODOO_METHOD_(int32_t, Read)(_In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) PURE;
        VOODOO_METHOD_(int32_t, Write)(_In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) PURE;
        VOODOO_METHOD_(IImage *, OpenImage)() CONST PURE;
    };

    /**
     * Provides abstraction to an image for specialized access.
     *
     * @par IID
     *      e6f3128d-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IImage, IObject, {0x8d, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;

        VOODOO_METHOD_(String, GetPath)() CONST PURE;
        VOODOO_METHOD_(const TextureDesc, GetDesc)() CONST PURE;
        VOODOO_METHOD_(uint32_t, GetData)(_In_ const TextureRegion pDesc, _In_ const uint32_t size, _In_opt_count_(size) void * const pBuffer) CONST PURE;
    };
    /**
     * @}
     */
}
