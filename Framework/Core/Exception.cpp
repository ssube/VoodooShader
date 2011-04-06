#include "Exception.hpp"

#include "Core.hpp"
#include "ILogger.hpp"

namespace VoodooShader
{
    Exception::Exception
    (
        _In_ char * module, _In_ char * message, 
        _In_ Core * core, 
        _In_ char * file, _In_ char * function, _In_ int line
    )
        : std::exception(message), mFmtMsg(NULL),
            mModule(module), mMessage(message), 
            mCore(core), 
            mFile(file), mFunction(function), mLine(line)
    {
        if ( core )
        {
            ILoggerRef logger = core->GetLogger();
            if ( logger.get() )
            {
                logger->Log
                (
                    LL_Error, 
                    module, 
                    "Exception in %s at %s (%d): %s",
                    file, function, line, message
                );
            }
        }
    }

    Exception::Exception
    (
        _In_ String module, _In_ String message, 
        _In_ Core * core, 
        _In_ char * file, _In_ char * function, _In_ int line
    )
        : std::exception(message.c_str()), mFmtMsg(NULL),
            mModule(module), mMessage(message), 
            mCore(core), 
            mFile(file), mFunction(function), mLine(line)
    {
        if ( core )
        {
            ILoggerRef logger = core->GetLogger();
            if ( logger.get() )
            {
                logger->Log
                (
                    LL_Error, 
                    module.c_str(), 
                    "Exception in %s at %s (%d): %s",
                    file, function, line, message.c_str()
                );
            }
        }
    }

    Exception::~Exception()
    {
        if ( mFmtMsg )
        {
            delete mFmtMsg;
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
