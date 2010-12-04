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

#include "Voodoo_GEM.hpp"

/**
* The core GEM wrapper class for D3D8 objects. This class is responsible for the primary abstraction
* from D3D8 to D3D9. This class is responsible for creating devices, giving it control over what is
* returned to the engine.   
* 
* @note The various IGEM3D8 interfaces provide an application-opaque wrapper that actually
* 		 implements a Direct3D 8.9 layer (8 to 9 translation). For use with D3D9 applications, the
* 		 IGEM3D9 interface set should be used.         
*/
class IGEM3DDevice8 
	: public IDirect3DDevice8
{
	/**
	* A pointer to the true underlying IDirect3DDevice9 object wrapped by this IGEM interface.            
	*/
	IDirect3DDevice9 * mRealDevice;
};
