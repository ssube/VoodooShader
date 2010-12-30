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

#ifndef VOODOO_CORE_HPP
#define VOODOO_CORE_HPP

#include "Meta.hpp"

#include "Exception.hpp"
#include "Logger.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Parameter.hpp"

namespace VoodooShader
{
	/**
	 * Core engine class for the Voodoo Shader Framework. Provides a flexible, 
	 * uniform API for a complete shader framework. Using various @ref Adapter 
	 * "adapters", it is possible to tie the Voodoo Core into almost any game or
	 * program, during or after development.
	 */
	class VOODOO_API Core
	{
	public:
		static Core * Create(std::string logfile = "Voodoo.log");
		static void Destroy(Core * core);

		/**
		 * Retrieve the Cg context associated with this Core.
		 * 
		 * @note Each Voodoo Core is associated with exactly one Cg context 
		 *		(Voodoo acts as an OO-wrapper around Cg with enhanced features).
		 */
		CGcontext GetCGContext();

		/**
		 * Retrieve the logger object associated with this core.    
		 * 
		 * @note If this Core is valid, this will always return a valid Logger 
		 *		attached to an open log file. If it doesn't, you have bigger 
		 *		problems to worry about than not logging.        
		 */
		Logger * GetLog();

		/**
		 * Attach an Adapter to this Core.
		 *
		 * @param adapter The Adapter to attach (will be used for all graphics 
		 *		calls).
		 * @throws Exception if an Adapter is already connected to this Core.
		 */
		void SetAdapter(Adapter * adapter);

		/**
		 * Retrieve the Adapter attached to this Core.
		 *
		 * @return A pointer to the Adapter or NULL if no Adapter is attached.
		 */
		Adapter * GetAdapter();

		/**
		 * Create a new shader effect from a file.            
		 */
		ShaderRef CreateShader(std::string filename, const char ** args);

		/**
		 * Registers a texture with this Core. Texture will not be used by the 
		 * shader linker unless they have been registered with the core. 
		 *
		 * @param name The texture name (must be unique).
		 * @param data A pointer to any texture data to be stored. This should 
		 *		be interpreted and manipulated by the Adapter, the Core merely 
		 *		stores the pointer.
		 * @return A shared pointer to the newly created Texture object, if 
		 *		successful.
		 * 
		 * @throws Exception if a texture with the same name already exists.
		 *
		 * @warning This should be called only from Adapter::CreateTexture().
		 */
		TextureRef CreateTexture(std::string name, void * data);

		/**
		 * Retrieves a texture from the Core's texture map by name. 
		 * 
		 * @param name The texture name.
		 * @return A shared pointer to the Texture object, if it exists 
		 *		(otherwise an empty	shared pointer).
		 *
		 * @note This will not create a texture, only retrieve an existing one.
		 */
		TextureRef GetTexture(std::string name);

		/**
		 * Removes a texture from the Core's texture map. 
		 */
		void RemoveTexture(TextureRef texture);

		/**
		 * Creates a global-level or virtual parameter. This parameter exists
		 * in the Cg runtime, but is not a part of any shader or program. This
		 * is useful for creating parameters that must be shared between 
		 * programs. Only parameters created with this function may be used in 
		 * Parameter::Link.
		 *
		 * @param name The name for this parameter.
		 * @param type True for texture, false for float4
		 * @todo Fix this to use a real parameter type parameter.
		 * @return A new parameter.
		 * @throws Exception if a parameter with the given name already exists.
		 */
		ParameterRef CreateParameter(std::string name, ParameterType type);
		ParameterRef Core::GetParameter(std::string name, ParameterType type);

	//protected:
		static void CGErrorHandler(CGcontext context, CGerror error, void * core);

	private:

		/**
		* Create a new Voodoo Core and associated Cg context.
		* 		 
		* @param logfile The filename for this Core's log (will be used by most 
		*		managers and connected adapters).
		* @return A reference to the new core.
		*
		* @note Avoid using more than one core at any point in time. I'm not 
		*		entirely sure how well the Cg runtime handles this. For most 
		*		games, a single render context is used,	so no more than one 
		*		adapter and core should be necessary.
		* @todo Test multi-core/multi-adapter systems. If anyone has info or 
		*		knows of a game/app	that uses multiple D3D/OGL render contexts, 
		*		please let me know.
		* @note You can not call this function externally, you must call 
		*		Core::Create(std::string) instead.
		*/
		Core(std::string logfile = "Voodoo.log");

		/**
		 * Default destructor for Voodoo @ref Core "Cores".
		 */
		~Core();

		Adapter * mAdapter;
		Logger * mLogger;

		// Manager objects:
		FullscreenManager *	mManagerFS;
		MaterialManager   *	mManagerMat;

		// Cg objects:
		CGcontext mCGContext;

		ShaderMap mShaders;
		TextureMap mTextures;
		ParameterMap mParameters;
	};
}

#endif
