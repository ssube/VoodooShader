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

#ifndef VOODOO_SHADER_HPP
#define VOODOO_SHADER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	class VOODOO_API Shader
	{
	public:
		Shader(Core * parent, String filename, const char ** args = NULL);
		Shader(Core * parent, CGeffect effect);

		String Name();

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
		size_t GetParameterCount();

		/**
		 * Retrieve a specific parameter from the shader. These may be modified
		 * at runtime and will automatically update Cg and the GPU with their
		 * value (in most cases).
		 *
		 * @param index The index of the parameter to retrieve.
		 * @return A shared reference to the parameter, or to NULL if it is not
		 *		found (the index is invalid).
		 */
		ParameterRef GetParameter(size_t index);

		/**
		 * Retrieve a specific parameter from the shader. These may be modified
		 * at runtime and will automatically update Cg and the GPU with their
		 * values (in most cases).
		 *
		 * @param name The name of the parameter to retrieve.
		 * @return A shared reference to the parameter, or to NULL if it is not
		 *		found.
		 */
		ParameterRef GetParameter(String name);

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
		/**
		 * Link a particular effect-level parameter against various core
		 * elements (exact behavior depends on param type).
		 *
		 * @param param The parameter to link.
		 */
		void LinkParameter(ParameterRef param);
		void LinkSampler(ParameterRef param);

		void SetupTechniques();

		Core * mCore;
		String mName;
		CGeffect mEffect;

		TechniqueRef mDefaultTechnique;
		TechniqueMap mTechniques;
		ParameterMap mParameters;
	};

	class VOODOO_API Technique
	{
	public:
		Technique(Shader * parent, CGtechnique cgTech);

		/**
		 * Retrieve a fully qualified technique name (including shader name)
		 * from the technique.
		 *
		 * @return This technique's name
		 */
		String Name();

		/**
		 * Retrieve the core this technique was compiled under.
		 *
		 * @return The core
		 */
		Core * GetCore();

		/**
		 * Retrieve the number of passes in this technique.
		 *
		 * @return The number of passes
		 */
		size_t GetPassCount();

		/**
		 * Retrieve a specific pass from this technique.
		 *
		 * @param index The pass number to retrieve
		 * @return A reference to the given pass
		 * @throws Exception if index is greater than the number of passes the
		 *		technique has (call Technique::GetPassCount() first to find the
		 *		number of passes)
		 */
		PassRef GetPass(size_t index);

		/**
		 * Retrieve the technique's final target. This is the surface the
		 * technique expects the results of the final pass to end up in. The
		 * technique may render to scratch textures, but the final result should
		 * go into this texture (not, for example, the lastshader texture).
		 *
		 * @return A reference to the target texture
		 */
		TextureRef GetTarget();

		void Link();

	private:
		Core * mCore;
		Shader * mParent;
		String mName;
		PassVector mPasses;
		CGtechnique mTechnique;
		TextureRef mTarget;
	};

	class VOODOO_API Pass
	{
	public:
		Pass(Technique * parent, CGpass cgPass);

		/**
		 * Retrieve the fully qualified pass name, including technique and
		 * shader name.
		 *
		 * @return The pass name
		 */
		String Name();

		/**
		 * Retrieve the core this pass was compiled under.
		 *
		 * @return The core
		 */
		Core * GetCore();

		/**
		 * Retrieve the target this pass should render to. This will be a texture
		 * that has been registered with the core. The results of the shader
		 * program(s), when run, should end up <em>only</em> in this texture
		 * (scratch textures may be used, but the program should not write to a
		 * common texture such as lastpass).
		 *
		 * @return A reference to the target texture.
		 */
		TextureRef GetTarget();

		/**
		 * Retrieve a specific program stage from this pass. Each pass may have
		 * many programs, each program controls one stage of the render process.
		 * The most commonly used stages are the vertex and fragment stages,
		 * which are supported by almost all hardware. Geometry stages are also
		 * available, but not as well supported. D3D11 tessellation control 
		 * stages are not yet supported by the Voodoo Shader Framework.
		 *
		 * @param stage The stage to retrieve.
		 * @return The program corresponding to the desired stage, or NULL if
		 *		the pass has no program for that stage or an unsupported or
		 *		unknown stage is requested.
		 */
		CGprogram GetProgram(ProgramStage stage);

		void Link();

	private:
		Core * mCore;
		Technique * mParent;

		String mName;
		TextureRef mTarget;
		CGpass mPass;

		CGprogram mGeometryProgram;
		CGprogram mVertexProgram;
		CGprogram mFragmentProgram;

		//! @todo Add support for D3D11 tessellation programs
		CGprogram mDomainProgram;
		CGprogram mHullProgram;
	};
}

#endif /*VOODOO_SHADER_HPP*/
