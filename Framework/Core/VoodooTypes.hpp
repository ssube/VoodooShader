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
        TF_Unknown  = 0x0,      /* !< Unknown texture format. */
        // Backbuffer formats
        TF_RGB5     = 0x101,    /* !< 5 bit RGB (1 bit X in D3D, may be R5G6B5 in OGL) */
        TF_RGB5A1   = 0x102,    /* !< 5 bit RGB, 1 bit alpha */
        TF_RGB8     = 0x103,    /* !< 8 bit RGB (8 bit X in D3D). Most common backbuffer format, common texture format. */
        TF_RGBA8    = 0x104,    /* !< 8 bit RGBA. Common texture format. */
        TF_RGB10A2  = 0x105,    /* !< 10 bit RGB, 2 bit A */
        // float texture formats
        TF_RGBA16F  = 0x201,    /* !< Half-precision RGBA. HDR format. */
        TF_RGBA32F  = 0x202,    /* !< Full-precision RGBA (float/single). HDR format. */
        // Depth-buffer formats
        TF_D16      = 0x401,    /* !< Half-precision depth (Z-buffer, see @ref depthbuffers for more info) */
        TF_D32      = 0x402     /* !< Full-precision depth (Z-buffer, see @ref depthbuffers for more info) */
    };
    /**
     * Parameter types valid for @ref VoodooShader::IParameter. These types are available across hardware, exposing most
     * common shader language types.
     */
    enum ParameterType : uint32_t
    {
        PT_Unknown      = 0x00,     /* !< Unknown parameter type. */
        // floats
        PT_Float1       = 0x11,     /* !< Single-component float. */
        PT_Float2       = 0x12,     /* !< Two-component float. */
        PT_Float3       = 0x13,     /* !< Three-component float. */
        PT_Float4       = 0x14,     /* !< Four-component float. */
        PT_Float1x1     = 0x11,
        PT_Float1x2     = 0x12,
        PT_Float1x3     = 0x13,
        PT_Float1x4     = 0x14,
        PT_Float2x1     = 0x21,
        PT_Float2x2     = 0x22,
        PT_Float2x3     = 0x23,
        PT_Float2x4     = 0x24,
        PT_Float3x1     = 0x31,
        PT_Float3x2     = 0x32,
        PT_Float3x3     = 0x33,
        PT_Float3x4     = 0x34,
        PT_Float4x1     = 0x41,
        PT_Float4x2     = 0x42,
        PT_Float4x3     = 0x43,
        PT_Float4x4     = 0x44,
        // Samplers
        PT_Sampler1D    = 0x101,    /* !< One-dimensional sampler (for a 1D texture, see @ref texturetypes "texture types" for more info). */
        PT_Sampler2D    = 0x102,    /* !< Two-dimensional sampler (for a 2D texture, see @ref texturetypes "texture types" for more info). */
        PT_Sampler3D    = 0x103,    /* !< Three-dimensional sampler (for a 3D/volume texture, see @ref texturetypes "texture types" for more info). */
        // Structs
        PT_Struct       = 0x1000
    };

    enum ParameterCategory : uint32_t
    {
        PC_Unknown      = 0x00,     /* !< Unknown parameter category. */
        PC_Bool         = 0x01,
        PC_Int          = 0x02,
        PC_Float        = 0x03,     /* !< float vector parameter (may have 1 to 4 components). */
        PC_Sampler      = 0x04,     /* !< Sampler parameter (may sample 1D to 3D textures). */
        PC_String       = 0x05,
        PC_Struct       = 0x06,
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
        UT_PVoid        = 0x0E,
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
#ifndef VOODOO_NO_BOOST
    typedef boost::uuids::uuid Uuid;
#else
    typedef struct
    {
        uint8_t data[16];
    } Uuid;
#endif

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

#define VSUCCESS(r) (((VoodooResult)(r)) >= 0)
#define VFAILURE(r) (((VoodooResult)(r)) <  0)
    /**
     * @defgroup voodoo_types_codes Return Codes
     * @{
     */
#define VSF_FAIL    -1
#define VSF_OK      0
#define VSFERR_INVALIDCALL      MAKE_HRESULT(VSF_FAIL, FACILITY_ITF, 0x1001)
#define VSFERR_INVALIDPARAMS    MAKE_HRESULT(VSF_FAIL, FACILITY_ITF, 0x1002)
#define VSFERR_INVALIDXML       MAKE_HRESULT(VSF_FAIL, FACILITY_ITF, 0x1003)
#define VSFERR_COMPILEFAILED    MAKE_HRESULT(VSF_FAIL, FACILITY_ITF, 0x1004)
#define VSFERR_INVALIDPROFILE   MAKE_HRESULT(VSF_FAIL, FACILITY_ITF, 0x1005)
#define VSFERR_INVALIDPROPERTY  MAKE_HRESULT(VSF_FAIL, FACILITY_ITF, 0x1006)
    /**
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
    struct TextureDesc;
    struct TextureRegion;
    struct Variant;
    struct Version;
    struct VertexStruct;
    struct LightStruct;
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
            Uuid *      VUuid;
            String *    VString;
            IObject *   VIObject;
            void *      VPVoid;
        };
    };    
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
    struct VertexStruct
    {
        Float4  Position;
        UByte4  Color;
        Float4  TexCoord[2];
    };
    /**
     * Shader-compatible light structure
     */
    struct LightStruct
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
     * @}
     * @}
     */
}
