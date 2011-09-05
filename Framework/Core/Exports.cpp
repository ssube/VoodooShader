#include "Exports.hpp"
#include "Null.hpp"

namespace VoodooShader
{

    Version VOODOO_CALL API_ModuleVersion()
    {
        Version coreVersion = VOODOO_META_VERSION_STRUCT(CORE);
        return coreVersion;
    }

    Int32 VOODOO_CALL API_ClassCount()
    {
        return 4;
    }

    const char * VOODOO_CALL API_ClassInfo
    (
        _In_ int number
    )
    {
        switch (number)
        {
            //case 0:
            //    return "VoodooCore";
        case 0:
            return "NullAdapter";
        case 1:
            return "NullLogger";
        case 2:
            return "NullFileSystem";
        case 3:
            return "NullHookManager";
        default:
            return nullptr;
        }
    }

    IObject * VOODOO_CALL API_ClassCreate
    (
        _In_ int number,
        _In_ Core * core
    )
    {
        switch (number)
        {
        case 0:
            return new VoodooNull::NullAdapter(core);
        case 1:
            return new VoodooNull::NullLogger(core);
        case 2:
            return new VoodooNull::NullFileSystem(core);
        case 3:
            return new VoodooNull::NullHookManager(core);
        default:
            return nullptr;
        }
    }
}