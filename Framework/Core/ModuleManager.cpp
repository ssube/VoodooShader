#include "ModuleManager.hpp"

namespace VoodooShader
{
    Module * ModuleManager::LoadModule( _In_ String name, bool absolute, bool prepend )
    {
        ModuleMap::iterator module = mModules.find(name);

        if ( module != mModules.end() )
        {
            return true;
        }

        // Build the full path
        String path;
        if ( absolute && prepend )
        {
            path = mBasePath + name;
        } else {
            path = name;
        }

        // Load the module
        HMODULE hmodule = NULL;
        if ( absolute )
        {
            hmodule = LoadLibraryEx(path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        } else {
            hmodule = LoadLibrary(path.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        }

        // First set of error checks
        if ( hmodule == NULL )
        {
            return false;
        }

        // Create struct and load symbols
        Module * module = new Module;

        module->Owned        = true;
        module->Handle       = hmodule;
        module->ClassCount   = GetProcAddress(hmodule, "ClassCount");
        module->ClassInfo    = GetProcAddress(hmodule, "ClassInfo");
        module->ClassCreate  = GetProcAddress(hmodule, "ClassCreate");
        module->ClassDestroy = GetProcAddress(hmodule, "ClassDestroy");
    }

    void ModuleManager::UnloadModule( _In_ Module * module )
    {
        if ( !module->Owned )
        {
            Throw(VOODOO_CORE_NAME, "Cannot unload modules that are not owned.", mCore);
        }
    }
}