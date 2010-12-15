#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <boost/format.hpp>

#include "Logger.hpp"
#include "Exception.hpp"

using namespace std;

namespace VoodooShader
{
	VOODOO_API Logger::Logger(const char * filename, bool append)
	{
		unsigned int flags = ios_base::out;
		if ( append )
		{
			flags |= ios_base::app;
		} else {
			flags |= ios_base::trunc;
		}
		this->mLogFile.open(filename, flags);

		if ( !this->mLogFile.is_open() )
		{
			Throw("Could not open log file!", NULL);
		}

		this->mFormat = new Formatter(this);
		this->mLocalTime = new tm();

		this->Log("Logger created.\n");
	}

	Logger::~Logger()
	{
		if ( this->mLogFile.is_open() )
		{
			this->mLogFile.close();
		}
		if ( this->mLocalTime )
		{
			delete this->mLocalTime;
		}
		if ( this->mFormat )
		{
			delete this->mFormat;
		}
	}

	/// \brief Adds a timestamp at the current position in the log
	///
	/// Writes a hour:minute:second timestamp at the current stream position in the log file.
	VOODOO_API void Logger::Timestamp()
	{
		time_t now = time(NULL);

		if ( localtime_s(this->mLocalTime, &now) == ERROR_SUCCESS )
		{
			this->Format("%02d.%02d.%02d :: ")
				.With(this->mLocalTime->tm_hour)
				.With(this->mLocalTime->tm_min )
				.With(this->mLocalTime->tm_sec ).Done(false);
		} else {
			this->mLogFile <<"Unknown  :: ";
		}
	}

	VOODOO_API void Logger::SetBufferSize(unsigned int bytes)
	{
		this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
	}

	/// Logs a simple timestamped line
	VOODOO_API void Logger::Log(const char* msg, bool timestamp)
	{
		if ( timestamp )
		{
			this->Timestamp();
		}
		this->mLogFile << msg;
	}

	/// Logs a simple timestamped string
	VOODOO_API void Logger::Log(string msg, bool timestamp)
	{
		if ( timestamp )
		{
			this->Timestamp();
		}
		this->mLogFile << msg;
	}

	/// \brief Dumps the log file to disk immediately.
	VOODOO_API void Logger::Dump()
	{
		this->mLogFile.flush();
	}

	/// \brief Opens a file for logging
	///
	/// Closes the open logfile (if any), then opens a new file (with the given name).
	///
	/// The new file is truncated if it exists and is opened for writing.
	VOODOO_API bool Logger::Open(const char* filename)
	{
		if ( this->mLogFile.is_open() )
		{
			this->mLogFile.close();
		}

		this->mLogFile.open(filename, ios_base::out|ios_base::trunc);

		if ( this->mLogFile.is_open() )
		{
			this->Log("Logger: Log file opened by Logger::Open.\n");
			return true;
		} else {
			return false;
		}
	}

	/// \brief Closes the open logfile, if any
	VOODOO_API void Logger::Close()
	{
		if ( this->mLogFile.is_open() )
		{
			this->Log("Logger: Log file closed by Logger::Close.\n");
			this->mLogFile.close();
		}
	}

	VOODOO_API Formatter & Logger::Format(std::string message)
	{
		return this->mFormat->Record(message);
	}

	VOODOO_API Formatter & Formatter::Record(string fmtString)
	{
		this->mFmtObj.parse(fmtString);
		return (*this);
	}

	VOODOO_API void Formatter::Done(bool timestamp)
	{
		try
		{
			this->mParent->Log(this->mFmtObj.str(), timestamp);
		} catch ( Exception & exc ) {
			this->mParent->Log("Voodoo Core: Logger error.", timestamp);
		}
	}

	VOODOO_API Formatter::Formatter(Logger * parent)
	{
		this->mParent = parent;
	}
}
