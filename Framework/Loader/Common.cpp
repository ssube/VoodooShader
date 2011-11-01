/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 */

#include "Loader_Common.hpp"

#include <vector>
#include <strsafe.h>

/**
 * @addtogroup VoodooLoader Voodoo/Loader @{ 
 */

VoodooShader::ICore * gVoodooCore = nullptr;
VoodooShader::IAdapter * gVoodooAdapter = nullptr;
VoodooShader::InitParams gInitParams;

void WINAPI ErrorMessage(const wchar_t * msg, ...)
{
    va_list args;
    va_start(args, msg);
    
    int bufsize = _vscwprintf(msg, args) + 1;
    std::vector<wchar_t> buffer(bufsize);

    _vsnwprintf_s(&buffer[0], bufsize, bufsize-1, msg, args);
    
    va_end(args);

    MessageBox(nullptr, &buffer[0], L"Voodoo Loader Error", MB_OK | MB_ICONWARNING);
}

/**
 * Load a module from the system directory (uses absolute path to guarantee proper file). 
 * 
 * @param libname The filename (and optionally extension) of the file to load. This may contain a path, but it will be 
 *     interpreted relative to the system directory. 
 * @return A handle to the module if loaded or a nullptr handle otherwise.
 */
HMODULE WINAPI LoadSystemLibrary(const wchar_t * libname)
{
    wchar_t path[MAX_PATH];

    GetSystemDirectory(path, MAX_PATH);
    StringCchCat(path, MAX_PATH, L"\\");
    StringCchCat(path, MAX_PATH, libname);

    return LoadLibrary(path);
}
/* @} */
