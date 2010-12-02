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

#ifndef VOODOO_TEXTURE_HPP
#define VOODOO_TEXTURE_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	class VOODOO_API Texture
	{
	public:
		Texture(std::string name = "", void * texture = NULL);

		inline void * Get()
		{
			return mTextureObject;
		};

		inline std::string Name()
		{
			return mName;
		};

	private:
		std::string mName;
		void * mTextureObject;
	};
}

/**
 * @page texturesanddepth Texture Formats and Depth Buffers
 *
 * @section textureformats Texture Formats
 * <div>
 * For most of these formats, DirectX and OpenGL equivalents are indicated
 * below, along with a notation indicating if the texture may be used as a
 * render-target or copied to and from.
 *
 * <table>
 *	<tr><th>Voodoo Format</th>	<th>DirectX Format</th>		<th>OpenGL Format</th>	<th>RT/Copy</th>	</tr>
 *	<tr><td>Unknown</td>		<td>D3DFMT_UNKNOWN</td>		<td>(unknown)</td>		<td>No</td>			</tr>
 *	<tr><td>RGB5</td>			<td>D3DFMT_X1R5G5B5</td>	<td>GL_RGB5</td>		<td>Yes</td>		</tr>
 *	<tr><td>RGB5A1</td>			<td>D3DFMT_A1R5G5B5</td>	<td>GL_RGB5_A1</td>		<td>Yes</td>		</tr>
 *	<tr><td>RGB8</td>			<td>D3DFMT_X8R8G8B8</td>	<td>GL_RGB8</td>		<td>Yes</td>		</tr>
 *	<tr><td>RGB8A1</td>			<td>D3DFMT_A8R8G8B8</td>	<td>GL_RGB8_A1</td>		<td>Yes</td>		</tr>
 *	<tr><td>RGB10A2</td>		<td>D3DFMT_A2R10G10B10</td>	<td>GL_RGB10_A2</td>	<td>Yes</td>		</tr>
 *	<tr><td>RGBA16F<sup>1</sup></td><td>D3DFMT_A16B16G16R16F</td><td>GL_RGBA16F</td><td>N/Y</td>		</tr>
 *	<tr><td>RGBA32F<sup>1</sup></td><td>D3DFMT_A32B32G32R32F</td><td>GL_RGBA32F</td><td>N/Y</td>		</tr>
 *	<tr><td>D16F</td>			<td>D3DFMT_A2R10G10B10</td>	<td>GL_DEPTH_COMPONENT16</td><td>N/Y-ish<sup>2</sup></td></tr>
 *	<tr><td>D32F</td>			<td>D3DFMT_A32B32G32R32F</td><td>GL_DEPTH_COMPONENT32</td><td>N/Y-ish<sup>2</sup></td></tr>
 * </table>
 * <em>Note <sup>1</sup>:</em> Floating-point texture formats (RGBA16F and RGBA32F) may not be supported on all
 * hardware.<br />
 * <em>Note <sup>2</sup>:</em> Depth textures suffer additional restrictions when used with the DirectX API. See
 * @ref depthbuffers "the depth buffers section" for more information.
 * </div>
 *
 * @section depthbuffers Depth Buffers
 * <div>
 *	DirectX 9 applies some stupid restrictions to depth textures. Adapters targetting this API should 
 *	work around these in some way and provide the Core with a valid texture containing the depth data. Due to
 *	the restrictions, the recommended format for this texture is R32F (D3DFMT_R32F). They should then create
 *	a texture using Core::CreateTexture with this alternate texture, and copy data from the depth-buffer to the
 *	readable texture each frame.
 * </div>
 * <div>
 *	This can be complicated and is always a pain to do, so it may be beneficial to use the prebuilt solution 
 *	in the generic Voodoo DirectX 9 adapter. This solution has been set up to use a D24S8 stencil buffer on
 *	the underlying hardware, while abstracting it to a R32F texture for shader access.
 *  This solution has been derived from work done in the Morrowind Graphics Extender project, and would 
 *	probably not exist without the efforts of phal and a few others from the Bethesda Softworks forum 
 *	(give them a hand for making this easy for the rest of us).
 * </div>
 */

#endif /*VOODOO_TEXTURE_HPP*/
