#include "Logger.hpp"

#include "Logger_Version.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

using namespace std;

namespace VoodooShader
{
    namespace XmlLogger
    {
        int API_ClassCount()
        {
            return 1;
        }

        const char * API_ClassInfo
        (
            _In_ int number
        )
        {
            if ( number == 0 )
            {
                return "XmlLogger";
            } else {
                return NULL;
            }
        }

        IObject * API_ClassCreate
        (
            _In_ int number, 
            _In_ Core * core
        )
        {
            if ( number == 0 )
            {
                return new XmlLogger::Logger(core);
            } else {
                return NULL;
            }
        }

        Logger::Logger(Core * core)
            : mCore(core), mLogLevel(LL_Initial)
        {
            this->mLocalTime = new tm();
        }

        Logger::~Logger()
        {
            this->Log(LL_Internal, VOODOO_LOGGER_NAME, "Logger destroyed.");

            if ( this->mLogFile.is_open() )
            {
                this->Close();
            }
            if ( this->mLocalTime )
            {
                delete this->mLocalTime;
            }
        }

        void Logger::DestroyObject()
        {
            delete this;
        }

        int Logger::GetObjectID()
        {
            return 0;
        }

        const char * Logger::GetObjectName()
        {
            return "Logger";
        }

        bool Logger::Open(const char* filename, bool append)
        {
            if ( this->mLogFile.is_open() )
            {
                this->Close();
            }

            unsigned int flags = ios_base::out;
            if ( append )
            {
                flags |= ios_base::app;
            } else {
                flags |= ios_base::trunc;
            }

            char fullname[MAX_PATH];
            StringCchCopyA(fullname, MAX_PATH, filename);
            StringCchCatA(fullname, MAX_PATH, ".xmllog");

            this->mLogFile.open(fullname, flags);

            if ( this->mLogFile.is_open() )
            {
                this->mLogFile << 
                    "<?xml version='1.0'?>\n" <<
                    "<?xml-stylesheet type=\"text/xsl\" href=\"VoodooLog.xsl\"?>\n" <<
                    "<VoodooLog "; 
                this->LogDate(); 
                this->LogTime(); 
                this->LogTicks();
                this->mLogFile << 
                    ">\n";

                this->Log(LL_Internal, VOODOO_LOGGER_NAME, "Log file opened by Logger::Open.");

                Version loggerVersion = VOODOO_META_VERSION_STRUCT(LOGGER);
                this->LogModule(loggerVersion);

                return true;
            } else {
                return false;
            }
        }

        void Logger::Close()
        {
            if ( this->mLogFile.is_open() )
            {
                this->Log(LL_Internal, VOODOO_LOGGER_NAME, "Log file closed by Logger::Close.");
                this->mLogFile << "</VoodooLog>\n";
                this->mLogFile.close();
            }
        }

        void Logger::LogTime()
        {
            if ( !this->mLogFile.is_open() ) return;

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
            if ( !this->mLogFile.is_open() ) return;

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
            if ( !this->mLogFile.is_open() ) return;

            char stamp[32];
            sprintf_s
            (
                stamp, 32,
                " ticks=\"%u\" ",
                GetTickCount()
            );
            this->mLogFile << stamp;
        }

        void Logger::LogModule
        (
            Version version
        )
        {
            if ( !this->mLogFile.is_open() ) return;

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
            if ( !this->mLogFile.is_open() ) return;

            this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
        }

        void Logger::SetLogLevel(size_t level)
        {
            mLogLevel = level;
        }

        void Logger::Log(size_t level, const char * module, const char * msg, ...)
        {
            if ( level < mLogLevel ) return;
            if ( !this->mLogFile.is_open() ) return;

            va_list args;
            char buffer[4096];

            va_start(args, msg);
            _vsnprintf_s(buffer, 4095, 4095, msg, args);
            buffer[4095] = 0;
            va_end(args);
        
            this->mLogFile << 
                "    <Message severity=\"" << level << "\" ";
            this->LogTime();
            this->LogTicks();
            this->mLogFile << 
                " source=\"" << module << "\">" << buffer << "</Message>\n";
        }

        void Logger::LogList(size_t level, const char * module, const char * msg, va_list args)
        {
            if ( level < mLogLevel ) return;
            if ( !this->mLogFile.is_open() ) return;

            char buffer[4096];

            _vsnprintf_s(buffer, 4095, 4095, msg, args);
            buffer[4095] = 0;

            this->mLogFile << 
                "    <Message severity=\"" << level << "\" ";
            this->LogTime();
            this->LogTicks();
            this->mLogFile <<" source=\"" << module << "\">" << buffer << "</Message>\n";
        }

        void Logger::Dump()
        {
            this->mLogFile.flush();
        }
    }
}
