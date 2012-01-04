/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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
    namespace XmlLogger
    {
        const Version * VOODOO_CALLTYPE API_ModuleVersion();
        const uint32_t  VOODOO_CALLTYPE API_ClassCount();
        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid);
        IObject *       VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ ICore * pCore);

        /**
         * @class VSXmlLogger
         *
         * Xml logger class, writes neatly formatted logs for use with the log viewer. Supports all Voodoo ILogger features
         * and debug logging (with no buffer).
         *
         * @par ClassId
         *      e6f312b7-05af-11e1-9e05-005056c00008
         *
         * @warning If the logger isn't destroyed properly, the main tag of the log file won't be closed and the log won't
         *      be valid XML.
         */
        VOODOO_CLASS(VSXmlLogger, ILogger, ({0xB7, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
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
            ~VSXmlLogger();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;

            VOODOO_METHOD(Open)(_In_ const String & filename, _In_ const bool append);
            /**
             * Opens a file for use by this Logger.
             *
             * @param pFile The name of the file to open (may contain an absolute or relative path).
             * @param append Flag specifying the open mode; if true, any existing log is truncated.
             * @return Success of the open operation.
             */
            VOODOO_METHOD(Open)(_In_ IFile * const pFile, _In_ const bool append);
            /**
             * Checks if the log file is open.
             */
            VOODOO_METHOD(IsOpen)() CONST;
            /**
             * Closes the log file, if one is open.
             */
            VOODOO_METHOD(Close)();
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
            VOODOO_METHOD(Flush)();
            /**
             * Set the default minimum message level. Messages below this level will not be logged.
             *
             * @param level The minimum log level.
             */
            VOODOO_METHOD_(void, SetFilter)(_In_ const uint32_t level);
            VOODOO_METHOD_(LogLevel, GetFilter)() CONST;
            VOODOO_METHOD_(void, SetFlags)(_In_ const LogFlags flags);
            VOODOO_METHOD_(LogFlags, GetFlags)() CONST;
            /**
             * Log a message, may be formatted with printf syntax. @param level The level for this message.
             *
             * @param level The log level.
             * @param source The logging module's name.
             * @param msg The message format string.
             */
            VOODOO_METHOD(LogMessage)(_In_ const uint32_t level, _In_ const String & source, _In_ const String & msg);
            
        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            
            std::wfstream m_LogFile;
            LogLevel m_Filter;
            LogFlags m_Flags;
        };
    }
    /**
     * @}
     */
}
