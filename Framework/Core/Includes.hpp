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
#pragma once

#ifndef VOODOO_NO_COMPILER_CHECKS
#   ifndef __cplusplus
#      error Voodoo requires a C++ compiler, preferably Microsoft Visual C++ 10 or better.
#   endif

#   ifndef _WIN32
#      error Voodoo must be built for the Win32 platform.
#   endif

#   ifdef _WIN64
#       error Voodoo is not yet compatible with 64-bit systems.
#   endif

#   ifndef _UNICODE
#       error Voodoo APIs require Unicode characterset enabled.
#   endif
#endif

#ifndef _NATIVE_NULLPTR_SUPPORTED
#   define nullptr NULL
#endif

#ifndef VOODOO_STRING_MACROS
#   define VOODOO_STRING_MACROS
#   define VOODOO_META_STRING_ARG(arg)  L ## #arg
#   define VOODOO_META_STRING_STR(arg)  L ## arg

#   define VOODOO_META_TOSTRING(arg)    VOODOO_META_STRING_ARG(arg)
#   define VOODOO_META_STRING(arg)      VOODOO_META_STRING_STR(arg)
#   define VSTR(arg)                    VOODOO_META_STRING_STR(arg)
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

#ifndef PURE
#   define PURE = 0
#endif

#ifndef CONST
#   define CONST const
#endif

#ifndef DECLSPEC_NOTHROW
#   define DECLSPEC_NOTHROW __declspec(nothrow)
#endif

#ifndef DECLSPEC_SELECTANY
#   define DECLSPEC_SELECTANY __declspec(selectany)
#endif

#ifndef DECLSPEC_NOVTABLE
#   define DECLSPEC_NOVTABLE __declspec(novtable)
#endif

// Extended memory debug routines.
// This will enable a lot of in-depth logging for a lot of memory data and will have a major speed hit.
#ifdef VOODOO_DEBUG_MEMORY
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

#define VOODOO_CALLTYPE     __stdcall
#define VOODOO_METHODTYPE   VOODOO_CALLTYPE

#define VOODOO_METHOD_(type, name)  virtual DECLSPEC_NOTHROW type VOODOO_CALLTYPE name
#define VOODOO_METHOD(name)         VOODOO_METHOD_(bool, name)

#define VOODOO_INTERFACE_(iname, ...)       DEFINE_IID(iname) = __VA_ARGS__; class DECLSPEC_NOVTABLE iname
#define VOODOO_INTERFACE(iname, ibase, ...) VOODOO_INTERFACE_(iname, __VA_ARGS__) : public ibase
#define VOODOO_CLASS(iname, ibase, ...)     DEFINE_CLSID(iname) = __VA_ARGS__; class iname : public ibase

#define VOODOO_PUBLIC_FUNC VOODOO_API VOODOO_CALLTYPE

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
    class RegexMatch;
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

    /* Refcount handlers */
#ifdef VOODOO_THREAD_SAFE
#   define SAFE_INCREMENT(x) InterlockedIncrement(&x)
#   define SAFE_DECREMENT(x) InterlockedDecrement(&x)
#else
#   define SAFE_INCREMENT(x) ++x
#   define SAFE_DECREMENT(x) --x
#endif

    /**
     * @defgroup voodoo_uuids Voodoo UUIDs
     * @{
     */
