#include "Includes.hpp"
#include "Logger.hpp"
#include "Exception.hpp"
#include "Version.hpp"

using namespace std;

namespace VoodooShader
{
    Logger::Logger(const char * filename, bool append)
        mLogLevel(LL_Info)
    {
        unsigned int flags = ios_base::out;
        if ( append )
        {
            flags |= ios_base::app;
        } else {
            flags |= ios_base::trunc;
        }
        this->mLogFile.open(filename, flags);

        if ( !this->mLogFile.is_open() )
        {
            Throw(VOODOO_CORE_NAME, "Could not open log file!", NULL);
        }

        this->mLocalTime = new tm();

        this->mLogFile << "<LogFile " << this->Timestamp() << ">\n";

        this->Log(LL_Info, VOODOO_CORE_NAME, "Logger created.");
    }

    Logger::~Logger()
    {
        this->Log(LL_Info, VOODOO_CORE_NAME, "Logger destroyed.");
        if ( this->mLogFile.is_open() )
        {
            this->mLogFile << "</LogFile>\n";
            this->mLogFile.close();
        }
        if ( this->mLocalTime )
        {
            delete this->mLocalTime;
        }
    }

    String Logger::Timestamp()
    {
        time_t now = time(NULL);

        if ( localtime_s(this->mLocalTime, &now) == ERROR_SUCCESS )
        {
            char stamp[32];
            sprintf_s(stamp, 32, " timestamp=\"%02d.%02d.%02d\" ", 
                this->mLocalTime->tm_hour,
                this->mLocalTime->tm_min,
                this->mLocalTime->tm_sec);
            return String(stamp);
        } else {
            return String();
        }
    }

    void Logger::SetBufferSize(unsigned int bytes)
    {
        this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
    }

    void Logger::SetLogLevel(LogLevel level)
    {
        mLogLevel = level;
    }

    void Logger::Log(LogLevel level, const char * module, const char * msg, ...)
    {
        if ( level < mLogLevel ) return;

        va_list args;
        char buffer[4096];

        va_start(args, msg);
        _vsnprintf_s(buffer, 4095, 4095, msg, args);
        buffer[4095] = 0;
        va_end(args);
        
        this->mLogFile << "<Message type=\"";

        switch ( level )
        {
        case LL_Info:
            this->mLogFile << "info";
            break;
        case LL_Debug:
            this->mLogFile << "debug";
            break;
        case LL_Warning:
            this->mLogFile << "warning";
            break;
        case LL_Error:
            this->mLogFile << "error";
            break;
        case LL_Fatal:
            this->mLogFile << "fatal";
            break;
        case LL_Unknown:
        default:
            this->mLogFile << "unknown";
        };

        this->mLogFile << "\" " << this->Timestamp() << " module=\"" << module << "\">" << buffer << "</Message>\n";
    }

    void Logger::LogList(LogLevel level, const char * module, const char * msg, va_list args)
    {
        if ( level < mLogLevel ) return;

        char buffer[4096];

        _vsnprintf_s(buffer, 4095, 4095, msg, args);
        buffer[4095] = 0;
        
        this->mLogFile << "\t<Message type=\"";

        switch ( level )
        {
        case LL_Info:
            this->mLogFile << "info";
            break;
        case LL_Debug:
            this->mLogFile << "debug";
            break;
        case LL_Warning:
            this->mLogFile << "warning";
            break;
        case LL_Error:
            this->mLogFile << "error";
            break;
        case LL_Fatal:
            this->mLogFile << "fatal";
            break;
        case LL_Unknown:
        default:
            this->mLogFile << "unknown";
        };

        this->mLogFile << "\" " << this->Timestamp() << " module=\"" << module << "\">" << buffer << "</Message>\n";
    }

    void Logger::Dump()
    {
        this->mLogFile.flush();
    }

    bool Logger::Open(const char* filename)
    {
        if ( this->mLogFile.is_open() )
        {
            this->mLogFile << "</LogFile>\n";
            this->mLogFile.close();
        }

        this->mLogFile.open(filename, ios_base::out|ios_base::trunc);

        if ( this->mLogFile.is_open() )
        {
            this->mLogFile << "<LogFile " << this->Timestamp() << ">\n";

            this->Log(LL_Info, VOODOO_CORE_NAME, "Logger: Log file opened by Logger::Open.");
            return true;
        } else {
            return false;
        }
    }

    void Logger::Close()
    {
        if ( this->mLogFile.is_open() )
        {
            this->Log(LL_Info, VOODOO_CORE_NAME, "Logger: Log file closed by Logger::Close.");
            this->mLogFile.close();
        }
    }
}
