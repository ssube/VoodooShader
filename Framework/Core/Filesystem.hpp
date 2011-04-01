/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * While the Voodoo Shader Framework, as a whole, is licensed under the GNU General Public license,
 * adapters designed to work with the framework do not constitute derivative works. In the case an
 * adapter needs to use this file (Adapter.hpp) or Meta.hpp in order to work with the Framework, and
 * is not licensed under the GNU GPL or a compatible license, please contact the developer to figure
 * out alternate licensing arrangements.
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

#ifndef VOODOO_FILESYSTEM_HPP
#define VOODOO_FILESYSTEM_HPP

#include "Meta.hpp"
#include "Object.hpp"

namespace VoodooShader
{
    class IFileSystem
        : public IObject
    {
    public:
        /**
         * Base virtual destructor for all object classes. This throws when called, and so
         * must be overridden.
         */
        virtual ~IFileSystem() throw()
        { };

        /**
         * Get the name of this object's class. May be used for identification or validation
         * purposes.
         * 
         * @note The returned string may be compared, but must not be deleted or modified.
         */
        virtual const char * GetObjectClass() = 0;

        virtual void AddDirectory(_In_ String path) = 0;
        virtual void RemoveDirectory(_In_ String path) = 0;

        virtual IFileRef GetFile
        (
            _In_ String name
        ) = 0;

        virtual IImageRef GetImage
        (
            _In_ String name
        ) = 0;
    };

    class IFile
        : public IObject
    {
    public:
        ~IFile() throw()
        { };

        virtual const char * GetObjectClass() = 0;

        virtual String GetPath() = 0;

        virtual bool Open
        (
            _In_ FileOpenMode mode
        ) = 0;

        virtual bool Close() = 0;

        virtual int Read(_In_ int count, _In_opt_count_(count) void * buffer) = 0;

        virtual bool Write(_In_ int count, _In_opt_count_(count) void * buffer) = 0;
    };

    class IImage
        : public IObject
    {
    public:
        ~IImage() throw()
        { };

        virtual const char * GetObjectClass() = 0;

        virtual TextureDesc GetImageDesc() = 0;

        virtual size_t CopyImageData(_In_ TextureRegion desc, _In_opt_ void * buffer) = 0;

        virtual void * GetImageData() = 0;

        virtual void FreeImageData() = 0;
    };
}

#endif /*VOODOO_FILESYSTEM_HPP*/