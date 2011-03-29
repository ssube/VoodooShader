/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

#ifdef VOODOO_IMPORT_LOGGER
#   define VOODOO_LOGGER_API __declspec(dllimport)
#else
#   define VOODOO_LOGGER_API __declspec(dllexport)
#endif

// Hide the DLL-interface warning
#pragma warning(disable:4251)

namespace VoodooShader
{
    namespace XmlLogger
    {
        /**
         * Classes and functions related to the standard XML implementation of ILogger.
         * 
         * @addtogroup VoodooXmlLogger Voodoo/XmlLogger
         * @{
         */

        /**
         * Return the module's version.
         */
        Version API_ModuleVersion();

        int API_ClassCount();

        const char * API_ClassInfo
        (
            _In_ int number
        );

        IObject * API_ClassCreate
        (
            _In_ int number, 
            _In_ Core * core
        );

        /**
         * Xml logger class, writes neatly formatted logs for use with the log viewer.
         * Supports all Voodoo ILogger features and debug logging (with no buffer).
         * 
         * @warning If the logger isn't destroyed properly, the main tag of
         *       the log file won't be closed and the log won't be valid.
         */
        class XmlLogger
            : public VoodooShader::ILogger
        {
        public:
            /**
             * Default constructor, opens a log file with the given name and mode.
             *
             * @param core The core to bind this logger to.
             */
            XmlLogger
            (
                _In_ Core * core
            );

            /** 
             * Default destructor, flushes and closes the log file (if open).
             */
            virtual ~XmlLogger();

            virtual const char * GetObjectClass();

            /**
             * Opens a file for use by this Logger.
             *
             * @param filename The name of the file to open (may contain an absolute
             *        or relative path).
             * @param append Flag specifying the open mode; if true, any existing log is truncated.
             * @return Success of the open operation.
             */
            virtual bool Open
            (
                _In_ const char * filename, 
                _In_ bool append
            );

            /**
             * Closes the log file, if one is open.
             */
            virtual void Close();

            /**
             * Set the default minimum message level. Messages below this level will
             * not be logged.
             *
             * @param level The minimum log level.
             */
            virtual void SetLogLevel
            (
                _In_ LogLevel level
            );

            /**
             * Write a formatted timestamp to the log. The timestamp will have the 
             * form <code>HHMMSS</code>. Leading zeros are guaranteed to be
             * present, so the timestamp length is 6 chars.
             *
             * @note If the system time cannot be retrieved, an error stamp of
             *          <code>000000</code> will be printed with an equal length.
             */
            virtual void LogTime();

            /**
             * Writes a formatted date to the log. The date will have the
             * form <code>YYYYMMDD</code>. Leading zeros are guaranteed to be present,
             * so the date length is 8 chars.
             *
             * @note If the system time cannot be retrieved, an error stamp of
             *          <code>00000000</code> will be printed with an equal length.
             */
            virtual void LogDate();

            /**
             * Writes a formatted tick to the log. The stamp will have the
             * form <code>xxxxxxxxx</code>, with a potentially varying length. This
             * records ticks (usually ms since system start).
             */
            virtual void LogTicks();

            /**
             * Writes a module stamp to the log. This records the name and version
             * info for a select module (used to log what modules were present during
             * a logging session).
             * 
             * @param module The module version info to log.
             */
            virtual void LogModule
            (
                _In_ Version module
            );

            /**
             * Log a message, may be formatted with printf syntax.
             *
             * @param level The level for this message.
             * @param module The logging module's name.
             * @param msg The message format string.
             * @param ... The parameters to insert.
             *
             * @warning This function has a maximum (formatted) message length of
             *        4096 characters. This can be changed if it becomes an issue.
             */
            virtual void Log
            (
                _In_ LogLevel level,
                _In_ const char * module,
                _In_ _Printf_format_string_ const char * msg, 
                ...
            );

            /**
             * Sets the internal buffer to a given size.
             *
             * @param bytes The size.
             * @note A size of 0 will force messages to be written directly to disk.
             *        This may have a notable performance hit, but makes debug 
             *        messages more likely to survive crashes.
             */
            virtual void SetBufferSize
            (
                _In_ unsigned int bytes
            );

            /**
             * Immediately writes all pending data to disk.
             *
             * @note This is useful for catchable errors which may have fatal 
             *        consequences (Exception calls this in case the exception is 
             *        uncaught).
             * @warning This may not (probably will not) be any good in case of a 
             *        segfault or other crash. If you need complete debug logging, 
             *        call Logger::SetBufferSize(unsigned int) with a buffer size of 0
             *        and all logged messages <em>should</em> make it to disk, even 
             *        during fatal crashes.
             */
            virtual void Dump();

        private:
            Core * mCore;
            LogLevel mLogLevel;
            std::fstream mLogFile;
            tm * mLocalTime;
        };
        /**
         * @}
         */
    }
}
