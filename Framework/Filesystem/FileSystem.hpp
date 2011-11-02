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
#pragma once

#include "IL/il.h"
#include "pugixml.hpp"

#define VOODOO_IMPORT
#define VOODOO_NO_BOOST
#define VOODOO_NO_CG
#include "VoodooFramework.hpp"

namespace VoodooShader
{
    /**
     * Primary Voodoo filesystem implementation. This provides a thin wrapper for the Windows file access API and DevIL, 
     * matching them to the Voodoo interfaces.
     * 
     * @note This module exports 1 class, named @p WFileSystem (@ref VoodooWFS::VSWFileSystem) and provides implementations of 
     *      @ref IFileSystem (VSWFileSystem), @ref IFile (VSWFile) and @ref IImage (VSWImage). The following global variables 
     *      are added when the filesystem object is created: 
     *          @li <code>\$(mygames)</code> Current user's My Games folder. 
     *          @li <code>\$(allgames)</code> Shared (all user's) My Games folder. 
     *          @li <code>\$(systemroot)</code> System path (e.g., <code>C:\\Windows\\System32</code>) 
     *      
     * @addtogroup voodoo_module_wfilesystem Voodoo/WFileSystem @{
     */
    namespace VoodooWFS
    {
        typedef std::list<String> StringList;

        const Version * API_ModuleVersion(void);
        uint32_t API_ClassCount(void);
        const char *API_ClassInfo(_In_ const uint32_t number);
        IObject *API_ClassCreate(_In_ const uint32_t number, _In_ ICore * pCore);

        /**
         * Provides a unified file management system for loading shaders and other resources. This file system implementation
         * is a thin wrapper for the Windows API, adding only a few nonstandard functions (directory searching and path
         * variables).
         */
        class VSWFileSystem :
            public IFileSystem
        {
        public:
            VSWFileSystem(_In_ ICore *core);

            virtual ~VSWFileSystem(void);
        
            virtual uint32_t AddRef(void) const throw();
            virtual uint32_t Release(void) const throw();
            virtual String ToString(void) const throw();
            virtual ICore * GetCore(void) const throw();
            
            /**
             * Add a directory to the search path. Directories are pushed to the front of the list, which is searched in 
             * order (last added has highest priority). 
             * 
             * @note This function uses Parser::ParseString() for all paths. Variables are evaluated when the path is added. 
             * @note This function will split paths at the ';' character, similar to how Windows treats the path variable. 
             *      This (combined with env vars) allows regular-form environment vars to be added to the file system.
             */
            virtual bool AddPath(_In_ const String & path) throw();

            /**
             * Removes a directory from the search path, following the same rules as adding (adding a string then removing the
             * same string will always work).
             * 
             * @param path The path to remove.
             */
            virtual bool RemovePath(_In_ const String & path) throw();
            
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
            virtual IFile * FindFile(_In_ const String & name) const throw();

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;

            StringList m_Directories;
        };

        class VSWFile :
            public IFile
        {
        public:
            /**
             * Creates a file object from a path (usually an absolute path).
             */
            VSWFile(_In_ ICore * pCore, _In_ const String & path);

            virtual ~VSWFile(void);

            virtual uint32_t AddRef(void) const throw();
            virtual uint32_t Release(void) const throw();
            virtual String ToString(void) const throw();
            virtual ICore * GetCore(void) const throw();

            /**
             * Retrieves this file's absolute path.
             * 
             * @return Internal path.
             */
            virtual String GetPath(void) const throw();

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
            virtual bool Open(_In_ FileOpenMode mode);

            /**
             * Closes the file, preventing further access. 
             * 
             * @return Success of the operation 
             */
            virtual bool Close(void);

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
            virtual int32_t Read(_In_ const int32_t count, _In_opt_bytecount_(count) void * const pBuffer);

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
            virtual int32_t Write(_In_ const int32_t count, _In_opt_bytecount_(count) void * const pBuffer);

            virtual IImage * OpenImage(void) const;

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
         * @todo Provide layer, cubemap and animation handling.
         * @todo Provide image saving. 
         */
        class VSWImage :
            public IImage
        {
        public:
            static VSWImage * Load(ICore * pCore, const String & name);

            VSWImage(ICore *core, const String & name, unsigned int image);

            virtual ~VSWImage(void);

            virtual uint32_t AddRef(void) const throw();
            virtual uint32_t Release(void) const throw();
            virtual String ToString(void) const throw();
            virtual ICore * GetCore(void) const throw();

            virtual String GetPath(void) const throw();

            /**
             * Retrieves texture format data from the image. The runtime can use this to set up a texture or decide if it 
             * can handle the texture format. 
             * 
             * @return Texture information.
             */
            virtual const TextureDesc * GetDesc(void) const throw();

            /**
             * Retrieves a portion of the texture data from the image. 
             * 
             * @param pDesc The region and format to be returned. 
             * @param size The size of the buffer
             * @param pBuffer The memory for the return data to be placed in. Must already be allocated, or nullptr. 
             * @return The number of bytes retrieved (or, if @p buffer is nullptr, the number that would be retrieved).
             * 
             * @warning Due to limitations in this library (or DevIL, not sure which), the only texture formats this function
             *      can convert into are @ref TF_RGB8, @ref TF_RGBA8, @ref TF_RGBA16F and @ref TF_RGBA32F. Others are not 
             *      supported and will cause this function to throw.
             *      
             * @note This can convert data between most formats, so the format given in @p desc will be the returned format. 
             *      This makes calculating the buffer size relatively easy.
             *      
             * @warning If this function converts formats or copies a large region, it will be slow. Avoid calling often.
             */
            virtual uint32_t GetData(_In_ const TextureRegion * pDesc, _In_ const uint32_t size, _In_opt_count_(size) void * const pBuffer) const throw();

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            String m_Name;

            IParser * m_Parser;
            unsigned int m_Image;
            TextureDesc m_Desc;
        };
    }
}

/* @} */
