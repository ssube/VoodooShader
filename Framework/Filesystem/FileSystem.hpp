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
         * resources. This filesystem implementation is a thin wrapper for the 
         * Windows API, adding only a few nonstandard functions (directory searching
         * and path variables).
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
            static IImageRef Load(Core * core, String name);

            Image(Core * core, String name, unsigned int image);

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
             * Retrieves a portion of the texture data from the image.
             * 
             * @param desc The region and format to be returned.
             * @param buffer The memory for the return data to be placed in. Must
             *      already be allocated, or null.
             * @return The number of bytes retrieved (or, if @arg buffer is null, the
             *      number that would be retrieved).
             * @throws Exception on invalid texture format.
             *      
             * @warning Due to limitations in this library (or DevIL, not sure which),
             *      the only texture formats this function can convert into are TF_RGB8,
             *      TF_RGBA8, TF_RGBA16F, and TF_RGBA32F. Others are not supported and
             *      will cause this function to throw.
             * @note This can convert data between most formats, so the format given in
             *      @arg desc will be the returned format. This makes calculating the
             *      buffer size relatively easy.
             * @warning If this function converts formats or copies a large region, it
             *      will be slow. Avoid calling often.
             */
            size_t CopyImageData(_In_ TextureRegion desc, _In_opt_ void * buffer);

            /**
             * Retrieves a pointer to the image data.
             * 
             * @return Pointer to the image data.
             * 
             * @warning The pointer provided should <em>not</em> be deleted. To free
             *     the data, call Image::FreeImageData().
             */
            void * GetImageData();

            void FreeImageData();

        private:
            Core * mCore;
            String mName;
            unsigned int mImage;
            TextureDesc mDesc;
        };
    }
}

/**
 * @page pathvars Path Variables
 * <p>
 *    The default filesystem implementation for Voodoo provides support for a small
 *    variety of variables to be used in paths. The syntax and use of these variables
 *    is described in this page.
 * </p>
 * @todo Move variable parsing into the Core or a dedicated parser class used by
 *       the Core and Filesystem. The scope of this parsing is to great for the
 *       filesystem alone and should not vary depending on filesystem implementation.
 *       
 * @section pathvarssyntax Syntax
 * <p>
 *    Variables are used in paths by surrounding the variable name with '$' symbols.
 *    For each variable, the <code>$varname$</code> portion will be removed and the
 *    value of <code>varname</code> used in place. Variables may be placed at any
 *    position in the path, but may not be nested.
 *    
 * @note The $ character is illegal in filenames, but may be useful when working
 *       with file streams.
 * </p>
 * <p>
 * Examples:
 * @code 
 * localroot = M:\VoodooShader\
 * gempath = M:\VoodooShader\Gem\
 * resourcedir = \resources\
 * 
 * $localroot$\subdir\file.png = M:\VoodooShader\\subdir\file.png
 * $gempath$\shaders\test.cgfx = M:\VoodooShader\Gem\\shaders\test.cgfx
 * $localroot$\$resourcedir$\image.dds = M:\VoodooShader\\\resources\\image.dds
 * @endcode
 * 
 * @note Repeated consecutive slashes will not cause errors with path parsing,
 *       although in some locations they have special meanings.
 * </p>
 *        
 * @subsection pathsvarsescape Escaping
 * @since 0.1.4.158
 * 
 * To insert a $ into the path, place two $ symbols consecutively (an empty 
 * variable name). 
 * @code
 * $localroot$\file.txt:$$Stream = M:\VoodooShader\\file.txt:$Stream
 * @endcode
 * 
 * @section pathvarserror Errors
 * <p>
 *    If a variable cannot be resolved, an error value will be used in place of the
 *    variable. This value is designed to cause the path to fail in any operations.
 *    The error value will replace the <code>$varname$</code> with 
 *    <code>--badvar:varname--</code>.
 * </p>
 * @section pathvarsbuiltin Built-In Variables
 * <p>
 *    Three built-in variables are provided for path use. These represent paths
 *    that are otherwise difficult to resolve and may vary each run. These variables
 *    are all \\-terminated.
 * </p>
 * @subsection pathvarsbuiltinlocal $localroot$
 * <p>
 *    The local root path is the location of the target executable, that is, the
 *    program that Voodoo is loaded into. This is the absolute path to the file.
 *    This path is retrieved from the Windows API during startup.
 * </p>
 * <p>
 *    Example:
 * @code
 * TargetEXE = H:\Morrowind\Morrowind.exe
 * $localroot$ = H:\Morrowind\
 * @endcode
 * </p>
 * @subsection pathvarsbuiltinglobal $globalroot$ 
 * <p>
 *    The global root path of Voodoo. This is the main Voodoo folder, containing
 *    most global resources and binaries. This path is retrieved from the registry
 *    by the Voodoo loader.
 * </p>
 * <p>
 *    Example:
 * @code
 * $globalroot$\bin\ = Voodoo binary path
 * @endcode
 * </p>
 * @subsection pathvarsbuiltinrun $runroot$
 * <p>
 *    The running root of Voodoo. This is the directory that the target was
 *    started in (the startup working directory). This path is retrieved from the
 *    Windows API by the loader during startup. This is the most variable of the
 *    builtin variables; it may change each run, depending on how the target
 *    is started.
 * </p>
 * 
 * @section pathvarsconfig Config Variables
 * @note Not yet implemented.
 * <p>
 *    When the filesystem module is loaded, it retrieves any variables from the
 *    config (using the XPath query "/VoodooConfig/Filesystem/Variables/Variable").
 *    These variables must have a @p name attribute and text. They are added to the
 *    filesystem's variable list. The text of these variables may contain other
 *    variables, standard parsing rules apply.
 * </p>
 * @note Variables with identical names will overwrite their previous value. The
 *       builtin variables cannot be overwritten.
 * 
 * @section pathvarsenviron Environment Variables
 * <p>
 *    If a variable name is given that cannot be found in the list of loaded and
 *    builtin variables, it will be assumed to be an environment variable. These
 *    variables are pulled directly from the process' environment and so may or
 *    may not be useful. 
 * </p>
 * @warning Care should be taken while using environment variables; config variables
 *          are much preferred.
 */