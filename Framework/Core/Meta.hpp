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
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#ifndef VOODOO_META_HPP
#define VOODOO_META_HPP

#ifndef VOODOO_IMPORT
#    define VOODOO_API __declspec(dllexport)
#else
#    define VOODOO_API __declspec(dllimport)
#endif

// Hide the DLL-interface warning
#pragma warning(disable:4251)

#include "Includes.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    /* Basic types */
    typedef bool                Boolean;

    typedef unsigned __int8     UInt8;
    typedef __int8              Int8;
    typedef unsigned __int16    UInt16;
    typedef __int16             Int16;
    typedef unsigned __int32    UInt32;
    typedef __int32             Int32;
    typedef unsigned __int64    UInt64;
    typedef __int64             Int64;

    typedef std::string         String;

    /**
     * Classes implemented entirely within the core module (these do not derive from IObject).
     * These core classes are available to all Voodoo modules and addons, but use requires
     * linking against the core library. These provide most of the basic functions needed by
     * the framework.
     * 
     * @addtogroup VoodooCore Voodoo/Core
     * @{
     */
    class Converter;
    class Core;
    class Exception;
    class Module;
    class ModuleManager;
    class Parameter;
    class Parser;
    class Pass;
    class Technique;
    class Texture;
    class Shader;

    /**  
     * Interface classes, providing standard access to various dynamic
     * objects. These are not implemented within the core but are instead
     * dynamically linked at runtime. All are derived from IObject.
     */
    class IObject;
    class IAdapter;
    class IFileSystem;
    class IFile;
    class IHookManager;
    class IImage;
    class ILogger;
    
    /**
     * Generic structs for passing simple data sets. Structs have no ctor
     * or dtor, nor methods; they should be created with the <code>{m,n,o}</code>
     * syntax. Macros may be available to construct some struct types.
     */
    struct TextureDesc;
    struct Version;

     /**
      * Macro to throw Voodoo @ref VoodooShader::Exception "exceptions" with extended debug info, 
      * particularly function, filename and line. These exceptions are also logged if possible 
      * (requires a valid core to be given). The Exception class derives from std::exception, so 
      * these are simple to handle.
      */
