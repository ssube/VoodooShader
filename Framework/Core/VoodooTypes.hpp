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

#include <cstdint>

#if !defined(VOODOO_NO_BOOST)
#   include <boost/intrusive_ptr.hpp>
#   include <boost/uuid/uuid.hpp>
#   if !defined(VOODOO_NO_STDLIB)
#       include <list>
#       include <map>
#       include <vector>
#   endif
#endif

#if !defined(VOODOO_NO_PUGIXML)
#   include "pugixml.hpp"
#endif

namespace VoodooShader
{    
    // Disable typed-enum warning
#pragma warning(push)
#pragma warning(disable: 4480)
    /**
     * @defgroup voodoo_types Voodoo Shader Types
     * @{
     * @defgroup voodoo_enums Enums
     * @{
     *
     * Texture formats for use by @ref VoodooShader::ITexture "Textures", describing the layout and size of the texture
     * data. These may not be implemented by the underlying graphics API exactly as they are indicated here, but the
     * available components and sizes are guaranteed to be equal to or greater than the indicated values. Further
     * information on texture formats and depth may be found on the @ref voodoo_textures_formats "texture formats chart".
     */
    enum TextureFormat : uint32_t
    {
        TF_Unknown      = 0x0000,    /* !< Unknown texture format. */
        // General texture formats
        TF_RGB5         = 0x0101,    /* !< 5 bit RGB (1 bit X in D3D, may be R5G6B5 in OGL) */
        TF_RGB5A1       = 0x0102,    /* !< 5 bit RGB, 1 bit alpha */
        TF_RGB8         = 0x0103,    /* !< 8 bit RGB (8 bit X in D3D). Most common backbuffer format, common texture format. */
        TF_RGBA8        = 0x0104,    /* !< 8 bit RGBA. Common texture format. */
        TF_RGB10A2      = 0x0105,    /* !< 10 bit RGB, 2 bit A */
        // Float texture formats
        TF_RGBA16F      = 0x0201,    /* !< Half-precision RGBA. HDR format. */
        TF_RGBA32F      = 0x0202,    /* !< Full-precision RGBA (float/single). HDR format. */
        // Depth texture formats
        TF_D16          = 0x0401,    /* !< Half-precision depth (Z-buffer, see @ref depthbuffers for more info) */
        TF_D32          = 0x0402,    /* !< Full-precision depth (Z-buffer, see @ref depthbuffers for more info) */
    };

    enum ParameterType : uint32_t
    {
        PT_Unknown      = 0x00,     /* !< Unknown parameter category. */
        PT_Bool         = 0x01,
        PT_Int          = 0x02,
        PT_Float        = 0x03,     /* !< float vector parameter (may have 1 to 4 components). */
        PT_String       = 0x04,
        PT_Sampler1D    = 0x0B,     /* !< Sampler parameter (may sample 1D to 3D textures). */
        PT_Sampler2D    = 0x0C,
        PT_Sampler3D    = 0x0D,
        PT_SamplerCube  = 0x0E,
    };

    enum ShaderStage : uint32_t
    {
        SS_Unknown      = 0x00,     /* !< Unknown program stage */
        SS_Vertex       = 0x01,     /* !< Vertex program stage (usually supported, see @ref programstages "program stages" for more info). */
        SS_Pixel        = 0x02,     /* !< Fragment program stage (usually supported, see @ref programstages "program stages" for more info). */
        SS_Geometry     = 0x03,     /* !< Geometry program stage (sometimes supported, see @ref programstages "program stages" for more info). */
        SS_Domain       = 0x04,     /* !< Domain program stage (not always supported, see @ref programstages "program stages" for more info). */
        SS_Hull         = 0x05,     /* !< Hull program stage (not always supported, see  @ref programstages "program stages" for more info). */
        SS_Compute      = 0x06,     /* !< Compute program stage. Rarely supported. */
    };

