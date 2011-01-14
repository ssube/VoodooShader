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

#include "Meta.hpp"

namespace VoodooShader
{
	/**
	 * Logfile management class, capable of opening, closing, writing to and 
	 * dumping log files. Throws std::runtime_error on problems opening the log
	 * and has timestamp and formatting capabilities.
	 */
	class VOODOO_API Logger
	{
	public:
		/**
		 * Default constructor, opens a log file with the given name and mode.
		 *
		 * @param filename Name of the log file to open.
		 * @param append If log file already exists, append to contents (the 
		 *		default value is false, which will truncate an existing file).
		 * @throws Exception if the log file cannot be opened.
		 */
		Logger(const char * filename, bool append = false);

		/** 
		 * Default destructor, flushes and closes the log file (if open).
		 */
		~Logger();

		/**
		 * Write a formatted timestamp to the log. The timestamp will have the 
		 * form <code>HH.MM.SS :: </code>. Leading zeros are guaranteed to be
		 * present, so the timestamp length remains constant.
		 */
		String Timestamp();

		/**
		 * Log a simple (pre-formatted) message.
		 *
		 * @param msg The message string.
		 * @param timestamp Prepend a timestamp to the message.
		 * @see Logger::Timestamp()
		 */
		void Log(const char * msg, ...);

		//void Log(bool timestamp, String msg, ...);

		void DebugLog(const char * msg, ...);

		/**
		 * Sets the internal buffer to a given size.
		 *
		 * @param bytes The size.
		 * @note A size of 0 will force messages to be written directly to disk.
		 *		This may have a notable performance hit, but makes debug 
		 *		messages more likely to survive	crashes.
		 * @note LogFormat strings will not be be written until Format::Done() 
		 *		is called, regardless of the buffer size.
		 */
		void SetBufferSize(unsigned int bytes);

		/**
		 * Immediately writes all pending data to disk.
		 *
		 * @note This is useful for catchable errors which may have fatal 
		 *		consequences (Exception calls this in case the exception is 
		 *		uncaught).
		 * @warning This may not (probably will not) be any good in case of a 
		 *		segfault or other crash. If you	need complete debug logging, 
		 *		call Logger::SetBufferSize(unsigned int) with a buffer size	of 0
		 *		and all logged messages <em>should</em> make it to disk, even 
		 *		during fatal crashes.
		 */
		void Dump();

		/**
		 * Opens a file for use by this Logger.
		 *
		 * @param filename The name of the file to open (may contain an absolute
		 *		or relative path).
		 * @return Success of the open operation.
		 */
		bool Open(const char* filename);

		/**
		 * Closes the log file, if one is open.
		 */
		void Close();

	private:
		std::fstream mLogFile;
		tm * mLocalTime;
	};
}
