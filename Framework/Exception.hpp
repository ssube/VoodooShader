/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (C) 2010 by Sean Sube
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
		Exception(const char * message, Core * core, char * file, 
			char * function, int line);
		Exception(std::string message, Core * core, char * file, 
			char * function, int line);

		std::string Message();

	private:
		const Core * mCore;
		const std::string mMessage;
		const char * mFile;
		const char * mFunction;
		const int mLine;
	};
}

#endif
