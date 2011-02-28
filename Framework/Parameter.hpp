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
		Parameter
        (
            __in __notnull Core * core, 
            __in String name, 
            __in_nz ParameterType type
        );

		Parameter
        (
            __in __notnull Shader * parent, 
            __in __notnull CGparameter param
        );

		String Name();

		CGparameter GetParameter();
		ParameterType GetType();

		/**
		 * Binds two parameters, forcing their values to be identical. This 
		 * should be used with care, as it has some requirements and 
		 * restrictions. It is used to control some of the more common variables
		 * found in shaders.
		 *
		 * @warning This <em>cannot</em> be used to bind one effect's parameter 
		 *		to another. It can only be used to bind effect parameters to
		 *		virtual or global parameters. If this is called on a non-virtual
		 *		parameter, it will throw.
		 */
		void Attach
        (
            __in ParameterRef param
        );

		void Set
        (
            __in TextureRef newTex
        );

		void Set
        (
            __in float newX
        );

		void Set
        (
            __in float newX, 
            __in float newY
        );

		void Set
        (
            __in float newX, 
            __in float newY, 
            __in float newZ
        );

		void Set
        (
            __in float newX, 
            __in float newY, 
            __in float newZ, 
            __in float newW
        );

		void Get
        (
            __out TextureRef & param
        );

		void Get
        (
            __out float & paramX
        );

		void Get
        (
            __out float & paramX, 
            __out float & paramY
        );

		void Get
        (
            __out float & paramX, 
            __out float & paramY, 
            __out float & paramZ
        );

		void Get
        (
            __out float & paramX, 
            __out float & paramY, 
            __out float & paramZ, 
            __out float & paramW
        );

		inline TextureRef GetTexture()
		{
			return mValueTexture;
		};

		inline float * GetFloat()
		{
			return mValueFloat;
		};

		void ForceUpdate();

	private:
		Shader * mParent;
		Core * mCore;

		bool mVirtual;
		CGparameter mParam;
		ParameterType mType;

		// Value
		TextureRef mValueTexture;

		union
		{
			float mValueMatrix[4][4];
			float mValueFloat[16];
		};
	};
}

#endif /*VOODOO_PARAMETER_HPP*/