    /**
     * Various hints and flags to the shader compiler. This should be used to hint various modules to use a particular
     * compilation method or API.
     * 
     * Some bits of this enum are reserved for various purposes:
     *  @li @a 0x000F: Compilation event (immediate, delay, etc).
     *  @li @a 0x0FF0: Debug and optimization settings.
     *  @li @a 0xF000: Target API settings.
     */
    enum CompileFlags : uint32_t
    {
        CF_Default      = 0x0000,   /* !< Compile with the core's default flags. */
        CF_DelayCompile = 0x0001,   /* !< Prevents automatic compiling of shaders and programs. This can be used to set many
                                          defines or uniforms without compilation overhead, but requires IProgram::Compile()
                                          be called before use. */
        CF_AvoidFlow    = 0x0010,   /* !< Hints the compiler to avoid flow-control instructions. */
        CF_PreferFlow   = 0x0020,   /* !< Hints the compiler to prefer flow-control instructions. */
        CF_Debug        = 0x0040,   /* !< Insert debug information during compilation. Always on in debug builds. */
        CF_NoOpt        = 0x0080,   /* !< Disables all optimizations. Useful for extended debugging. */
        CF_OptLevel0    = 0x0100,   /* !< Lowest optimization level. Quickly produces slow code. */
        CF_OptLevel1    = 0x0200,   /* !< Low optimization level. */
        CF_OptLevel2    = 0x0400,   /* !< Medium optimization level. The default. */
        CF_OptLevel3    = 0x0800,   /* !< Highest optimization level. Slowly produces fast code. */
        CF_DirectX9     = 0x1000,
        CF_DirectX10    = 0x2000,
        CF_DirectX11    = 0x4000,
        CF_OpenGL       = 0x8000,
    };

    /**
     * File stream type for seek and get operations.
     */
    enum StreamType : uint32_t
    {
        ST_Unknown      = 0x00,
        ST_Get          = 0x01,
        ST_Put          = 0x02
    };

    /**
     * File seek offset modes.
     */
    enum SeekMode : uint32_t
    {
        SM_Unknown      = 0x00,
        SM_Begin        = 0x01,     /* !< Seek relative to the beginning of the file (an absolute offset). */
        SM_Current      = 0x02,     /* !< Seek relative to the current position (forward or back). */
        SM_End          = 0x03      /* !< Seek relative to the end of the file. */
    };

    /**
     * File find modes.
     */
    enum GetFileMode : uint32_t
    {
        FF_Unknown      = 0x00,
        FF_CreateOnly   = 0x01,     /* !< Create the file in the first possible directory, or fail if it already exists. */
        FF_OpenOnly     = 0x02,     /* !< Open the file, or fail if it does not exist. */
        FF_AlwaysOpen   = 0x03,     /* !< Open the file if it exists, or create it if it does not. */
    };

    /**
     * File open access modes.
     */
    enum FileOpenMode : uint32_t
    {
        FO_Unknown      = 0x00,
        FO_Read         = 0x01,     /* !< Read-only access. */
        FO_Write        = 0x02,     /* !< Write-only access. */
        FO_ReadWrite    = 0x03,     /* !< Read/write access. */
        FO_Ate          = 0x10,     /* !< Set position to the end of the file (if not set, position defaults to the beginning). */
        FO_Append       = 0x20,     /* !< Write operations are performed at the end of the file. Not compatible with FM_Read. */
        FO_Truncate     = 0x40,     /* !< If the file existed, all contents are erased. Not compatible with FM_CreateOnly. */
    };

