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

#ifndef VOODOO_FILESYSTEM_HPP
#define VOODOO_FILESYSTEM_HPP

#include "Meta.hpp"
#include "IObject.hpp"

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

        virtual void AddDirectory(_In_ String path) = 0;
        virtual void RemoveDirectory(_In_ String path) = 0;

        virtual IFileRef FindFile
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

        virtual String GetPath() = 0;

        virtual bool Open
        (
            _In_ FileOpenMode mode
        ) = 0;

        virtual IImageRef OpenImage() = 0;

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

        virtual String GetPath() = 0;

        virtual TextureDesc GetDesc() = 0;

        virtual size_t GetData(_In_ TextureRegion desc, _In_opt_ void * buffer) = 0;
    };
}

#endif /*VOODOO_FILESYSTEM_HPP*/