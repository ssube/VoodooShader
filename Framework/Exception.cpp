
#include "Exception.hpp"
#include "Core.hpp"

namespace VoodooShader
{
	Exception::Exception
	(
		const char * message, Core * core, char * file, char * function, 
		int line
	)
		: std::exception(message),  mMessage(message), mCore(core), mFile(file),
			mFunction(function), mLine(line)
	{
		if ( core )
		{
			core->Log("Voodoo Core: Exception in %s at %s (%d): %s\n",
				file, function, line, message);
		}
	}

	Exception::Exception
	(
		String message, Core * core, char * file, char * function, int line
	)
		: std::exception(message.c_str()), mMessage(message), mCore(core), 
			mFile(file), mFunction(function), mLine(line)
	{
		if ( core )
		{
			core->Log("Voodoo Core: Exception in %s at %s (%d): %s\n", 
				file, function, line, message);
		}
	}

	String Exception::Message()
	{
		char buffer[1024];
		ZeroMemory(buffer, 1024);

		sprintf_s(buffer, 1023, "VoodooShader::Exception in file %s at %s (line %d): %s", mFile, mFunction, mLine, mMessage);

		return String(buffer);
	}
}