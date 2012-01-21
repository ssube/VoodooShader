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
#pragma once

#define VOODOO_IMPORT
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        class CVoodoo3D8;
        class CVoodoo3DDevice8;
        class CVoodoo3DTexture8;
        class CVoodoo3DSurface8;
    }

    namespace VoodooDX9
    {
        class CVoodoo3D9;
        class CVoodoo3DDevice9;
    }
}

extern VoodooShader::ICoreRef gpVoodooCore;
extern VoodooShader::ILoggerRef gpVoodooLogger;

const VoodooShader::Version * VOODOO_CALLTYPE API_PluginInit(_In_ VoodooShader::ICore * pCore);
const uint32_t VOODOO_CALLTYPE API_ClassCount();
const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * refid);
VoodooShader::IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore);

bool WINAPI InstallDllHook(LPTSTR name, LPCSTR symbol, LPVOID pDest);
int WINAPI InstallKnownHooks();
