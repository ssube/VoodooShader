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
 **/

#pragma once

/**
 * Versioning functions
 */
#define VOODOO_META__STRING(arg) #arg
#define VOODOO_META_STRING(arg)  VOODOO_META__STRING(arg)
#ifdef _DEBUG
#define VOODOO_META_DEBUG_BOOL  true
#define VOODOO_META_DEBUG_STRING " (DEBUG)"
#else
#define VOODOO_META_DEBUG_BOOL false
#define VOODOO_META_DEBUG_STRING
#endif

/**
 * Creates a comma-separated chain of version tokens (not string).
 */
#define VOODOO_META_VERSION_CHAIN(token) \
    VOODOO_##token##_VERSION_MAJOR, VOODOO_##token##_VERSION_MINOR, VOODOO_##token##_VERSION_PATCH, \
    VOODOO_##token##_VERSION_REV

/**
 * Creates a basic version string separated with periods.
 */
#define VOODOO_META_STRING_VERSION_BASIC(token) \
    VOODOO_META_STRING(VOODOO_##token##_VERSION_MAJOR) "."\
    VOODOO_META_STRING(VOODOO_##token##_VERSION_MINOR) "."\
    VOODOO_META_STRING(VOODOO_##token##_VERSION_PATCH) "."\
    VOODOO_META_STRING(VOODOO_##token##_VERSION_REV) VOODOO_META_DEBUG_STRING

/**
 * Creates an extended version string with the module's full name and version.
 */
#define VOODOO_META_STRING_VERSION_FULL(token) \
    VOODOO_##token##_PRETTYNAME " (version "VOODOO_META_STRING_VERSION_BASIC(token) ")"

#define VOODOO_META_STRING_NAME_BASIC(token) VOODOO_##token##_NAME VOODOO_META_DEBUG_STRING
#define VOODOO_META_STRING_NAME_FULL(token)  VOODOO_##token##_PRETTYNAME VOODOO_META_DEBUG_STRING

/**
 * Creates a Version structure with data for the given module.
 */
#define VOODOO_META_VERSION_STRUCT(token) { VOODOO_##token##_NAME, VOODOO_META_VERSION_CHAIN(token), VOODOO_META_DEBUG_BOOL }

// Global defs
#define VOODOO_GLOBAL_NAME              "Voodoo/Framework"
#define VOODOO_GLOBAL_PRETTYNAME        "Voodoo Shader Framework"
#define VOODOO_GLOBAL_VERSION_MAJOR     0
#define VOODOO_GLOBAL_VERSION_MINOR     4
#define VOODOO_GLOBAL_VERSION_PATCH     0
#define VOODOO_GLOBAL_VERSION_REV       247
#define VOODOO_GLOBAL_COPYRIGHT_BRIEF   "Copyright (c) 2010-2011 by Sean Sube"
#define VOODOO_GLOBAL_COPYRIGHT_FULL    "Voodoo Shader Framework, Copyright (c) 2010-2011 by Sean Sube.&lt;br /&gt;\n" \
    "The Voodoo Shader Framework comes with ABSOLUTELY NO WARRANTY.&lt;br /&gt;\n" \
    "This is free software and you are welcome to redistribute it under certain conditions.&lt;br /&gt;\n" \
    "Please see the included license file for more details."

// Core defs
#define VOODOO_CORE_NAME                "Voodoo/Core"
#define VOODOO_CORE_PRETTYNAME          "Voodoo Shader Framework Core"
#define VOODOO_CORE_VERSION_MAJOR       VOODOO_GLOBAL_VERSION_MAJOR
#define VOODOO_CORE_VERSION_MINOR       VOODOO_GLOBAL_VERSION_MINOR
#define VOODOO_CORE_VERSION_PATCH       VOODOO_GLOBAL_VERSION_PATCH
#define VOODOO_CORE_VERSION_REV         VOODOO_GLOBAL_VERSION_REV

// External defs Cg
#define VOODOO_CG_NAME                  "Cg"
#define VOODOO_CG_PRETTYNAME            "nVidia Cg Toolkit (July 2010)"
#define VOODOO_CG_VERSION_MAJOR         3
#define VOODOO_CG_VERSION_MINOR         0
#define VOODOO_CG_VERSION_PATCH         0
#define VOODOO_CG_VERSION_REV           7

// VC
#define VOODOO_VC_NAME                  "MSVC"
#define VOODOO_VC_PRETTYNAME            "Microsoft Visual C++"
#define VOODOO_VC_VERSION_MAJOR         _MSC_VER
#define VOODOO_VC_VERSION_MINOR         0
#define VOODOO_VC_VERSION_PATCH         _MSC_FULL_VER
#define VOODOO_VC_VERSION_REV           _MSC_BUILD
