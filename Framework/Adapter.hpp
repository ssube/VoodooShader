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
	 * Graphics adapter class, responsible for interfacing the Voodoo Core with
	 * a given graphics program. This class should be implemented for a specific
	 * graphics application; some generic adapters are provided, but in most cases
	 * application-specific behavior will make these only partially helpful. This
	 * class does include a generic error-handler function, which adapters may
	 * choose not to override if they do not need specific error-handling.
	 */
	class VOODOO_API Adapter
	{
	public:
		virtual bool LoadPass(PassRef pass);
		virtual void BindPass(PassRef pass);
		virtual void UnbindPass();

		virtual void DrawQuad(bool fullscreen = true, float * coords = NULL);

		virtual void ApplyParameter(ParameterRef param);

		virtual bool ConnectTexture(ParameterRef param, TextureRef texture);
		virtual TextureRef CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
			bool mipmaps, TextureFormat format);

		virtual void HandleError(CGcontext context, CGerror error, void * core);
	};
}

#endif
