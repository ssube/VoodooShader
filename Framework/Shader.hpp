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
	public:
		Shader(Core * parent, std::string filename, const char ** args = NULL);
		Shader(Core * parent, CGeffect effect);

		std::string Name();

		/**
		 * Set a technique from this shader to be used as the default technique.
		 * Some Adapter functions simply retrieve the default technique to draw
		 * with. 
		 * 
		 * @param name The name of a technique within this shader to use.
		 *
		 * @note This takes a name and not a TechniqueRef to prevent cross-
		 *		linking shaders (which would be a Bad Thing, at best).
		 * 
		 * @throws Exception if the name is not a valid technique.
		 */
		void SetDefaultTechnique(String name);

		/**
		 * Retrieves the default technique from this shader. All drawing should
		 * be done with the default technique: it is guaranteed to be valid for
		 * the current shader profiles and API. The default technique is the
		 * first technique to validate.
		 *
		 * @return A reference to the default technique.
		 * 
		 * @note To influence the priority of techniques, simply place the most
		 *		specific or least compatible first (often high-quality ones) and
		 *		count down with the most compatible and simplest technique last.
		 *		The first valid technique found becomes the default.
		 */
		TechniqueRef GetDefaultTechnique();

		/**
		 * Retrieve the number of effect-level parameters in this shader. These
		 * hold a single value for all passes.
		 * 
		 * @return The parameter count.
		 */
		size_t GetParamCount();

		/**
		 * Retrieve a specific parameter from the shader. These may be modified
		 * at runtime and will automatically update Cg and the GPU with their
		 * value (in most cases).
		 *
		 * @param index The index of the parameter to retrieve.
		 * @return A shared reference to the parameter.
		 */
		ParameterRef GetParam(size_t index);

		/**
		 * Retrieve a technique from the shader by name. Most cases should use
		 * the default technique, but some specific applications may want a
		 * particular technique. 
		 *
		 * @param name The technique name to find.
		 * @return A shared reference to the technique if it is found, an empty
		 *		shared reference otherwise.
		 */
		TechniqueRef GetTechnique(String name);

		/**
		 * Retrieve the Core this Shader is a child of.
		 */
		Core * GetCore();

		/**
		 * Initialize delayed linking (or relinking) for this shader. This
		 * function rebuilds the technique and pass structure, but <em>does not
		 * </em> reload or recompile the effect (Cg effects are handled by the
		 * core).
		 */
		void Link();

	private:
		void SetupParameters();
		void SetupTechniques();

		Core * mCore;
		std::string mName;
		CGeffect mEffect;

		TechniqueRef mDefaultTechnique;
		TechniqueMap mTechniques;
		ParameterMap mParameters;
	};

	class VOODOO_API Technique
	{
	public:
		Technique(Shader * parent, CGtechnique cgTech);

		String Name();

		Core * GetCore();

		size_t GetPassCount();
		PassRef GetPass(size_t index);

		TextureRef GetTarget();

		void Link();

	private:
		Core * mCore;
		Shader * mParent;
		std::string mName;
		PassVector mPasses;
		CGtechnique mTechnique;
		TextureRef mTarget;
	};

	class VOODOO_API Pass
	{
	public:
		Pass(Technique * parent, CGpass cgPass);

		String Name();

		Core * GetCore();

		TextureRef GetTarget();

		CGprogram GeometryProgram();
		CGprogram VertexProgram();
		CGprogram FragmentProgram();

		void Link();

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
