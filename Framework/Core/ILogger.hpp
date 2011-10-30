/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
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
     * @addtogroup VoodooCore 
     * @{ 
     */

    /**
    * Defines the interface for a Logger class.
    */
    class ILogger :
        public IObject
    {
    public:
        virtual ~ILogger(void) throw() {};
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Opens a file for use by this logger. 
         * 
         * @param filename The name of the file to open (may contain an absolute or relative path). 
         * @param append Flag specifying the open mode; if true, any existing log is truncated. 
         * @return Success of the open operation.
         */
        virtual bool Open(_In_ const IFile * pFile, _In_ bool append) throw() = 0;

        /**
         * Closes the log file, if one is open. 
         */
        virtual void Close(void) throw() = 0;

        /**
         * Immediately writes all pending data to disk. @note This is useful for catchable errors which may have fatal 
         * consequences (Exception calls this in case the exception is uncaught). 
         * 
         * @warning This may not (probably will not) be any good in case of a segfault or other crash. If you need complete 
         *     debug logging, call Logger::SetBufferSize(int32_t) with a buffer size of 0 and all logged messages @e should 
         *     make it to disk, even during fatal crashes.
         */
        virtual void Flush(void) throw() = 0;

        /**
         * Set the default minimum message level. Messages below this level will not be logged. 
         * 
         * @param level The minimum log level.
         */
        virtual void SetLogLevel(_In_ LogLevel level) throw() = 0;

        virtual LogLevel GetLogLevel(void) const throw() = 0;

        /**
         * Writes a module stamp to the log. This records the name and version info for a select module (used to log what 
         * modules were present during a logging session).
         * 
         * @param module The module version info to log.
         */
        virtual void LogModule(_In_ const Version * pModule) throw() = 0;

        /**
         * Log a message, may be formatted with printf syntax. 
         * 
         * @param level The level for this message. 
         * @param module The logging module's name. 
         * @param msg The message format string. 
         * @param ... The parameters to insert.
         */
        virtual void Log
        (
            _In_ LogLevel level,
            _In_ const String & module,
            _In_ _Printf_format_string_ const String & msg,
            ...
        ) throw() = 0;

        virtual void SetFlags(_In_ LogFlags flags) throw() = 0;

        virtual LogFlags GetFlags(void) const throw() = 0;
    };
    
    /**
     * @}
     */
}
