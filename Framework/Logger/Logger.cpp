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
        const char *XmlLoggerName = "XmlLogger";

        Version VOODOO_CALL API_ModuleVersion(void)
        {
            Version moduleVersion = VOODOO_META_VERSION_STRUCT(LOGGER);

            return moduleVersion;
        }

        int VOODOO_CALL API_ClassCount(void)
        {
            return 1;
        }

        const char *VOODOO_CALL API_ClassInfo (_In_ int number)
        {
            if (number == 0)
            {
                return XmlLoggerName;
            }
            else
            {
                return nullptr;
            }
        }
        IObject * VOODOO_CALL API_ClassCreate (_In_ int number, _In_ ICore * core)
        {
            if (number == 0)
            {
                return new XmlLogger(core);
            }
            else
            {
                return nullptr;
            }
        }

        XmlLogger::XmlLogger(_In_ ICore *core) :
        m_Core(core),
            m_LogLevel(LL_Initial)
        {
            this->m_LocalTime = new tm();
        }

        XmlLogger::~XmlLogger(void)
        {
            if (this->m_LogFile.is_open())
            {
                this->Close();
            }

            if (this->m_LocalTime)
            {
                delete this->m_LocalTime;
            }
        }

        String XmlLogger::ToString(void)
        {
            return XmlLoggerName;
        }

        ICore *XmlLogger::GetCore(void)
        {
            return m_Core;
        }

        bool XmlLogger::Open(IFile * pFile, bool append)
        {
            if (this->m_LogFile.is_open())
            {
                this->Close();
            }

            unsigned int flags = ios_base::out;

            if (append)
            {
                flags |= ios_base::app;
            }
            else
            {
                flags |= ios_base::trunc;
            }

            String fullname = pFile->GetPath();

            this->m_LogFile.open(fullname, flags);

            if (this->m_LogFile.is_open())
            {
#ifdef _DEBUG
                this->SetFlags(LF_Flush);
#endif

                stringstream logMsg;

                logMsg << "<?xml version='1.0'?>\n<VoodooLog ";
                logMsg << this->LogDate();
                logMsg << this->LogTime();
                logMsg << this->LogTicks();
                logMsg << ">\n";

#ifdef _DEBUG

                // cout << logMsg.str();
#endif
                m_LogFile << logMsg.str();

                this->Log(LL_IntInfo, VOODOO_LOGGER_NAME, "Log file opened by XmlLogger::Open.");

                Version loggerVersion = VOODOO_META_VERSION_STRUCT(LOGGER);

                this->LogModule(loggerVersion);

                return true;
            }
            else
            {
                return false;
            }
        }

        void XmlLogger::Close(void)
        {
            if (this->m_LogFile.is_open())
            {
                this->m_LogFile << "</VoodooLog>\n";
                this->m_LogFile.close();
            }
        }

        String XmlLogger::LogTime(void)
        {
            time_t now = time(nullptr);

            if (localtime_s(this->m_LocalTime, &now) == 0)
            {
                stringstream stamp;

                stamp << " time=\"" << put_time(m_LocalTime, "%H%M%S") << "\" ";
                return stamp.str();
            }
            else
            {
                return String(" time=\"000000\" ");
            }
        }

        String XmlLogger::LogDate(void)
        {
            time_t now = time(nullptr);

            if (localtime_s(this->m_LocalTime, &now) == 0)
            {
                stringstream stamp;

                stamp << " date=\"" << put_time(m_LocalTime, "%Y%m%d") << "\" ";
                return stamp.str();
            }
            else
            {
                return String(" date=\"00000000\" ");
            }
        }

        String XmlLogger::LogTicks(void)
        {
            stringstream stamp;

            stamp << " ticks=\"" << GetTickCount() << "\" ";
            return stamp.str();
        }

        void XmlLogger::LogModule(Version version)
        {
            if (!this->m_LogFile.is_open()) return;

            stringstream logMsg;

            logMsg <<
                "    <Module name=\"" << version.Name << "\" " <<
                " major=\"" << version.Major << "\" " <<
                " minor=\"" << version.Minor << "\" " <<
                " patch=\"" << version.Patch << "\" " <<
                " rev=\"" << version.Rev << "\" " <<
                " debug=\"" << version.Debug << "\" />\n";

#ifdef VSF_DEBUG_CONSOLE
            cout << logMsg.str();
#endif
            m_LogFile << logMsg.str();

            if (m_Flags & LF_Flush)
            {
                m_LogFile << flush;
            }
        }

        void XmlLogger::SetFlags(LogFlags flags)
        {
            m_Flags = flags;
        }

        LogFlags XmlLogger::GetFlags(void)
        {
            return m_Flags;
        }

        void XmlLogger::SetLogLevel(LogLevel level)
        {
            m_LogLevel = level;
        }

        LogLevel XmlLogger::GetLogLevel(void)
        {
            return m_LogLevel;
        }

        void XmlLogger::Log(LogLevel level, const char *module, const char *msg, ...)
        {
            va_list args;

            if (!this->m_LogFile.is_open()) return;

            LogLevel mask = (LogLevel) (level & m_LogLevel);

            if (!(mask & LL_Severity) || !(mask & LL_Origin)) return;

            try
            {
                char buffer[4096];

                va_start(args, msg);
                _vsnprintf_s(buffer, 4096, 4096, msg, args);
                buffer[4095] = 0;
                va_end(args);

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
                logMsg << buffer;
                logMsg << "</Message>\n";

#ifdef _DEBUG
                if (level == LL_IntInfo || level & (LL_Warning | LL_Error))
                {
                    OutputDebugString(logMsg.str().c_str());
                }
#ifdef VSF_DEBUG_CONSOLE
                cout << logMsg.str();
#endif
#endif
                m_LogFile << logMsg.str();

                if (m_Flags & LF_Flush)
                {
                    m_LogFile << flush;
                }
            }
            catch(std::exception exc)
            {
#ifdef _DEBUG
                OutputDebugString(exc.what());
#else
                UNREFERENCED_PARAMETER(exc);
#endif
            }
        }

        void XmlLogger::Flush(void)
        {
            if (this->m_LogFile.is_open())
            {
                this->m_LogFile.flush();
            }
        }
    }
}
