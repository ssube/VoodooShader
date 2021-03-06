/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

#define VOODOO_IMPORT
#include "VoodooFramework.hpp"

#include "IL/il.h"
#include "pugixml.hpp"

#pragma warning(push,3)
#include <fstream>
#pragma warning(pop)

namespace VoodooShader
{
    /**
     * Primary Voodoo filesystem implementation. This provides a thin wrapper for the Windows file access API and DevIL,
     * matching them to the Voodoo interfaces.
     *
     * @note This module exports 1 class, named @p WFileSystem (@ref VoodooWFS::VSWFileSystem) and provides implementations
     *      of @ref IFileSystem (VSWFileSystem), @ref IFile (VSWFile) and @ref IImage (VSWImage). The following global
     *      variables are added when the filesystem object is created:
     *          @li <code>\$(mygames)</code> Current user's My Games folder.
     *          @li <code>\$(allgames)</code> Shared (all user's) My Games folder.
     *          @li <code>\$(systemroot)</code> System path (e.g., <code>C:\\Windows\\System32</code>)
     *
     * @addtogroup voodoo_module_wfilesystem Voodoo/WFileSystem
     * @{
     */
    namespace VoodooWFS
    {
        typedef std::list<String> StringList;

        const Version * VOODOO_CALLTYPE API_PluginInit(_In_ ICore * pCore);
        void            VOODOO_CALLTYPE API_PluginReset(_In_ ICore * pCore);
        const uint32_t  VOODOO_CALLTYPE API_ClassCount();
        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid);
        IObject *       VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ ICore * pCore);

        /**
         * class VSWFileSystem
         *
         * Provides a unified file management system for loading shaders and other resources. This file system
         * implementation is a thin wrapper for the Windows API, adding only a few nonstandard functions (directory
         * searching and path variables).
         *
         * @par ClassId
         *      e6f312b1-05af-11e1-9e05-005056c00008
         */
        VOODOO_CLASS(VSWFileSystem, IFileSystem, ({0xCE, 0xCF, 0x87, 0xA6, 0x3C, 0x06, 0xE1, 0x11, 0xB2, 0x2E, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
        {
        public:
            VSWFileSystem(_In_ ICore *core);
            ~VSWFileSystem();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid clsid, _Outptr_result_maybenull_ IObject ** ppOut);
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;

            VOODOO_METHOD(AddPath)(_In_ CONST String & path, _In_ CONST PathType type = VSPath_Directory);
            VOODOO_METHOD(RemovePath)(_In_ CONST String & path);
            VOODOO_METHOD_(IFile *, GetFile)(_In_ CONST String & name, _In_ CONST GetFileMode mode) CONST;

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;

            StringList m_Directories;
        };

        /**
         * File mechanics implementation.
         */
        class VSWFileImpl;

        /**
         * @class VSWFile
         *
         * @par ClassId
         *      e6f312b0-05af-11e1-9e05-005056c00008
         */
        VOODOO_CLASS(VSWFile, IFile, ({0xB0, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
        {
        public:
            /**
             * Creates a file object from a path (usually an absolute path).
             */
            VSWFile(_In_ ICore * pCore, _In_ const String & path);
            ~VSWFile();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid clsid, _Outptr_result_maybenull_ IObject ** ppOut);
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;

            VOODOO_METHOD_(String, GetPath)() CONST;
            VOODOO_METHOD(Open)(_In_ CONST FileOpenMode mode);
            VOODOO_METHOD(Close)();
            VOODOO_METHOD(Seek)(_In_ CONST StreamType stream, _In_ CONST SeekMode mode, _In_ CONST int32_t offset);
            VOODOO_METHOD_(int32_t, Tell)(_In_ CONST StreamType stream);
            VOODOO_METHOD_(int32_t, Read)(_In_ CONST int32_t count, _In_opt_bytecount_(count) void * pBuffer);
            VOODOO_METHOD_(int32_t, Write)(_In_ CONST int32_t count, _In_opt_bytecount_(count) void * pBuffer);

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            String m_Path;

            std::fstream m_File;
        };
    }
    /**
     * @}
     */
}
