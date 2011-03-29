#include "Exception.hpp"

#include "Core.hpp"

namespace VoodooShader
{
    Exception::Exception
    (
        char * module, char * message, 
        Core * core, 
        char * file, char * function, int line
    )
        : std::exception(message), mFmtMsg(NULL),
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
        String module, String message, 
        Core * core, 
        char * file, char * function, int line
    )
        : std::exception(message.c_str()), mFmtMsg(NULL),
            mModule(module), mMessage(message), 
            mCore(core), 
            mFile(file), mFunction(function), mLine(line)
    {
        if ( core )
        {
            core->Log
            (
                LL_Error, 
                module.c_str(), 
                "Exception in %s at %s (%d): %s", 
                file, function, line, message.c_str()
            );
        }
    }

    const char * Exception::what()
    {
        if ( mFmtMsg == NULL )
        {
            mFmtMsg = new char[1024];
            ZeroMemory(mFmtMsg, 1024);

            sprintf_s
            (
                mFmtMsg, 
                1024, 
                "VoodooShader::Exception in module %s, file %s at %s (line %d): %s", 
                mModule.c_str(), mFile, mFunction, mLine, mMessage.c_str()
            );
        }

        return mFmtMsg;
    }
}
