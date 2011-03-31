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
        Version API_ModuleVersion()
        {
            Version moduleVersion = VOODOO_META_VERSION_STRUCT(LOGGER);
            return moduleVersion;
        }

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
                return new XmlLogger(core);
            } else {
                return NULL;
            }
        }

        XmlLogger::XmlLogger(_In_ Core * core)
            : mCore(core), mLogLevel(LL_All)
        {
            this->mLocalTime = new tm();
        }

        XmlLogger::~XmlLogger()
        {
            this->Log(LL_Internal, VOODOO_LOGGER_NAME, "Logger destroyed.");
            this->Dump();

            if ( this->mLogFile.is_open() )
            {
                this->Close();
            }

            if ( this->mLocalTime )
            {
                delete this->mLocalTime;
            }
        }

        const char * XmlLogger::GetObjectClass()
        {
            return "XmlLogger";
        }

        bool XmlLogger::Open(const char* filename, bool append)
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
                this->SetBufferSize(0);

                this->mLogFile << "<?xml version='1.0'?>\n<VoodooLog "; 
                this->LogDate(); 
                this->LogTime(); 
                this->LogTicks();
                this->mLogFile << ">\n";

                this->Log(LL_Internal, VOODOO_LOGGER_NAME, "Log file opened by XmlLogger::Open.");

                Version loggerVersion = VOODOO_META_VERSION_STRUCT(LOGGER);
                this->LogModule(loggerVersion);

                return true;
            } else {
                return false;
            }
        }

        void XmlLogger::Close()
        {
            if ( this->mLogFile.is_open() )
            {
                this->Log(LL_Internal, VOODOO_LOGGER_NAME, "Log file closed by XmlLogger::Close.");
                this->mLogFile << "</VoodooLog>\n";
                this->mLogFile.close();
            }
        }

        void XmlLogger::LogTime()
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

        void XmlLogger::LogDate()
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

        void XmlLogger::LogTicks()
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

        void XmlLogger::LogModule
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

        void XmlLogger::SetBufferSize(unsigned int bytes)
        {
            if ( !this->mLogFile.is_open() ) return;

            this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
        }

        void XmlLogger::SetLogLevel(LogLevel level)
        {
            mLogLevel = level;
        }

        void XmlLogger::Log(LogLevel level, const char * module, const char * msg, ...)
        {
            if ( level < mLogLevel ) return;
            if ( !this->mLogFile.is_open() ) return;

            char buffer[4096];
            va_list args;

            va_start(args, msg);

            _vsnprintf_s(buffer, 4095, 4095, msg, args);
            buffer[4095] = 0;

            va_end(args);

            this->mLogFile << "    <Message severity=\"";
            this->mLogFile << level;
            this->mLogFile << "\" ";
            this->LogTime();
            this->LogTicks();
            this->mLogFile << " source=\"" << module << "\">" << buffer << "</Message>\n";
        }

        void XmlLogger::Dump()
        {
            this->mLogFile.flush();
        }
    }
}
