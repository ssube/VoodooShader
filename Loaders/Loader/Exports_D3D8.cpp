/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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
        gFunc_Direct3DCreate8 = (Type_Direct3DCreate8)FindFunction(TEXT("d3d8.dll"), true, "Direct3DCreate8", &gModule_D3D8);
    }

    LPVOID pD3D8 = gFunc_Direct3DCreate8(sdkVersion);
    
    if (!pD3D8)
    {
        return pD3D8;
    }

    if (InterlockedCompareExchange(&gSingleExport, 1, 0) == 0)
    {
        GlobalLog(VSTR("Loading Voodoo Shader from %s."), VSTR(__FUNCTION__));
        if (LoadVoodoo())
        {
            VoodooShader::Variant D3DObj;
            ZeroMemory(&D3DObj, sizeof(VoodooShader::Variant));
            D3DObj.Type = VoodooShader::UT_UInt32;
            D3DObj.Components = 1;
            D3DObj.VUInt32.X = sdkVersion;

            gVoodooCore->GetAdapter()->SetProperty(VSTR("SdkVersion"), &D3DObj);

            ZeroMemory(&D3DObj, sizeof(VoodooShader::Variant));
            D3DObj.Type = VoodooShader::UT_PVoid;
            D3DObj.Components = 1;
            D3DObj.VPVoid = pD3D8;

            if (gVoodooCore->GetAdapter()->SetProperty(L"IDirect3D8", &D3DObj))
            {
                pD3D8 = D3DObj.VPVoid;
            }
        }

        gSingleExport = false;
    }

    return pD3D8;
}