    /**
     * Log message levels. These are set up to quickly filter messages based on severity and source. Each message must have
     * a severity and source bit set, which the logger tests against its internal filter, like so:
     *
     * @code
     * LogLevel maskedLevel = level & storedLevel;
     * if ( (maskedLevel & LL_Origin) && (maskedLevel & LL_Severity) )
     * {
     *     // Log message
     * }
     * @endcode
     */
    enum LogLevel : uint32_t
    {
        LL_Unknown      = 0x00,
        // Severity
        LL_Debug        = 0x0001,   /* !< Verbose debugging message. May cause significant performance hit. */
        LL_Info         = 0x0002,   /* !< General information. */
        LL_Warning      = 0x0004,   /* !< Potential problem, not fatal within program or function scope. */
        LL_Error        = 0x0008,   /* !< Problem, fatal at function scope but not program. */
        // Source
        LL_External     = 0x0100,   /* !< Completely external source (network, pipe, IPC, etc). */
        LL_Module       = 0x0200,   /* !< Voodoo plugin module. Module must conform to Voodoo plugin API. */
        LL_Core         = 0x0400,   /* !< Voodoo core module. Must not be used elsewhere. */
        LL_System       = 0x0800,   /* !< System message (OS message, system API error, STL exception, etc). */
        // Flags
        LL_Critical     = 0x010000, /* !< Special flag: message must be logged, regardless of settings. Typically for notices. */
        // Compound
        LL_Exception    = LL_Debug | LL_Critical, /* !< Caught exception from Voodoo or other code. */
        LL_Notice       = LL_Info  | LL_Critical, /* !< Important info (module loaded, copyright banner, etc). */
        // User
        LL_CoreDebug    = LL_Core | LL_Debug,
        LL_CoreException= LL_Core | LL_Exception,
        LL_CoreInfo     = LL_Core | LL_Info,
        LL_CoreNotice   = LL_Core | LL_Notice,
        LL_CoreWarning  = LL_Core | LL_Warning,
        LL_CoreError    = LL_Core | LL_Error,
        LL_ModDebug     = LL_Module | LL_Debug,
        LL_ModException = LL_Module | LL_Exception,
        LL_ModInfo      = LL_Module | LL_Info,
        LL_ModNotice    = LL_Module | LL_Notice,
        LL_ModWarning   = LL_Module | LL_Warning,
        LL_ModError     = LL_Module | LL_Error,
        // Masks
        LL_Severity     = LL_Debug | LL_Info | LL_Warning | LL_Error,
        LL_Flags        = LL_Critical,
        LL_Origin       = LL_External | LL_Module | LL_Core | LL_System,
        // Other
        LL_Default      = LL_Info | LL_Warning | LL_Module | LL_Core | LL_System,
        LL_All          = LL_Severity | LL_Origin | LL_Flags
    };

    enum LogFlags  : uint32_t
    {
        LF_Unknown      = 0x00,
        LF_Flush        = 0x01      /* !< Log will be flushed to disk after every message. */
    };

    /**
     * String parsing flags. These modify the behavior of the string parser.
     */
    enum ParseFlags : uint32_t
    {
        PF_None         = 0x00,

        PF_SlashSingle  = 0x0001,   /* !< Compress any consecutive slashes into one. */
        PF_SlashDouble  = 0x0002,   /* !< Double any slashes found. */
        PF_SlashEscape  = 0x0004,   /* !< Prefix any slashes with a backslash. */
        PF_SlashOnly    = 0x0010,   /* !< Replace all slashes with forward slashes. */
        PF_SlashBack    = 0x0020,   /* !< Replace all slashes with back slashes. */
        PF_SlashTrail   = 0x0100,   /* !< Append a slash to the end of the string. */

        PF_PathRoot     = 0x001000, /* !< Isolate the root of a path (eg "C:\file.txt" becomes "C:"). */
        PF_PathOnly     = 0x002000, /* !< Strip a trailing filename, if one is present (eg "C:\dir\file.txt" becomes "C:\dir"). */
        PF_PathFile     = 0x004000, /* !< Strip a path, leaving only filename or last element (eg "C:\dir\file.txt" becomes "file.txt", and "C:\dir\other" becomes "other"). */
        PF_PathExt      = 0x008000, /* !< Isolate the file extension, if one is present (eg "C:\dir\file.txt" becomes "txt"). */
        PF_PathCanon    = 0x010000, /* !< Canonicalize the path (parse any relative tokens, eg "C:\dir\..\file.txt" becomes "C:\file.txt"). */

        PF_SlashFlags   = 0x00000137,
        PF_PathFlags    = 0x000FF000,
    };

    enum VariableType  : uint32_t
    {
        VT_Normal       = 0x00,
        VT_State        = 0x01,
        VT_System       = 0x10
    };

    enum UnionType : uint32_t
    {
        UT_Unknown      = 0x00,
        UT_None         = 0x01,
        UT_Bool         = 0x02,
        UT_Int8         = 0x03,
        UT_UInt8        = 0x04,
        UT_Int16        = 0x05,
        UT_UInt16       = 0x06,
        UT_Int32        = 0x07,
        UT_UInt32       = 0x08,
        UT_Float        = 0x09,
        UT_Double       = 0x0A,
        UT_Uuid         = 0x0B,
        UT_String       = 0x0C,
        UT_IObject      = 0x0D,
        UT_PVoid        = 0x0F,
    };

