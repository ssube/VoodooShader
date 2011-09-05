#include "Module.hpp"

#include "Core.hpp"
#include "Exception.hpp"
#include "ILogger.hpp"
#include "IObject.hpp"
#include "Parser.hpp"

#include <regex>

namespace VoodooShader
{
    ModuleManager::ModuleManager(_In_ Core * core)
            : m_Core(core)
    {
    }

    ModuleManager::~ModuleManager()
    {
        m_Classes.clear();
        m_Modules.clear();
    }

    bool ModuleManager::LoadPath(_In_ String path, _In_ String filter)
    {
        String mask = m_Core->GetParser()->Parse(path) + "\\*";

        WIN32_FIND_DATA findFile;
        HANDLE searchHandle = FindFirstFile(mask.c_str(), &findFile);

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();
            if (error == ERROR_FILE_NOT_FOUND)
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning, VOODOO_CORE_NAME, "No plugin files found in directory %s.", path.c_str()
                );

                return false;
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning, VOODOO_CORE_NAME, "Error searching directory %s.", path.c_str()
                );

                return false;
            }
        }

        std::regex compfilter(filter);

        do
        {
            String module = findFile.cFileName;

            if (regex_match(module, compfilter))
            {
                this->LoadFile(module);
            }
        }
        while (FindNextFile(searchHandle, &findFile) != 0);

        return true;
    }

    bool ModuleManager::LoadFile(_In_ String name)
    {
        ILoggerRef logger = m_Core->GetLogger();

        // Build the full path
        String path = m_Core->GetParser()->Parse(name);

        // Check for already loaded
        if (m_Modules.find(path) != m_Modules.end())
        {
            return true;
        }

        // Create struct and load functions
        ModuleRef rawmodule = Module::Load(path);

        if (rawmodule == nullptr)
        {
            if (logger.get())
            {
                logger->Log
                (
                    LL_Error, VOODOO_CORE_NAME, "Unable to load module %s.",
                    path.c_str()
                );
            }

            return nullptr;
        }

        ModuleRef module(rawmodule);
        m_Modules[path] = module;

        // Register classes from module
        Version moduleversion = rawmodule->ModuleVersion();
        if (moduleversion.Debug != VOODOO_META_DEBUG_BOOL && logger.get())
        {
            logger->Log(LL_Warning, VOODOO_CORE_NAME, "Debug build mismatch with module %s.", moduleversion.Name.c_str());
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

        return module;
    }

    void * ModuleManager::FindFunction(_In_ String module, _In_ String name)
    {
        HMODULE hmodule = GetModuleHandle(module.c_str());

        if (hmodule)
        {
            return GetProcAddress(hmodule, name.c_str());
        }
        else
        {
            return NULL;
        }
    }

    bool ModuleManager::ClassExists(_In_ String name)
    {
        return (m_Classes.find(name) != m_Classes.end());
    }

    IObject * ModuleManager::CreateObject(_In_ String name)
    {
        ILoggerRef logger = m_Core->GetLogger();
        ClassMap::iterator classiter = m_Classes.find(name);

        if (classiter != m_Classes.end())
        {
            ModuleRef module = classiter->second.first.lock();
            int number = classiter->second.second;

            if (module.get())
            {
                try
                {
                    IObject * object = module->CreateClass(number, m_Core);

                    if (object == nullptr)
                    {
                        if (logger.get())
                        {
                            logger->Log(LL_Error, VOODOO_CORE_NAME, "Error creating instance of class %s.", name.c_str());
                        }
                    }

                    return object;
                }
                catch (std::exception & exc)
                {
                    if (logger.get())
                    {
                        logger->Log(LL_Error, VOODOO_CORE_NAME, "Error creating class %s: %s", name.c_str(), exc.what());
                    }
                    return nullptr;
                }
            }
            else
            {
                if (logger.get())
                {
                    logger->Log(LL_Error, VOODOO_CORE_NAME, "Unable to lock module (possibly unloaded) for class %s.", name.c_str());
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

    ModuleRef Module::Load(_In_ String path)
    {
        // Load the module
        HMODULE hmodule = LoadLibraryEx(path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if (hmodule != NULL)
        {
            using namespace VoodooShader::Functions;

            Module * module = new Module(hmodule);

            module->m_ModuleVersion = (VersionFunc) GetProcAddress(hmodule, "ModuleVersion");
            module->m_ClassCount    = (CountFunc) GetProcAddress(hmodule, "ClassCount");
            module->m_ClassInfo     = (InfoFunc) GetProcAddress(hmodule, "ClassInfo");
            module->m_ClassCreate   = (CreateFunc) GetProcAddress(hmodule, "ClassCreate");

            if (module->m_ModuleVersion == nullptr ||
                    module->m_ClassCount == nullptr ||
                    module->m_ClassInfo == nullptr ||
                    module->m_ClassCreate == nullptr
               )
            {
                //FreeLibrary(hmodule);
                delete module;
                return nullptr;
            }
            else
            {
                return ModuleRef(module);
            }
        }
        else
        {
            return nullptr;
        }
    }

    Module::Module(HMODULE hmodule)
            : m_Handle(hmodule)
    {
    }

    Module::~Module()
    {
        if (m_Handle != INVALID_HANDLE_VALUE)
        {
            FreeLibrary(m_Handle);
            m_Handle = NULL;
        }
    }

    Version Module::ModuleVersion()
    {
        return (*m_ModuleVersion)();
    }

    int Module::ClassCount()
    {
        return (*m_ClassCount)();
    }

    const char * Module::ClassInfo(_In_ int number)
    {
        return (*m_ClassInfo)(number);
    }

    IObject * Module::CreateClass(_In_ int number, _In_ Core * core)
    {
        return (*m_ClassCreate)(number, core);
    }
}