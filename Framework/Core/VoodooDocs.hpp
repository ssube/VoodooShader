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

/**
 * @file
 * This file serves to keep documentation out of the primary headers, providing a cleaner appearance to users of the
 * library without sacrificing doc quality. All macros and other content in VoodooFramework.hpp is documented here.
 *
 * @file VoodooFramework.hpp
 * This file is the public-facing include for the Voodoo Shader core. All libraries using the Voodoo Shader Framework should
 * include this file, and optionally `Support.inl`; this file includes all other headers necessary to use Voodoo
 * Shader.
 * 
 * @defgroup voodoo_macros Macros
 * @{
 * 
 * @def VOODOO_SDK_VERSION
 * Current core version. This rarely changes; it is only incremented when breaking changes occur within the core. Anything
 * creating a core should use this instead of providing a static number to @ref CreateCore.
 *
 * @def VOODOO_STRING_ARG(arg)
 * Turns the text given as @a arg into a string.
 * 
 * @def VOODOO_STRING_STR(arg)
 * Prepends the appropriate qualifiers for the current characterset to string @a arg.
 * 
 * @def VOODOO_TOSTRING(arg)
 * @sa VOODOO_STRING_ARG(arg)
 *
 * @def VOODOO_STRING(arg)
 * @sa VOODOO_STRING_STR(arg) 
 * 
 * @def VSTR(arg)
 * @sa VOODOO_STRING_STR(arg)
 * 
 * @defgroup voodoo_macros_decl Declaration Macros
 * @{
 * 
 * @def VOODOO_STRIP(...)
 * Strips parentheses from an arbitrary list of arguments.
 * 
 * @def DEFINE_UUID(name)
 * Declares and defines a Uuid variable with specific qualifiers (particularly extern "C", const and selectany).
 *
 * @def DEFINE_IID(name)
 * Defines an interface-type Uuid.
 * @sa DEFINE_UUID(...)
 * 
 * @def DEFINE_CLSID(name)
 * Defines a class-type Uuid.
 * @sa DEFINE_UUID(...)
 * 
 * @def DEFINE_LIBID(name)
 * Defines a library-type Uuid.
 * @sa DEFINE_UUID(...)
 * 
 * @def VOODOO_CALLTYPE
 * Declares the standard Voodoo Shader function calling convention.
 * 
 * @def VOODOO_METHODTYPE
 * Declares the calling convention for interface methods. This is typically the same as functions.
 * @sa VOODOO_CALLTYPE
 * 
 * @def VOODOO_API
 * Declares the DLL visibility type for functions. When building the core, this is typically export; otherwise it defaults
 * to import to simplify using the framework.
 * 
 * @def VOODOO_METHOD_(type, name)
 * Declares a method with full qualifiers, named @a name and with a return type of @a type.
 * 
 * @def VOODOO_METHOD(name)
 * Declares a method with full qualifiers, named @a name and with the default return type of bool.
 * @sa VOODOO_METHOD_(type, name)
 * 
 * @def VOODOO_INTERFACE_(iname, uuid)
 * Declares an interface-type Uuid for @a iname with the value given in @a uuid and an interface named @a iname.
 * 
 * @def VOODOO_INTERFACE(iname, ibase, uuid)
 * Declares an interface-type Uuid for @a iname with the value given in @a uuid and an interface named @a iname, inheriting
 * from @a ibase.
 * 
 * @def VOODOO_CLASS(cname, ibase, uuid)
 * Declares a class-type Uuid for @a cname with the value given in @a uuid and a class named @a cname, inheriting from
 * @a ibase.
 * 
 * @}
 * @defgroup voodoo_macros_version Version Macros
 * A variety of macros to simplify project versioning and resources.
 * Many of these depend on the project version macros being set for the project and available. For all these macros to work,
 * the following must be defined:
 * @code
 * VOODOO_TOKEN_LIBID           // Module LibId, initializer list for Uuid ({0x00, ...})
 * VOODOO_TOKEN_AUTHOR          // Author, string
 * VOODOO_TOKEN_NAME            // Basic name (eg, Voodoo/Core), string
 * VOODOO_TOKEN_PRETTYNAME      // Pretty name (eg, Voodoo Shader Framework Core), string
 * VOODOO_TOKEN_VERSION_MAJOR   // X.y.z.w of version, uint32_t
 * VOODOO_TOKEN_VERSION_MINOR   // x.Y.z.w of version, uint32_t
 * VOODOO_TOKEN_VERSION_PATCH   // x.y.Z.w of version, uint32_t
 * VOODOO_TOKEN_VERSION_BUILD   // x.y.z.W of version, uint32_t
 * VOODOO_TOKEN_VERSION_ID      // Code revision (framework uses git describe), string
 * @endcode
 * 
 * The TOKEN portion should be replaced with a project-unique value, typically the last part of NAME. When using the version
 * macros which take a token, this value should be given. Used as it is in the framework, this allows all version info to
 * remain up-to-date (particularly among itself) and simplifies it by providing all necessary data preformatted.
 * @{
 * 
 * @def VOODOO_DEBUG_BOOL
 * The current debug state as a bool keyword.
 * 
 * @def VOODOO_DEBUG_STRING
 * The current debug state notation as a string.
 * 
 * @def VOODOO_STRING_NAME_INTERNAL
 * Expands to the current project's name, as a string.
 * 
 * @def VOODOO_STRING_NAME_TARGET
 * Expands to the current project's output filename, without path (as a string).
 * 
 * @def VOODOO_STRING_NAME_BASIC(token)
 * Expands to the basic project name (simple name and debug note).
 * 
 * @def VOODOO_STRING_NAME_FULL(token)
 * Expands to the full project name (pretty name and debug note).
 * 
 * @def VOODOO_STRING_AUTHOR(token)
 * Expands to the project author.
 * 
 * @def VOODOO_STRING_VERSION_BASIC(token)
 * Expands to a basic version string, with the form "x.y.z.w (DEBUG)".
 * 
 * @def VOODOO_STRING_VERSION_FULL(token)
 * Expands to the project's version id and debug note.
 * 
 * @def VOODOO_VERSION_CHAIN(token)
 * Expands to a comma-separated chain of version info tokens, used in file resources or to initialize part of a Version 
 * struct. This includes the 4-part pretty version.
 * 
 * @def VOODOO_VERSION_STRUCT(token)
 * Expands to a full set of initializers for a Version struct, including all relevent project info.
 * @}
 * @}
 */