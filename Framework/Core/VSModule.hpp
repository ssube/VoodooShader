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

#include "IModule.hpp"

namespace VoodooShader
{
    class VSModuleManager : 
        public IModuleManager
    {
    public:
        VSModuleManager(_In_ ICore * pCore);

        virtual ~VSModuleManager(void);
        
        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual bool LoadPath(_In_ const String & path, _In_ const String & filter);
        virtual bool LoadFile(_In_ const IFile * pFile);
        virtual bool ClassExists(_In_ const String & name) const;
        _Check_return_ virtual IObject * CreateObject(_In_ const String & name) const;
        _Check_return_ virtual void * FindFunction(_In_ const String & module, _In_ const String & name) const;

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
        ModuleMap m_Modules;
        ClassMap m_Classes;
    };

    class VSModule : 
        public IModule
    {
    public:
        static VSModule * Load(_In_ String path);

        VSModule(_In_ HMODULE hmodule);

        virtual ~VSModule(void);
        
        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual const Version * ModuleVersion(void) const throw();
        virtual const uint32_t ClassCount(void) const;
        virtual const char * ClassInfo(_In_ const uint32_t number) const;
        virtual IObject * CreateClass(_In_ const uint32_t number, _In_ ICore * pCore);

    private:
        mutable uint32_t m_Refs;
        HMODULE m_Handle;
        Functions::VersionFunc m_ModuleVersion;
        Functions::CountFunc m_ClassCount;
        Functions::InfoFunc m_ClassInfo;
        Functions::CreateFunc m_ClassCreate;
    };
    /* @} */
}
