#include "Logger.hpp"

using namespace std;

namespace VoodooShader
{
    enum PrivateLogLevel
    {
        PLL_Initial = 0x00,
        PLL_Internal = 0xFF
    };

    Logger::Logger(const char * filename, bool append)
        : mLogLevel(PLL_Initial)
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
            throw std::exception("Could not open log file!");
        }

        this->mLocalTime = new tm();

        this->mLogFile << 
            "<?xml version='1.0'?>\n" <<
            "<?xml-stylesheet type=\"text/xsl\" href=\"VoodooLog.xsl\"?>\n" <<
            "<LogFile "; 
                this->LogDate(); 
                this->LogTime(); 
                this->LogTicks();
        this->mLogFile << 
            ">\n";

        this->Log(PLL_Internal, "Voodoo/Logger", "Logger created.");
    }

    Logger::~Logger()
    {
        this->Log(PLL_Internal, "Voodoo/Logger", "Logger destroyed.");
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

    void Logger::LogTime()
    {
        time_t now = time(NULL);

        if ( localtime_s(this->mLocalTime, &now) == 0 )
        {
            char stamp[32];
            sprintf_s
            (
                stamp, 32, 
                " time=\"%02d%02d%02d\" ", 
                this->mLocalTime->tm_hour,
                this->mLocalTime->tm_min,
                this->mLocalTime->tm_sec
            );

            this->mLogFile << stamp;
        } else {
            this->mLogFile << " time=\"000000\" ";
        }
    }

    void Logger::LogDate()
    {
        time_t now = time(NULL);

        if ( localtime_s(this->mLocalTime, &now) == 0 )
        {
            char stamp[32];
            sprintf_s
            (
                stamp, 32, 
                " date=\"%04d%02d%02d\" ", 
                this->mLocalTime->tm_year + 1900,
                this->mLocalTime->tm_mon + 1,
                this->mLocalTime->tm_mday
            );

            this->mLogFile << stamp;
        } else {
            this->mLogFile << " date=\"00000000\" ";
        }
    }

    void Logger::LogTicks()
    {
        char stamp[32];
        sprintf_s
        (
            stamp, 32,
            " ticks=\"%u\" ",
            //GetTickCount()
            0
        );
        this->mLogFile << stamp;
    }

    void Logger::LogModule
    (
        Version version
    )
    {
        this->mLogFile << 
            "    <Module name=\"" << version.Name << "\" " <<
            " major=\"" << version.Major << "\" " <<
            " minor=\"" << version.Minor << "\" " <<
            " patch=\"" << version.Patch << "\" " <<
            " rev=\"" << version.Rev << "\" " <<
            " debug=\"" << version.Debug << "\" />\n";
    }

    void Logger::SetBufferSize(unsigned int bytes)
    {
        this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
    }

    void Logger::SetLogLevel(size_t level)
    {
        mLogLevel = level;
    }

    void Logger::Log(size_t level, const char * module, const char * msg, ...)
    {
        if ( level < mLogLevel ) return;

        va_list args;
        char buffer[4096];

        va_start(args, msg);
        _vsnprintf_s(buffer, 4095, 4095, msg, args);
        buffer[4095] = 0;
        va_end(args);
        
        this->mLogFile << 
            "    <Message level=\"" << level << "\" ";
        this->LogTime();
        this->LogTicks();
        this->mLogFile << 
            " module=\"" << module << "\">" << buffer << "</Message>\n";
    }

    void Logger::LogList(size_t level, const char * module, const char * msg, va_list args)
    {
        if ( level < mLogLevel ) return;

        char buffer[4096];

        _vsnprintf_s(buffer, 4095, 4095, msg, args);
        buffer[4095] = 0;

        this->mLogFile << 
            "    <Message level=\"" << level << "\" ";
        this->LogTime();
        this->LogTicks();
        this->mLogFile <<" module=\"" << module << "\">" << buffer << "</Message>\n";
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
            this->mLogFile << 
                "<?xml version='1.0'?>\n" <<
                "<?xml-stylesheet type=\"text/xsl\" href=\"VoodooLog.xsl\"?>\n" <<
                "<LogFile "; 
            this->LogDate(); 
            this->LogTime(); 
            this->LogTicks();
            this->mLogFile << 
                ">\n";

            this->Log(PLL_Internal, "Voodoo/Logger", "Logger: Log file opened by Logger::Open.");
            return true;
        } else {
            return false;
        }
    }

    void Logger::Close()
    {
        if ( this->mLogFile.is_open() )
        {
            this->Log(PLL_Internal, VOODOO_CORE_NAME, "Logger: Log file closed by Logger::Close.");
            this->mLogFile.close();
        }
    }
}
