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

#define VOODOO_NO_BOOST
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

/**
 * @addtogroup voodoo_module_loader Voodoo/Loader
 * @{
 */
extern VoodooShader::ICore * gVoodooCore;

extern HINSTANCE gLoaderHandle;

const VoodooShader::Version *   VOODOO_CALLTYPE API_ModuleVersion();
const uint32_t                  VOODOO_CALLTYPE API_ClassCount();
const wchar_t *                 VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * pUuid);
VoodooShader::IObject *         VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore);

bool WINAPI LoadVoodoo();
bool WINAPI UnloadVoodoo();
/**
 * @}
 */