    enum VertexFlags : uint32_t
    {
        VF_None         = 0x00,
        VF_Transformed  = 0x01,
        VF_Buffer       = 0x02,
    };
#pragma warning(pop)
    /**
     * @}
     * @defgroup voodoo_classes_conditional Conditional Classes
     * @{
     */
#if !defined(VOODOO_NO_BOOST)
    typedef boost::uuids::uuid Uuid;
#else
    typedef struct
    {
        uint8_t data[16];
    } Uuid;
#endif
    /**
     * @defgroup voodoo_classes_uuids Predefined Uuids
     * @{
     */
    namespace PropIds
    {
        DEFINE_UUID(D3D8Object)    = {0xe0, 0xc0, 0x4d, 0x6a, 0x21, 0x37, 0x43, 0x68, 0x9b, 0x80, 0xfa, 0xe4, 0x30, 0x94, 0x52, 0xa5};
        DEFINE_UUID(D3D9Object)    = {0x9c, 0x34, 0x50, 0x6a, 0x72, 0xf4, 0x48, 0x82, 0xb1, 0xa4, 0x61, 0xc0, 0x30, 0x9a, 0xc6, 0x15};
        DEFINE_UUID(D3D8Device)    = {0xb4, 0x44, 0x4f, 0xfb, 0x7a, 0x5b, 0x47, 0xa5, 0x88, 0x5f, 0xa4, 0x34, 0x31, 0xb9, 0x08, 0xf7};
        DEFINE_UUID(D3D9Device)    = {0x63, 0xd7, 0xff, 0xbf, 0x54, 0x43, 0x45, 0x7b, 0xa9, 0xc7, 0xf4, 0x11, 0xf2, 0x8b, 0x3b, 0x0e};
        DEFINE_UUID(D3D9Texture)   = {0xb3, 0x82, 0x30, 0x82, 0x53, 0x62, 0x46, 0xd4, 0xb8, 0x98, 0xb3, 0x58, 0x95, 0x0f, 0x6f, 0x0e};
        DEFINE_UUID(D3D9Surface)   = {0x4c, 0xe7, 0xa6, 0xb5, 0x13, 0x1b, 0x4d, 0x08, 0x84, 0x4c, 0x55, 0xa6, 0x17, 0xb6, 0xb3, 0x5d};
        DEFINE_UUID(D3DX9Effect)   = {0xb5, 0x8d, 0xc7, 0xf0, 0xde, 0x72, 0x4e, 0xcd, 0xa0, 0x0c, 0x01, 0x8e, 0xae, 0xf5, 0x63, 0x4b};
        DEFINE_UUID(D3DX9Handle)   = {0xdf, 0x34, 0x48, 0x55, 0xc1, 0x1e, 0x4f, 0xf6, 0x8c, 0x0a, 0x1e, 0x1f, 0xba, 0xea, 0xb6, 0xa8};
        DEFINE_UUID(D3DX9PassId)   = {0x3b, 0xfe, 0x94, 0x9c, 0x11, 0x5e, 0x48, 0x12, 0xb1, 0xa2, 0x77, 0xc0, 0x41, 0xbb, 0xcd, 0x69};
        DEFINE_UUID(D3DSdkVersion) = {0xf7, 0xc8, 0xec, 0x01, 0x95, 0x7f, 0x48, 0xf1, 0x91, 0xbc, 0x1a, 0x9b, 0xa0, 0x84, 0xec, 0xc4};
        DEFINE_UUID(OpenGLTexture) = {0x98, 0x9f, 0x58, 0x94, 0x19, 0xb9, 0x47, 0x8f, 0xb0, 0xb7, 0x0f, 0x40, 0xbc, 0x76, 0xeb, 0xd5};
        DEFINE_UUID(OpenGLContext) = {0x1e, 0x88, 0xc6, 0x7a, 0x49, 0xa6, 0x42, 0x2b, 0xb7, 0x95, 0xb1, 0x54, 0x26, 0x90, 0xf7, 0x19};
        DEFINE_UUID(DI8Version)    = {0xe6, 0x22, 0x28, 0x0b, 0x2c, 0x19, 0x41, 0xdd, 0xa0, 0xa4, 0x25, 0x57, 0xd9, 0x71, 0x4b, 0xc3};
        DEFINE_UUID(DI8Device)     = {0x79, 0x41, 0x74, 0x93, 0x65, 0x19, 0x42, 0x4f, 0xa1, 0x75, 0x89, 0x44, 0x3c, 0x6e, 0xeb, 0x82};
        DEFINE_UUID(DS8Device)     = {0x35, 0x67, 0xe0, 0x47, 0x13, 0x41, 0x44, 0x30, 0xa3, 0xb6, 0x12, 0xf9, 0x6e, 0x71, 0x8b, 0x9c};
        DEFINE_UUID(DIADevice)     = {0xa0, 0x4b, 0xa7, 0xfe, 0x46, 0x09, 0x45, 0x9e, 0x8c, 0xc4, 0xdb, 0x2a, 0x05, 0xa5, 0x92, 0x89};
        DEFINE_UUID(DIWDevice)     = {0x21, 0xd7, 0xdd, 0xc7, 0x17, 0xac, 0x45, 0xf7, 0x9e, 0xd3, 0x74, 0x0a, 0x9a, 0x86, 0x67, 0xb5};
    }
    /**
     * @}
     */
#if !defined(VOODOO_NO_PUGIXML)
    typedef pugi::xml_document * XmlDocument;
    typedef pugi::xml_node * XmlNode;
#else
    typedef void * XmlDocument;
    typedef void * XmlNode;
#endif
    /**
     * @}
     * @defgroup voodoo_types Basic Types 
     * @{
     */
    /**
     * Standard return type for Voodoo Shader functions, indicating various levels of success or failure.
     */
    typedef int32_t VoodooResult;

#define VSUCCESS(vr) (((VoodooResult)(vr)) >= 0)
#define VFAILURE(vr) (((VoodooResult)(vr)) <  0)
    /**
     * @defgroup voodoo_types_codes Return Codes
     * @{
     */
#define VSF_OK                  MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0000)
#define VSF_FAIL                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0000)

#define VSFOK_PROPERTYCHANGED   MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x1001)
#define VSFOK_REDUNDANT         MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x1002)

#define VSFERR_INVALIDCALL      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1001)
#define VSFERR_INVALIDPARAMS    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1002)
#define VSFERR_INVALIDXML       MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1003)
#define VSFERR_COMPILEFAILED    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1004)
#define VSFERR_INVALIDPROFILE   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1005)
#define VSFERR_INVALIDPROPERTY  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1006)
#define VSFERR_FILENOTFOUND     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1007)
#define VSFERR_NOTIMPLEMENTED   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1008)
#define VSFERR_CONFLICTING      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1009)
#define VSFERR_INVALIDUUID      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x100A)
#define VSFERR_PROPERTYNOTFOUND MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x100B)
    /**
     * @}
     * @}
     * @defgroup voodoo_types_vectors Vectors
     * @{
     */
    template <typename ValType>
    struct Vector1
    {
        ValType X;
    };

