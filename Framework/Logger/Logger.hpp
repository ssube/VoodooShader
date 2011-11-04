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
#pragma once

#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#define VOODOO_IMPORT
#define VOODOO_NO_CG
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

namespace VoodooShader
{
    namespace XmlLogger
    {
        /**
         * Provides a simple implementation of ILogger, printing a neatly-formatted Xml log. These logs can be opened with
         * the standard log viewer and support all features.
         *
         * @note In debug builds, this logger also prints all messages to cout. If a debugger is attached, warnings and
         *      errors are printed to its output window.
         *
         * @addtogroup voodoo_module_xmllogger Voodoo/XmlLogger
         * @{
         */

        const Version * VOODOO_CALL API_ModuleVersion(void);
        const uint32_t  VOODOO_CALL API_ClassCount(void);
        const wchar_t * VOODOO_CALL API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid);
        IObject *       VOODOO_CALL API_ClassCreate(_In_ const uint32_t index, _In_ ICore * pCore);

        /* VSXmlLogger: e6f312b7-05af-11e1-9e05-005056c00008 */
        DEFINE_CLSID(VSXmlLogger, 0xB7, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);

        /**
         * Xml logger class, writes neatly formatted logs for use with the log viewer. Supports all Voodoo ILogger features
         * and debug logging (with no buffer).
         *
         * @warning If the logger isn't destroyed properly, the main tag of the log file won't be closed and the log won't
         *      be valid XML.
         */
        class VSXmlLogger :
            public ILogger
        {
        public:
            /**
             * Default constructor, opens a log file with the given name and mode.
             *
             * @param pCore The core to bind this logger to.
             */
            VSXmlLogger(_In_ ICore * pCore);

            /**
             * Default destructor, flushes and closes the log file (if open).
             */
            virtual ~VSXmlLogger(void);

            virtual uint32_t AddRef(void) const throw();
            virtual uint32_t Release(void) const throw();
            virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
            virtual String ToString(void) const throw();
            virtual ICore * GetCore(void) const throw();

            virtual bool Open(_In_ const String & filename, _In_ const bool append) throw();

            /**
             * Opens a file for use by this Logger.
             *
             * @param pFile The name of the file to open (may contain an absolute or relative path).
             * @param append Flag specifying the open mode; if true, any existing log is truncated.
             * @return Success of the open operation.
             */
            virtual bool Open(_In_ IFile * const pFile, _In_ const bool append) throw();

            /**
             * Closes the log file, if one is open.
             */
            virtual void Close(void) throw();

            /**
             * Immediately writes all pending data to disk.
             *
             * @note This is useful for catchable errors which may have fatal consequences (Exception calls this in case the
             *      exception is uncaught).
             *
             * @warning This may not (probably will not) be any good in case of a segfault or other crash. If you need
             *      complete debug logging, call Logger::SetBufferSize(unsigned int) with a buffer size of 0 and all logged
             *      messages @em should make it to disk, even during fatal crashes
             */
            virtual void Flush(void) throw();

            /**
             * Set the default minimum message level. Messages below this level will not be
             * logged. @param level The minimum log level.
             */
            virtual void SetLogLevel(_In_ const LogLevel level) throw();

            virtual const LogLevel GetLogLevel(void) const throw();

            /**
             * Writes a module stamp to the log. This records the name and version info for a select module (used to log
             * what modules were present during a logging session).
             *
             * @param pModule The module version info to log.
             */
            virtual void LogModule(_In_ const Version * const pModule) throw();

            /**
             * Log a message, may be formatted with printf syntax. @param level The level for this message.
             *
             * @param module The logging module's name.
             * @param msg The message format string.
             * @param ... The parameters to insert.
             */
            virtual void Log
            (
                _In_ const LogLevel level,
                _In_ const String & module,
                _In_ _Printf_format_string_ const String & msg,
                ...
            ) throw();

            virtual void SetFlags(_In_ const LogFlags flags);

            virtual const LogFlags GetFlags(void) const throw();

        private:
            /**
             * Formats a timestamp for the log. The timestamp will have the form <code>HHMMSS</code>. Leading zeros are
             * guaranteed to be present, so the timestamp length is 6 chars.
             *
             * @note If the system time cannot be retrieved, an equal-length error stamp of <code>000000</code> will be
             *      returned.
             */
            String LogTime(void) const;

            /**
             * Formats a date for the log. The date will have the form <code>YYYYMMDD</code>. Leading zeros are guaranteed
             * to be present, so the date length is 8 chars.
             *
             * @note If the system time cannot be retrieved, an equal-length error stamp of <code>00000000</code> will be
             *      returned.
             */
            String LogDate(void) const;

            /**
             * Formats the system's current tick count. The stamp will have the form <code>xxxxxxxxx</code>, with a
             * potentially varying length. This records ticks, usually ms since system start.
             */
            String LogTicks(void) const;

            mutable uint32_t m_Refs;
            ICore * m_Core;

            LogLevel m_LogLevel;
            std::wfstream m_LogFile;
            tm * m_LocalTime;
            LogFlags m_Flags;
        };

        /**
         * @}
         */
    }
}
