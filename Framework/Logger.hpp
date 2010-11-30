/**************************************************************************\
			This file is part of the Voodoo Shader Framework.

	All source code,  binaries and  other original  content is 
	     Copyright (c) 2010 by Sean Sube, all rights reserved.
	Neither this  project nor any portion of it may  be modified or distributed, or used for 
	public or private demonstration or any other purpose without express written permission.
\**************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <boost/format.hpp>

#include "Meta.hpp"

namespace VoodooShader
{
	// Forward-declare the Logger class so it can be used in log_format
	class Logger;

	/**
	 * Logging Format class.
	 *
	 * @note Formats strings using Boost::Format. See docs for full available syntax.
	 * @note Only used internally in Logger.
	 */
	class VOODOO_API Formatter
	{
	public:
		/**
		 * Create a log format object.
		 * 
		 * @param parent The Logger to be attached as the parent.
		 */
		Formatter(Logger * parent);

		/**
		 * Begins logging a message (passes an initial formatting string).
		 *
		 * @param fmtString A format string, formed according to the boost::format docs.
		 * @return A reference to this Formatter.
		 */
		Formatter & Record(std::string fmtString);

		/**
		 * Passes a single argument to this format object.
		 *
		 * @param varString A value or object to be formatted and placed into the format stream.
		 * @return A reference to this Formatter.
		 */
		template<class T> 
		Formatter & With(T & varString)
		{
			try
			{
				(this->mFmtObj) % (varString);
			} catch ( ... ) { 
				(this->mFmtObj) % ("Invalid operand");
			}
				
			return (*this);
		}

		/**
		 * Passes a single argument to this format object.
		 *
		 * @param varString A value or object to be formatted and placed into the format stream.
		 * @return A reference to this Formatter.
		 */
		template<class T> 
		Formatter & With(const T & varString)
		{
			try
			{
				(this->mFmtObj) % (varString);
			} catch ( ... ) { 
				(this->mFmtObj) % ("Invalid operand");
			}
				
			return (*this);
		}

		/**
		 * Finalizes a formatted string and prints it to the log.
		 *
		 * @param timestamp Prepend a timestamp to the logged message.
		 */
		void Done(bool timestamp = true);

	private:
		Logger * mParent;
		boost::format mFmtObj;
	};

	/**
	 * Logfile management class, capable of opening, closing, writing to and dumping log files.
	 * Throws runtime_errors on open problems and has timestamp and formatting capabilities.
	 */
	class VOODOO_API Logger
	{
	public:
		/**
		 * Default constructor, opens a log file with the given name and mode.
		 *
		 * @param filename Name of the log file to open.
		 * @param append If log file already exists, append to contents (the default value is
		 *		false, which will truncate an existing file).
		 * @throws Exception if the log file cannot be opened.
		 */
		Logger(const char * filename, bool append = false);

		/** 
		 * Default destructor, flushes and closes the log file (if open).
		 */
		~Logger();

		/**
		 * Write a formatted timestamp to the log. The timestamp will have the form:<br />
		 * "<code>HH.MM.SS :: </code>"
		 */
		void Timestamp();

		/**
		 * Log a simple (pre-formatted) message.
		 *
		 * @param msg The message string.
		 * @param timestamp Prepend a timestamp to the message.
		 * @see Logger::Timestamp()
		 */
		void Log(const char * msg, bool timestamp = true);

		/**
		 * Log a simple (pre-formatted) message.
		 *
		 * @param msg The message string.
		 * @param timestamp Prepend a timestamp to the message.
		 * @see Logger::Timestamp()
		 */
		void Log(std::string msg, bool timestamp = true);

		/** 
		 * Allows for formatted logging.
		 * This object is a little confusing to use, calls should be of the form:<br />
		 * <code>Log->Format("msg").With(param).Done();</code>
		 *
		 * @return A reference to this @ref Logger "Logger's" internal Formatter object. 
		 *		This may be used for delayed formatting, but only with caution.
		 */
		Formatter & Format(std::string message);

		/**
		 * Sets the internal buffer to a given size.
		 *
		 * @param bytes The size.
		 * @note A size of 0 will force messages to be written directly to disk. This may
		 *		have a notable performance hit, but makes debug messages more likely to survive
		 *		crashes.
		 * @note LogFormat strings will not be be written until Format::Done() is called, 
		 *		regardless of the buffer size.
		 */
		void SetBufferSize(unsigned int bytes);

		/**
		 * Immediately writes all pending data to disk.
		 *
		 * @note This is useful for catchable errors which may have fatal consequences (Exception calls this
		 *		 in case the exception is uncaught).
		 * @warning This may not (probably will not) be any good in case of a segfault or other crash. If you
		 *			need complete debug logging, call Logger::SetBufferSize(unsigned int) with a buffer size
		 *			of 0 and all logged messages <em>should</em> make it to disk, even during fatal crashes.
		 */
		void Dump();

		/**
		 * Opens a file for use by this Logger.
		 *
		 * @param filename The name of the file to open (may contain an absolute or relative path).
		 * @return Success of the open operation.
		 */
		bool Open(const char* filename);

		/**
		 * Closes the log file, if one is open.
		 */
		void Close();

	private:
		std::fstream mLogFile;
		Formatter * mFormat;
		tm * mLocalTime;
	};
}