    template <typename ValType>
    struct Vector2
    {
        ValType X, Y;
    };

    template <typename ValType>
    struct Vector3
    {
        ValType X, Y, Z;
    };

    template <typename ValType>
    struct Vector4
    {
        ValType X, Y, Z, W;
    };

    typedef Vector1<int8_t>     Byte1;
    typedef Vector2<int8_t>     Byte2;
    typedef Vector3<int8_t>     Byte3;
    typedef Vector4<int8_t>     Byte4;
    typedef Vector1<uint8_t>    UByte1;
    typedef Vector2<uint8_t>    UByte2;
    typedef Vector3<uint8_t>    UByte3;
    typedef Vector4<uint8_t>    UByte4;
    typedef Vector1<int16_t>    Short1;
    typedef Vector2<int16_t>    Short2;
    typedef Vector3<int16_t>    Short3;
    typedef Vector4<int16_t>    Short4;
    typedef Vector1<uint16_t>   UShort1;
    typedef Vector2<uint16_t>   UShort2;
    typedef Vector3<uint16_t>   UShort3;
    typedef Vector4<uint16_t>   UShort4;
    typedef Vector1<int32_t>    Int1;
    typedef Vector2<int32_t>    Int2;
    typedef Vector3<int32_t>    Int3;
    typedef Vector4<int32_t>    Int4;
    typedef Vector1<uint32_t>   UInt1;
    typedef Vector2<uint32_t>   UInt2;
    typedef Vector3<uint32_t>   UInt3;
    typedef Vector4<uint32_t>   UInt4;
    typedef Vector1<float>      Float1;
    typedef Vector2<float>      Float2;
    typedef Vector3<float>      Float3;
    typedef Vector4<float>      Float4;
    typedef Vector1<double>     Double1;
    typedef Vector2<double>     Double2;
    typedef Vector3<double>     Double3;
    typedef Vector4<double>     Double4;
    /**
     * @}
     */
    struct Light;
    struct ParameterDesc;
    struct TextureDesc;
    struct TextureRegion;
    struct Variant;
    struct Version;
    struct Vertex;
    /**
     * @}
     * @defgroup voodoo_interfaces Interfaces
     * @{
     */
    class IAdapter;
    class ICore;
    class IEffect;
    class IFile;
    class IFileSystem;
    class IHookManager;
    class IImage;
    class ILogger;
    class IModule;
    class IModuleManager;
    class IObject;
    class IParameter;
    class IParser;
    class IPass;
    class ITechnique;
    class ITexture;
    /**
     * @}
     * @defgroup voodoo_classes_utility Utility Classes
     * @{
     */
    class Exception;
    class Format;
    class Regex;
    class RegexMatch;
    class String;
    /**
     * @}
     * @defgroup voodoo_functions Function Typedefs
     * Function pointer types for module interfaces.
     * @{
     */
    namespace Functions
    {
        typedef const Version * (VOODOO_CALLTYPE * ModuleVersionFunc)();
        typedef const uint32_t  (VOODOO_CALLTYPE * ModuleCountFunc)();
        typedef const wchar_t * (VOODOO_CALLTYPE * ModuleInfoFunc)(const uint32_t, Uuid *);
        typedef IObject *       (VOODOO_CALLTYPE * ModuleCreateFunc)(const uint32_t, ICore *);
        typedef ICore *         (VOODOO_CALLTYPE * CoreCreateFunc)(uint32_t);
        typedef VoodooResult    (VOODOO_CALLTYPE * CallbackFunc)();
    }
    /**
     * @}
     * @defgroup voodoo_references Reference Typedefs
     * To provide smart intrusive pointers, Boost is required.
     * @{
     */
#if !defined(VOODOO_NO_BOOST)
    void VOODOO_PUBLIC_FUNC intrusive_ptr_add_ref(IObject * obj);
    void VOODOO_PUBLIC_FUNC intrusive_ptr_release(IObject * obj);

