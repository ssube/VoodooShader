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

#ifndef VOODOO_PARAMETER_HPP
#define VOODOO_PARAMETER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	class VOODOO_API Parameter
	{
	public:
		/**
		 * Virtual (global-level) parameter constructor. This will create a 
		 * named parameter in the Cg runtime and register it in the given Core. 
		 */
		Parameter(Core * core, String name, ParameterType type);
		Parameter(Shader * parent, CGparameter param);

		/**
		 * Binds two parameters, forcing their values to be identical. This should
		 * be used with care, as it has some requirements and restrictions. It is
		 * used to control some of the more common variables found in shaders.
		 *
		 * @warning This <em>cannot</em> be used to bind one effect's parameter to
		 *		another. It can only be used to bind effect parameters to virtual
		 *		or global parameters. If this is called on a non-virtual parameter,
		 *		it will throw.
		 */
		void Attach(ParameterRef param);

		CGparameter Param(void);

		std::string Name();

		ParameterType Type(void);

		void Set(TextureRef newTex);
		void Set(float newX);
		void Set(float newX, float newY);
		void Set(float newX, float newY, float newZ);
		void Set(float newX, float newY, float newZ, float newW);

		void Get(TextureRef & param);
		void Get(float & paramX);
		void Get(float & paramX, float & paramY);
		void Get(float & paramX, float & paramY, float & paramZ);
		void Get(float & paramX, float & paramY, float & paramZ, float & paramW);

		inline TextureRef GetTexture()
		{
			return mValueTexture;
		};

		inline float * GetFloat()
		{
			return mValueFloat;
		};

	private:
		ShaderRef mParent;

		bool mVirtual;
		CGparameter mParam;
		ParameterType mType;

		// Value
		TextureRef mValueTexture;
		float mValueFloat[4];
	};
}

#endif /*VOODOO_PARAMETER_HPP*/
