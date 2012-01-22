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

#if !defined(VOODOO_STRING_MACROS)
#   define VOODOO_STRING_MACROS
#   ifdef _UNICODE
#       define VOODOO_STRING_ARG(arg)  L ## #arg
#       define VOODOO_STRING_STR(arg)  L ## arg
#   else
#       define VOODOO_STRING_ARG(arg)  #arg
#       define VOODOO_STRING_STR(arg)  arg
#   endif
#   define VOODOO_TOSTRING(arg)    VOODOO_STRING_ARG(arg)
#   define VOODOO_STRING(arg)      VOODOO_STRING_STR(arg)
#   define VSTR(arg)               VOODOO_STRING_STR(arg)
#endif

#if defined(_DEBUG)
#   define VOODOO_DEBUG_BOOL   true
#   define VOODOO_DEBUG_STRING VSTR(" (DEBUG)")
#else
#   define VOODOO_DEBUG_BOOL   false
#   define VOODOO_DEBUG_STRING
#endif

#define VOODOO_STRING_NAME_INTERNAL        VOODOO_TOSTRING(BUILD_PROJECTNAME)
#define VOODOO_STRING_NAME_TARGET          VOODOO_TOSTRING(BUILD_FILENAME)

#define VOODOO_STRING_NAME_BASIC(token)    VOODOO_##token##_NAME VOODOO_DEBUG_STRING
#define VOODOO_STRING_NAME_FULL(token)     VOODOO_##token##_PRETTYNAME VOODOO_DEBUG_STRING
#define VOODOO_STRING_AUTHOR(token)        VOODOO_##token##_AUTHOR
#define VOODOO_STRING_VERSION_BASIC(token) \
    VOODOO_TOSTRING(VOODOO_##token##_VERSION_MAJOR) VSTR(".")\
    VOODOO_TOSTRING(VOODOO_##token##_VERSION_MINOR) VSTR(".")\
    VOODOO_TOSTRING(VOODOO_##token##_VERSION_PATCH) VSTR(".")\
    VOODOO_TOSTRING(VOODOO_##token##_VERSION_BUILD) VOODOO_DEBUG_STRING
#define VOODOO_STRING_VERSION_FULL(token) VOODOO_##token##_VERSION_ID VOODOO_DEBUG_STRING
#define VOODOO_VERSION_CHAIN(token) \
    VOODOO_##token##_VERSION_MAJOR, VOODOO_##token##_VERSION_MINOR, VOODOO_##token##_VERSION_PATCH, \
    VOODOO_##token##_VERSION_BUILD
#define VOODOO_VERSION_STRUCT(token) \
    { VOODOO_##token##_LIBID, VOODOO_VERSION_CHAIN(token), VOODOO_DEBUG_BOOL, \
      VOODOO_##token##_NAME, VOODOO_##token##_VERSION_ID }

// Copyright info
#define VOODOO_GLOBAL_COPYRIGHT_BRIEF   VSTR("Copyright © 2010-2012 by Sean Sube")
#define VOODOO_GLOBAL_COPYRIGHT_FULL    VSTR("Voodoo Shader Framework, Copyright &copy; 2010-2012 by Sean Sube.\n") \
    VSTR("The Voodoo Shader Framework comes with ABSOLUTELY NO WARRANTY.\n") \
    VSTR("This is free software and you are welcome to redistribute it under certain conditions.\n") \
    VSTR("Please see the included license file for more details.")

// Global defs
#define VOODOO_GLOBAL_LIBID             {0xCA, 0xCF, 0x87, 0xA6, 0x3C, 0x06, 0xE1, 0x11, 0xB2, 0x2E, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}
#define VOODOO_GLOBAL_AUTHOR            VSTR("Sean Sube, et al.")
#define VOODOO_GLOBAL_NAME              VSTR("VoodooShader")
#define VOODOO_GLOBAL_PRETTYNAME        VSTR("Voodoo Shader")
#define VOODOO_GLOBAL_VERSION_MAJOR     0
#define VOODOO_GLOBAL_VERSION_MINOR     5
#define VOODOO_GLOBAL_VERSION_PATCH     8
#include "BuildVersion.hpp"

// Core defs
#define VOODOO_CORE_LIBID               {0xCB, 0xCF, 0x87, 0xA6, 0x3C, 0x06, 0xE1, 0x11, 0xB2, 0x2E, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}
#define VOODOO_CORE_AUTHOR              VOODOO_GLOBAL_AUTHOR
#define VOODOO_CORE_NAME                VSTR("Voodoo_Core")
#define VOODOO_CORE_PRETTYNAME          VSTR("Voodoo Shader Framework Core")
#define VOODOO_CORE_VERSION_MAJOR       VOODOO_GLOBAL_VERSION_MAJOR
#define VOODOO_CORE_VERSION_MINOR       VOODOO_GLOBAL_VERSION_MINOR
#define VOODOO_CORE_VERSION_PATCH       VOODOO_GLOBAL_VERSION_PATCH
#define VOODOO_CORE_VERSION_BUILD       VOODOO_GLOBAL_VERSION_BUILD
#define VOODOO_CORE_VERSION_ID          VOODOO_GLOBAL_VERSION_ID
