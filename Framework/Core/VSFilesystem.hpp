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

#include "Includes.hpp"

#include "IFilesystem.hpp"

namespace VoodooShader
{
    class VSFileSystem :
        public IFileSystem
    {
    public:
        VSFileSystem(_In_ ICore * pCore);
        virtual ~VSFileSystem(void);

        virtual int32_t AddRef() const throw();
        virtual int32_t Release() const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual bool AddPath(_In_ const String & dir) throw();
        virtual bool RemovePath(_In_ const String & dir) throw();
        virtual IFile * FindFile(_In_ const String & name) const throw();

    private:
        mutable int32_t m_Refs;
        ICore * m_Core;
    };
}
