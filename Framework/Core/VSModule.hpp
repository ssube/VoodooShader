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

#include "IModule.hpp"

namespace VoodooShader
{
    /**
     * @par CLSID
     *      e6f312a0-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSModuleManager, IModuleManager, {0xA0, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VSModuleManager(_In_ ICore * pCore);
        virtual ~VSModuleManager();

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD(LoadPath)(_In_ const String & path, _In_ const String & filter);
        VOODOO_METHOD(LoadFile)(_In_ const IFile * pFile);
        VOODOO_METHOD(LoadFile)(_In_ const String & filename);
        VOODOO_METHOD(ClassExists)(_In_ const Uuid & clsid) const;
        VOODOO_METHOD(ClassExists)(_In_ const String & name) const;
        _Check_return_ VOODOO_METHOD_(IObject *, CreateObject)(_In_ const Uuid & clsid) const;
        _Check_return_ VOODOO_METHOD_(IObject *, CreateObject)(_In_ const String & name) const;
        _Check_return_ VOODOO_METHOD_(void *, FindFunction)(_In_ const String & module, _In_ const String & name) const;

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;

        ModuleMap m_Modules;
        ClassMap m_Classes;
        ClassNameMap m_ClassNames;
    };

    /**
     * @par CLSID
     *      e6f3129f-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSModule, IModule, {0x9F, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        static VSModule * Load(_In_ ICore * const pCore, _In_ const String & path);

        VSModule(_In_ ICore * pCore, _In_ HMODULE hmodule);
        virtual ~VSModule();

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD_(const Version *, ModuleVersion)() CONST;
        VOODOO_METHOD_(const uint32_t, ClassCount)() CONST;
        VOODOO_METHOD_(const wchar_t *, ClassInfo)(_In_ const uint32_t number, _Out_ Uuid * pUuid) CONST;
        VOODOO_METHOD_(IObject *, CreateClass)(_In_ const uint32_t number, _In_ ICore * pCore) CONST;

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;

        HMODULE m_Handle;
        Functions::VersionFunc m_ModuleVersion;
        Functions::CountFunc m_ClassCount;
        Functions::InfoFunc m_ClassInfo;
        Functions::CreateFunc m_ClassCreate;
    };
}
