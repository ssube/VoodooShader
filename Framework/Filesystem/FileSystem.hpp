/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (c) 2010-2011 by Sean Sube
*
*
* This program is free software; you can redistribute it and/or modify it under the terms of the 
* GNU General Public License as published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

namespace VoodooShader
{
    namespace VoodooWVFS
    {
        typedef std::list<String> StringList;

        Version API_ModuleVersion();
        int API_ClassCount();
        const char * API_ClassInfo(_In_ int number);
        IObject * API_ClassCreate( _In_ int number, _In_ Core * core );

        /**
         * Provides a unified file management system for loading shaders and other
         * resources.
         */
        class FileSystem
            : public IFileSystem
        {
        public:
            FileSystem
            (
                _In_ Core * core
            );

            ~FileSystem();

            const char * GetObjectClass();

            /**
             * Add a directory to the search path. Directories are pushed to the
             * front of the list, which is searched in order (last added has highest
             * priority).
             * 
             * @note @arg name may contain a number of variables, see 
             *    @sa filenamevars "the filename specs" for the full list.
             */
            void AddDirectory
            (
                _In_ String dir
            );

            void RemoveDirectory
            (
                _In_ String dir
            );

            /**
             * Resolves a relative filename (usually just filename and extension,
             * but may include directories) into an absolute filename by searching
             * the list of resource directories registered with this manager.
             *
             * @param name The filename to search for.
             * @return If the file is found, a reference to an unopened file object
             *      is returned. If no file is found, an empty reference is returned.
             *      
             * @note @arg name may contain a number of variables, see 
             *    @sa filenamevars "the filename specs" for the full list.
             */
            IFileRef GetFile
            (
                _In_ String name
            );

            IImageRef GetImage
            (
                _In_ String name
            );

            String ParsePath(_In_ String rawpath);

        private:
            StringList mDirectories;
            Core * mCore;
        };

        class File
            : public IFile
        {
        public:
            /**
             * Creates a file object from a path (usually an absolute path). This
             * should usually not be called directly, FileManager::GetFile(String)
             * will automatically resolve and return paths to simplify things.
             */
            File
            (
                _In_ Core * core, 
                _In_ String name
            );

            ~File();

            const char * GetObjectClass();

            String GetPath();

            /**
             * Opens the file for read-write access. 
             * 
             * @param mode The mode to open the file in.
             * @return Whether the file was successfully opened.
             *
             * @note If this file handle was returned by a FileSystem and the absolute
             *      path is still valid, this function should always succeed. In single-
             *      threaded or fullscreen scenarios with few user tasks, this is usually 
             *      the case.
             * @warning If this file handle uses a relative path, then it is subject to
             *      changes in working directory by the any module in the process.
             *      FileSystem::GetFile() uses an absolute path in the constructor and
             *      is not susceptible to this.
             */
            bool Open
            (
                _In_ FileOpenMode mode
            );

            /**
             * Closes the file, preventing further access.
             * 
             * @return Success of the operation
             */
            bool Close();

            /**
             * Reads a chunk of data from the file. The file must have been opened for
             * reading previously, or this call will fail.
             * 
             * @param count The number of bytes to read, -1 for all.
             * @param buffer The buffer to be read into (may be null, see notes).
             * @return The number of bytes read.
             * 
             * @note If @arg buffer is null, the number of bytes that would have been 
             *    read is returned but the file position is unchanged. If @arg count is
             *    -1, this returns the filesize remaining.
             */
            int Read(_In_ int count, _In_opt_count_(count) void * buffer);

            /**
             * Writes a chunk of data to the file. The file must have been opened
             * for writing.
             * 
             * @param count The number of bytes to write.
             * @param buffer The data to write.
             * @return Success of the write operation.
             * 
             * @note If @arg buffer is null, @arg count zeroes are written into the
             *    file. This is useful for padding binary formats.
             */
            bool Write(_In_ int count, _In_opt_count_(count) void * buffer);

        private:
            HANDLE mHandle;
            String mName;
            Core * mCore;
        };

        class Image
            : public IImage
        {
        public:
            Image(Core * core, unsigned int image);

            ~Image();

            const char * GetObjectClass();

            /**
             * Retrieves texture format data from the image. The runtime can use this
             * to set up a texture or decide if it can handle the texture format.
             * 
             * @return Texture information.
             */
            TextureDesc GetImageDesc();

            /**
             * Retrieves a pointer to the image data.
             * 
             * @return Pointer to the image data.
             * 
             * @warning The pointer provided should <em>not</em> be deleted.
             */
            void * GetImageData();

        private:
            Core * mCore;
            unsigned int mImage;
        };
    }
}