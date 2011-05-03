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

#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <atlstr.h>

#include "VoodooFramework.hpp"

/**
 * @addtogroup VoodooLoader Voodoo/Loader
 * @{
 */

void WINAPI ErrorMessage(LPTSTR pMsg, ...);

VARIANT * PtrVariant(void * ptr);
HMODULE LoadSystemLibrary(const PTCHAR libname);

bool WINAPI VoodooStartup();
bool WINAPI VoodooShutdown();

bool WINAPI GetLoader(HINSTANCE hLoader);
bool WINAPI GetRunRoot();
bool WINAPI GetLocalRoot();
bool WINAPI GetGlobalRoot();

extern IVoodooCore * gVoodooCore;
extern IVoodooAdapter * gVoodooAdapter;
extern InitParams gInitParams;

/**
 * @}
 */