#define Throw(module, msg, core) throw Exception(module, msg, core, __FILE__, __FUNCTION__, __LINE__)

    /**
     * Function pointer types for module interfaces.
     */
    namespace Functions
    {
        typedef Int32        (*CountFunc  )();
        typedef const char * (*InfoFunc   )(Int32);
        typedef IObject *    (*CreateFunc )(Int32, Core *);
        typedef Version      (*VersionFunc)();
    };

    typedef std::shared_ptr<Module>             ModuleRef;
    typedef std::weak_ptr<Module>               ModulePtr;
    typedef std::shared_ptr<ModuleManager>      ModuleManagerRef;
    typedef std::weak_ptr<ModuleManager>        ModuleManagerPtr;
    typedef std::shared_ptr<Parameter>          ParameterRef;
    typedef std::weak_ptr<Parameter>            ParameterPtr;
    typedef std::shared_ptr<Parser>             ParserRef;
    typedef std::weak_ptr<Parser>               ParserPtr;
    typedef std::shared_ptr<Pass>               PassRef;
    typedef std::weak_ptr<Pass>                 PassPtr;
    typedef std::shared_ptr<Shader>             ShaderRef;
    typedef std::weak_ptr<Shader>               ShaderPtr;
    typedef std::shared_ptr<Technique>          TechniqueRef;
    typedef std::weak_ptr<Technique>            TechniquePtr;
    typedef std::shared_ptr<Texture>            TextureRef;
    typedef std::weak_ptr<Texture>              TexturePtr;

    typedef std::shared_ptr<IAdapter>           IAdapterRef;
    typedef std::shared_ptr<IFile>              IFileRef;
    typedef std::shared_ptr<IFileSystem>        IFileSystemRef;
    typedef std::shared_ptr<IHookManager>       IHookManagerRef;
    typedef std::shared_ptr<IImage>             IImageRef;
    typedef std::shared_ptr<ILogger>            ILoggerRef;

    /**
     * Collection types for most common objects. These provide
     * specialized containers and save typing.
     */
    // Shader collections
    typedef std::map<String, ShaderRef>         ShaderMap;
    typedef std::list<ShaderRef>                ShaderList;
    typedef std::vector<ShaderRef>              ShaderVector;

    // Technique collections
    typedef std::map<String, TechniqueRef>      TechniqueMap;
    typedef std::list<TechniqueRef>             TechniqueList;
    typedef std::vector<TechniqueRef>           TechniqueVector;

    // Pass collections
    typedef std::map<String, PassRef>           PassMap;
    typedef std::list<PassRef>                  PassList;
    typedef std::vector<PassRef>                PassVector;

    // Parameter collections
    typedef std::map<String, ParameterRef>      ParameterMap;
    typedef std::list<ParameterRef>             ParameterList;
    typedef std::vector<ParameterRef>           ParameterVector;

    // Texture collections
    typedef std::map<String, TextureRef>        TextureMap;
    typedef std::list<TextureRef>               TextureList;
    typedef std::vector<TextureRef>             TextureVector;

    // Module management types
    typedef std::map<String, ModuleRef>         ModuleMap; /*!< Provides a map of modules, keyed by name. */
    typedef std::pair<ModulePtr, int>           ClassID;   /*!< Defines the necessary data to create 
                                                                a class (source module and class number). */
    typedef std::map<String, ClassID>           ClassMap;  /*!< Provides a map of classes, keyed by name. */

    // Miscellaneous collections
    typedef std::map<TextureRef, ShaderRef>     MaterialMap;
    typedef std::map<String, String>            Dictionary;

    /**
     * Texture formats for use by @ref VoodooShader::Texture "Textures", describing the layout and size of the texture data. 
     * These may not be  implemented by the underlying graphics API exactly as they are indicated  here, but the available 
     * components and sizes are guaranteed to be equal to or greater than the indicated values. Further information on 
     * texture formats and depth may be found on the @ref Textures "texture formats page". 
     */
    enum TextureFormat
    {
        TF_Unknown = 0,         /*!< Unknown texture format */
        // Backbuffer formats
        TF_RGB5,                /*!< 5 bit RGB (1 bit X in DX, may be R5G6B5 in OGL) */
        TF_RGB5A1,              /*!< 5 bit RGB, 1 bit alpha */
        TF_RGB8,                /*!< 8 bit RGB (8 bit X in DX). Most common backbuffer format, common texture format. */
        TF_RGBA8,               /*!< 8 bit RGBA. Common texture format. */
        TF_RGB10A2,             /*!< 10 bit RGB, 2 bit A */
        // Float texture formats
        TF_RGBA16F,             /*!< Half-precision RGBA. HDR format. */
        TF_RGBA32F,             /*!< Full-precision RGBA (float/single). HDR format. */
        // Depth-buffer formats
        TF_D16,                 /*!< Half-precision depth (Z-buffer, see @ref depthbuffers for more info) */
        TF_D32,                 /*!< Full-precision depth (Z-buffer, see @ref depthbuffers for more info) */
        // Max
        TF_Max = 0x7FFFFFFF,    /*!< Highest possible value, forcing dword type */
    };

    /**
     * Parameter types for use by @ref VoodooShader::Parameter "Parameters." These types are
     * available across hardware, exposing most common variables.
     */
    enum ParameterType
    {    
        PT_Unknown      = 0x00,         /*!< Unknown parameter type */    
        // Floats
        PT_Float1       = 0x11,         /*!< Single-component float vector */
        PT_Float2       = 0x12,         /*!< Two-component float vector */
        PT_Float3       = 0x13,         /*!< Three-component float vector */
        PT_Float4       = 0x14,         /*!< Four-component float vector */
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
        PT_Sampler1D    = 0x101,        /*!< One-dimensional sampler (for a 1D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler2D    = 0x102,        /*!< Two-dimensional sampler (for a 2D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler3D    = 0x103,        /*!< Three-dimensional sampler (for a 3D/volume texture, see @ref texturetypes "texture types" for more info) */
        // Structs
        PT_Struct       = 0x1000,
        // Max
        PT_Max          = 0x7FFFFFFF    /*!< Highest possible value, forcing dword type */
    };

    enum ParameterCategory
    {    
        PC_Unknown      = 0x00,         /*!< Unknown parameter category */
        PC_Float        = 0x01,         /*!< Float vector parameter (may have 1 to 4 components) */
        PC_Sampler      = 0x02,         /*!< Sampler parameter (may sample 1D to 3D textures) */
        PC_Struct       = 0x04,
        PC_Max          = 0x7FFFFFFF    /*!< Highest possible value, forcing dword type */   
    };

    enum ProgramStage
    {
        PS_Unknown      = 0x00,         /*!< Unknown program stage */
        PS_Vertex       = 0x01,         /*!< Vertex program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Fragment     = 0x02,         /*!< Fragment program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Geometry     = 0x03,         /*!< Geometry program stage (sometimes supported, see @ref programstages "program stages" for more info) */
        PS_Domain       = 0x04,         /*!< Domain program stage (not always supported, see @ref programstages "program stages" for more info) */
        PS_Hull         = 0x05,         /*!< Hull program stage (not always supported, see @ref programstages "program stages" for more info) */
        PS_Max          = 0x7FFFFFFF    /*!< Highest possible value, forcing dword type */ 
    };

    enum TextureStage
    {
        TS_Unknown      = 0x00,         /*!< Unknown texture type */
        TS_Shader       = 0x01,         /*!< Shader target texture */
        TS_Pass         = 0x02,         /*!< Pass target texture */
        TS_Max          = 0x7FFFFFFF    /*!< Highest possible value, forcing dword type */  
    };

    enum FileOpenMode
    {
        FM_Unknown      = 0x00,
        FM_Read         = 0x01,
        FM_Write        = 0x02,
        FM_ReadWrite    = 0x03,
        FM_CreateOnly   = 0x10,
        FM_OpenOnly     = 0x20,
        FM_AlwaysOpen   = 0x30,
        FM_Max          = 0x7FFFFFFF
    };

    /**
     * Log message levels. These are set up to quickly filter based on various flags using binary
     * logic. Flags are ORed together, with lower-priority flags having all higher-priority flags
     * set. When checking a log call, the condition is:
     * @code
     * LogLevel maskedLevel = level & storedLevel;
     * if ( (maskedLevel & LL_Origin) && (maskedLevel & LL_Severity) )
     * { 
     *     // Log
     * @endcode
     */
    enum LogLevel
    {
        LL_Unknown      = 0x00,
        LL_Debug        = 0x01,
        LL_Info         = 0x02,
        LL_Warning      = 0x04,
        LL_Error        = 0x08,
        LL_External     = 0x10,
        LL_Plugin       = 0x20,
        LL_Module       = 0x40,
        LL_Framework    = 0x80,
        LL_Internal     = 0x82,
        LL_Initial      = 0xFE,
        LL_All          = 0xFF,
        LL_Max          = 0x7FFFFFFF
    };

    /**
     * String parsing flags. These modify the behavior of the string parser.
     */
    enum ParseFlags
    {
        PF_None         = 0x0000,
        PF_SingleSlash  = 0x0001,
        PF_SlashOnly    = 0x0002,
        PF_BackslashOnly= 0x0004,
        PF_Lowercase    = 0x0010,
        PF_Uppercase    = 0x0020,
        PF_VarName      = 0x0100,
        PF_Max          = 0x7FFFFFFF
    };

    enum VariableType
    {
        VT_Normal       = 0x00,
        VT_State        = 0x01,
        VT_System       = 0x10,
        VT_Max          = 0x7FFFFFFF
    };
    /**
     * @}
     */

    /**
     * Initialization parameters.
     */
    struct InitParams
    {
        String GlobalRoot;
        String LocalRoot;
        String RunRoot;
        String Target;
        String Loader;
        String Config;
    };

    /**
     * Describes a texture, including size and format.
     */
    struct TextureDesc
    {
        int Width;
        int Height;
        int Depth;
        bool Mipmaps;
        bool RenderTarget;
        TextureFormat Format;
    };

    /**
     * Describes a portion of a texture. This defines a cube region and provides
     * an optional format the region should be set up as.
     */
    struct TextureRegion
        : public TextureDesc
    {
        int OffX;
        int OffY;
        int OffZ;
    };

    /**
     * Describes the precise version of a particular library, including name, 
     * main version, revision and debug status.
     */
    struct Version
    {
        String Name;
        int Major;
        int Minor;
        long Patch;
        long Rev;
        bool Debug;
    };

    struct float2
    {
        float X, Y;
    };

    struct float3
        : public float2
    {
        float Z;
    };

    struct float4
        : public float3
    {
        float W;
    };

    /**
     * Pretransformed vertex structure
     */
    struct VertexStruct
    {
        float3  Position;
        float   Winding;
        float2  TexCoord;
    };

    /**
     * Shader-compatible light structure
     */
    struct LightStruct
    {
        int     Type;
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
};

#endif /*VOODOO_META_HPP*/
