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

#ifndef VOODOO_SHADER_HPP
#define VOODOO_SHADER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	class VOODOO_API Shader
	{
	public:
		Shader(Core * parent, std::string filename, const char ** args = NULL);
		Shader(Core * parent, CGeffect effect);

		TechniqueRef GetDefaultTechnique();

		size_t NumParams();
		ParameterRef GetParam(size_t index);

		inline std::string Name()
		{
			return mName;
		};

		inline Core * GetCore()
		{
			return mCore;
		}

	protected:
		void LinkShader();

		void SetupTechniques();

	private:

		Core * mCore;
		std::string mName;
		CGeffect mEffect;

		TechniqueRef mDefaultTechnique;
		TechniqueMap mTechniques;
		ParameterVector mParameters;
	};

	class VOODOO_API Technique
	{
	public:
		Technique(Shader * shader, CGtechnique technique);

		inline std::string Name()
		{
			std::string name = mParent->Name();
			name += "::";
			name += mName;
			return name;
		};

		inline Core * GetCore()
		{
			return mCore;
		}

		size_t NumPasses();
		PassRef GetPass(size_t index);

	private:
		void SetupPasses();

		Core * mCore;
		Shader * mParent;
		std::string mName;
		PassVector mPasses;
		CGtechnique mTechnique;
	};

	class VOODOO_API Pass
	{
	public:
		Pass(Technique * tech, CGpass pass);

		inline std::string Name()
		{
			std::string name = mParent->Name();
			name += "::";
			name += mName;
			return name;
		};

		inline Core * GetCore()
		{
			return mCore;
		}

		TextureRef Target();

		CGprogram GeometryProgram();
		CGprogram VertexProgram();
		CGprogram FragmentProgram();

	private:
		Core * mCore;
		Technique * mParent;

		std::string mName;
		TextureRef mTarget;
		CGpass mPass;

		CGprogram mGeometryProgram;
		CGprogram mVertexProgram;
		CGprogram mFragmentProgram;
	};
}

#endif /*VOODOO_SHADER_HPP*/
