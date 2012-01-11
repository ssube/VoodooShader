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

#include "DX9_Module.hpp"

#include "DX9_Adapter.hpp"
#include "DX9_Version.hpp"

VoodooShader::ICoreRef gpVoodooCore = nullptr;
VoodooShader::ILoggerRef gpVoodooLogger = nullptr;

VoodooShader::IEffect * testEffect = nullptr;

IDirect3DVertexBuffer9 * gpFSQuadVerts = nullptr;
IDirect3DSurface9 *backbufferSurf = nullptr;

IDirect3DSurface9 *surface_Frame0 = nullptr;
VoodooShader::ITexture* texture_Frame0 = nullptr;
IDirect3DSurface9 *surface_Pass0 = nullptr;
VoodooShader::ITexture* texture_Pass0 = nullptr;
IDirect3DSurface9 * surface_Scratch = nullptr;
VoodooShader::ITexture* texture_Scratch = nullptr;

volatile LONG gObjectLock = 0;

const VoodooShader::Version * VOODOO_CALLTYPE API_ModuleVersion()
{
    static const VoodooShader::Version dx9version = VOODOO_VERSION_STRUCT(DX89);
    return &dx9version;
}

const uint32_t VOODOO_CALLTYPE API_ClassCount()
{
    return 1;
}

const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * refid)
{
    if (!refid)
    {
        return nullptr;
    }

    if (index == 0)
    {
        *refid = VoodooShader::VoodooDX9::CLSID_DX9Adapter;
        return L"DX9Adapter";
    }
    else
    {
        return nullptr;
    }
}

VoodooShader::IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore)
{
    if (index == 0)
    {
        return new VoodooShader::VoodooDX9::DX9Adapter(pCore);
    }
    else
    {
        return nullptr;
    }
}
