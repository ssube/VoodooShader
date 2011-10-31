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

#include "VSModule.hpp"

#include "Exception.hpp"
#include "Regex.hpp"
#include "Version.hpp"

#include "ICore.hpp"
#include "IFilesystem.hpp"
#include "ILogger.hpp"
#include "IObject.hpp"
#include "IParser.hpp"

namespace VoodooShader
{
    VSModuleManager::VSModuleManager(_In_ ICore *core) :
        m_Core(core)
    { }

    VSModuleManager::~VSModuleManager(void)
    {
        m_Classes.clear();
        m_Modules.clear();
    }

    uint32_t VSModuleManager::AddRef() const
    {
        return ++m_Refs;
    }

    uint32_t VSModuleManager::Release() const
    {
        uint32_t count = --m_Refs;
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    String VSModuleManager::ToString() const
    {
        return L"VSModuleManager";
    }

    ICore * VSModuleManager::GetCore() const
    {
        return m_Core;
    }

    bool VSModuleManager::LoadPath(_In_ const String & path, _In_ const String & filter)
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
                IFile * modulefile = m_Core->GetFileSystem()->FindFile(module);
                this->LoadFile(modulefile);
            }
        } while (FindNextFile(searchHandle, &findFile) != 0);

        return true;
    }
    
    bool VSModuleManager::LoadFile(_In_ const String & filename)
    {
        ILoggerRef logger = m_Core->GetLogger();

        // Check for already loaded
        if (m_Modules.find(filename) != m_Modules.end())
        {
            return true;
        }

        // Create struct and load functions
        IModule* rawmodule = VSModule::Load(filename);

        if (rawmodule == nullptr)
        {
            if (logger.get())
            {
                logger->Log(LL_Error, VOODOO_CORE_NAME, L"Unable to load module %s.", filename.GetData());
            }

            return false;
        }

        IModule* module(rawmodule);

        m_Modules[filename] = module;

        // Register classes from module
        const Version * moduleversion = rawmodule->ModuleVersion();

        if (moduleversion->Debug != VOODOO_META_DEBUG_BOOL && logger.get())
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


    bool VSModuleManager::LoadFile(_In_ const IFile * pFile)
    {
        if (pFile)
        {
            return this->LoadFile(pFile->GetPath());
        } else {
            return nullptr;
        }
    }

    void * VSModuleManager::FindFunction(_In_ const String & module, _In_ const String & name) const
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

    bool VSModuleManager::ClassExists(_In_ const String & name) const
    {
        return (m_Classes.find(name) != m_Classes.end());
    }

    IObject * VSModuleManager::CreateObject(_In_ const String & name) const
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

    VSModule * VSModule::Load(_In_ String path)
    {
        // Load the module
        HMODULE hmodule = LoadLibraryEx(path.GetData(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if (hmodule != nullptr)
        {
            VSModule * module = new VSModule(hmodule);

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

    VSModule::VSModule(HMODULE hmodule) : 
        m_Handle(hmodule)
    { }

    VSModule::~VSModule(void)
    {
        if (m_Handle != INVALID_HANDLE_VALUE)
        {
            FreeLibrary(m_Handle);
            m_Handle = nullptr;
        }
    }

    const Version * VSModule::ModuleVersion(void) const
    {
        return m_ModuleVersion();
    }

    const uint32_t VSModule::ClassCount(void) const
    {
        return m_ClassCount();
    }

    const char * VSModule::ClassInfo(_In_ const uint32_t number) const
    {
        return m_ClassInfo(number);
    }

    IObject * VSModule::CreateClass(_In_ const uint32_t number, _In_ ICore * const pCore)
    {
        return m_ClassCreate(number, pCore);
    }
}