    typedef boost::intrusive_ptr<IAdapter>       IAdapterRef;
    typedef boost::intrusive_ptr<ICore>          ICoreRef;
    typedef boost::intrusive_ptr<IEffect>        IEffectRef;
    typedef boost::intrusive_ptr<IFile>          IFileRef;
    typedef boost::intrusive_ptr<IFileSystem>    IFileSystemRef;
    typedef boost::intrusive_ptr<IHookManager>   IHookManagerRef;
    typedef boost::intrusive_ptr<IImage>         IImageRef;
    typedef boost::intrusive_ptr<ILogger>        ILoggerRef;
    typedef boost::intrusive_ptr<IModule>        IModuleRef;
    typedef boost::intrusive_ptr<IModuleManager> IModuleManagerRef;
    typedef boost::intrusive_ptr<IObject>        IObjectRef;
    typedef boost::intrusive_ptr<IParameter>     IParameterRef;
    typedef boost::intrusive_ptr<IParser>        IParserRef;
    typedef boost::intrusive_ptr<IPass>          IPassRef;
    typedef boost::intrusive_ptr<ITechnique>     ITechniqueRef;
    typedef boost::intrusive_ptr<ITexture>       ITextureRef;
    /**
     * @}
     * @defgroup voodoo_collections Collections
     * To provide collections of Voodoo Shader objects, smart pointers are required.
     * @{
     */
#if !defined(VOODOO_NO_STDLIB)
    typedef std::pair<String, String>            StringPair;
    typedef std::map<String, String>             StringMap;
    typedef std::list<String>                    StringList;
    typedef std::vector<String>                  StringVector;
    typedef std::map<String, IEffectRef>         EffectMap;
    typedef std::list<IEffectRef>                EffectList;
    typedef std::vector<IEffectRef>              EffectVector;
    typedef std::map<String, ITechniqueRef>      TechniqueMap;
    typedef std::list<ITechniqueRef>             TechniqueList;
    typedef std::vector<ITechniqueRef>           TechniqueVector;
    typedef std::map<String, IPassRef>           PassMap;
    typedef std::list<IPassRef>                  PassList;
    typedef std::vector<IPassRef>                PassVector;
    typedef std::map<String, IParameterRef>      ParameterMap;
    typedef std::list<IParameterRef>             ParameterList;
    typedef std::vector<IParameterRef>           ParameterVector;
    typedef std::map<String, ITextureRef>        TextureMap;
    typedef std::list<ITextureRef>               TextureList;
    typedef std::vector<ITextureRef>             TextureVector;
    typedef std::map<String, Variant>            VariantMap;
    typedef std::list<Variant>                   VariantList;
    typedef std::vector<Variant>                 VariantVector;
    typedef std::map<Uuid, Variant>              PropertyMap;
    typedef std::pair<String, uint32_t>          Variable;
    typedef std::map<String, Variable>           VariableMap;
    typedef std::map<Uuid, IModuleRef>           ModuleMap;
    typedef std::pair<IModuleRef, uint32_t>      ClassSource;
    typedef std::map<Uuid, ClassSource>          ClassMap;
    typedef std::map<String, Uuid>               StrongNameMap;
    typedef std::map<ITextureRef, IEffectRef>    MaterialMap;
#endif
#endif
    /**
     * @}
     * @addtogroup voodoo_structs
     * @{
     * Describes the precise version of a particular library, including name, main version, revision and debug status.
     */
    struct Version
    {
        Uuid            LibId;
        int32_t         Major;
        int32_t         Minor;
        int32_t         Patch;
        int32_t         Build;
        bool            Debug;
        const wchar_t * Name;
        const wchar_t * RevId;
    };
    /**
     * Property variant type. Consists of the value type (filled union field), components in the value (for vector
     * fields), and the value union capable of containing all common basic, vector and pointer types used in the
     * library.
     */
    struct Variant
    {
        UnionType   Type;
        uint32_t    Components;
        union
        {
            bool        VBool;
            Byte4       VInt8;
            UByte4      VUInt8;
            Short4      VInt16;
            UShort4     VUInt16;
            Int4        VInt32;
            UInt4       VUInt32;
            Float4      VFloat;
            Double4     VDouble;
            Uuid *      VPUuid;
            String *    VPString;
            IObject *   VPIObject;
            void *      VPVoid;
        };
    };
    /**
     * @defgroup voodoo_variant_decl Variant Declaration & Init
     * @{
     */
#define DECLARE_VARIANT(name) Variant name; ZeroMemory(&name, sizeof(Variant))
#define INITIALIZE_VARIANTC(name, type, comp)       name.Type = UT_##type; name.Components = comp
#define INITIALIZE_VARIANT1(name, type, x)          INITIALIZE_VARIANTC(name, type, 1); name.V##type.X = x
#define INITIALIZE_VARIANT2(name, type, x, y)       INITIALIZE_VARIANTC(name, type, 2); name.V##type.X = x; name.V##type.Y = y
#define INITIALIZE_VARIANT3(name, type, x, y, z)    INITIALIZE_VARIANTC(name, type, 3); name.V##type.X = x; name.V##type.Y = y; name.V##type.Z = z
#define INITIALIZE_VARIANT4(name, type, x, y, z, w) INITIALIZE_VARIANTC(name, type, 4); name.V##type.X = x; name.V##type.Y = y; name.V##type.Z = z; name.V##type.W = w

