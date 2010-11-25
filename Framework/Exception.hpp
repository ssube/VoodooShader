
#ifndef VOODOO_EXCEPTION_HPP
#define VOODOO_EXCEPTION_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	/**
	 * Core engine class for the Voodoo Shader Framework. 
	 */
	class VOODOO_API Exception
		: public std::exception
	{
	public:
		Exception(const char * message, Core * core, char * file, char * function, int line);
		Exception(std::string message, Core * core, char * file, char * function, int line);

		const Core * mCore;
		const std::string mMessage;
		const char * mFile;
		const char * mFunction;
		const int mLine;
	};
}

#endif