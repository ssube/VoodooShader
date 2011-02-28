/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (c) 2010-2011 by Sean Sube
*
*
* While the Voodoo Shader Framework, as a whole, is licensed under the GNU General Public license,
* adapters designed to work with the framework do not constitute derivative works. In the case an
* adapter needs to use this file (Adapter.hpp) or Meta.hpp in order to work with the Framework, and
* is not licensed under the GNU GPL or a compatible license, please contact the developer to figure
* out alternate licensing arrangements.
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
	 *
	 * @note This vertex format provides a float3 position and float2 texture
	 *		coordinate. For compatibility with Direct3D, a RHW value is also
	 *		included (the vertex format is D3DFVF_XYZRHW|D3DFVF_TEX1). OpenGL
	 *		adapters may ignore this winding value. The members of the vert are
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
	 * some generic adapters are provided, but in most cases 
	 * application-specific behavior will make these only partially helpful. 
	 * This class does include a generic error-handler function, which adapters 
	 * may choose not to override if they do not need specific error-handling.
	 */
	class VOODOO_API Adapter
	{
	public:
		/**
		 * Retrieves the adapter module version. This can be used to verify
		 * support between modules or versions, or from plugins.
		 *
		 * @return The adapter version.
		 */
		virtual Version GetVersion() = 0;

		/**
		 * Loads a program in a manner that is compatible with the Adapter's
		 * underlying API. For Cg-supported APIs, this uses the appropriate 
		 * module loading function (cgD3D9LoadProgram, cgGLLoadProgram, etc).
		 * This may involve additional compilation or linking.
		 *
		 * @param pass The pass to be loaded (not a shared pointer, it is 
		 *		assumed the pass will be valid for the duration of the call).
		 * @return Whether or not the pass was successfully loaded.
		 */
		virtual bool LoadPass
        (
            __in Pass * pass
        ) = 0;

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
		virtual void BindPass
        (
            __in PassRef pass
        ) = 0;

		/**
		 * Unbinds the last bound pass. This resets the shader state of the 
		 * underlying graphics API to the same state as before the pass was 
		 * bound. 
		 *
		 * @note Adapters must restore all states modified by an 
		 *		Adapter::BindPass() call. They should cache the data, but the 
		 *		exact implementation is left to the Adapter.
		 *
		 * @note If Adapter::BindPass() is called, render states are likely to
		 *		change. These are unlikely to be reset until 
		 *		Adapter::UnbindPass() is called.
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
		 * @note The quad draw operation must meet the following requirements:
		 *		 <ol>
		 *			<li>Depth and stencil buffers must not be written to.</li>
		 *			<li>Alpha testing, culling, depth testing and other states 
		 *				that could cull	portions of the quad must be disabled
		 *				(the whole quad must be drawn).</li>
		 *			<li>Alpha-blending should be disabled entirely at this time,
		 *				in future versions blending may be used during some
		 *				passes.</li>
		 *			<li>Any render states modified must be restored to their 
		 *				original state before the function returns.</li>
		 *		 </ol>
		 */
		virtual void DrawQuad
        (
            __in __notnull Vertex * vertexData
        ) = 0;

		/**
		 * Downloads a parameter's value from system RAM to VRAM, verifying that
		 * the value on the GPU is the latest value set in the CPU memory buffer
		 * (all parameter set commands operate on the system RAM buffer for
		 * speed).
		 */
		virtual void ApplyParameter
        (
            __in ParameterRef param
        ) = 0;

		/**
		 * Helper function to handle binding and drawing a single effect. 
		 * This must take targets into consideration, and should contain 
		 * <em>all</em> code necessary to render a basic post-processing or
		 * deferred effect.<br />
		 * In some Adapter setups and APIs, this may involve blitting textures
		 * or handling various surfaces and RTT features. These should be
		 * implemented here to provide a clean interface for basic shader
		 * drawing.<br />
		 * This function should assume that a fullscreen/postprocessing
		 * effect is being drawn and use the default vertex setup (done by
		 * calling <code>this->DrawQuad(NULL);</code>).
		 */
		virtual void DrawShader
        (
            __in ShaderRef shader
        ) = 0;

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
		virtual TextureRef CreateTexture
        (
            __in String name, 
            __in size_t width, 
			__in size_t height, 
            __in size_t depth, 
            __in_opt bool mipmaps, 
            __in TextureFormat format
        ) = 0;

		/**
		 * Connects a texture to a sampler-type parameter. This is performed 
		 * differently in each API, but often uses Cg-provided functions (in 
		 * OpenGL, cgGLSetTextureParameter). The parameter and texture should 
		 * be connected for the duration of the shader's life.
		 * 
		 * @param param The parameter to bind to (must be a sampler type).
		 * @param texture The texture to be bound.
		 * @return Whether or not the binding was successful.
		 */
		virtual bool ConnectTexture
        (
            __in ParameterRef param, 
            __in TextureRef texture
        ) = 0;

		/**
		 * A generic error-handling callback provided to the Cg runtime. This 
		 * will be called in event of any Cg error.
		 *
		 * @param context The associated Cg context.
		 * @param error The error raised.
		 * @param core The currently active Voodoo Core.
		 */
		virtual void HandleError
        (
            __in CGcontext context, 
            __in CGerror error, 
            __in __notnull void * core
        ) = 0;
	};
}

#endif
