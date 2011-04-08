#include "Logger.hpp"

#include "Logger_Version.hpp"

#include <stdio.h>
#include <sstream>
#include <iomanip>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

using namespace std;

namespace VoodooShader
{
    namespace XmlLogger
    {
        //const char * XmlLoggerName = "{B531CD82-F741-4926-9929-0619E1317502}";
        const char * XmlLoggerName = "XmlLogger";

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
                return XmlLoggerName;
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
            : mCore(core), mLogLevel(LL_Initial)
        {
            this->mLocalTime = new tm();
        }

        XmlLogger::~XmlLogger()
        {
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
            return XmlLoggerName;
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
#ifdef _DEBUG
                this->SetBufferSize(0);
#endif

                stringstream logMsg;

                logMsg << "<?xml version='1.0'?>\n<VoodooLog "; 
                logMsg << this->LogDate(); 
                logMsg << this->LogTime(); 
                logMsg << this->LogTicks();
                logMsg << ">\n";

#ifdef _DEBUG
                cout << logMsg.str();
#endif
                mLogFile << logMsg.str();

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

        String XmlLogger::LogTime()
        {
            time_t now = time(NULL);

            if ( localtime_s(this->mLocalTime, &now) == 0 )
            {
                stringstream stamp;
                stamp << " time=\"" << put_time(mLocalTime, "%H%M%S") << "\" ";
                //stamp.fill('0');
                //stamp << " time=\"" << setw(2) << mLocalTime->tm_hour << setw(2) << mLocalTime->tm_min << setw(2) << mLocalTime->tm_sec << "\" ";
                return stamp.str();
            } else {
                return String(" time=\"000000\" ");
            }
        }

        String XmlLogger::LogDate()
        {
            time_t now = time(NULL);

            if ( localtime_s(this->mLocalTime, &now) == 0 )
            {
                stringstream stamp;
                stamp << " date=\"" << put_time(mLocalTime, "%Y%m%d") << "\" ";
                //stamp.fill('0');
                //stamp << " date=\"" << setw(4) << ( mLocalTime->tm_year + 1900 ) << setw(2) << (mLocalTime->tm_min + 1) << setw(2) << mLocalTime->tm_sec << "\" ";
                return stamp.str();
            } else {
                return String(" date=\"00000000\" ");
            }
        }

        String XmlLogger::LogTicks()
        {
            stringstream stamp;
            stamp << " ticks=\"" << GetTickCount() << "\" ";
            return stamp.str();
        }

        void XmlLogger::LogModule
        (
            Version version
        )
        {
            if ( !this->mLogFile.is_open() ) return;

            stringstream logMsg;

            logMsg << "    <Module name=\"" << version.Name << "\" " <<
                " major=\"" << version.Major << "\" " <<
                " minor=\"" << version.Minor << "\" " <<
                " patch=\"" << version.Patch << "\" " <<
                " rev=\"" << version.Rev << "\" " <<
                " debug=\"" << version.Debug << "\" />\n";

#ifdef _DEBUG
            cout << logMsg;
#endif
            mLogFile << logMsg;
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
            va_list args;

            if ( !this->mLogFile.is_open() ) return;
            if ( !((level & mLogLevel) & LL_Severity) || !((level & mLogLevel) & LL_Origin) ) return;

            try
            {
                // Format the message in memory to prevent partial messages from being dumped
                stringstream logMsg;

                logMsg << "    <Message severity=\"";
                logMsg << level;
                logMsg << "\" ";
                logMsg << this->LogTime();
                logMsg << this->LogTicks();
                logMsg << " source=\"";
                logMsg << module;
                logMsg << "\">";

                char buffer[4096];
                va_start(args, msg);
                _vsnprintf_s(buffer, 4095, 4095, msg, args);
                buffer[4095] = 0;
                va_end(args);

                logMsg << buffer;

                logMsg << "</Message>\n";

#ifdef _DEBUG
                if ( level & ( LL_Warning | LL_Error ) )
                {
                    OutputDebugString(logMsg.str().c_str());
                }

                cout << logMsg.str();
#endif
                mLogFile << logMsg.str();
            } catch ( std::exception exc ) {
#ifdef _DEBUG
                OutputDebugString(exc.what());
#else
                UNREFERENCED_PARAMETER(exc);
#endif
            }
        }

        void XmlLogger::Dump()
        {
            if ( this->mLogFile.is_open() )
            {
                this->mLogFile.flush();
            }
        }
    }
}
