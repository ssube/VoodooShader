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
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

#include <afxtempl.h>
#import <msxml6.dll>

#include "Cg/cg.h"

#include "Version.hpp"
#include "Voodoo.hpp"

[module(dll, name="CoodooCore", helpstring="Voodoo Shader Framework Core Library")];
[emitidl];

namespace VoodooShader
{
    DEFINE_GUID(CLSID_Core,         0xC4693A1C, 0xEDC6, 0x4794, 0x93, 0x0C, 0x25, 0x74, 0x97, 0xFC, 0x87, 0x22);
    DEFINE_GUID(CLSID_Pass,         0xcff48724, 0x61dc, 0x4390, 0xbb, 0x19, 0x25, 0xf7, 0x73, 0x01, 0xd7, 0x39);
    DEFINE_GUID(CLSID_Parser,       0x4261CE17, 0xF55D, 0x4BBE, 0x80, 0xAD, 0x43, 0x9A, 0xAB, 0x15, 0x7E, 0x3C);
    DEFINE_GUID(CLSID_Sampler,      0xCB669091, 0x84B2, 0x4B72, 0x85, 0xA1, 0x41, 0x28, 0xE0, 0x0F, 0xA9, 0x56);
    DEFINE_GUID(CLSID_Scalar,       0xF530DD21, 0x9A1A, 0x4F25, 0x89, 0xB9, 0x46, 0xE9, 0x76, 0xEC, 0xCD, 0x64);
    DEFINE_GUID(CLSID_Shader,       0xFC7FA3C9, 0xDCB3, 0x47FC, 0x96, 0x45, 0x80, 0xF4, 0xA9, 0x6D, 0x2E, 0x51);
    DEFINE_GUID(CLSID_Technique,    0x60674c0b, 0x29e3, 0x4344, 0x85, 0xe8, 0xea, 0x9c, 0x50, 0x50, 0x67, 0x95);
    DEFINE_GUID(CLSID_Texture,      0xCA2AE4FC, 0x136F, 0x426C, 0x9C, 0x92, 0xFB, 0x66, 0x5D, 0x21, 0x60, 0x26);

    class Core;
    class Parser;
    class Pass;
    class Sampler;
    class Scalar;
    class Shader;
    class Technique;
    class Texture;

    // Creates an interface to a string-format class ID. The ID may be in registry form or a ProgID.
    HRESULT WINAPI InstanceFromString(_In_ BSTR lpStr, _In_ REFIID iid, _In_ void ** pp);
    CGparameter WINAPI CreateVirtualParameter(IVoodooCore * pCore, ParameterType Type);
}
