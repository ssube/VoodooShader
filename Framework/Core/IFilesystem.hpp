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
 **/
#pragma once

#include "IObject.hpp"

namespace VoodooShader
{
    class IFileSystem :
        public IObject
    {
    public:
        /**
         * Base virtual destructor for all object classes. This throws when called, and so must be overridden.
         */
        virtual ~IFileSystem(void) throw() { };
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 AddRef(void) throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 Release(void) throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) throw() = 0;

        virtual void AddDirectory(_In_ String path) = 0;
        virtual void RemoveDirectory(_In_ String path) = 0;

        virtual IFile * FindFile(_In_ String name) = 0;
    };

    class IFile :
        public IObject
    {
    public:
        ~IFile(void) throw() { };
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 AddRef(void) throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 Release(void) throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) throw() = 0;

        virtual String GetPath(void) = 0;

        virtual Bool Open(_In_ FileOpenMode mode) = 0;

        virtual IImage * OpenImage(void) = 0;

        virtual Bool Close(void) = 0;

        virtual Int32 Read(_In_ Int32 count, _In_opt_count_(count) void * pBuffer) = 0;

        virtual Bool Write(_In_ Int32 count, _In_opt_count_(count) void * pBuffer) = 0;
    };

    class IImage :
        public IObject
    {
    public:
        ~IImage(void) throw() { };
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 AddRef(void) throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 Release(void) throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) throw() = 0;

        virtual String GetPath(void) = 0;

        virtual TextureDesc GetDesc(void) = 0;

        virtual Int32 GetData(_In_ TextureRegion desc, _In_opt_ void * pBuffer) = 0;
    };
}
