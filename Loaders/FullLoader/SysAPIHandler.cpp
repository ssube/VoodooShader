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

#include "SysAPIHandler.hpp"

#include <vector>
#include <tchar.h>
#include <strsafe.h>

/**
 * @addtogroup VoodooLoader Voodoo/Loader @{
 */
void WINAPI ErrorMessage(const LPTSTR msg, ...)
{
    va_list args;
    va_start(args, msg);

    int bufsize = _vsctprintf(msg, args) + 1;
    std::vector<TCHAR> buffer(bufsize);

    _vsntprintf_s(&buffer[0], bufsize, bufsize-1, msg, args);

    va_end(args);

    MessageBox(nullptr, &buffer[0], TEXT("Voodoo Loader Error"), MB_OK | MB_ICONWARNING);
}

/**
 * Load a module from the system directory (uses absolute path to guarantee proper file).
 *
 * @param libname The filename (and optionally extension) of the file to load. This may contain a path, but it will be
 *     interpreted relative to the system directory.
 * @return A handle to the module if loaded or a nullptr handle otherwise.
 */
HMODULE WINAPI LoadSystemLibrary(const LPTSTR libname)
{
    TCHAR path[MAX_PATH];

    GetSystemDirectory(path, MAX_PATH);
    StringCchCat(path, MAX_PATH, TEXT("\\"));
    StringCchCat(path, MAX_PATH, libname);

    return LoadLibrary(path);
}
/**
 * @}
 */
