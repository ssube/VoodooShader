//#include <boost/format.hpp>

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
	}

	/**
	 * Writes a timestamp to the log file, if the system time can be retrieved.
	 * The timestamp is of the form "<code>HH:MM:SS :: </code>" and leading
	 * zeros are retained so the length will always be 12 characters.
	 * 
	 * @note If the system time cannot be retrieved, an error stamp will be
	 *		printed with an equal length.
	 */
	VOODOO_API String Logger::Timestamp()
	{
		time_t now = time(NULL);

		if ( localtime_s(this->mLocalTime, &now) == ERROR_SUCCESS )
		{
			char stamp[16];
			sprintf_s(stamp, 16, "%02d.%02d.%02d :: ", 
				this->mLocalTime->tm_hour,
				this->mLocalTime->tm_min,
				this->mLocalTime->tm_sec);
			return stamp;
		} else {
			return "Unknown  :: ";
		}
	}

	VOODOO_API void Logger::SetBufferSize(unsigned int bytes)
	{
		this->mLogFile.rdbuf()->pubsetbuf(0, bytes);
	}

	/// Logs a simple timestamped string
	VOODOO_API void Logger::Log(const char * msg, ...)
	{
		va_list args;
		char buffer[4096];

		vsnprintf(buffer, 4095, msg, args);
		buffer[4095] = 0;

		this->mLogFile << this->Timestamp() << buffer;
	}

	VOODOO_API void Logger::DebugLog(const char * msg, ...)
	{
#ifdef _DEBUG
		va_list args;
		this->Log(msg, args);
#endif
	}

	/// \brief Dumps the log file to disk immediately.
	VOODOO_API void Logger::Dump()
	{
		this->mLogFile.flush();
	}

	/// \brief Opens a file for logging
	///
	/// Closes the open logfile (if any), then opens a new file (with the given 
	///		name).
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
}
