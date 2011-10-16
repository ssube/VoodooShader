/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 **/
#pragma once

#include <functional>
#include <list>
#include <map>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef VSF_DEBUG_MEMORY
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif

#ifndef VOODOO_IMPORT
#   define VOODOO_API __declspec(dllexport)
#else
#   define VOODOO_API __declspec(dllimport)
#endif
#define VOODOO_CALL __stdcall

#ifndef VOODOO_NO_CG
#   include "Cg/cg.h"
#else
    typedef int CGbool;
    typedef void *CGcontext;
    typedef void *CGprogram;
    typedef void *CGparameter;
    typedef void *CGobj;
    typedef void *CGbuffer;
    typedef void *CGeffect;
    typedef void *CGtechnique;
    typedef void *CGpass;
    typedef void *CGstate;
    typedef void *CGstateassignment;
    typedef void *CGannotation;
    typedef void *CGhandle;
#endif

namespace VoodooShader
{
    /* Basic types */
    typedef bool Bool;
    typedef unsigned __int8 UInt8;
    typedef __int8 Int8;
    typedef unsigned __int16 UInt16;
    typedef __int16 Int16;
    typedef unsigned __int32 UInt32;
    typedef __int32 Int32;
    typedef unsigned __int64 UInt64;
    typedef __int64 Int64;
    typedef float Float;

    /* Custom basic types */
    class Exception;
    template <typename T> class Reference;
    class String;
    class Regex;

    namespace Xml
    {
        class Document;
        class Node;

        namespace XPath
        {
            class Query;
             
            class NodeSet;
            class NodeSetIter;
        }
    }

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

    /* Reference typedefs */
    typedef Reference<IAdapter> IAdapterRef;
    typedef Reference<ICore> ICoreRef;
    typedef Reference<IFile> IFileRef;
    typedef Reference<IFileSystem> IFileSystemRef;
    typedef Reference<IHookManager> IHookManagerRef;
    typedef Reference<IImage> IImageRef;
    typedef Reference<ILogger> ILoggerRef;
    typedef Reference<IModule> IModuleRef;
    typedef Reference<IModuleManager> IModuleManagerRef;
    typedef Reference<IObject> IObjectRef;
    typedef Reference<IParameter> IParameterRef;
    typedef Reference<IParser> IParserRef;
    typedef Reference<IPass> IPassRef;
    typedef Reference<IShader> IShaderRef;
    typedef Reference<ITechnique> ITechniqueRef;
    typedef Reference<ITexture> ITextureRef;

    /* Collections */
    typedef std::map<String, String> Dictionary;
    typedef std::map<String, IShaderRef> ShaderMap;
    typedef std::list<IShaderRef> ShaderList;
    typedef std::vector<IShaderRef> ShaderVector;
    typedef std::map<String, ITechniqueRef> TechniqueMap;
    typedef std::list<ITechniqueRef> TechniqueList;
    typedef std::vector<ITechniqueRef> TechniqueVector;
    typedef std::map<String, IPassRef> PassMap;
    typedef std::list<IPassRef> PassList;
    typedef std::vector<IPassRef> PassVector;
    typedef std::map<String, IParameterRef> ParameterMap;
    typedef std::list<IParameterRef> ParameterList;
    typedef std::vector<IParameterRef> ParameterVector;
    typedef std::map<String, ITextureRef> TextureMap;
    typedef std::list<ITextureRef> TextureList;
    typedef std::vector<ITextureRef> TextureVector;
    typedef std::map<String, IModuleRef> ModuleMap;
    typedef std::pair<IModuleRef, Int32> ClassID;
    typedef std::map<String, ClassID> ClassMap;
    typedef std::map<ITextureRef, IShaderRef> MaterialMap;

    /**
     * Texture formats for use by @ref VoodooShader::Texture "Textures", describing the layout and size of the texture data. 
     * These may not be implemented by the underlying graphics API exactly as they are indicated here, but the available 
     * components and sizes are guaranteed to be equal to or greater than the indicated values. Further information on 
     * texture formats and depth may be found on the @ref Textures "texture formats page".
     */
    enum TextureFormat
    {
        TF_Unknown  = 0x0, /* !< Unknown texture format */

