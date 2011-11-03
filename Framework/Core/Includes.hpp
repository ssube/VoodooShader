/**
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
#pragma once

#include <functional>

#ifndef VOODOO_NO_ERRORS
#ifndef __cplusplus
#   error Voodoo requires a C++ compiler, preferably Microsoft Visual C++ v10.
#endif

#ifndef _WIN32
#   error Voodoo must be built for the Win32 platform.
#endif

#ifdef _WIN64
#   error Voodoo is not yet compatible with 64-bit systems.
#endif

#ifndef _UNICODE
#   error Voodoo APIs require Unicode characterset enabled.
#endif
#endif

#ifndef _NATIVE_NULLPTR_SUPPORTED
#   define nullptr NULL
#endif

#ifndef VOODOO_STRING_MACROS
#   define VOODOO_STRING_MACROS
#   define VOODOO_META_STRING_ARG(arg) L ## #arg
#   define VOODOO_META_STRING_STR(arg) L ## arg

#   define VOODOO_META_TOSTRING(arg)   VOODOO_META_STRING_ARG(arg)
#   define VOODOO_META_STRING(arg)     VOODOO_META_STRING_STR(arg)
#endif

#include <cstdint>

#ifndef VOODOO_NO_COLLECTIONS
#   include <list>
#   include <map>
#   include <vector>
#endif

#ifndef VOODOO_NO_BOOST
#   include <boost/intrusive_ptr.hpp>
#   include <boost/uuid/uuid.hpp>
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef EXTERN_C
#   define EXTERN_C extern "C"
#endif
#ifndef DECLSPEC_SELECTANY
#   define DECLSPEC_SELECTANY __declspec(selectany)
#endif

#ifdef VSF_DEBUG_MEMORY
#   define _CRTDBG_MAP_ALLOC
#   include <stdlib.h>
#   include <crtdbg.h>

#   define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#   define new DBG_NEW
#endif

#ifndef VOODOO_IMPORT
#   define VOODOO_API __declspec(dllexport)
#else
#   define VOODOO_API __declspec(dllimport)
#endif
#define VOODOO_CALL __stdcall
#define VOODOO_PUBLIC_FUNC VOODOO_API VOODOO_CALL

#ifndef VOODOO_NO_CG
#   include "Cg/cg.h"
#else
    typedef int    CGbool;
    typedef void * CGcontext;
    typedef void * CGprogram;
    typedef void * CGparameter;
    typedef void * CGobj;
    typedef void * CGbuffer;
    typedef void * CGeffect;
    typedef void * CGtechnique;
    typedef void * CGpass;
    typedef void * CGstate;
    typedef void * CGstateassignment;
    typedef void * CGannotation;
    typedef void * CGhandle;
#endif

#ifndef VOODOO_NO_PUGIXML
#   include "pugixml.hpp"
#endif

namespace VoodooShader
{
    /* Custom basic types */
    class Exception;
    class Regex;
    class String;

#ifndef VOODOO_NO_PUGIXML
    typedef pugi::xml_document * XmlDocument;
#else
    typedef void * XmlDocument;
#endif

#ifndef VOODOO_NO_BOOST
    typedef boost::uuids::uuid Uuid;
#else
    typedef struct
    {
        uint8_t data[16];
    } Uuid;
#endif

    /* Basic structs */
    struct InitParams;
    struct TextureDesc;
    struct TextureRegion;
    struct Version;

    /* Geometry-related structs */
    struct float2;
    struct float3;
    struct float4;
    struct VertexStruct;
    struct LightStruct;

    /* Interfaces */
    /**
     * @defgroup voodoo_framework_interfaces Voodoo Framework interfaces
     * @{
     */
    class IAdapter;
    class ICore;
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
    class IShader;
    class ITechnique;
    class ITexture;
    /**
     * @}
     */

    /* Uuids */
#define DEFINE_UUID(name, d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df) \
    EXTERN_C const Uuid DECLSPEC_SELECTANY name = {d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,da,db,dc,dd,de,df}
