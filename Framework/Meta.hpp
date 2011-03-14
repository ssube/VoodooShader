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

#ifdef _DEBUG
#   define VOODOO_DEBUG
#   define VOODOO_DEBUG_VALUE 1
#else
#   define VOODOO_DEBUG_VALUE 0
#endif

// Hide the DLL-interface warning
#pragma warning(disable:4251)

#include "Includes.hpp"
#include "Reference.hpp"

#define VOODOO_CORE_COPYRIGHT\
    "Voodoo Shader Framework, Copyright (c) 2010-2011 by Sean Sube&lt;br /&gt;\
    The Voodoo Shader Framework comes with ABSOLUTELY NO WARRANTY.&lt;br /&gt;\
    This is free software and you are welcome to redistribute it under certain conditions.&lt;br /&gt;\
    Please see the included license file for more details.&lt;/div&gt;"

namespace VoodooShader
{
    class Core;
    class Adapter;
    class Logger;

    class FullscreenManager;
    class MaterialManager;

    class Shader;
    class Texture;
    
    class Technique;
    class Pass;
    class Parameter;

    class FileSystem;
    class File;
    class Image;

    class Exception;

    class Converter;

#define Throw(module, msg, core) throw Exception(module, msg, core, __FILE__, __FUNCTION__, __LINE__);

    typedef std::string String;

    // Reference-counted pointer types
    typedef Reference<Shader>                   ShaderRef;
    typedef Reference<Texture>                  TextureRef;
    typedef Reference<Technique>                TechniqueRef;
    typedef Reference<Pass>                     PassRef;
    typedef Reference<Parameter>                ParameterRef;
    typedef Reference<File>                     FileRef;
    typedef Reference<Image>                    ImageRef;

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

    // Miscellaneous collections
    typedef std::map<TextureRef, ShaderRef>     MaterialMap;
    typedef std::map<String, CGeffect>          CGEffectMap;
    typedef std::vector<String>                 StringVector;
    typedef std::list<String>                   StringList;

    /**
     * Texture formats for use by @ref VoodooShader::Texture "Textures". These 
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
        // Max
        TF_Count                /*!< Enumerator values count. */
    };

    /**
     * Parameter types for use by @ref VoodooShader::Parameter "Parameters." 
     * These are generally handled within the Cg runtime and rarely change in
     * hardware, the common ones (float4 and such) are identical across the 
     * board.
     */
    enum ParameterType
    {
        PT_Unknown = 0,         /*!< Unknown parameter type */
        // Float-vectors
        PT_Float1,              /*!< Single-component float vector */
        PT_Float2,              /*!< Two-component float vector */
        PT_Float3,              /*!< Three-component float vector */
        PT_Float4,              /*!< Four-component float vector */
        // Samplers
        PT_Sampler1D,           /*!< One-dimensional sampler (for a 1D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler2D,           /*!< Two-dimensional sampler (for a 2D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler3D,           /*!< Three-dimensional sampler (for a 3D/volume texture, see @ref texturetypes "texture types" for more info) */
        // Matrices
        PT_Matrix,              /*!< Generic float4x4 matrix type */
        // Max
        PT_Count                /*!< Enumerator values count */
    };

    enum ParameterCategory
    {
        PC_Unknown = 0,         /*!< Unknown parameter category */
        // Valid categories
        PC_Float,               /*!< Float vector parameter (may have 1 to 4 components) */
        PC_Sampler,             /*!< Sampler parameter (may sample 1D to 3D textures) */
        PC_Matrix,              /*!< Matrix parameter, 4x4 components */
        // Max
        PC_Count                /*!< Enumerator values count */
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
        // Max
        PS_Count                /*!< Enumerator values count */
    };

    enum TextureType
    {
        TT_Unknown = 0,         /*!< Unknown texture type */
        // Generic types
        TT_Generic,             /*!< Generic texture type, no special function */
        // Special types
        TT_ShaderTarget,        /*!< Shader target texture */
        TT_PassTarget,          /*!< Pass target texture */
        // Max
        TT_Count                /*!< Enumerator values count */
    };

    enum FileOpenMode
    {
        FM_Unknown  = 0x00,     /*!< Unknown file open mode */
        // Valid modes
        FM_Read     = 0x01,     /*!< Read-only file open mode */
        FM_Write    = 0x02,     /*!< Read/write file open mode */
        // Max
        FM_Count                /*!< Enumerator values count */
    };

    /**
     * Log message levels. These are spread out so additional levels can be
     * added in the future without affecting behavior. The lower the value,
     * the more verbose and less likely to be logged a message is. The Logger
     * object discards messages less than its internal value. Warnings and
     * errors, especially fatal errors, should always be logged.
     * 
     * @note The enum values here indicate the <em>lowest</em> value
     *          associated with that message type. All values up to the next
     *          enum value are considered part of the same type.
     */
    enum LogLevel
    {
        LL_Unknown  = 0x00,     /*!< Log level unknown */
        // Working values
        LL_Debug    = 0x10,     /*!< Verbose debug log messages */
        LL_Info     = 0x30,     /*!< Informational log messages */
        LL_Warning  = 0x50,     /*!< Warning log messages */
        LL_Error    = 0xB0,     /*!< General error log messages */
        LL_Fatal    = 0xF0,     /*!< Fatal error log messages */
        // Max value
        LL_Max                  /*!< Enumerator values count */
    };

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
     * Describes the precise version of a particular library, including name, 
     * main version and revision.
     * 
     * @todo Decide if Version should contain module name or UID (potentially useful
     *          for Adapter ID).
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

#endif /*VOODOO_META_HPP*/