        // Backbuffer formats
        TF_RGB5     = 0x101, /* !< 5 bit RGB (1 bit X in DX, may be R5G6B5 in OGL) */
        TF_RGB5A1   = 0x102, /* !< 5 bit RGB, 1 bit alpha */
        TF_RGB8     = 0x103, /* !< 8 bit RGB (8 bit X in DX). Most common backbuffer format, common texture format. */
        TF_RGBA8    = 0x104, /* !< 8 bit RGBA. Common texture format. */
        TF_RGB10A2  = 0x105, /* !< 10 bit RGB, 2 bit A */

        // Float texture formats
        TF_RGBA16F  = 0x201, /* !< Half-precision RGBA. HDR format. */
        TF_RGBA32F  = 0x202, /* !< Full-precision RGBA (float/single). HDR format. */

        // Depth-buffer formats
        TF_D16      = 0x401, /* !< Half-precision depth (Z-buffer, see @ref depthbuffers for more info) */
        TF_D32      = 0x402, /* !< Full-precision depth (Z-buffer, see @ref depthbuffers for more info) */

        // Max
        TF_Max      = 0x7FFFFFFF /* !< Highest possible value, forcing dword type */
    };

    /**
     * Parameter types for use by @ref VoodooShader::Parameter "Parameters." These
     * types are available across hardware, exposing most common variables
     */
    enum ParameterType
    {
        PT_Unknown  = 0x00, /* !< Unknown parameter type */
        // Floats
        PT_Float1   = 0x11, /* !< Single-component float vector */
        PT_Float2   = 0x12, /* !< Two-component float vector */
        PT_Float3   = 0x13, /* !< Three-component float vector */
        PT_Float4   = 0x14, /* !< Four-component float vector */
        PT_Float1x1 = 0x11,
        PT_Float1x2 = 0x12,
        PT_Float1x3 = 0x13,
        PT_Float1x4 = 0x14,
        PT_Float2x1 = 0x21,
        PT_Float2x2 = 0x22,
        PT_Float2x3 = 0x23,
        PT_Float2x4 = 0x24,
        PT_Float3x1 = 0x31,
        PT_Float3x2 = 0x32,
        PT_Float3x3 = 0x33,
        PT_Float3x4 = 0x34,
        PT_Float4x1 = 0x41,
        PT_Float4x2 = 0x42,
        PT_Float4x3 = 0x43,
        PT_Float4x4 = 0x44,
        // Samplers
        PT_Sampler1D = 0x101, /* !< One-dimensional sampler (for a 1D texture, see 
                               *    @ref texturetypes "texture types" for more info) */
        PT_Sampler2D = 0x102, /* !< Two-dimensional sampler (for a 2D texture, see 
                               *    @ref texturetypes "texture types" for more info) */
        PT_Sampler3D = 0x103, /* !< Three-dimensional sampler (for a 3D/volume texture, see 
                               *    @ref texturetypes "texture types" for more info) */
        // Structs
        PT_Struct    = 0x1000,
        // Max
        PT_Max       = 0x7FFFFFFF /* !< Highest possible value, forcing dword type */
    };

    enum ParameterCategory
    {
        PC_Unknown      = 0x00, /* !< Unknown parameter category */
        PC_Float        = 0x01, /* !< Float vector parameter (may have 1 to 4 components) */
        PC_Sampler      = 0x02, /* !< Sampler parameter (may sample 1D to 3D textures) */
        PC_Struct       = 0x04,
        PC_Max          = 0x7FFFFFFF /* !< Highest possible value, forcing dword type */
    };

