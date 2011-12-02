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

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_framework_interfaces
     * @{
     */

    /**
     * @class ILogger
     *
     * Provides structured and formatted logging for module listing and level-filtered messages.
     *
     * @par IID
     *      e6f3128e-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(ILogger, IObject, {0x8e, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;

        /**
         * Opens a file for use by this logger.
         *
         * @param filename The name of the file to open (may contain an absolute or relative path, will be parsed).
         * @param append Flag specifying the open mode; if true, any existing log is truncated.
         * @return Success of the open operation.
         */
        VOODOO_METHOD(Open)(_In_ const String & filename, _In_ const bool append) PURE;
        /**
         * Opens a file for use by this logger.
         *
         * @param pFile The file to open.
         * @param append Flag specifying the open mode; if true, any existing log is truncated.
         * @return Success of the open operation.
         */
        VOODOO_METHOD(Open)(_In_ IFile * const pFile, _In_ const bool append) PURE;
        /**
         * Closes the log file, if one is open.
         */
        VOODOO_METHOD_(void, Close)() PURE;
        /**
         * Immediately writes all pending data to disk. @note This is useful for catchable errors which may have fatal
         * consequences (Exception calls this in case the exception is uncaught).
         *
         * @warning This may not (probably will not) be any good in case of a segfault or other crash. If you need complete
         *     debug logging, call Logger::SetBufferSize(int32_t) with a buffer size of 0 and all logged messages @e should
         *     make it to disk, even during fatal crashes.
         */
        VOODOO_METHOD_(void, Flush)() PURE;
        /**
         * Set the default minimum message level. Messages below this level will not be logged.
         *
         * @param level The minimum log level.
         */
        VOODOO_METHOD_(void, SetLogLevel)(_In_ const LogLevel level) PURE;
        /**
         * Get the current log level.
         *
         * @return The log level.
         */
        VOODOO_METHOD_(const LogLevel, GetLogLevel)() CONST PURE;
        /**
         * Writes a module stamp to the log. This records the name and version info for a select module (used to log what
         * modules were present during a logging session).
         *
         * @param pModule The module version info to log.
         */
        VOODOO_METHOD_(void, LogModule)(_In_ const Version * const pModule) PURE;
        /**
         * Log a message, may be formatted with printf syntax.
         *
         * @param level The level for this message.
         * @param source The source of the log message, usually the calling module's name.
         * @param format The message format string. Make sure to insert Unicode strings with %s and ASCII with %S.
         * @param ... The parameters to insert.
         */
        virtual void Log
        (
            _In_ const LogLevel level,
            _In_ const wchar_t * source,
            _In_ _Printf_format_string_ const wchar_t * format,
            ...
        ) PURE;
        /**
         * Set log flags, controls handling of the file (flush after message, formatting, etc).
         *
         * @param flags New flag settings.
         */
        VOODOO_METHOD_(void, SetFlags)(_In_ const LogFlags flags) PURE;
        /**
         * Get the current log flags.
         *
         * @return Current flags.
         */
        VOODOO_METHOD_(const LogFlags, GetFlags)() CONST PURE;
    };
    /**
     * @}
     */
}
