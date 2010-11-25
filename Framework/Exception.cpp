
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
			Log->Format("Voodoo Exception in %s at %s (%d): %s")
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
			Log->Format("Voodoo Exception in %s at %s (%d): %s")
				.With(file).With(function).With(line).With(message).Done();
			Log->Dump();
		}
	}
}