    enum ProgramStage
    {
        PS_Unknown      = 0x00, /* !< Unknown program stage */
        PS_Vertex       = 0x01, /* !< Vertex program stage (usually supported, see 
                                 *    @ref programstages "program stages" for more info) */
        PS_Fragment     = 0x02, /* !< Fragment program stage (usually supported, see 
                                 *    @ref programstages "program stages" for more info) */
        PS_Geometry     = 0x03, /* !< Geometry program stage (sometimes supported, see 
                                 *    @ref programstages "program stages" for more info) */
        PS_Domain       = 0x04, /* !< Domain program stage (not always supported, see 
                                 *    @ref programstages "program stages" for more info) */
        PS_Hull         = 0x05, /* !< Hull program stage (not always supported, see 
                                 *    @ref programstages "program stages" for more info) */  
        PS_Max          = 0x7FFFFFFF /* !< Highest possible value, forcing dword type */
    };

    enum TextureStage
    {
        TS_Unknown      = 0x00, /* !< Unknown texture stage */
        TS_Shader       = 0x01, /* !< Shader target texture */
        TS_Pass         = 0x02, /* !< Pass target texture */
        TS_Max          = 0x7FFFFFFF /* !< Highest possible value, forcing dword type */
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
        LL_IntDebug     = 0x81,
        LL_IntInfo      = 0x82,
        LL_IntWarn      = 0x84,
        LL_IntError     = 0x88,
        LL_Initial      = 0xFE,
        // Masks
        LL_Severity     = 0x0F,
        LL_Origin       = 0xF0,
        LL_All          = 0xFF,
        LL_Max          = 0x7FFFFFFF
    };

    enum LogFlags 
    { 
        LF_Unknown      = 0x00, 
        LF_Flush        = 0x01, 
        LF_Max          = 0x7FFFFFFF 
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
        const char * GlobalRoot;
        const char * LocalRoot;
        const char * RunRoot;
        const char * Target;
        const char * Loader;
        const char * Config;
    };

    /**
     * Describes a texture, including size and format.
     */
    struct TextureDesc
    {
        Int32 Width;
        Int32 Height;
        Int32 Depth;
        Bool Mipmaps;
        Bool RenderTarget;
        TextureFormat Format;
    };

    /**
     * Describes a portion of a texture. This defines a cube region and provides an optional format the region should be
     * set up as.
     */
    struct TextureRegion :
        public TextureDesc
    {
        Int32 OffX;
        Int32 OffY;
        Int32 OffZ;
    };

    /**
     * Describes the precise version of a particular library, including name, main version, revision and debug status.
     */
    struct Version
    {
        const char * Name;
        Int32 Major;
        Int32 Minor;
        Int32 Patch;
        Int32 Rev;
        Bool Debug;
    };

    struct Float2
    {
        Float X, Y;
    };

    struct Float3 :
        public Float2
    {
        Float Z;
    };

    struct Float4 :
        public Float3
    {
        Float W;
    };

    /**
     * Pre-transformed vertex structure
     */
    struct VertexStruct
    {
        Float3 Position;
        Float Winding;
        Float2 TexCoord;
    };

    /**
     * Shader-compatible light structure
     */
    struct LightStruct
    {
        Int32 Type;
        Float4 Diffuse;
        Float4 Specular;
        Float4 Ambient;
        Float3 Position;
        Float3 Direction;
        Float Range;
        Float Falloff;
        Float3 Attenuation;
        Float Theta;
        Float Phi;
    };

    /**
     * Function pointer types for module interfaces. 
     */
    namespace Functions
    {
        typedef std::function<Int32 (void)> CountFunc;
        typedef std::function<String (Int32)> InfoFunc;
        typedef std::function<IObject * (Int32, ICore *)> CreateFunc;
        typedef std::function<Version ()> VersionFunc;
    };

    /**
     * Macro to throw Voodoo @ref VoodooShader::Exception "exceptions" with extended debug info, particularly function,
     * filename and line. These exceptions are also logged if possible (requires a valid core to be given). The Exception
     * class derives from std::exception, so these are simple to handle.
     */
#   define Throw(module, msg, core) throw Exception(module, msg, core, __FILE__, __FUNCTION__, __LINE__)
}