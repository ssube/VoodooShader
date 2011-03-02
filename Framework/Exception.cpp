#include "Includes.hpp"
#include "Exception.hpp"
#include "Core.hpp"

namespace VoodooShader
{
    Exception::Exception
    (
        const char * module, const char * message, 
        Core * core, 
        const char * file, const char * function, const int line
    )
        : std::exception(message), 
            mModule(module), mMessage(message), 
            mCore(core), 
            mFile(file), mFunction(function), mLine(line)
    {
        if ( core )
        {
            core->Log
            (
                LL_Error, 
                module, 
                "Exception in %s at %s (%d): %s",
                file, function, line, message
            );
        }
    }

    Exception::Exception
    (
        const char * module, String message, 
        Core * core, 
        const char * file, const char * function, const int line
    )
        : std::exception(message.c_str()), 
            mModule(module), mMessage(message), 
            mCore(core), 
            mFile(file), mFunction(function), mLine(line)
    {
        if ( core )
        {
            core->Log
            (
                LL_Error, 
                module, 
                "Exception in %s at %s (%d): %s", 
                file, function, line, message
            );
        }
    }

    String Exception::Message()
    {
        char buffer[1024];
        ZeroMemory(buffer, 1024);

        sprintf_s
        (
            buffer, 
            1023, 
            "VoodooShader::Exception in module %s, file %s at %s (line %d): %s", 
            mModule, mFile, mFunction, mLine, mMessage.c_str()
        );

        return String(buffer);
    }
}