    inline Variant CreateVariant()                      { DECLARE_VARIANT(var); return var; }
    inline Variant CreateVariant(const UnionType t)     { DECLARE_VARIANT(var); var.Type = t; return var; }
    inline Variant CreateVariant(const bool & v)        { DECLARE_VARIANT(var); INITIALIZE_VARIANTC(var, Bool, 0); var.VBool = v; return var; }
    inline Variant CreateVariant(const int8_t & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, Int8, v);         return var; }
    inline Variant CreateVariant(const Byte2 & v)       { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, Int8, v.X, v.Y);  return var; }
    inline Variant CreateVariant(const Byte3 & v)       { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, Int8, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const Byte4 & v)       { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, Int8, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(const uint8_t & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, UInt8, v);         return var; }
    inline Variant CreateVariant(const UByte2 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, UInt8, v.X, v.Y); return var; }
    inline Variant CreateVariant(const UByte3 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, UInt8, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const UByte4 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, UInt8, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(const int16_t & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, Int16, v);         return var; }
    inline Variant CreateVariant(const Short2 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, Int16, v.X, v.Y);  return var; }
    inline Variant CreateVariant(const Short3 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, Int16, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const Short4 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, Int16, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(const uint16_t & v)    { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, UInt16, v);        return var; }
    inline Variant CreateVariant(const UShort2 & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, UInt16, v.X, v.Y); return var; }
    inline Variant CreateVariant(const UShort3 & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, UInt16, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const UShort4 & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, UInt16, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(const int32_t & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, Int32, v);         return var; }
    inline Variant CreateVariant(const Int2 & v)        { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, Int32, v.X, v.Y);  return var; }
    inline Variant CreateVariant(const Int3 & v)        { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, Int32, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const Int4 & v)        { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, Int32, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(const uint32_t & v)    { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, UInt32, v);        return var; }
    inline Variant CreateVariant(const UInt2 & v)       { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, UInt32, v.X, v.Y); return var; }
    inline Variant CreateVariant(const UInt3 & v)       { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, UInt32, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const UInt4 & v)       { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, UInt32, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(const float & v)       { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, Float, v);         return var; }
    inline Variant CreateVariant(const Float2 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, Float, v.X, v.Y);  return var; }
    inline Variant CreateVariant(const Float3 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, Float, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const Float4 & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, Float, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(const double & v)      { DECLARE_VARIANT(var); INITIALIZE_VARIANT1(var, Double, v);        return var; }
    inline Variant CreateVariant(const Double2 & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT2(var, Double, v.X, v.Y); return var; }
    inline Variant CreateVariant(const Double3 & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT3(var, Double, v.X, v.Y, v.Z); return var; }
    inline Variant CreateVariant(const Double4 & v)     { DECLARE_VARIANT(var); INITIALIZE_VARIANT4(var, Double, v.X, v.Y, v.Z, v.W); return var; }
    inline Variant CreateVariant(Uuid * pV)             { DECLARE_VARIANT(var); INITIALIZE_VARIANTC(var, Uuid, 0); var.VPUuid = pV; }
    inline Variant CreateVariant(String * pV)           { DECLARE_VARIANT(var); INITIALIZE_VARIANTC(var, String, 0); var.VPString = pV; }
    inline Variant CreateVariant(IObject * pV)          { DECLARE_VARIANT(var); INITIALIZE_VARIANTC(var, IObject, 0); var.VPIObject = pV; }
    inline Variant CreateVariant(void * pV)             { DECLARE_VARIANT(var); INITIALIZE_VARIANTC(var, PVoid, 0); var.VPVoid = pV; }
    template<typename T>
    inline Variant CreateVariant(T * pV)                { DECLARE_VARIANT(var); INITIALIZE_VARIANTC(var, PVoid, 0); var.VPVoid = reinterpret_cast<void*>(pV); return var; }
    /**
     * @}
     */
    /**
     * Describes a texture, including size and format.
     */
    struct TextureDesc
    {
        UInt3         Size;
        bool          Mipmaps;
        bool          RenderTarget;
        TextureFormat Format;
    };
    /**
     * Describes a portion of a texture. This defines a cube region and provides an optional format the region should be
     * set up as.
     */
    struct TextureRegion :
        public TextureDesc
    {
        UInt3 Origin;
    };
    /**
     * Standard vertex structure.
     */
    struct VertexDesc
    {
        Float4  Position;
        UByte4  Color;
        Float4  TexCoord[2];
    };
    /**
     * Shader-compatible light structure
     */
    struct LightDesc
    {
        uint32_t Type;
        Float4   Diffuse;
        Float4   Specular;
        Float4   Ambient;
        Float3   Position;
        Float3   Direction;
        float    Range;
        float    Falloff;
        Float3   Attenuation;
        float    Theta;
        float    Phi;
    };
    /**
     * Parameter description.
     */
    struct ParameterDesc
    {
        ParameterType Type;
        uint32_t      Rows;
        uint32_t      Columns;
        uint32_t      Elements;
    };
    /**
     * @}
     * @}
     */
}
