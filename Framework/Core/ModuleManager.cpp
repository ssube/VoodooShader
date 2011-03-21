#include "ModuleManager.hpp"

#include "Core.hpp"
#include "Exception.hpp"

namespace VoodooShader
{
    ModuleManager::ModuleManager( _In_ Core * core )
        : mCore(core)
    {
        mBasePath = mCore->GetBasePath() + "\\";
    }

    ModuleRef ModuleManager::LoadModule( _In_ String name, bool absolute, bool prepend )
    {
        // Build the full path
        String path;
        if ( absolute && prepend )
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
        ModuleRef module(new Module(path));

        // Register classes from module
        bool success = module->Register(mCore);

        if ( !success )
        {
            return ModuleRef();
        }

        mModules[path] = module;

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

    void * ModuleManager::CreateClass( _In_ String name )
    {
        ClassMap::iterator classiter = mClasses.find(name);

        if ( classiter != mClasses.end() )
        {
            return classiter->second.first->CreateClass(classiter->second.second, mCore);
        } else {
            return NULL;
        }
    }

    void ModuleManager::DestroyClass( _In_ String name, _In_ void * inst )
    {
        ClassMap::iterator classiter = mClasses.find(name);

        if ( classiter != mClasses.end() )
        {
            return classiter->second.first->DestroyClass(classiter->second.second, inst);
        } 
    }

    Module::Module(String path)
        : mOwned(true)
    {
        // Load the module
        mHandle = LoadLibraryEx(path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

        // First set of error checks
        if ( mHandle != NULL )
        {
            mRegModule = (Functions::RegFunc)GetProcAddress(mHandle, "RegisterModule");

            if ( mRegModule == NULL )
            {
                FreeLibrary(mHandle);
                mHandle = NULL;
            }
        }
    }

    Module::~Module()
    {
        FreeLibrary(mHandle);
    }

    bool Module::Register( _In_ Core * core )
    {
        if ( mRegModule == NULL )
        {
            return false;
        } else {
            return (*mRegModule)(core, this);
        }
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

    void * Module::CreateClass(_In_ int number, _In_ Core * core)
    {
        if ( mClassCreate == NULL )
        {
            return NULL;
        } else {
            return (*mClassCreate)(number, core);
        }
    }

    void Module::DestroyClass(_In_ int number, _In_ void * inst)
    {
        if ( mClassDestroy == NULL )
        {
            return;
        } else {
            return (*mClassDestroy)(number, inst);
        }
    }
}