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

#include "IFileSystem.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_core_null Null Implementations
     * @ingroup voodoo_core
     * 
     * @{
     */

    /**
     * @addtogroup voodoo_uuids 
     */
    DEFINE_CLSID(VSFileSystem) = {0x9C, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

    /**
     * Voodoo Shader null file system implementation. Returns true/nullptr as needed. <em>Does not have associated IFile or
     * IImage implementations, as they will never be returned.</em>
     * 
     * @par CLSID:
     *      e6f3129c-05af-11e1-9e05-005056c00008 
     */
    class VSFileSystem :
        public IFileSystem
    {
    public:
        VSFileSystem(_In_ ICore * pCore);
        virtual ~VSFileSystem(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual bool QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual bool AddPath(_In_ const String & dir) throw();
        virtual bool RemovePath(_In_ const String & dir) throw();
        virtual IFile * FindFile(_In_ const String & name) const throw();

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
    };
    /**
     * @}
     */
}
