#include "Includes.hpp"
#include "Logger.hpp"
#include "Exception.hpp"

using namespace std;

namespace VoodooShader
{
    Logger::Logger(const char * filename, bool append)
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
            Throw("Could not open log file!", NULL);
        }

        this->mLocalTime = new tm();

        this->mLogFile << "<Log " << this->Timestamp().c_str() << ">\n";

        this->Log("Logger created.\n");
    }

    Logger::~Logger()
    {
        if ( this->mLogFile.is_open() )
        {
            this->mLogFile << "</Log>\n";
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
        }
    }

    void Logger::SetBufferSize(unsigned int bytes)
    {
        this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
    }

    void Logger::Log(LogLevel level, const char * msg, ...)
    {
        va_list args;
        char buffer[4096];

        va_start(args, msg);
        _vsnprintf_s(buffer, 4095, 4095, msg, args);
        buffer[4095] = 0;
        va_end(args);

        switch ( level )
        {
        case LL_Info:
            this->mLogFile << " <Info ";
            break;
        case LL_Warning:
            this->mLogFile << " <Warning ";
            break;
        case LL_Error:
            this->mLogFile << " <Error ";
            break;
        };

        this->mLogFile << this->Timestamp() << ">" << buffer;

        switch ( level )
        {
        case LL_Info:
            this->mLogFile << "</Info>\n";
            break;
        case LL_Warning:
            this->mLogFile << "</Warning>\n";
            break;
        case LL_Error:
            this->mLogFile << "</Error>\n";
            break;
        };
    }

    void Logger::LogList(LogLevel level, const char * msg, va_list args)
    {
        char buffer[4096];

        _vsnprintf_s(buffer, 4095, 4095, msg, args);
        buffer[4095] = 0;

        switch ( level )
        {
        case LL_Info:
            this->mLogFile << " <Info ";
            break;
        case LL_Warning:
            this->mLogFile << " <Warning ";
            break;
        case LL_Error:
            this->mLogFile << " <Error ";
            break;
        };

        this->mLogFile << this->Timestamp() << ">" << buffer;

        switch ( level )
        {
        case LL_Info:
            this->mLogFile << "</Info>\n";
            break;
        case LL_Warning:
            this->mLogFile << "</Warning>\n";
            break;
        case LL_Error:
            this->mLogFile << "</Error>\n";
            break;
        };
    }

    void Logger::Dump()
    {
        this->mLogFile.flush();
    }

    bool Logger::Open(const char* filename)
    {
        if ( this->mLogFile.is_open() )
        {
            this->mLogFile << "</Log>\n";
            this->mLogFile.close();
        }

        this->mLogFile.open(filename, ios_base::out|ios_base::trunc);

        if ( this->mLogFile.is_open() )
        {
            this->mLogFile << "<Log " << this->Timestamp().c_str() << ">\n";

            this->Log("Logger: Log file opened by Logger::Open.\n");
            return true;
        } else {
            return false;
        }
    }

    void Logger::Close()
    {
        if ( this->mLogFile.is_open() )
        {
            this->Log("Logger: Log file closed by Logger::Close.\n");
            this->mLogFile.close();
        }
    }
}
