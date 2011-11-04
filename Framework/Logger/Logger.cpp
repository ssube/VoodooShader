/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <boost/uuid/uuid_io.hpp>

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
        static const Version moduleVersion = VOODOO_META_VERSION_STRUCT(LOGGER);
        const wchar_t * name_VSXmlLogger = L"VSXmlLogger";
        static const Uuid clsid_VSXmlLogger = CLSID_VSXmlLogger;

        const Version * VOODOO_CALL API_ModuleVersion(void)
        {
            return &moduleVersion;
        }

        const uint32_t VOODOO_CALL API_ClassCount(void)
        {
            return 1;
        }

        const wchar_t * VOODOO_CALL API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid)
        {
            if (!pUuid)
            {
                return nullptr;
            }

            if (index == 0)
            {
                *pUuid = clsid_VSXmlLogger;
                return name_VSXmlLogger;
            }

            return nullptr;
        }

        IObject * VOODOO_CALL API_ClassCreate (_In_ const uint32_t number, _In_ ICore * pCore)
        {
            if (number == 0)
            {
                return new VSXmlLogger(pCore);
            }
            else
            {
                return nullptr;
            }
        }

        VSXmlLogger::VSXmlLogger(_In_ ICore * pCore) :
            m_Core(pCore), m_LogLevel(LL_Initial)
        {
            this->m_LocalTime = new tm();
        }

        VSXmlLogger::~VSXmlLogger(void)
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

        uint32_t VSXmlLogger::AddRef() const
        {
            return ++m_Refs;
        }

        uint32_t VSXmlLogger::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        bool VSXmlLogger::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_VSXmlLogger;
                    return true;
                } else {
                    return false;
                }
            } else {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                } else if (clsid == IID_ILogger) {
                    *ppOut = static_cast<const ILogger*>(this);
                } else if (clsid == CLSID_VSXmlLogger) {
                    *ppOut = static_cast<const VSXmlLogger*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String VSXmlLogger::ToString(void) const
        {
            return name_VSXmlLogger;
        }

        ICore * VSXmlLogger::GetCore(void) const
        {
            return m_Core;
        }

        bool VSXmlLogger::Open(_In_ const String & filename, _In_ const bool append)
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

            this->m_LogFile.open(filename.GetData(), flags);

            if (this->m_LogFile.is_open())
            {
#ifdef _DEBUG
                this->SetFlags(LF_Flush);
#endif

                wstringstream logMsg;

                logMsg << "<?xml version='1.0'?>\n<VoodooLog ";
                logMsg << this->LogDate().GetData();
                logMsg << this->LogTime().GetData();
                logMsg << this->LogTicks().GetData();
                logMsg << ">\n";

#ifdef _DEBUG
                std::wcout << logMsg.str();
#endif
                m_LogFile << logMsg.str();

                this->Log(LL_ModInfo, VOODOO_LOGGER_NAME, L"Log file opened by VSXmlLogger::Open.");

                this->LogModule(API_ModuleVersion());

                return true;
            }
            else
            {
                return false;
            }
        }

        bool VSXmlLogger::Open(_In_ IFile *const pFile, _In_ const bool append)
        {
            return this->Open(pFile->GetPath(), append);
        }

        void VSXmlLogger::Close(void)
        {
            if (this->m_LogFile.is_open())
            {
                this->m_LogFile << L"</VoodooLog>\n";
                this->m_LogFile.close();
            }
        }

        void VSXmlLogger::Flush(void)
        {
            if (this->m_LogFile.is_open())
            {
                this->m_LogFile.flush();
            }
        }

        void VSXmlLogger::LogModule(const Version * const pVersion)
        {
            if (!this->m_LogFile.is_open()) return;

            wstringstream logMsg;

            logMsg <<
                "    <Module uuid=\"" << pVersion->LibID << "\" " <<
                " name=\"" << pVersion->Name << "\" " <<
                " major=\"" << pVersion->Major << "\" " <<
                " minor=\"" << pVersion->Minor << "\" " <<
                " patch=\"" << pVersion->Patch << "\" " <<
                " rev=\""   << pVersion->Rev << "\" " <<
                " debug=\"" << pVersion->Debug << "\" />\n";

#ifdef VSF_DEBUG_CONSOLE
            cout << logMsg.str();
#endif
            m_LogFile << logMsg.str();

            if (m_Flags & LF_Flush)
            {
                m_LogFile << flush;
            }
        }

        void VSXmlLogger::Log(const LogLevel level, const String & module, const String & msg, ...)
        {
            va_list args;

            if (!this->m_LogFile.is_open()) return;

            LogLevel mask = (LogLevel) (level & m_LogLevel);

            if (!(mask & LL_Severity) || !(mask & LL_Origin)) return;

            try
            {
                va_start(args, msg);
                String fmtmsg = String::FormatV(msg, args);
                va_end(args);

                // Format the message in memory to prevent partial messages from being dumped
                wstringstream logMsg;

                logMsg << "    <Message severity=\"";
                logMsg << level;
                logMsg << "\" ";
                logMsg << this->LogTime().GetData();
                logMsg << this->LogTicks().GetData();
                logMsg << " source=\"";
                logMsg << module.GetData();
                logMsg << "\">";
                logMsg << fmtmsg.GetData();
                logMsg << "</Message>\n";

#ifdef _DEBUG
                if (level & (LL_Warning | LL_Error))
                {
                    OutputDebugString(logMsg.str().c_str());
                }
#   ifdef VSF_DEBUG_CONSOLE
                cout << logMsg.str();
#   endif
#endif
                m_LogFile << logMsg.str();

                if (m_Flags & LF_Flush)
                {
                    m_LogFile << flush;
                }
            }
            catch(const std::exception & exc)
            {
#ifdef _DEBUG
                OutputDebugStringA(exc.what());
#else
                UNREFERENCED_PARAMETER(exc);
#endif
            }
        }

        void VSXmlLogger::SetFlags(const LogFlags flags)
        {
            m_Flags = flags;
        }

        const LogFlags VSXmlLogger::GetFlags(void) const
        {
            return m_Flags;
        }

        void VSXmlLogger::SetLogLevel(const LogLevel level)
        {
            m_LogLevel = level;
        }

        const LogLevel VSXmlLogger::GetLogLevel(void) const
        {
            return m_LogLevel;
        }

        String VSXmlLogger::LogTime(void) const
        {
            time_t now = time(nullptr);

            if (localtime_s(this->m_LocalTime, &now) == 0)
            {
                wstringstream stamp;

                stamp << L" time=\"" << put_time(m_LocalTime, L"%H%M%S") << L"\" ";
                return stamp.str();
            }
            else
            {
                return String(L" time=\"000000\" ");
            }
        }

        String VSXmlLogger::LogDate(void) const
        {
            time_t now = time(nullptr);

            if (localtime_s(this->m_LocalTime, &now) == 0)
            {
                wstringstream stamp;

                stamp << L" date=\"" << put_time(m_LocalTime, L"%Y%m%d") << L"\" ";
                return stamp.str();
            }
            else
            {
                return String(L" date=\"00000000\" ");
            }
        }

        String VSXmlLogger::LogTicks(void) const
        {
            return String::Format(L" ticks=\"%d\" ", GetTickCount());
        }
    }
}
