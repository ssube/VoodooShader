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

#include "IL/il.h"
#include "pugixml.hpp"

#define VOODOO_IMPORT
#define VOODOO_NO_CG
#include "VoodooFramework.hpp"

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

        const Version * VOODOO_CALLTYPE API_ModuleVersion();
        const uint32_t  VOODOO_CALLTYPE API_ClassCount();
        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid);
        IObject *       VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ ICore * pCore);

        /**
         * Provides a unified file management system for loading shaders and other resources. This file system
         * implementation is a thin wrapper for the Windows API, adding only a few nonstandard functions (directory
         * searching and path variables).
         * 
         * @par CLSID
         *      e6f312b1-05af-11e1-9e05-005056c00008
         */
        VOODOO_CLASS(VSWFileSystem, IFileSystem, {0xB1, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
        {
        public:
            VSWFileSystem(_In_ ICore *core);

            virtual ~VSWFileSystem();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;

            /**
             * Add a directory to the search path. Directories are pushed to the front of the list, which is searched in
             * order (last added has highest priority).
             *
             * @note This function uses Parser::ParseString() for all paths. Variables are evaluated when the path is added.
             * @note This function will split paths at the ';' character, similar to how Windows treats the path variable.
             *      This (combined with env vars) allows regular-form environment vars to be added to the file system.
             */
            VOODOO_METHOD(AddPath)(_In_ const String & path);

            /**
             * Removes a directory from the search path, following the same rules as adding (adding a string then removing
             * the same string will always work).
             *
             * @param path The path to remove.
             */
            VOODOO_METHOD(RemovePath)(_In_ const String & path);

            /**
             * Resolves a relative filename (usually just filename and extension, but may include directories) into an
             * absolute filename by searching the list of resource directories registered with this manager.
             *
             * @param name The filename to search for.
             * @return If the file is found, a reference to an unopened file object is returned.
             *
             * @note This functions uses Parser::ParseString() on the path. It is evaluated once when the function enters,
             *      then appended to each stored path until a valid file is found.
             */
            VOODOO_METHOD_(IFile *, FindFile)(_In_ const String & name) CONST;

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;

            StringList m_Directories;
        };

        /**
         * @par CLSID
         *      e6f312b0-05af-11e1-9e05-005056c00008
         */
        VOODOO_CLASS(VSWFile, IFile, {0xB0, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
        {
        public:
            /**
             * Creates a file object from a path (usually an absolute path).
             */
            VSWFile(_In_ ICore * pCore, _In_ const String & path);

            virtual ~VSWFile();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;

            /**
             * Retrieves this file's absolute path.
             *
             * @return Internal path.
             */
            VOODOO_METHOD_(String, GetPath)() CONST;

            /**
             * Opens the file for read-write access.
             *
             * @param mode The mode to open the file in.
             * @return Whether the file was successfully opened.
             *
             * @note If this file handle was returned by a VSWFileSystem and the absolute path is still valid, this function
             *      should always succeed. In single-threaded or fullscreen scenarios with few user tasks, this is usually
             *      the case.
             *
             * @warning If this file handle uses a relative path, then it is subject to changes in working directory by the
             *      any module in the process. VSWFileSystem::GetFile() uses an absolute path in the constructor and is not
             *      susceptible to this.
             */
            VOODOO_METHOD(Open)(_In_ FileOpenMode mode);

            /**
             * Closes the file, preventing further access.
             *
             * @return Success of the operation
             */
            VOODOO_METHOD(Close)();

            /**
             * Reads a chunk of data from the file. The file must have been opened for reading previously, or this call will
             * fail.
             *
             * @param count The number of bytes to read, -1 for all.
             * @param pBuffer The buffer to be read into (may be nullptr, see notes).
             * @return The number of bytes read.
             *
             * @note If @arg buffer is nullptr, the number of bytes that would have been read is returned but the file
             *      position is unchanged. If @arg count is -1, this returns the filesize remaining
             */
            VOODOO_METHOD_(int32_t, Read)(_In_ const int32_t count, _In_opt_bytecount_(count) void * const pBuffer);

            /**
             * Writes a chunk of data to the file. The file must have been opened for writing.
             *
             * @param count The number of bytes to write.
             * @param pBuffer The data to write.
             * @return Success of the write operation.
             *
             * @note If @p buffer is nullptr, @p count zeros are written into the file. This is useful for padding binary
             *      formats.
             */
            VOODOO_METHOD_(int32_t, Write)(_In_ const int32_t count, _In_opt_bytecount_(count) void * const pBuffer);

            VOODOO_METHOD_(IImage *, OpenImage)() CONST;

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            String m_Path;

            HANDLE m_Handle;
        };

        /**
         * Provides image loading, using the DevIL library. This class provides internal loading and conversion, and can
         * manage 1-3 dimensional images (regular textures and volumes).
         *
         * @par CLSID
         *      e6f312b2-05af-11e1-9e05-005056c00008
         *      
         * @todo Provide layer, cubemap and animation handling.
         * @todo Provide image saving.
         */
        VOODOO_CLASS(VSWImage, IImage, {0xB2, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
        {
        public:
            static VSWImage * Load(ICore * pCore, const String & name);

            VSWImage(ICore *core, const String & name, unsigned int image);

            virtual ~VSWImage();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;

            VOODOO_METHOD_(String, GetPath)() CONST;

            /**
             * Retrieves texture format data from the image. The runtime can use this to set up a texture or decide if it
             * can handle the texture format.
             *
             * @return Texture information.
             */
            VOODOO_METHOD_(const TextureDesc *, GetDesc)() CONST;

            /**
             * Retrieves a portion of the texture data from the image.
             *
             * @param pDesc The region and format to be returned.
             * @param size The size of the buffer
             * @param pBuffer The memory for the return data to be placed in. Must already be allocated, or nullptr.
             * @return The number of bytes retrieved (or, if @p buffer is nullptr, the number that would be retrieved).
             *
             * @warning Due to limitations in this library (or DevIL, not sure which), the only texture formats this
             *      function can convert into are @ref TF_RGB8, @ref TF_RGBA8, @ref TF_RGBA16F and @ref TF_RGBA32F. Others
             *      are not supported and will cause this function to throw.
             *
             * @note This can convert data between most formats, so the format given in @p desc will be the returned format.
             *      This makes calculating the buffer size relatively easy.
             *
             * @warning If this function converts formats or copies a large region, it will be slow. Avoid calling often.
             */
            VOODOO_METHOD_(uint32_t, GetData)
            (
                _In_ const TextureRegion * pDesc,
                _In_ const uint32_t size,
                _In_opt_count_(size) void * const pBuffer
            ) CONST;

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            String m_Name;

            IParser * m_Parser;
            unsigned int m_Image;
            TextureDesc m_Desc;
        };
    }
    /**
     * @}
     */
}
