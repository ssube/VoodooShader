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

#include "Exception.hpp"
#include "Regex.hpp"

#include "ICore.hpp"
#include "ILogger.hpp"
#include "IModule.hpp"
#include "IObject.hpp"
#include "IParser.hpp"

namespace VoodooShader
{
    IModuleManager::IModuleManager(_In_ ICore *core) :
        m_Core(core)
    { }

    IModuleManager::~IModuleManager(void)
    {
        m_Classes.clear();
        m_Modules.clear();
    }

    bool IModuleManager::LoadPath(_In_ String path, _In_ String filter)
    {
        String mask = m_Core->GetParser()->Parse(path) + "\\*";
        WIN32_FIND_DATA findFile;
        HANDLE searchHandle = FindFirstFile(filter.c_str(), &findFile);

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();

            if (error == ERROR_FILE_NOT_FOUND)
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "No plugin files found in directory %s.",
                    path.c_str()
                );

                return false;
            }
            else
            {
                m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, "Error searching directory %s.", path.c_str());

                return false;
            }
        }

        Regex compfilter(filter);

        do
        {
            String module = findFile.cFileName;

            if (compfilter.Match(module))
            {
                this->LoadFile(module);
            }
        } while (FindNextFile(searchHandle, &findFile) != 0);

        return true;
    }

    bool IModuleManager::LoadFile(_In_ String name)
    {
        ILoggerRef logger(m_Core->GetLogger());

        // Build the full path
        String path = m_Core->GetParser()->Parse(name);

        // Check for already loaded
        if (m_Modules.find(path) != m_Modules.end())
        {
            return true;
        }

        // Create struct and load functions
        IModule* rawmodule = IModule::Load(path);

        if (rawmodule == nullptr)
        {
            if (logger.get())
            {
                logger->Log(LL_Error, VOODOO_CORE_NAME, "Unable to load module %s.", path.c_str());
            }

            return nullptr;
        }

        IModule* module(rawmodule);

        m_Modules[path] = module;

        // Register classes from module
        Version moduleversion = rawmodule->ModuleVersion();

        if (moduleversion.Debug != VOODOO_META_DEBUG_BOOL && logger.get())
        {
            logger->Log
                (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Debug build mismatch with module %s.",
                moduleversion.Name.c_str()
                );
        }

        if (logger.get())
        {
            logger->LogModule(module->ModuleVersion());
        }

        int classCount = module->ClassCount();

        for (int curClass = 0; curClass < classCount; ++curClass)
        {
            const char *classname = module->ClassInfo(curClass);

            if (classname)
            {
                m_Classes[classname] = ClassID(module, curClass);
            }
        }

        return module;
    }

    void *IModuleManager::FindFunction(_In_ String module, _In_ String name)
    {
        HMODULE hmodule = GetModuleHandle(module.c_str());

        if (hmodule)
        {
            return GetProcAddress(hmodule, name.c_str());
        }
        else
        {
            return nullptr;
        }
    }

    bool IModuleManager::ClassExists(_In_ String name)
    {
        return (m_Classes.find(name) != m_Classes.end());
    }

    IObject *IModuleManager::CreateObject(_In_ String name)
    {
        ILogger* logger = m_Core->GetLogger();
        ClassMap::iterator classiter = m_Classes.find(name);

        if (classiter != m_Classes.end())
        {
            IModule* module = classiter->second.first.lock();
            int number = classiter->second.second;

            if (module.get())
            {
                try
                {
                    IObject *object = module->CreateClass(number, m_Core);

                    if (object == nullptr)
                    {
                        if (logger.get())
                        {
                            logger->Log
                                (
                                LL_Error,
                                VOODOO_CORE_NAME,
                                "Error creating instance of class %s.",
                                name.c_str()
                                );
                        }
                    }

                    return object;
                }
                catch(std::exception & exc)
                {
                    if (logger.get())
                    {
                        logger->Log
                            (
                            LL_Error,
                            VOODOO_CORE_NAME,
                            "Error creating class %s: %s",
                            name.c_str(),
                            exc.what()
                            );
                    }

                    return nullptr;
                }
            }
            else
            {
                if (logger.get())
                {
                    logger->Log
                        (
                        LL_Error,
                        VOODOO_CORE_NAME,
                        "Unable to lock module (possibly unloaded) for class %s.",
                        name.c_str()
                        );
                }

                return nullptr;
            }
        }
        else
        {
            if (logger.get())
            {
                logger->Log(LL_Error, VOODOO_CORE_NAME, "Class %s not found.", name.c_str());
            }

            return nullptr;
        }
    }

    IModule * IModule::Load(_In_ String path)
    {

        // Load the module
        HMODULE hmodule = LoadLibraryEx(path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if (hmodule != nullptr)
        {
            using namespace VoodooShader:: Functions;
            IModule *module = new IModule(hmodule);

            module->m_ModuleVersion = (VersionFunc) GetProcAddress(hmodule, "ModuleVersion");
            module->m_ClassCount = (CountFunc) GetProcAddress(hmodule, "ClassCount");
            module->m_ClassInfo = (InfoFunc) GetProcAddress(hmodule, "ClassInfo");
            module->m_ClassCreate = (CreateFunc) GetProcAddress(hmodule, "ClassCreate");

            if
            (
                module->m_ModuleVersion == nullptr ||
                module->m_ClassCount == nullptr ||
                module->m_ClassInfo == nullptr ||
                module->m_ClassCreate == nullptr
            )
            {
                // FreeLibrary(hmodule);
                delete module;
                return nullptr;
            }
            else
            {
                return module;
            }
        }
        else
        {
            return nullptr;
        }
    }

    IModule::IModule(HMODULE hmodule) 
        : m_Handle(hmodule)
    { }

    IModule::~IModule(void)
    {
        if (m_Handle != INVALID_HANDLE_VALUE)
        {
            FreeLibrary(m_Handle);
            m_Handle = nullptr;
        }
    }

    Version IModule::ModuleVersion(void)
    {
        return m_ModuleVersion();
    }

    int IModule::ClassCount(void)
    {
        return m_ClassCount();
    }

    String IModule::ClassInfo(_In_ Int32 number)
    {
        return m_ClassInfo(number);
    }

    IObject * IModule::CreateClass(_In_ Int32 number, _In_ ICore * pCore)
    {
        return m_ClassCreate(number, pCore);
    }
}
