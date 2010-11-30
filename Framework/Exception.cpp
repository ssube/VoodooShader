
#include "Exception.hpp"
#include "Core.hpp"

namespace VoodooShader
{
	Exception::Exception(const char * message, Core * core, char * file, char * function, int line)
		: std::exception(message),
		  mMessage(message), mCore(core), mFile(file), mFunction(function), mLine(line)
	{
		if ( core )
		{
			Logger * Log = core->GetLog();
			Log->Format("Voodoo Core: Exception in %s at %s (%d): %s\n")
				.With(file).With(function).With(line).With(message).Done();
			Log->Dump();
		}
	}

	Exception::Exception(std::string message, Core * core, char * file, char * function, int line)
		: std::exception(message.c_str()),
		  mMessage(message), mCore(core), mFile(file), mFunction(function), mLine(line)
	{
		if ( core )
		{
			Logger * Log = core->GetLog();
			Log->Format("Voodoo Core: Exception in %s at %s (%d): %s\n")
				.With(file).With(function).With(line).With(message).Done();
			Log->Dump();
		}
	}

	std::string Exception::Message()
	{
		char buffer[16];
		_itoa_s(mLine, buffer, 16, 10);

		std::string msg = "Voodoo Exception in ";
		msg += mFile;
		msg += " at ";
		msg += mFunction;
		msg += " (";
		msg += buffer;
		msg += "): ";
		msg += mMessage;
		return msg;
	}
}