#define DEFINE_UUID_INTR(name, d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df) \
    DEFINE_UUID(IID_##name,  d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df)
#define DEFINE_UUID_IMPL(name, d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df) \
    DEFINE_UUID(CLSID_##name,  d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df)
#define DEFINE_UUID_LIB(name, d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df) \
    DEFINE_UUID(LIBID_##name,  d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, da, db, dc, dd, de, df)

    DEFINE_UUID_INTR(IObject,       0x87, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IAdapter,      0x88, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(ICore,         0x89, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IFile,         0x8a, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IFileSystem,   0x8b, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IHookManager,  0x8c, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IImage,        0x8d, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(ILogger,       0x8e, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IModule,       0x8f, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IModuleManager,0x90, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IParameter,    0x91, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IParser,       0x92, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IPass,         0x93, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(IShader,       0x94, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(ITechnique,    0x95, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);
    DEFINE_UUID_INTR(ITexture,      0x96, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);

    /* Reference typedefs */
#ifndef VOODOO_NO_BOOST
    // Boost intrusive_ptr functions
    void VOODOO_PUBLIC_FUNC intrusive_ptr_add_ref(IObject * obj);
    void VOODOO_PUBLIC_FUNC intrusive_ptr_release(IObject * obj);

    typedef boost::intrusive_ptr<IAdapter>       IAdapterRef;
    typedef boost::intrusive_ptr<ICore>          ICoreRef;
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
    typedef boost::intrusive_ptr<IShader>        IShaderRef;
    typedef boost::intrusive_ptr<ITechnique>     ITechniqueRef;
    typedef boost::intrusive_ptr<ITexture>       ITextureRef;
#endif

    /* Collections */
#if !defined(VOODOO_NO_COLLECTIONS) && !defined(VOODOO_NO_BOOST)
    typedef std::map<String, String>             Dictionary;
    typedef std::map<String, IShaderRef>         ShaderMap;
    typedef std::list<IShaderRef>                ShaderList;
    typedef std::vector<IShaderRef>              ShaderVector;
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
    typedef std::map<String, IModuleRef>         ModuleMap;
    typedef std::pair<IModuleRef, int32_t>       ClassID;
    typedef std::map<Uuid, ClassID>              ClassMap;
    typedef std::map<String, Uuid>               ClassNameMap;
    typedef std::map<ITextureRef, IShaderRef>    MaterialMap;
#endif

#pragma warning(push)
#pragma warning(disable: 4480)
    /**
     * Texture formats for use by @ref VoodooShader::ITexture "Textures", describing the layout and size of the texture
     * data. These may not be implemented by the underlying graphics API exactly as they are indicated here, but the
     * available components and sizes are guaranteed to be equal to or greater than the indicated values. Further
     * information on texture formats and depth may be found on the @ref voodoo_textures_formats "texture formats chart".
     */
    enum TextureFormat : int32_t
    {
        TF_Unknown  = 0x0,      /* !< Unknown texture format */

        // Backbuffer formats
        TF_RGB5     = 0x101,    /* !< 5 bit RGB (1 bit X in DX, may be R5G6B5 in OGL) */
        TF_RGB5A1   = 0x102,    /* !< 5 bit RGB, 1 bit alpha */
        TF_RGB8     = 0x103,    /* !< 8 bit RGB (8 bit X in DX). Most common backbuffer format, common texture format. */
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
    enum ParameterType : int32_t
    {
        PT_Unknown      = 0x00,     /* !< Unknown parameter type */
        // floats
        PT_Float1       = 0x11,     /* !< Single-component float vector */
        PT_Float2       = 0x12,     /* !< Two-component float vector */
        PT_Float3       = 0x13,     /* !< Three-component float vector */
        PT_Float4       = 0x14,     /* !< Four-component float vector */
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
        PT_Sampler1D    = 0x101,    /* !< One-dimensional sampler (for a 1D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler2D    = 0x102,    /* !< Two-dimensional sampler (for a 2D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler3D    = 0x103,    /* !< Three-dimensional sampler (for a 3D/volume texture, see @ref texturetypes "texture types" for more info) */
        // Structs
        PT_Struct       = 0x1000
    };

    enum ParameterCategory : int32_t
    {
        PC_Unknown      = 0x00,     /* !< Unknown parameter category */
        PC_Float        = 0x01,     /* !< float vector parameter (may have 1 to 4 components) */
        PC_Sampler      = 0x02,     /* !< Sampler parameter (may sample 1D to 3D textures) */
        PC_Struct       = 0x04
    };

    enum ProgramStage : int32_t
    {
        PS_Unknown      = 0x00,     /* !< Unknown program stage */
        PS_Vertex       = 0x01,     /* !< Vertex program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Fragment     = 0x02,     /* !< Fragment program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Geometry     = 0x03,     /* !< Geometry program stage (sometimes supported, see @ref programstages "program stages" for more info) */
        PS_Domain       = 0x04,     /* !< Domain program stage (not always supported, see @ref programstages "program stages" for more info) */
        PS_Hull         = 0x05      /* !< Hull program stage (not always supported, see  @ref programstages "program stages" for more info) */
    };

    enum TextureStage : int32_t
    {
        TS_Unknown      = 0x00,     /* !< Unknown texture stage */
        TS_Shader       = 0x01,     /* !< Shader target texture */
        TS_Pass         = 0x02      /* !< Pass target texture */
    };

    enum FileOpenMode : int32_t
    {
        FM_Unknown      = 0x00,
        FM_Read         = 0x01,
        FM_Write        = 0x02,
        FM_ReadWrite    = 0x03,
        FM_CreateOnly   = 0x10,
        FM_OpenOnly     = 0x20,
        FM_AlwaysOpen   = 0x30
    };

    /**
     * Log message levels. These are set up to quickly filter based on various flags using binary logic.
     * Flags are ORed together, with lower-priority flags having all higher-priority flags set. When
     * checking a log call, the condition is:
     * @code
     * LogLevel maskedLevel = level & storedLevel;
     * if ( (maskedLevel & LL_Origin) && (maskedLevel & LL_Severity) )
     * {
     *     // Log message
     * }
     * @endcode
     */
    enum LogLevel : int32_t
    {
        LL_Unknown      = 0x00,
        LL_Debug        = 0x01,
        LL_Info         = 0x02,
        LL_Warning      = 0x04,
        LL_Error        = 0x08,
        LL_External     = 0x10,
        LL_Plugin       = 0x20,
        LL_Module       = 0x40,
        LL_Core         = 0x80,
        // Usable values
        LL_ExtDebug     = 0x11,
        LL_ExtInfo      = 0x12,
        LL_ExtWarn      = 0x14,
        LL_ExtError     = 0x18,
        LL_PlugDebug    = 0x21,
        LL_PlugInfo     = 0x22,
        LL_PlugWarn     = 0x24,
        LL_PlugError    = 0x28,
        LL_ModDebug     = 0x41,
        LL_ModInfo      = 0x42,
        LL_ModWarn      = 0x44,
        LL_ModError     = 0x48,
        LL_CoreDebug    = 0x81,
        LL_CoreInfo     = 0x82,
        LL_CoreWarn     = 0x84,
        LL_CoreError    = 0x88,
        LL_Initial      = 0xFE,
        // Masks
        LL_Severity     = 0x0F,
        LL_Origin       = 0xF0,
        LL_All          = 0xFF
    };

    enum LogFlags  : int32_t
    {
        LF_Unknown      = 0x00,
        LF_Flush        = 0x01
    };

    /**
     * String parsing flags. These modify the behavior of the string parser.
     */
    enum ParseFlags : int32_t
    {
        PF_None         = 0x0000,
        PF_SingleSlash  = 0x0001,
        PF_SlashOnly    = 0x0002,
        PF_BackslashOnly= 0x0004,
        PF_Lowercase    = 0x0010,
        PF_Uppercase    = 0x0020,
        PF_VarName      = 0x0100
    };

    enum VariableType  : int32_t
    {
        VT_Normal       = 0x00,
        VT_State        = 0x01,
        VT_System       = 0x10
    };
#pragma warning(pop)
    /**
     * @}
     */

    /**
     * Initialization parameters.
     */
    struct InitParams
    {
        const wchar_t * GlobalRoot;
        const wchar_t * LocalRoot;
        const wchar_t * RunRoot;
        const wchar_t * Target;
        const wchar_t * Loader;
        const wchar_t * Config;
    };

    /**
     * Describes a texture, including size and format.
     */
    struct TextureDesc
    {
        int32_t       Width;
        int32_t       Height;
        int32_t       Depth;
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
        int32_t OffX;
        int32_t OffY;
        int32_t OffZ;
    };

    /**
     * Describes the precise version of a particular library, including name, main version, revision and debug status.
     */
    struct Version
    {
        const wchar_t * Name;
        Uuid            LibID;
        int32_t         Major;
        int32_t         Minor;
        int32_t         Patch;
        int32_t         Rev;
        bool            Debug;
    };

    struct float2
    {
        float X, Y;
    };

    struct float3 :
        public float2
    {
        float Z;
    };

    struct float4 :
        public float3
    {
        float W;
    };

    /**
     * Pre-transformed vertex structure
     */
    struct VertexStruct
    {
        float3 Position;
        float  Winding;
        float2 TexCoord;
    };

    /**
     * Shader-compatible light structure
     */
    struct LightStruct
    {
        int32_t Type;
        float4  Diffuse;
        float4  Specular;
        float4  Ambient;
        float3  Position;
        float3  Direction;
        float   Range;
        float   Falloff;
        float3  Attenuation;
        float   Theta;
        float   Phi;
    };

    /**
     * Function pointer types for module interfaces.
     */
    namespace Functions
    {
        typedef const Version * (VOODOO_CALL * VersionFunc)();
        typedef const uint32_t (VOODOO_CALL * CountFunc)();
        typedef const wchar_t * (VOODOO_CALL * InfoFunc)(_In_ const uint32_t, _Out_ Uuid *);
        typedef IObject * (VOODOO_CALL * CreateFunc)(_In_ const uint32_t, _In_ ICore *);
    }

    /**
     * Creates a new core. This function is exported and meant for use by the loader.
     *
     * @param pInitParams Setup parameters for this core.
     * @param catchErrors If true, all exceptions thrown by the core ctor are caught within the function. Otherwise, they
     *      are returned.
     * @return A new ICore object.
     *
     * @throws std::exception in case of errors, if catchErrors is false.
     */
    _Check_return_ ICore * VOODOO_CALL CreateCore(_In_ const InitParams * const pInitParams, _In_ bool catchErrors = true);

    /**
     * Macro to throw Voodoo @ref VoodooShader::Exception "exceptions" with extended debug info, particularly function,
     * filename and line. These exceptions are also logged if possible (requires a valid core to be given). The Exception
     * class derives from std::exception, so these are simple to handle.
     */
#   define Throw(module, msg, core) throw Exception(module, msg, core, __FILE__, __FUNCTION__, __LINE__)
}
