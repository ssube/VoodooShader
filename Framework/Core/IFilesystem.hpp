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

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_framework_interfaces
     * @{
     */

    /**
     * @class IFileSystem
     *
     * Provides abstraction of the local filesystem to allow for files, multiple search directories, archives, and even
     * network access. Currently provides a readonly filesystem.
     *
     * @iid e6f31288-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IFileSystem, IObject, {0x8b, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
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
         * @name Search Path Methods
         * Add or remove directories from the search path.
         * @{
         */
        /**
         * Adds a directory to this file system's search path.
         *
         * @param path The path to add (may include variables, will be parsed, may be absolute or relative).
         *
         * @note This does not check for duplicates, so care should be taken not to add the same directory repeatedly.
         *     Directory search order is important for file priority.
         *
         * @note This function uses Parser::ParseString() for all paths. Variables are evaluated when the path is added.
         * @note This function will split paths at the ';' character, similar to how Windows treats the path variable. This
         *      combined with parser environment access allows regular-form environment vars to be added to the file system.
         */
        VOODOO_METHOD(AddPath)(_In_ const String & path) PURE;
        /**
         * Removes a directory from the search path, following the same rules as adding (adding a string then removing the
         * same string will always work).
         *
         * @param path The path to remove.
         *
         * @note This function must Parser::ParseString() for all paths. Variables are evaluated when the path is added.
         * @note This function will split paths at the ';' character, similar to how Windows treats the path variable. This
         *      combined with parser environment access allows regular-form environment vars to be added to the file system.
         */
        VOODOO_METHOD(RemovePath)(_In_ const String & path) PURE;
        /**
         * @}
         * @name Search Methods
         * File search methods.
         * @{
         */
        /**
         * Resolves a relative filename (usually just filename and extension, but may include directories) into an
         * absolute filename by searching the list of resource directories registered with this manager. Depending on the
         * mode given, the file may be opened or created.
         *
         * @param name The file name to find (must be parsed).
         * @param mode The file mode to use.
         * @return The file, if found or created (depending on mode).
         */
        VOODOO_METHOD_(IFile *, GetFile)(_In_ const String & name, _In_ const GetFileMode mode = FF_OpenOnly) CONST PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
