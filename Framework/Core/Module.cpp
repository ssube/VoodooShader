#include "Module.hpp"

#include "Core.hpp"
#include "Exception.hpp"
#include "Object.hpp"

namespace VoodooShader
{
    ModuleManager::ModuleManager( _In_ Core * core )
        : mCore(core), mBasePath(mCore->GetBasePath())
    {
    }

    ModuleRef ModuleManager::LoadModule( _In_ String name, bool prepend )
    {
        // Build the full path
        String path;
        if ( prepend )
        {
            path = mBasePath + name;
        } else {
            path = name;
        }

        // Check for already loaded
        ModuleMap::iterator moduleiter = mModules.find(path);

        if ( moduleiter != mModules.end() )
        {
            return moduleiter->second;
        }

        // Create struct and load functions
        Module * rawmodule = Module::Load(path);

        if ( rawmodule == NULL )
        {
            return NULL;
        }

        ModuleRef module(rawmodule);
        mModules[path] = module;

        // Register classes from module

        int classCount = module->ClassCount();
        for ( int curClass = 0; curClass < classCount; ++curClass )
        {
            const char * classname = module->ClassInfo(curClass);

            if ( classname )
            {
                mClasses[classname] = ClassID(module, curClass);
            }
        }

        return module;
    }

    void * ModuleManager::FindFunction( _In_ String module, _In_ String name )
    {
        HMODULE hmodule = GetModuleHandle(module.c_str());

        if ( hmodule )
        {
            return GetProcAddress(hmodule, name.c_str());
        } else {
            return NULL;
        }
    }

    IObject * ModuleManager::CreateClass( _In_ String name )
    {
        ClassMap::iterator classiter = mClasses.find(name);

        if ( classiter != mClasses.end() )
        {
            ModuleRef module = classiter->second.first.lock();
            int number = classiter->second.second;

            if ( module.get() )
            {
                return module->CreateClass(number, mCore);
            } else {
                return NULL;
            }
        } else {
            return NULL;
        }
    }

    Module * Module::Load( _In_ String path )
    {
        // Load the module
        HMODULE hmodule = LoadLibraryEx(path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if ( hmodule != NULL )
        {
            Module * module = new Module(hmodule);

            module->mClassCount  = (Functions::CountFunc )GetProcAddress(hmodule, "ClassCount" );
            module->mClassInfo   = (Functions::InfoFunc  )GetProcAddress(hmodule, "ClassInfo"  );
            module->mClassCreate = (Functions::CreateFunc)GetProcAddress(hmodule, "ClassCreate");

            if ( !(module->mClassCount && module->mClassInfo && module->mClassCreate ) )
            {
                FreeLibrary(hmodule);
                delete module;
                return NULL;
            } else {
                return module;
            }
        } else {
            return NULL;
        }
    }

    Module::Module(HMODULE hmodule)
        : mOwned(true), mHandle(hmodule)
    {
    }

    Module::~Module()
    {
        FreeLibrary(mHandle);
        mHandle = NULL;
    }

    int Module::ClassCount()
    {
        if ( mClassCount == NULL )
        {
            return 0;
        } else {
            return (*mClassCount)();
        }
    }

    const char * Module::ClassInfo( _In_ int number )
    {
        if ( mClassInfo == NULL )
        {
            return NULL;
        } else {
            return (*mClassInfo)(number);
        }
    }

    IObject * Module::CreateClass(_In_ int number, _In_ Core * core)
    {
        if ( mClassCreate == NULL )
        {
            return NULL;
        } else {
            return (*mClassCreate)(number, core);
        }
    }
}