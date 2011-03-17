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

#ifndef VOODOO_VERSION_HPP
#define VOODOO_VERSION_HPP

// Versioning functions -------------------------------------------------------
#define VOODOO_META__STRING(arg) #arg
#define VOODOO_META_STRING(arg) VOODOO_META__STRING(arg)

#ifdef _DEBUG
#    define VOODOO_META_DEBUG_STRING " (DEBUG)"
#else
#    define VOODOO_META_DEBUG_STRING
#endif

#define VOODOO_META_VERSION_CHAIN(token)\
    VOODOO_##token##_VERSION_MAJOR,\
    VOODOO_##token##_VERSION_MINOR,\
    VOODOO_##token##_VERSION_PATCH,\
    VOODOO_##token##_VERSION_GITREV

#define VOODOO_META_STRING_VERSION_BASIC(token)\
    VOODOO_META_STRING(VOODOO_##token##_VERSION_MAJOR) "."\
    VOODOO_META_STRING(VOODOO_##token##_VERSION_MINOR) "."\
    VOODOO_META_STRING(VOODOO_##token##_VERSION_PATCH) "."\
    VOODOO_META_STRING(VOODOO_##token##_VERSION_GITREV)\
    VOODOO_META_DEBUG_STRING

#define VOODOO_META_STRING_VERSION_FULL(token)\
    VOODOO_##token##_PRETTYNAME " (version " VOODOO_META_STRING_VERSION_BASIC(token) ")"

#define VOODOO_META_STRING_NAME_BASIC(token) VOODOO_##token##_NAME VOODOO_META_DEBUG_STRING
#define VOODOO_META_STRING_NAME_FULL(token) VOODOO_##token##_PRETTYNAME VOODOO_META_DEBUG_STRING

#define VOODOO_META_VERSION_STRUCT(token)\
    { VOODOO_##token##_NAME, VOODOO_META_VERSION_CHAIN(token) }
//-----------------------------------------------------------------------------


// Global defs ----------------------------------------------------------------
#define VOODOO_GLOBAL_VERSION_GITREV    111
#define VOODOO_GLOBAL_COPYRIGHT_BRIEF   "Copyright (c) 2010-2011 by Sean Sube"
#define VOODOO_GLOBAL_COPYRIGHT_FULL\
    "Voodoo Shader Framework, Copyright (c) 2010-2011 by Sean Sube&lt;br /&gt;\
    The Voodoo Shader Framework comes with ABSOLUTELY NO WARRANTY.&lt;br /&gt;\
    This is free software and you are welcome to redistribute it under certain conditions.&lt;br /&gt;\
    Please see the included license file for more details.&lt;/div&gt;"
//-----------------------------------------------------------------------------


// Core defs ------------------------------------------------------------------
#define VOODOO_CORE_NAME                "Voodoo/Core"
#define VOODOO_CORE_PRETTYNAME          "Voodoo Shader Framework Core"
#define VOODOO_CORE_VERSION_MAJOR       0
#define VOODOO_CORE_VERSION_MINOR       1
#define VOODOO_CORE_VERSION_PATCH       13
#define VOODOO_CORE_VERSION_GITREV      VOODOO_GLOBAL_VERSION_GITREV
//-----------------------------------------------------------------------------


// External defs --------------------------------------------------------------
#define VOODOO_CG_NAME                  "Cg"
#define VOODOO_CG_PRETTYNAME            "nVidia Cg (July 2010)"
#define VOODOO_CG_VERSION_MAJOR         3
#define VOODOO_CG_VERSION_MINOR         0
#define VOODOO_CG_VERSION_PATCH         0
#define VOODOO_CG_VERSION_GITREV        7   
//----------------------------------------------------------------------------- 

namespace VoodooShader
{
    /**
     * Describes the precise version of a particular library, including name, 
     * main version, revision and debug status.
     */
    struct Version
    {
        char * Name;
        int Major;
        int Minor;
        long Patch;
        long Rev;
        bool Debug;
    };
}

#endif /*VOODOO_VERSION_HPP*/