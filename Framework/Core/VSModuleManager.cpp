/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

#include "shlwapi.h"

namespace VoodooShader
{
    DebugCache(VSModuleManager);

    VSModuleManager::VSModuleManager(_In_ ICore *core) :
        m_Refs(0), m_Core(core)
    {
        AddThisToDebugCache(VSModuleManager);
    }

    VSModuleManager::~VSModuleManager()
    {
        RemoveThisFromDebugCache(VSModuleManager);

        m_Classes.clear();
        m_Modules.clear();
    }

    uint32_t VOODOO_METHODTYPE VSModuleManager::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSModuleManager::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    bool VOODOO_METHODTYPE VSModuleManager::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSModuleManager;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            }
            else if (clsid == IID_IModuleManager)
            {
                *ppOut = static_cast<const IModuleManager*>(this);
            }
            else if (clsid == CLSID_VSModuleManager)
            {
                *ppOut = static_cast<const VSModuleManager*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSModuleManager::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return VSTR("VSModuleManager()");
    }

    ICore * VOODOO_METHODTYPE VSModuleManager::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    bool VOODOO_METHODTYPE VSModuleManager::IsLoaded(_In_ const String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return (m_ModuleNames.find(name) != m_ModuleNames.end());
    }

    bool VOODOO_METHODTYPE VSModuleManager::IsLoaded(_In_ const Uuid & libid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return (m_Modules.find(libid) != m_Modules.end());
    }

    bool VOODOO_METHODTYPE VSModuleManager::LoadPath(_In_ const String & path, _In_ const String & filter)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        String mask = m_Core->GetParser()->Parse(path) + VSTR("\\*");

        WIN32_FIND_DATA findFile;
        HANDLE searchHandle = FindFirstFile(mask.GetData(), &findFile);

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();

            if (error == ERROR_FILE_NOT_FOUND)
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreWarning, VOODOO_CORE_NAME,
                    Format("No plugins found in directory '%1%'.") << path
                );

                return false;
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreWarning, VOODOO_CORE_NAME, 
                    Format("Error searching directory '%1%'.") << path
                );

                return false;
            }
        }

        Regex compfilter;
        if (filter.IsEmpty())
        {
            compfilter.SetExpr(VSTR(".*\\.dll"));
        }
        else
        {
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
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        ILoggerRef logger = m_Core->GetLogger();

        String fullname = m_Core->GetParser()->Parse(filename, PF_PathCanon);

        // Check for relative
        if (PathIsRelative(fullname.GetData()))
        {
            fullname = m_Core->GetParser()->Parse(String(VSTR("$(path)\\")) + fullname, PF_PathCanon);
        }

        // Check for already loaded
        if (m_ModuleNames.find(fullname) != m_ModuleNames.end())
        {
            return true;
        }

        // Create struct and load functions
        IModuleRef module = VSModule::Load(m_Core, fullname);

        if (module == nullptr)
        {
            if (logger)
            {
                logger->LogMessage
                (
                    LL_CoreError, VOODOO_CORE_NAME, 
                    Format("Unable to load module '%1%'.") << filename
                );
            }

            return false;
        }

        // Register classes from module
        const Version * moduleversion = module->ModuleVersion();

        if (!moduleversion)
        {
            if (logger)
            {
                logger->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, Format("Null version returned by module '%1%'.") << fullname);
            }
            return false;
        }

        m_ModuleNames[fullname] = moduleversion->LibId;

        if (moduleversion->Debug != VOODOO_DEBUG_BOOL && logger)
        {
            logger->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME,
                Format("Debug build mismatch with module '%1%'.") << moduleversion->Name
            );
        }

        if (logger)
        {
            logger->LogMessage(LL_CoreNotice, VOODOO_CORE_NAME, Format("Loaded module: %1%") << *module->ModuleVersion());
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
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (pFile)
        {
            return this->LoadFile(pFile->GetPath());
        }
        else
        {
            return nullptr;
        }
    }

    bool VOODOO_METHODTYPE VSModuleManager::ClassExists(_In_ const Uuid & clsid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return (m_Classes.find(clsid) != m_Classes.end());
    }

    bool VOODOO_METHODTYPE VSModuleManager::ClassExists(_In_ const String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        Uuid clsid;
        if (!name.ToUuid(&clsid))
        {
            StrongNameMap::const_iterator classiter = m_ClassNames.find(name);
            if (classiter != m_ClassNames.end())
            {
                clsid = classiter->second;
            }
            else
            {
                return false;
            }
        }

        return this->ClassExists(clsid);
    }

    IObject * VOODOO_METHODTYPE VSModuleManager::CreateObject(_In_ const Uuid & clsid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
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
                    logger->LogMessage
                    (
                        LL_CoreError, VOODOO_CORE_NAME,
                        Format("Error creating instance of class %1%.") << clsid
                    );
                }
            }

            return object;
        }
        else
        {
            if (logger)
            {
                logger->LogMessage
                (
                    LL_CoreError, VOODOO_CORE_NAME, 
                    Format("Class %1% not found.") << clsid
                );
            }

            return nullptr;
        }
    }

    IObject * VOODOO_METHODTYPE VSModuleManager::CreateObject(_In_ const String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        Uuid clsid;
        if (!name.ToUuid(&clsid))
        {
            StrongNameMap::const_iterator classiter = m_ClassNames.find(name);
            if (classiter != m_ClassNames.end())
            {
                clsid = classiter->second;
            }
            else
            {
                return nullptr;
            }
        }

        return this->CreateObject(clsid);
    }
}
