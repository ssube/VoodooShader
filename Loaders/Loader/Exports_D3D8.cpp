/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */

#include "ExportLoadpoints.hpp"

#include "VoodooTie.hpp"

#include "Support.inl"

#include <stdlib.h>

#ifdef _DEBUG
#   define VOODOO_DEBUGBREAK DebugBreak()
#else
#   define VOODOO_DEBUGBREAK
#endif

/**
 * Direct3D 8
 */
typedef LPVOID (WINAPI * Type_Direct3DCreate8)(UINT);

HMODULE gModule_D3D8;
Type_Direct3DCreate8 gFunc_Direct3DCreate8;

LPVOID WINAPI VSDirect3DCreate8(UINT sdkVersion)
{
    VOODOO_DEBUGBREAK;

    if (!gFunc_Direct3DCreate8)
    {
        gFunc_Direct3DCreate8 = (Type_Direct3DCreate8)FindFunction(TEXT("d3d8.dll"), "Direct3DCreate8", &gModule_D3D8);
    }

    LPVOID pD3D8 = gFunc_Direct3DCreate8(sdkVersion);

    if (LoadVoodoo())
    {
        VoodooShader::Variant D3DObj;
        D3DObj.Type = VoodooShader::UT_PVoid;
        D3DObj.VPVoid = pD3D8;

        gVoodooCore->GetAdapter()->SetProperty(L"IDirect3D8", &D3DObj);

        if (D3DObj.VPVoid != pD3D8)
        {
            pD3D8 = D3DObj.VPVoid;
        }
    }

    return pD3D8;
}
