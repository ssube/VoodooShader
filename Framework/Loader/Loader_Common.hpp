/**
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
#pragma once

#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define VOODOO_NO_BOOST
#define VOODOO_NO_CG
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

/** 
 * @addtogroup voodoo_module_loader @{ 
 */
void WINAPI ErrorMessage(const wchar_t * msg, ...);

HMODULE WINAPI LoadSystemLibrary(const wchar_t * libname);

extern VoodooShader::ICore * gVoodooCore;
extern VoodooShader::IAdapter * gVoodooAdapter;
extern VoodooShader::InitParams gInitParams;

/** 
 * @} 
 */
