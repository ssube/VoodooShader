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

#include "Frost_Module.hpp"

/**
 * Applies a variation of the popular NWN camera hack to the engine in-memory.
 * This technique is simpler to manipulate and customize than the traditional byte-patcher. 
 * 
 * @todo Update this to use customizable settings when they're implemented.
 */
void CameraHack()
{
	VoodooCore->GetLog()->Log("Voodoo Frost: Applying camera hack...\n");

	float maxDist	=  120.0f;
	float minDist	=   -0.2f;
	float angle		= 1000.0f;

	DWORD oldProtect, finalProtect;

	BOOL lockStatus = VirtualProtect( (PVOID)0x004A9000, 0x1000, PAGE_EXECUTE_READWRITE, &oldProtect);

	if ( lockStatus != 0 )
	{
		memcpy(	(PVOID)	0x004A93ED,	&minDist,	sizeof(float) );
		memcpy( (PVOID)	0x004A93F7,	&maxDist,	sizeof(float) );
		memcpy(	(PVOID)	0x004A940B,	&angle,		sizeof(float) );
		memcpy( (PVOID)	0x004A9695,	&maxDist,	sizeof(float) );
		memcpy(	(PVOID)	0x004A968B,	&minDist,	sizeof(float) );

		VirtualProtect( (PVOID)0x004A9000, 0x1000, oldProtect, &finalProtect);

		VoodooCore->GetLog()->Log("Voodoo Frost: Camera hack successfully applied.\n");
	} else {
		DWORD error = GetLastError();
		
		VoodooCore->GetLog()->Format("Voodoo Frost: Camera hack failed with code %d.\n")
			.With(error).Done();
	}
}
