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
#include "Version.hpp"

#include "ICore.hpp"
#include "IFilesystem.hpp"
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

    int32_t IModuleManager::AddRef() const
    {
        return ++m_Refs;
    }

    int32_t IModuleManager::Release() const
    {
        int32_t count = --m_Refs;
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    String IModuleManager::ToString() const
    {
        return L"ModuleManager";
    }

    ICore * IModuleManager::GetCore() const
    {
        return m_Core;
    }

    bool IModuleManager::LoadPath(_In_ const String & path, _In_ const String & filter)
    {
        String mask = m_Core->GetParser()->Parse(path) + L"\\*";
        WIN32_FIND_DATA findFile;
        HANDLE searchHandle = FindFirstFile(filter.GetData(), &findFile);

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();

            if (error == ERROR_FILE_NOT_FOUND)
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    L"No plugin files found in directory %s.",
                    path.GetData()
                );

                return false;
            } else {
                m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, L"Error searching directory %s.", path.GetData());

                return false;
            }
        }

        Regex compfilter(filter);

        do
        {
            String module = findFile.cFileName;

            if (compfilter.Match(module))
            {
                IFile * modulefile = this->m_Core->GetFileSystem()->FindFile(module);
                this->LoadFile(modulefile);
            }
        } while (FindNextFile(searchHandle, &findFile) != 0);

        return true;
    }

    bool IModuleManager::LoadFile(_In_ const IFile * pFile)
    {
        ILoggerRef logger = m_Core->GetLogger();

        // Build the full path
        String path = pFile->GetPath();

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
                logger->Log(LL_Error, VOODOO_CORE_NAME, L"Unable to load module %s.", path.GetData());
            }

            return false;
        }

        IModule* module(rawmodule);

        m_Modules[path] = module;

        // Register classes from module
        const Version * moduleversion = rawmodule->ModuleVersion();

        if (moduleversion->Debug != VOODOO_META_DEBUG_bool && logger.get())
        {
            logger->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                L"Debug build mismatch with module %s.",
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
            const char * classname = module->ClassInfo(curClass);

            if (classname)
            {
                m_Classes[classname] = ClassID(module, curClass);
            }
        }

        return true;
    }

    void * IModuleManager::FindFunction(_In_ const String & module, _In_ const String & name) const
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

    bool IModuleManager::ClassExists(_In_ const String & name) const
    {
        return (m_Classes.find(name) != m_Classes.end());
    }

    IObject * IModuleManager::CreateObject(_In_ const String & name) const
    {
        ILogger* logger = m_Core->GetLogger();
        ClassMap::const_iterator classiter = m_Classes.find(name);

        if (classiter != m_Classes.end())
        {
            IModuleRef module = classiter->second.first;
            int number = classiter->second.second;

            if (module)
            {
                try
                {
                    IObject *object = module->CreateClass(number, m_Core);

                    if (object == nullptr)
                    {
                        if (logger)
                        {
                            logger->Log
                            (
                                LL_Error,
                                VOODOO_CORE_NAME,
                                L"Error creating instance of class %s.",
                                name.GetData()
                            );
                        }
                    }

                    return object;
                } catch(const std::exception & exc) {
                    if (logger)
                    {
                        logger->Log
                        (
                            LL_Error,
                            VOODOO_CORE_NAME,
                            L"Error creating class %s: %s",
                            name.GetData(),
                            exc.what()
                        );
                    }

                    return nullptr;
                }
            } else {
                if (logger)
                {
                    logger->Log
                    (
                        LL_Error,
                        VOODOO_CORE_NAME,
                        L"Unable to lock module (possibly unloaded) for class %s.",
                        name.GetData()
                    );
                }

                return nullptr;
            }
        } else {
            if (logger)
            {
                logger->Log(LL_Error, VOODOO_CORE_NAME, L"Class %s not found.", name.GetData());
            }

            return nullptr;
        }
    }

    IModule * IModule::Load(_In_ String path)
    {
        // Load the module
        HMODULE hmodule = LoadLibraryEx(path.GetData(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if (hmodule != nullptr)
        {
            IModule * module = new IModule(hmodule);

            module->m_ModuleVersion = reinterpret_cast<Functions::VersionFunc>(GetProcAddress(hmodule, "ModuleVersion"));
            module->m_ClassCount = reinterpret_cast<Functions::CountFunc>(GetProcAddress(hmodule, "ClassCount"));
            module->m_ClassInfo = reinterpret_cast<Functions::InfoFunc>(GetProcAddress(hmodule, "ClassInfo"));
            module->m_ClassCreate = reinterpret_cast<Functions::CreateFunc>(GetProcAddress(hmodule, "ClassCreate"));

            if
            (
                module->m_ModuleVersion == nullptr ||
                module->m_ClassCount == nullptr ||
                module->m_ClassInfo == nullptr ||
                module->m_ClassCreate == nullptr
            )
            {
                delete module;
                return nullptr;
            } else {
                return module;
            }
        } else {
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

    const Version * IModule::ModuleVersion(void) const
    {
        return m_ModuleVersion();
    }

    int IModule::ClassCount(void) const
    {
        return m_ClassCount();
    }

    const char * IModule::ClassInfo(_In_ int32_t number) const
    {
        return m_ClassInfo(number);
    }

    IObject * IModule::CreateClass(_In_ int32_t number, _In_ ICore * pCore)
    {
        return m_ClassCreate(number, pCore);
    }
}
