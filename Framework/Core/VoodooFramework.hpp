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

#if !defined(__cplusplus)
#   error Voodoo Shader requires a C++ compiler, preferably Microsoft Visual C++ 10 or better.
#elif !defined(_WIN32) 
#   error Voodoo Shader must be built for the Win32 platform.
#elif !defined(_UNICODE) || (!defined(wchar_t) && !defined(_NATIVE_WCHAR_T_DEFINED))
#   error Voodoo Shader APIs require Unicode character set enabled.
#elif defined(VOODOO_SDK_VERSION)
#   error Voodoo SDK version already defined. This should only be defined by the primary framework header.
#endif

#define VOODOO_SDK_VERSION 0

#pragma warning(push,3)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#pragma warning(pop)

#if !defined(CONST)
#   define CONST const
#endif
#if !defined(EXPLICIT)
#   define EXPLICIT explicit
#endif
#if !defined(VIRTUAL)
#   define VIRTUAL virtual
#endif
#if !defined(PURE)
#   define PURE = 0
#endif
#if !defined(DECLSPEC_DLLIMPORT)
#   define DECLSPEC_DLLIMPORT __declspec(dllimport)
#endif
#if !defined(DECLSPEC_DLLEXPORT)
#   define DECLSPEC_DLLEXPORT __declspec(dllexport)
#endif
#if !defined(DECLSPEC_NOTHROW)
#   define DECLSPEC_NOTHROW __declspec(nothrow)
#endif
#if !defined(DECLSPEC_SELECTANY)
#   define DECLSPEC_SELECTANY __declspec(selectany)
#endif
#if !defined(DECLSPEC_NOVTABLE)
#   define DECLSPEC_NOVTABLE __declspec(novtable)
#endif
#if !defined(STDCALL)
#   define STDCALL __stdcall
#endif

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

#if !defined(VOODOO_EXPORT)
#   define VOODOO_API               DECLSPEC_DLLIMPORT
#   define VOODOO_EXPORT_REGION     1
#   define VOODOO_INTERNAL_REGION   0
#else
#   define VOODOO_API               DECLSPEC_DLLEXPORT
#   define VOODOO_EXPORT_REGION     0
#   define VOODOO_INTERNAL_REGION   1
#endif

#define VOODOO_STRIP(...)           __VA_ARGS__

#define DEFINE_UUID(name)           EXTERN_C CONST Uuid DECLSPEC_SELECTANY name
#define DEFINE_IID(name)            DEFINE_UUID(IID_##name)
#define DEFINE_CLSID(name)          DEFINE_UUID(CLSID_##name)
#define DEFINE_LIBID(name)          DEFINE_UUID(LIBID_##name)

#define VOODOO_CALLTYPE             STDCALL
#define VOODOO_METHODTYPE           VOODOO_CALLTYPE

#define VOODOO_FUNCTION(type, name) VOODOO_API type VOODOO_CALLTYPE name
#define VOODOO_METHOD_(type, name)  VIRTUAL DECLSPEC_NOTHROW type VOODOO_METHODTYPE name
#define VOODOO_METHOD(name)         VOODOO_METHOD_(VoodooResult, name)
#define VOODOO_METHODDEF_(type, name) type VOODOO_METHODTYPE name
#define VOODOO_METHODDEF(name)      VOODOO_METHODDEF_(VoodooResult, name)

#define VOODOO_INTERFACE_(iname, uuid)       DEFINE_IID(iname) = VOODOO_STRIP##uuid; \
    class DECLSPEC_NOVTABLE iname
#define VOODOO_INTERFACE(iname, ibase, uuid) DEFINE_IID(iname) = VOODOO_STRIP##uuid; \
    class DECLSPEC_NOVTABLE iname : public ibase
#define VOODOO_CLASS(cname, ibase, uuid)     DEFINE_CLSID(cname) = VOODOO_STRIP##uuid; \
    class cname : public ibase

#include "VoodooDebug.hpp"
#include "VoodooTypes.hpp"
#include "VoodooVersion.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "Regex.hpp"
#include "Stream.hpp"
#include "String.hpp"
#include "StringFormat.hpp"

#include "IObject.hpp"
#include "IBinding.hpp"
#include "ICore.hpp"
#include "IFile.hpp"
#include "IFileSystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"
#include "IParser.hpp"
#include "IPlugin.hpp"
#include "IPluginServer.hpp"

#include "IResource.hpp"
#include "IEffect.hpp"
#include "IPass.hpp"
#include "ITechnique.hpp"
#include "IParameter.hpp"
#include "ITexture.hpp"

#if defined(VOODOO_EXPORT) || defined(VOODOO_STATIC_IMPORT)
namespace VoodooShader
{
    /**
     * Creates a new core. This function is exported and meant for use by the loader.
     *
     * @param   version     Version identifier for this core, should always be <tt>VOODOO_SDK_VERSION</tt>.
     * @return  A new ICore object, if one was created successfully. The core is not initialized, and so ICore::Init() must
     *      be called prior to use.
     *      
     * @note As of 0.5.8, this is a singleton; it is not possible to create more than one core. This may be changed in the
     *      future. Relying on this to return the same object repeatedly is undefined.
     */
    _Check_return_ VOODOO_FUNCTION(ICore *, CreateCore)(uint32_t version);
    _Check_return_ VOODOO_FUNCTION(ILogger *, CreateLogger)();
    /**
     * Retrieve the singleton variable parser.
     */
    _Check_return_ VOODOO_FUNCTION(IParser *, CreateParser)();
    /**
     * Retrieve the singleton module manager, for loading and plugin use.
     * 
     * @note This does not have a version parameter and will always instantiate and return the latest module manager 
     *      available, for security and compatibility reasons. These is no way to create older managers, and they typically
     *      will be removed from the code.
     */
    _Check_return_ VOODOO_FUNCTION(IPluginServer *, CreateServer)();
}
#endif
