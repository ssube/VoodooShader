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

#ifndef VOODOO_ADAPTER_HPP
#define VOODOO_ADAPTER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	/**
	 * Generic vertex format for use with Adapter draw calls. This format is
	 * compatible with both OpenGL and DirectX.
	 *
	 * @note Because of the draw mechanism for adapters, most draws with
	 *		user-provided vertexes will not use vertex buffers of any sort. 
	 *		This can hurt performance if used heavily, so the Core avoids
	 *		drawing through adapters as much as possible.
	 * @note For simplicity, this uses the D3DFVF_XYZRHW format, which requires
	 *		a winding value. OpenGL draws can often ignore this. The members are
	 *		ordered so that <code>&x</code> is a valid float3 with the position
	 *		and <code>&tu</code> is a valid float2 with the texture coordinate.
	 */
	struct Vertex
	{
		float x, y, z;
		float winding;
		float tu, tv;
	};

	/**
	 * Graphics adapter class, responsible for interfacing the Voodoo Core with
	 * a given graphics program. 
	 *
	 * This class should be implemented for a specific graphics application; 
	 * some generic adapters* are provided, but in most cases 
	 * application-specific behavior will make these only partially helpful. 
	 * This class does include a generic error-handler function, which adapters 
	 * may choose not to override if they do not need specific error-handling.
	 */
	class VOODOO_API Adapter
	{
	public:
		/**
		 * Causes the Adapter to load a pass for its API. This usually involved 
		 * the final compilation and any API-specific linking. Often, this is 
		 * done with Cg-provided functions (in D3D9, cgD3D9LoadProgram). 
		 *
		 * @param pass The pass to be loaded (not a shared pointer, it is 
		 *		assumed the pass will be valid for the duration of the call).
		 * @return Whether or not the pass was successfully loaded.
		 */
		virtual bool LoadPass(Pass * pass) = 0;

		/**
		* Causes the adapter to load a pass for drawing. The adapter may choose 
		* which programs to bind and how to handle the binding, as well as 
		* managing any errors that may arise.
		*
		* @param pass A shared pointer to the pass to be bound. This should be a
		*		valid pass, but	Adapters must validate it.
		*
		* @note Adapters may, at their discretion, bind some or even no programs
		*		to the true	graphics API. An example of this is the Direct3D 9 
		*		adapter, which binds only vertex and fragment programs, ignoring
		*		any geometry, domain or hull programs.
		* @note The Adapter may implement passes using deferred parameters; if 
		*		so, it <em>must</em> update all pass parameters, including 
		*		effect-level and global-level parameters used in the pass, 
		*		before this function returns. These updates must be performed so
		*		they take effect for any draw calls occurring after this call.
		*/
		virtual void BindPass(PassRef pass) = 0;

		/**
		 * Unbinds the last bound pass. This resets the shader state of the 
		 * underlying graphics API to the same state as before the pass was 
		 * bound. 
		 *
		 * @note Adapters must restore all states modified by an 
		 *		Adapter::BindPass() call. They should cache the data, but the 
		 *		exact implementation is left to the Adapter.
		 */
		virtual void UnbindPass() = 0;

		/**
		 * Draws a quad to the screen.
		 *
		 * @param vertexData This must be NULL or contain vertex data.
		 *
		 * @note In APIs which do not not support quads, this should be 
		 *		implemented as two tris or another geometry form capable of 
		 *		taking the same vertexes in the same order as an OpenGL quad.
		 * @note The quad must meet a set of requirements:
		 *		 <ol>
		 *			<li>The depth buffer should not be written to.</li>
		 *			<li>The quad must be drawn in front of all other geometry
		 *				(no depth test).</li>
		 *			<li>The quad must be drawn, it must not be culled.</li>
		 *			<li>Alpha-blending and testing should be disabled entirely.
		 *				</li>
		 *			<li>Render states must be identical to their original status
		 *				when the function returns.</li>
		 *		 </ol>
		 *
		 * @note The vertex format is as follows:
		 *		<code>
		 *			struct vertex
		 *			{
		 *				float3 position;
		 *				float winding;
		 *				float2 texcoord0;
		 *			};
		 *		</code>
		 */
		virtual void DrawQuad(Vertex * vertexData) = 0;

		virtual void ApplyParameter(ParameterRef param) = 0;

		/**
		 * Helper function to handle binding and drawing a single effect. 
		 * This must take targets into consideration, and should contain 
		 * <em>all</em> code necessary to render a basic post-processing or
		 * deferred effect.<br />
		 * In some Adapter setups and APIs, this may involve blitting textures
		 * or handling various surfaces and RTT features. These should be
		 * implemented here to provide a clean 
		 */
		virtual void DrawShader(ShaderRef shader) = 0;

		/**
		 * Creates a named texture within the API and registers it with the 
		 * Core. The various texture parameters are specified here in an 
		 * API-independent form.
		 *
		 * @param name The name of the texture, usually a fully-qualified name.
		 * @param width The width of the texture in pixels.
		 * @param height The height of the texture in pixels.
		 * @param depth The depth of the texture in pixels.
		 * @param mipmaps Flag for mipmap generation or not.
		 * @param format The texture's format, must be a Voodoo supported format.
		 * @return A shared pointer to the created texture.
		 *
		 * @note Only Voodoo texture formats are supported, API-specific formats
		 *		are not.
		 * @note This must automatically register textures with the Adapter's 
		 *		associated Core.
		 * @note This must create a texture that can either be rendered directly
		 *		into or that can have the backbuffer copied into it quickly, 
		 *		depending on how the Adapter and API implement 
		 *		render-to-texture.
		 */
		virtual TextureRef CreateTexture(std::string name, size_t width, 
			size_t height, size_t depth, bool mipmaps, TextureFormat format) 
			= 0;

		/**
		 * Connects a texture to a sampler-type parameter. This is performed 
		 * differently in each API, but often uses Cg-provided functions (in 
		 * OpenGL, cgGLSetTextureParameter). The parameter and texture should 
		 * be connected for the shader's life.
		 * 
		 * @param param The parameter to bind to (must be a sampler type).
		 * @param texture The texture to be bound.
		 * @return Whether or not the binding was successful.
		 */
		virtual bool ConnectTexture(ParameterRef param, TextureRef texture) = 0;

		/**
		 * A generic error-handling callback provided to the Cg runtime. This 
		 * will be called in event of any Cg error.
		 *
		 * @param context The associated Cg context.
		 * @param error The error raised.
		 * @param core The currently active Voodoo Core.
		 */
		virtual void HandleError(CGcontext context, CGerror error, void * core) 
			= 0;
	};
}

#endif