#define DEFINE_UUID(name)     EXTERN_C CONST Uuid DECLSPEC_SELECTANY name
#define DEFINE_IID(name)      DEFINE_UUID(IID_##name)
#define DEFINE_CLSID(name)    DEFINE_UUID(CLSID_##name)
#define DEFINE_LIBID(name)    DEFINE_UUID(LIBID_##name)
    /**
     * @}
     */

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
    enum TextureFormat : uint32_t
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
    enum ParameterType : uint32_t
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

    enum ParameterCategory : uint32_t
    {
        PC_Unknown      = 0x00,     /* !< Unknown parameter category */
        PC_Float        = 0x01,     /* !< float vector parameter (may have 1 to 4 components) */
        PC_Sampler      = 0x02,     /* !< Sampler parameter (may sample 1D to 3D textures) */
        PC_Struct       = 0x04
    };

    enum ProgramStage : uint32_t
    {
        PS_Unknown      = 0x00,     /* !< Unknown program stage */
        PS_Vertex       = 0x01,     /* !< Vertex program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Fragment     = 0x02,     /* !< Fragment program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Geometry     = 0x03,     /* !< Geometry program stage (sometimes supported, see @ref programstages "program stages" for more info) */
        PS_Domain       = 0x04,     /* !< Domain program stage (not always supported, see @ref programstages "program stages" for more info) */
        PS_Hull         = 0x05      /* !< Hull program stage (not always supported, see  @ref programstages "program stages" for more info) */
    };

    enum TextureStage : uint32_t
    {
        TS_Unknown      = 0x00,     /* !< Unknown texture stage */
        TS_Shader       = 0x01,     /* !< Shader target texture */
        TS_Pass         = 0x02      /* !< Pass target texture */
    };

    enum FileOpenMode : uint32_t
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
     * Log message levels. These are set up to quickly filter messages based on severity and source. Each message must have
     * a severity and source bit set, which the logger tests against its internal filter, like so:

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
        LL_Initial      = 0xFC,
        // Masks
        LL_Severity     = 0x0F,
        LL_Origin       = 0xF0,
        LL_All          = 0xFF
    };

    enum LogFlags  : uint32_t
    {
        LF_Unknown      = 0x00,
        LF_Flush        = 0x01
    };

    /**
     * String parsing flags. These modify the behavior of the string parser.
     */
    enum ParseFlags : uint32_t
    {
        PF_None         = 0x00,

        PF_SlashSingle  = 0x0001,
        PF_SlashDouble  = 0x0002,
        PF_SlashOnly    = 0x0004,
        PF_SlashBack    = 0x0008,
        PF_SlashTrail   = 0x0010,

        PF_PathRoot     = 0x001000,
        PF_PathOnly     = 0x002000,
        PF_PathFile     = 0x004000,
        PF_PathExt      = 0x008000,
        PF_PathCanon    = 0x010000,

        PF_SlashFlags   = 0x0000001F,
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
     * Describes the precise version of a particular library, including name, main version, revision and debug status.
     */
    struct Version
    {
        Uuid            LibID;
        int32_t         Major;
        int32_t         Minor;
        int32_t         Patch;
        int32_t         Build;
        bool            Debug;
        const wchar_t * Name;
        const wchar_t * RevID;
    };

    /**
     * Property variant type.
     */
    struct Variant
    {
        UnionType Type;
        union
        {
            bool        VBool;
            int8_t      VInt8;
            uint8_t     VUInt8;
            int16_t     VInt16;
            uint16_t    VUInt16;
            int32_t     VInt32;
            uint32_t    VUInt32;
            float       VFloat;
            double      VDouble;
            Uuid *      VUuid;
            String *    VString;
            IObject *   VIObject;
            void *      VPVoid;
        } Value;
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

    typedef Vector2<float>      Float2;
    typedef Vector3<float>      Float3;
    typedef Vector4<float>      Float4;
    typedef Vector2<int32_t>    Int2;
    typedef Vector3<int32_t>    Int3;
    typedef Vector4<int32_t>    Int4;
    typedef Vector2<uint32_t>   UInt2;
    typedef Vector3<uint32_t>   UInt3;
    typedef Vector4<uint32_t>   UInt4;

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
        Float4 Position;
        Float4 Color;
        Float4 TexCoord[2];
    };

    /**
     * Shader-compatible light structure
     */
    struct LightStruct
    {
        int32_t Type;
        Float4  Diffuse;
        Float4  Specular;
        Float4  Ambient;
        Float3  Position;
        Float3  Direction;
        float   Range;
        float   Falloff;
        Float3  Attenuation;
        float   Theta;
        float   Phi;
    };

    /**
     * Function pointer types for module interfaces.
     */
    namespace Functions
    {
        typedef const Version * (VOODOO_CALLTYPE * ModuleVersionFunc)();
        typedef const uint32_t  (VOODOO_CALLTYPE * ModuleCountFunc)();
        typedef const wchar_t * (VOODOO_CALLTYPE * ModuleInfoFunc)(_In_ const uint32_t, _Out_ Uuid *);
        typedef IObject *       (VOODOO_CALLTYPE * ModuleCreateFunc)(_In_ const uint32_t, _In_ ICore *);
        typedef ICore *         (VOODOO_CALLTYPE * CoreCreateFunc)(_In_ const InitParams * const);
    }
}
