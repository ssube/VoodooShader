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

#ifndef VOODOO_META_HPP
#define VOODOO_META_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#import <msxml6.dll>

#include "Cg/cg.h"

#include "Common.hpp"
#include "Version.hpp"
#include "Voodoo.hpp"

[module(dll, name="CoodooCore", helpstring="Voodoo Shader Framework Core Library")];
[emitidl];

namespace VoodooShader
{
    DEFINE_GUID(CLSID_Core,         0xC4693A1C, 0xEDC6, 0x4794, 0x93, 0x0C, 0x25, 0x74, 0x97, 0xFC, 0x87, 0x22);
    DEFINE_GUID(CLSID_Parser,       0x4261CE17, 0xF55D, 0x4BBE, 0x80, 0xAD, 0x43, 0x9A, 0xAB, 0x15, 0x7E, 0x3C);
    DEFINE_GUID(CLSID_Parameter,    0xF530DD21, 0x9A1A, 0x4F25, 0x89, 0xB9, 0x46, 0xE9, 0x76, 0xEC, 0xCD, 0x64);
    DEFINE_GUID(CLSID_Shader,       0xFC7FA3C9, 0xDCB3, 0x47FC, 0x96, 0x45, 0x80, 0xF4, 0xA9, 0x6D, 0x2E, 0x51);
    DEFINE_GUID(CLSID_Texture,      0xCA2AE4FC, 0x136F, 0x426C, 0x9C, 0x92, 0xFB, 0x66, 0x5D, 0x21, 0x60, 0x26);


#define _BSTR(x) SysAllocString(L##x)

    // Creates an interface to a string-format class ID. The ID may be in registry form or a ProgID.
    HRESULT WINAPI InstanceFromString(_In_ BSTR lpStr, _In_ REFIID iid, _In_ void ** pp)
    {
        // Error check
        if ( lpStr == NULL || pp == NULL )
        {
            return E_INVALIDARG;
        }

        CLSID clsid;
        // Check string type
        if ( lpStr[0] == '{' )
        {
            // Registry-format string.
            HRESULT getHR = CLSIDFromString(lpStr, &clsid);
            if ( FAILED(getHR) )
            {
                return getHR;
            }
        } else {
            // Assume ProgID
            HRESULT getHR = CLSIDFromProgID(lpStr, &clsid);
            if ( FAILED(getHR) )
            {
                return getHR;
            }
        }

        return CoCreateInstance(clsid, NULL, NULL, iid, pp);
    }
}

#endif /*VOODOO_META_HPP*/
