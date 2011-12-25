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

#include "VSModuleManager.hpp"

#include "VSModule.hpp"

#include "ICore.hpp"
#include "IFile.hpp"
#include "IFilesystem.hpp"
#include "ILogger.hpp"
#include "IObject.hpp"
#include "IParser.hpp"

#include "Exception.hpp"
#include "Regex.hpp"
#include "Version.hpp"

#include "shlwapi.h"

namespace VoodooShader
{
    VSModuleManager::VSModuleManager(_In_ ICore *core) :
        m_Refs(0), m_Core(core)
    { }

    VSModuleManager::~VSModuleManager()
    {
        m_Classes.clear();
        m_Modules.clear();
    }

    uint32_t VOODOO_METHODTYPE VSModuleManager::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSModuleManager::Release() CONST
    {
        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    bool VOODOO_METHODTYPE VSModuleManager::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSModuleManager;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IModuleManager) {
                *ppOut = static_cast<const IModuleManager*>(this);
            } else if (clsid == CLSID_VSModuleManager) {
                *ppOut = static_cast<const VSModuleManager*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSModuleManager::ToString() CONST
    {
        return VSTR("VSModuleManager()");
    }

    ICore * VOODOO_METHODTYPE VSModuleManager::GetCore() CONST
    {
        return m_Core;
    }

    bool VOODOO_METHODTYPE VSModuleManager::LoadPath(_In_ const String & path, _In_ const String & filter)
    {
        String mask = m_Core->GetParser()->Parse(path) + VSTR("\\*");

        WIN32_FIND_DATA findFile;
        HANDLE searchHandle = FindFirstFile(mask.GetData(), &findFile);

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();

            if (error == ERROR_FILE_NOT_FOUND)
            {
                m_Core->GetLogger()->Log
                (
                    LL_CoreWarn, VOODOO_CORE_NAME,
                    VSTR("No plugins found in directory '") VPFVSTR VSTR("'."),
                    path.GetData()
                );

                return false;
            } else {
                m_Core->GetLogger()->Log
                (
                    LL_CoreWarn, VOODOO_CORE_NAME, 
                    VSTR("Error searching directory '") VPFVSTR VSTR("'."), path.GetData()
                );

                return false;
            }
        }

        Regex compfilter;
        if (filter.IsEmpty())
        {
            compfilter.SetExpr(VSTR(".*\\.dll"));
        } else {
            compfilter.SetExpr(filter);
        }

        do
        {
            String module = findFile.cFileName;

            if (compfilter.Find(module))
            {
                IFile * modulefile = m_Core->GetFileSystem()->GetFile(module);
                this->LoadFile(modulefile);
            }
        } while (FindNextFile(searchHandle, &findFile) != 0);

        return true;
    }

    bool VOODOO_METHODTYPE VSModuleManager::LoadFile(_In_ const String & filename)
    {
        ILoggerRef logger = m_Core->GetLogger();

        String fullname = m_Core->GetParser()->Parse(filename, PF_PathCanon);

        // Check for relative
        if (PathIsRelative(fullname.GetData()))
        {
            fullname = m_Core->GetParser()->Parse(String(VSTR("$(path)\\")) + fullname, PF_PathCanon);
        }

        // Check for already loaded
        if (m_Modules.find(fullname) != m_Modules.end())
        {
            return true;
        }

        // Create struct and load functions
        IModuleRef module = VSModule::Load(m_Core, fullname);

        if (module == nullptr)
        {
            if (logger.get())
            {
                logger->Log(LL_CoreError, VOODOO_CORE_NAME, VSTR("Unable to load module '") VPFVSTR VSTR("'."), filename.GetData());
            }

            return false;
        }

        m_Modules[fullname] = module;

        // Register classes from module
        const Version * moduleversion = module->ModuleVersion();

        if (moduleversion->Debug != VOODOO_META_DEBUG_BOOL && logger.get())
        {
            logger->Log
                (
                LL_CoreWarn,
                VOODOO_CORE_NAME,
                VSTR("Debug build mismatch with module '") VPFVSTR VSTR("'."),
                moduleversion->Name
                );
        }

        if (logger.get())
        {
            logger->LogModule(module->ModuleVersion());
        }

        int classCount = module->ClassCount();

        for (int curClass = 0; curClass < classCount; ++curClass)
        {
            Uuid clsid;
            ZeroMemory(&clsid, sizeof(Uuid));

            const wchar_t * classname = module->ClassInfo(curClass, &clsid);

            if (classname)
            {
                m_Classes.insert(std::pair<Uuid, ClassSource>(clsid, ClassSource(module, curClass)));
                m_ClassNames.insert(std::pair<String, Uuid>(classname, clsid));
            }
        }

        return true;
    }


    bool VOODOO_METHODTYPE VSModuleManager::LoadFile(_In_ const IFile * pFile)
    {
        if (pFile)
        {
            return this->LoadFile(pFile->GetPath());
        } else {
            return nullptr;
        }
    }

    void * VOODOO_METHODTYPE VSModuleManager::FindFunction(_In_ const String & module, _In_ const String & name) CONST
    {
        HMODULE hmodule = GetModuleHandle(module.GetData());

        if (hmodule)
        {
            int32_t len = name.ToCharStr(0, nullptr);
            std::vector<char> buffer(len);
            len = name.ToCharStr(len, &buffer[0]);
            return GetProcAddress(hmodule, &buffer[0]);
        }
        else
        {
            return nullptr;
        }
    }

    bool VOODOO_METHODTYPE VSModuleManager::ClassExists(_In_ const Uuid & clsid) CONST
    {
        return (m_Classes.find(clsid) != m_Classes.end());
    }

    bool VOODOO_METHODTYPE VSModuleManager::ClassExists(_In_ const String & name) CONST
    {
        Uuid clsid;
        if (!name.ToUuid(&clsid))
        {
            ClassNameMap::const_iterator classiter = m_ClassNames.find(name);
            if (classiter != m_ClassNames.end())
            {
                clsid = classiter->second;
            } else {
                return false;
            }
        }

        return this->ClassExists(clsid);
    }

    IObject * VOODOO_METHODTYPE VSModuleManager::CreateObject(_In_ const Uuid & clsid) CONST
    {
        ILogger* logger = m_Core->GetLogger();
        ClassMap::const_iterator classiter = m_Classes.find(clsid);

        if (classiter != m_Classes.end())
        {
            IModuleRef module = classiter->second.first;
            uint32_t index = classiter->second.second;

            IObject * object = module->CreateClass(index, m_Core);

            if (object == nullptr)
            {
                if (logger)
                {
                    logger->Log
                    (
                        LL_CoreError, VOODOO_CORE_NAME,
                        VSTR("Error creating instance of class '") VPFVSTR VSTR("'."),
                        String(clsid).GetData()
                    );
                }
            }

            return object;
        } else {
            if (logger)
            {
                logger->Log(LL_CoreError, VOODOO_CORE_NAME, 
                    VSTR("Class '") VPFVSTR VSTR("' not found."), String(clsid).GetData());
            }

            return nullptr;
        }
    }

    IObject * VOODOO_METHODTYPE VSModuleManager::CreateObject(_In_ const String & name) CONST
    {
        Uuid clsid;
        if (!name.ToUuid(&clsid))
        {
            ClassNameMap::const_iterator classiter = m_ClassNames.find(name);
            if (classiter != m_ClassNames.end())
            {
                clsid = classiter->second;
            } else {
                return nullptr;
            }
        }

        return this->CreateObject(clsid);
    }
}
