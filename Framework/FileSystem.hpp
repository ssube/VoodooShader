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

#include "Meta.hpp"

namespace VoodooShader
{
    /**
     * Provides a unified file management system for loading shaders and other
     * resources.
     */
    class FileSystem
    {
    public:
        FileSystem
        (
            _In_ Core * core
        );

        ~FileSystem();

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
         */
        FileRef GetFile
        (
            _In_ String name
        );

    private:
        StringList mDirectories;
        Core * mCore;
    };

    class File
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

        /**
         * Opens the file for read-write access. 
         * 
         * @param mode The mode to open the file in.
         * @return Whether the file was successfully opened.
         *
         * @note If this file handle was returned by a FileSystem and the absolute
         *      path is still valid, this function should always succeed. In single-
         *      threaded or fullscreen scenarios, this is usually correct.
         * @warning If this file handle uses a relative path, then it is subject to
         *      changes in working directory by the any module in the process.
         *      FileSystem::GetFile() uses an absolute path in the constructor and
         *      is not susceptible to this.
         */
        bool Open
        (
            _In_ FileOpenMode mode
        );

        bool Close();



    private:
        HANDLE mHandle;
        String mName;
        Core * mCore;
    };
}