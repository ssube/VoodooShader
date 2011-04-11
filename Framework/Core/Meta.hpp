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
    class FullscreenManager;
    class MaterialManager;
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
        typedef int          (*CountFunc  )();
        typedef const char * (*InfoFunc   )(int);
        typedef IObject *    (*CreateFunc )(int, Core *);
        typedef Version      (*VersionFunc)();
    };

    typedef std::string                         String;
    //typedef std::shared_ptr<Core>               CoreRef;
    //typedef std::weak_ptr<Core>                 CorePtr;
    typedef std::shared_ptr<FullscreenManager>  FullscreenManagerRef;
    typedef std::weak_ptr<FullscreenManager>    FullscreenManagerPtr;
    typedef std::shared_ptr<MaterialManager>    MaterialManagerRef;
    typedef std::weak_ptr<MaterialManager>      MaterialManagerPtr;
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
    typedef std::pair<ModulePtr, int>           ClassID; /*!< Defines the necessary data to create a class (source module and class number). */
    typedef std::map<String, ClassID>           ClassMap; /*!< Provides a map of classes, keyed by name. */

    // Miscellaneous collections
    typedef std::map<TextureRef, ShaderRef>     MaterialMap;
    typedef std::map<String, String>            Dictionary;

    /**
     * Texture formats for use by @ref VoodooShader::Texture "Textures",
     * describing the layout and size of the texture data. These 
     * may not be  implemented by the underlying graphics API exactly as they 
     * are indicated  here, but the available components and sizes are 
     * guaranteed to be equal to or greater than the indicated values. Further 
     * information on texture formats and depth may be found on the 
     * @ref Textures "texture formats page". 
     */
    enum TextureFormat
    {
        TF_Unknown = 0,         /*!< Unknown texture format */
        // Backbuffer formats
        TF_RGB5,                /*!< 5 bit RGB (1 bit X in DX, may be R5G6B5 in OGL) */
        TF_RGB5A1,              /*!< 5 bit RGB, 1 bit alpha */
        TF_RGB8,                /*!< 8 bit RGB (8 bit X in DX) */
        TF_RGBA8,               /*!< 8 bit RGBA */
        TF_RGB10A2,             /*!< 10 bit RGB, 2 bit A */
        // Float texture formats
        TF_RGBA16F,             /*!< Half-precision RGBA */
        TF_RGBA32F,             /*!< Full-precision RGBA (float/single) */
        // Depth-buffer formats
        TF_D16,                 /*!< Half-precision depth (Z-buffer, see @ref depthbuffers "the depth buffers notes" for more info) */
        TF_D32,                 /*!< Full-precision depth (Z-buffer, see @ref depthbuffers "the depth buffers notes" for more info) */
        // Padding value
        TF_Max = 0xFFFFFFFF,    /*!< Highest possible value, forcing dword type */
    };

    /**
     * Parameter types for use by @ref VoodooShader::Parameter "Parameters." These types are
     * available across hardware, exposing most common variables.
     */
    enum ParameterType
    {
        PT_Unknown = 0,         /*!< Unknown parameter type */
        // Float-vectors
        PT_Float1,              /*!< Single-component float vector */
        PT_Float2,              /*!< Two-component float vector */
        PT_Float3,              /*!< Three-component float vector */
        PT_Float4,              /*!< Four-component float vector */
        // Matrices
        PT_Matrix,              /*!< Generic float4x4 matrix type */
        // Samplers
        PT_Sampler1D,           /*!< One-dimensional sampler (for a 1D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler2D,           /*!< Two-dimensional sampler (for a 2D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler3D,           /*!< Three-dimensional sampler (for a 3D/volume texture, see @ref texturetypes "texture types" for more info) */
        // Padding value
        PT_Max = 0xFFFFFFFF,    /*!< Highest possible value, forcing dword type */
    };

    enum ParameterCategory
    {
        PC_Unknown = 0,         /*!< Unknown parameter category */
        // Valid categories
        PC_Float,               /*!< Float vector parameter (may have 1 to 4 components) */
        PC_Sampler,             /*!< Sampler parameter (may sample 1D to 3D textures) */
        PC_Matrix,              /*!< Matrix parameter, 4x4 components */
        // Padding value
        PC_Max = 0xFFFFFFFF,    /*!< Highest possible value, forcing dword type */
    };

    enum ProgramStage
    {
        PS_Unknown = 0,         /*!< Unknown program stage */
        // Basic stages
        PS_Vertex,              /*!< Vertex program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Fragment,            /*!< Fragment program stage (usually supported, see @ref programstages "program stages" for more info) */
        // Geometry stages
        PS_Geometry,            /*!< Geometry program stage (sometimes supported, see @ref programstages "program stages" for more info) */
        // Tessellation stages
        PS_Domain,              /*!< Domain program stage (not always supported, see @ref programstages "program stages" for more info) */
        PS_Hull,                /*!< Hull program stage (not always supported, see @ref programstages "program stages" for more info) */
        // Padding value
        PS_Max = 0xFFFFFFFF,    /*!< Highest possible value, forcing dword type */
    };

    enum TextureType
    {
        TT_Unknown = 0,         /*!< Unknown texture type */
        // Generic types
        TT_Generic,             /*!< Generic texture type, no special function */
        // Special types
        TT_ShaderTarget,        /*!< Shader target texture */
        TT_PassTarget,          /*!< Pass target texture */
        // Padding value
        TT_Max = 0xFFFFFFFF,    /*!< Highest possible value, forcing dword type */
    };

    enum FileOpenMode
    {
        FM_Unknown  = 0x00,     /*!< Unknown file open mode */
        // Valid modes
        FM_Read     = 0x01,     /*!< Read-only file open mode */
        FM_Write    = 0x02,     /*!< Read/write file open mode */
        // Padding value
        FM_Max = 0xFFFFFFFF,    /*!< Highest possible value, forcing dword type */
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
        LL_Unknown      = 0x00,     /*!< Log level unknown */
        // Severity values
        LL_Debug        = 0x01,
        LL_Info         = 0x02,
        LL_Warning      = 0x04,
        LL_Error        = 0x08,
        LL_Severity     = 0x0F,
        // Origin values
        LL_API          = 0x10,
        LL_Framework    = 0x20,
        LL_Adapter      = 0x40,
        LL_Origin       = 0xF0,
        //LL_Framework    = 0x80,
        // Logger internal values
        LL_Internal     = 0x82,     /*!< Log level for Logger-internal messages */
        LL_Initial      = 0xFE,     /*!< Initial log level of Logger */
        // Working values
        LL_All          = 0xFF,     /*!< All messages will be logged */
        // Padding value
        LL_Max = 0xFFFFFFFF,        /*!< Highest possible value, forcing dword type */
    };

    /**
     * String parsing flags. These modify the behavior of the string parser.
     */
    enum ParseFlags
    {
        PF_None             = 0x00,     /*!< No special behavior */
        // Slash flags
        PF_SingleSlash      = 0x01,     /*!< No repeated slashes (useful for XPath). */
        PF_SlashOnly        = 0x02,     /*!< Replace all backslashes with forward slashes. */
        PF_BackslashOnly    = 0x04,     /*!< Replace all forward slashes with backslashes. */
        // Case flags
        PF_Lowercase        = 0x08,
        PF_Uppercase        = 0x10,
        // State blocks
        PF_RetainState      = 0x20,
        // Padding value
        PF_Max = 0xFFFFFFFF,            /*!< Highest possible value, forcing dword type */
    };
    /**
     * @}
     */

    /**
     * Describes a texture, including size and format.
     */
    struct TextureDesc
    {
        size_t Width;
        size_t Height;
        size_t Depth;
        bool Mipmaps;
        TextureFormat Format;
    };

    /**
     * Describes a portion of a texture. This defines a cube region and provides
     * an optional format the region should be set up as.
     */
    struct TextureRegion
    {
        size_t OffX;
        size_t OffY;
        size_t OffZ;
        size_t Width;
        size_t Height;
        size_t Depth;
        TextureFormat Format;
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
};

#endif /*VOODOO_META_HPP*/
