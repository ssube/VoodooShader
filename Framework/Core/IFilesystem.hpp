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
        virtual ~IFileSystem(void) throw() {};
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Adds a directory to this file system's search path.
         * 
         * @param path The path to add (may include variables, will be parsed, may be absolute or relative).
         * 
         * @note This does not check for duplicates, so care should be taken not to add the same directory repeatedly.
         *     Directory search order is important for file priority.
         */
        virtual bool AddPath(_In_ const String & path) throw() = 0;

        /**
         * Removes a directory from the search path, following the same rules as adding (adding a string then removing the
         * same string will always work).
         * 
         * @param path The path to remove.
         */
        virtual bool RemovePath(_In_ const String & path) throw() = 0;

        /**
         * Resolves a relative filename (usually just filename and extension, but may include directories) into an 
         * absolute filename by searching the list of resource directories registered with this manager. 
         * 
         * @param The file name to find.
         * @return The file, if found.
         */
        virtual IFile * FindFile(_In_ const String & name) const throw() = 0;
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
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Gets the absolute path of this file.
         */
        virtual String GetPath(void) const throw() = 0;

        virtual bool Open(_In_ FileOpenMode mode) throw() = 0;

        virtual bool Close(void) throw() = 0;

        virtual int32_t Read(_In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) throw() = 0;

        virtual int32_t Write(_In_ const int32_t size, _In_opt_count_(size) void * const pBuffer) throw() = 0;

        virtual IImage * OpenImage(void) const throw() = 0;
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
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        virtual String GetPath(void) const throw() = 0;

        virtual TextureDesc GetDesc(void) const throw() = 0;

        virtual uint32_t GetData(_In_ const TextureRegion * pDesc, _In_ const uint32_t size, _In_opt_count_(size) void * const pBuffer) const throw() = 0;
    };
}
