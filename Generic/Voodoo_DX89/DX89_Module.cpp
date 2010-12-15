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

#include "DX89_Module.hpp"

#include "IVoodoo3D8.hpp"
#include "IVoodoo3DDevice8.hpp"
#include "IVoodoo3DSurface8.hpp"
#include "IVoodoo3DTexture8.hpp"

VoodooShader::Core * VoodooCore = NULL;
D3DCAPS8 d3d8Caps;

IVoodoo3D8 * VoodooObject = NULL;
IVoodoo3DDevice8 * VoodooDevice = NULL;

VOODOO_API_DX89 void * __stdcall Direct3DCreate8(UINT version)
{
	// Voodoo DX8.9 Init function
	VoodooCore = VoodooShader::Core::Create("Voodoo_GEM.log");

#ifdef _DEBUG
	VoodooCore->GetLog()->SetBufferSize(0);
#endif

	VoodooCore->GetLog()->Format("Voodoo GEM: Direct3DCreate8 called, SDK version: %d.\n").With(version).Done();

	//Load the real d3d8 dll and get device caps
	char Path[MAX_PATH];
	GetSystemDirectoryA (Path, MAX_PATH);
	strcat_s (Path, MAX_PATH, "\\d3d8.dll");

	HMODULE d3ddll = LoadLibraryA(Path);
	D3DFunc8 d3d8func = (D3DFunc8)GetProcAddress (d3ddll, "Direct3DCreate8");

	if (d3d8func == NULL) 
	{
		VoodooCore->GetLog()->Log("Voodoo DX8.9: Could not find D3D8 create true func.\n");
		return 0;
	}

	IDirect3D8 * TempObject = (d3d8func)(version);
	HRESULT hr = TempObject->GetDeviceCaps (0, D3DDEVTYPE_HAL, &d3d8Caps);
	if (hr != D3D_OK) 
	{ 
		VoodooCore->GetLog()->Log("Voodoo DX8.9: Could not get D3D8 caps.\n");
	}
	TempObject->Release();

	FreeLibrary(d3ddll);

	// Call DX9 to create a real device with the latest version
	IDirect3D9 * object = Direct3DCreate9(D3D_SDK_VERSION);
	// Turn it into a FakeObject and return it.
	IVoodoo3D8 * vObj = new IVoodoo3D8(object);
	VoodooObject = vObj;
	return vObj;
}

// Visual Studio-specific linker directive, forces the function to be exported with and
// without decoration. The actual symbol is undecorated, but I'd rather allow exception
// handling than use extern "C".
#pragma comment(linker, "/export:Direct3DCreate8=?Direct3DCreate8@@YGPAXI@Z")

//=============================================================================

