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
	typedef std::map<std::string, Technique*> TechniqueMap_Ptr;
	typedef std::vector<Pass*> PassVector_Ptr;

	class VOODOO_API Shader
	{
		friend class Core;
	public:
		Shader();

		Technique * GetDefaultTechnique();

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
		static ShaderRef Create(Core * parent, std::string filename, const char ** args = NULL);
		static ShaderRef Create(Core * parent, CGeffect effect);

		void Link();
		void SetupParameters();
		void SetupTechniques();

	private:
		Core * mCore;
		std::string mName;
		CGeffect mEffect;

		Technique * mDefaultTechnique;
		TechniqueMap_Ptr mTechniques;
		ParameterMap mParameters;
	};

	class VOODOO_API Technique
	{
		friend class Shader;
	public:
		Technique();

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
		Pass * GetPass(size_t index);

	protected:
		static Technique * Create(Shader * parent, CGtechnique cgTech);

	private:
		void Link();

		Core * mCore;
		Shader * mParent;
		std::string mName;
		PassVector_Ptr mPasses;
		CGtechnique mTechnique;
	};

	class VOODOO_API Pass
	{
		friend class Technique;
	public:
		Pass();

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

		TextureRef Target()
		{
			return mTarget;
		}

		CGprogram GeometryProgram();
		CGprogram VertexProgram();
		CGprogram FragmentProgram();

	protected:
		static Pass * Create(Technique * parent, CGpass cgPass);

	private:
		void Link();

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
