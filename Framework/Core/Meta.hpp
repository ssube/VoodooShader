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

#include "Includes.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    /**
     * Interfaces for core classes and functionality. Implementations for all of these are
     * available within the core.
     * 
     * @note For IAdapter, IFileSystem, IHookManager and ILogger, only null implementations are
     *    given. These may provide limited or partial functionality.
     * 
     * @addtogroup VoodooCore Voodoo/Core
     * @{
     */
    
    /**
     * Generic structs for passing simple data sets. Macros may be available to construct some 
     * struct types.
     */
    struct TextureDesc;
    struct TextureRegion;
    struct Version;
    struct Vertex;

    /**
     * Standard interfaces.
     */
    interface DECLSPEC_UUID("76073706-54E6-457D-B891-52A80410BEEF") IVoodooAdapter;
    interface DECLSPEC_UUID("94158AA0-87AD-4F99-89F2-7E7B8E9D5CDB") IVoodooConverter;
    interface DECLSPEC_UUID("31420F71-1050-45D9-838F-8735BBB8EDEA") IVoodooCore;
    interface DECLSPEC_UUID("73926B89-5899-4E58-A714-851812188F15") IVoodooFile;
    interface DECLSPEC_UUID("229B5CF6-99BF-4250-9088-128FE299767E") IVoodooFileSystem;
    interface DECLSPEC_UUID("4939253A-71F0-44F5-94ED-711B62E6B4DB") IVoodooFsManager;
    interface DECLSPEC_UUID("38C848CD-915A-49EB-92A4-A463053C62A8") IVoodooHookSystem;
    interface DECLSPEC_UUID("7EF1C1F0-A474-4115-BF5A-441C83474536") IVoodooImage;
    interface DECLSPEC_UUID("41482ECA-13DA-4D12-B254-F7E55D4DC28D") IVoodooLogger;
    interface DECLSPEC_UUID("F11D985D-5865-4646-8CBB-44ECDDEE371E") IVoodooMatManager;
    interface DECLSPEC_UUID("FC21372A-6EA6-4CA5-811C-5ED4835973B7") IVoodooParameter;
    interface DECLSPEC_UUID("41F3AEE5-0F0E-4BF4-9512-DC290E37EB7B") IVoodooParser;
    interface DECLSPEC_UUID("C8BEBCB5-B62F-48B8-82CE-802220B5E930") IVoodooPass;
    interface DECLSPEC_UUID("1BD418B4-7D6C-4CFB-8138-339D26ACB378") IVoodooTechnique;
    interface DECLSPEC_UUID("0F33959B-242E-4282-8510-BE6C9997F464") IVoodooTexture;
    interface DECLSPEC_UUID("7FFF4B7A-A831-4D98-A3DD-F5DA592BE748") IVoodooShader;

    typedef IVoodooAdapter      *LPVOODOOADAPTER,       *PVOODOOADAPTER;
    typedef IVoodooConverter    *LPVOODOOCONVERTER,     *PVOODOOCONVERTER;
    typedef IVoodooCore         *LPVOODOOCORE,          *PVOODOOCORE;
    typedef IVoodooFile         *LPVOODOOFILE,          *PVOODOOFILE;
    typedef IVoodooFileSystem   *LPVOODOOFILESYSTEM,    *PVOODOOFILESYSTEM;
    typedef IVoodooFsManager    *LPVOODOOFSMANAGER,     *PVOODOOFSMANAGER;
    typedef IVoodooHookSystem   *LPVOODOOHOOKSYSTEM,    *LPVOODOOHOOKSYSTEM;
    typedef IVoodooImage        *LPVOODOOIMAGE,         *LPVOODOOIMAGE;
    typedef IVoodooLogger       *LPVOODOOLOGGER,        *LPVOODOOLOGGER;
    typedef IVoodooMatManager   *LPVOODOOMATMANAGER,    *LPVOODOOMATMANAGER;
    typedef IVoodooParameter    *LPVOODOOPARAMETER,     *LPVOODOOPARAMETER;
    typedef IVoodooParser       *LPVOODOOPARSER,        *LPVOODOOPARSER;
    typedef IVoodooPass         *LPVOODOOPASS,          *LPVOODOOPASS;
    typedef IVoodooTechnique    *LPVOODOOTECHNIQUE,     *LPVOODOOTECHNIQUE;
    typedef IVoodooTexture      *LPVOODOOTEXTURE,       *LPVOODOOTEXTURE;
    typedef IVoodooShader       *LPVOODOOSHADER,        *PVOODOOSHADER;

    [
        object, dual, pointer_default(unique),
        uuid("76073706-54E6-457D-B891-52A80410BEEF"),
        helpstring("Adapter Interface")
    ]
    __interface IVoodooAdapter
        : IUnknown
    {
        STDMETHOD(LoadPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(UnloadPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(BindPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(UnbindPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(DrawGeometry)([in] UINT Vertexes, [in, optional] Vertex * pVertexData) PURE;
        STDMETHOD(ApplyParameter)([in] IVoodooParameter * pParameter) PURE;
        STDMETHOD(DrawShader)([in] IVoodooShader * pShader) PURE;
        STDMETHOD(CreateTexture)([in] LPCSTR pName, [in] TextureDesc Description, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(LoadTexture)([in] IVoodooImage * pImage, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(ConnectTexture)([in] IVoodooParameter * pParameter, [in] IVoodooTexture * pTexture) PURE;
        STDMETHOD(HandleError)([in, optional] void * pContext, [in] DWORD Error, [in, optional] IVoodooCore * pCore) PURE;
    };

    [
        object, dual, pointer_default(unique),
        uuid("31420F71-1050-45D9-838F-8735BBB8EDEA"),
        helpstring("Core Interface")
    ]
    __interface IVoodooCore
        : IUnknown
    {
        STDMETHOD(GetParser)([out, retval] IVoodooParser ** pParser) PURE;
        STDMETHOD(GetHookSystem)([out, retval] IVoodooHookSystem ** ppHookManager) PURE;
        STDMETHOD(GetFileSystem)([out, retval] IVoodooFileSystem ** ppFileSystem) PURE;
        STDMETHOD(GetAdapter)([out, retval] IVoodooAdapter ** ppAdapter) PURE;
        STDMETHOD(GetLogger)([out, retval] IVoodooLogger ** ppLogger) PURE;
        STDMETHOD(GetConfig)([out, retval] void ** ppConfig) PURE;
        [propget, id(7)] STDMETHOD(GetCgContext)([out, retval] void ** ppCgContext) PURE;
        [propput, id(7)] STDMETHOD(SetCgContext)([in] void * pCgContext) PURE;

        STDMETHOD(CreateShader)([in] IVoodooFile * pFile, [in, optional] LPCSTR * ppArgs, [out, retval] IVoodooShader ** ppShader) PURE;
        STDMETHOD(CreateParameter)([in] LPCSTR pName, [in] ParameterType Type, [out, retval] IVoodooParameter ** ppParameter) PURE;
        STDMETHOD(AddTexture)([in] LPCSTR pName, [in] void * pData) PURE;
        STDMETHOD(GetTexture)([in] LPCSTR pName, [out, retval] IVoodooTexture ** ppTexture) PURE;

        [propget, id(13)] STDMETHOD(GetStageTexture)([in] TextureType Stage, [out, retval] IVoodooTexture ** ppTexture) PURE;
        [propput, id(13)] STDMETHOD(SetStageTexture)([in] TextureType Stage, [in] IVoodooTexture * pTexture) PURE;

    };

    [
        object, dual, pointer_default(unique),
        uuid("FC21372A-6EA6-4CA5-811C-5ED4835973B7"),
        helpstring("Parameter Interface")
    ]
    __interface IVoodooParameter
        : IUnknown
    {
        STDMETHOD(GetName)([in] LPCSTR pName) PURE;
        STDMETHOD(GetCgParameter)([out, retval] void ** ppCgParameter) PURE;
        STDMETHOD(GetType)([out, retval] ParameterType * pType) PURE;
        STDMETHOD(AttachParameter)([in] IVoodooParameter * pParameter) PURE;
        STDMETHOD(ForceUpdate)() PURE;

        STDMETHOD(SetValue)([in, optional] IVoodooTexture * pTexture) PURE;
        STDMETHOD(SetValue)([in] float X) PURE;
        STDMETHOD(SetValue)([in] float X, [in] float Y) PURE;
        STDMETHOD(SetValue)([in] float X, [in] float Y, [in] float Z) PURE;
        STDMETHOD(SetValue)([in] float X, [in] float Y, [in] float Z, [in] float W) PURE;
        STDMETHOD(SetValue)([in, size_is(pData + sizeof(float)*16)] float * pData) PURE;

        STDMETHOD(GetValue)([out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(GetValue)([out] float * pX) PURE;
        STDMETHOD(GetValue)([out] float * pX, [out] float * pY) PURE;
        STDMETHOD(GetValue)([out] float * pX, [out] float * pY, [out] float * pZ) PURE;
        STDMETHOD(GetValue)([out] float * pX, [out] float * pY, [out] float * pZ, [out] float * pW) PURE;
        STDMETHOD(GetValue)([out] float * pData) PURE;
    };
    
    typedef std::string                         String;

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
    typedef std::map<String, ModuleRef>         ModuleMap; /*!< Provides a map of modules, keyed by 
                                                                name. */
    typedef std::pair<ModulePtr, int>           ClassID;   /*!< Defines the necessary data to create 
                                                                a class (source module and class number). */
    typedef std::map<String, ClassID>           ClassMap;  /*!< Provides a map of classes, keyed by name. */

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
        TF_RGB8,                /*!< 8 bit RGB (8 bit X in DX). Most common backbuffer format, 
                                     common texture format. */
        TF_RGBA8,               /*!< 8 bit RGBA. Common texture format. */
        TF_RGB10A2,             /*!< 10 bit RGB, 2 bit A */
        // Float texture formats
        TF_RGBA16F,             /*!< Half-precision RGBA. HDR format. */
        TF_RGBA32F,             /*!< Full-precision RGBA (float/single). HDR format. */
        // Depth-buffer formats
        TF_D16,                 /*!< Half-precision depth (Z-buffer, see @ref depthbuffers 
                                     for more info) */
        TF_D32,                 /*!< Full-precision depth (Z-buffer, see @ref depthbuffers 
                                     for more info) */
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
        // Samplers
        PT_Sampler1D,           /*!< One-dimensional sampler (for a 1D texture, see 
                                     @ref texturetypes "texture types" for more info) */
        PT_Sampler2D,           /*!< Two-dimensional sampler (for a 2D texture, see 
                                     @ref texturetypes "texture types" for more info) */
        PT_Sampler3D,           /*!< Three-dimensional sampler (for a 3D/volume texture, see 
                                     @ref texturetypes "texture types" for more info) */
        // Float-vectors
        PT_Float1,              /*!< Single-component float vector */
        PT_Float2,              /*!< Two-component float vector */
        PT_Float3,              /*!< Three-component float vector */
        PT_Float4,              /*!< Four-component float vector */
        // Matrices
        PT_Matrix,              /*!< Generic float4x4 matrix type */
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
        FM_Unknown   = 0x00,     /*!< Unknown file open mode */
        // Valid modes
        FM_Read      = 0x01,     /*!< Read-only file open mode */
        FM_Write     = 0x02,     /*!< Read/write file open mode */
        // Padding value
        FM_Max = 0xFFFFFFFF,     /*!< Highest possible value, forcing dword type */
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
        LL_SDebug       = 0x01,
        LL_SInfo        = 0x02,
        LL_SWarning     = 0x04,
        LL_SError       = 0x08,
        LL_Severity     = 0x0F,
        // Origin values
        LL_OAPI         = 0x10,
        LL_OExternal    = 0x20,
        LL_OModule      = 0x40,
        LL_OFramework   = 0x80,
        LL_Origin       = 0xF0,
        // Logger internal values
        LL_Internal     = 0x82,     /*!< Log level for Logger-internal messages */
        LL_Initial      = 0xFE,     /*!< Initial log level of Logger */
        // Default values
        LL_Debug        = 0x41,
        LL_Info         = 0x42,
        LL_Warning      = 0x44,
        LL_Error        = 0x48,
        // Misc values
        LL_All          = 0xFF,     /*!< All messages will be logged */
        // Padding value
        LL_Max    = 0xFFFFFFFF,     /*!< Highest possible value, forcing dword type */
    };

    /**
     * String parsing flags. These modify the behavior of the string parser.
     */
    enum ParseFlags
    {
        PF_None             = 0x0000,     /*!< No special behavior */
        // Slash flags
        PF_SingleSlash      = 0x0001,     /*!< No repeated slashes (useful for XPath). */
        PF_SlashOnly        = 0x0002,     /*!< Replace all backslashes with forward slashes. */
        PF_BackslashOnly    = 0x0004,     /*!< Replace all forward slashes with backslashes. */
        PF_SlashFlags       = 0x0007,
        // Case flags
        PF_Lowercase        = 0x0010,
        PF_Uppercase        = 0x0020,
        PF_CaseFlags        = 0x0030,
        // General flags
        PF_VarName          = 0x0100,
        // Padding value
        PF_Max          = 0xFFFFFFFF,     /*!< Highest possible value, forcing dword type */
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

    /**
     * Generic vertex format for use with IAdapter draw calls. This format is
     * compatible with both OpenGL and DirectX.
     *
     * @note Because of the draw mechanism for adapters, most draws with
     *        user-provided vertexes will not use vertex buffers of any sort. 
     *        This can hurt performance if used heavily, so drawing through 
     *        IAdapter::DrawQuad() should be avoided as much as possible.
     *        Adapters may draw internally, of course, having full control over
     *        the graphics API.
     *
     * @note This vertex format provides a float3 position and float2 texture
     *        coordinate. For compatibility with Direct3D, a RHW value is also
     *        included (the vertex format is D3DFVF_XYZRHW|D3DFVF_TEX1). OpenGL
     *        adapters may ignore this winding value. The members of the vert are
     *        ordered so that <code>&x</code> is a valid float3 with the position
     *        and <code>&tu</code> is a valid float2 with the texture coordinate.
     */
    struct Vertex
    {
        float x, y, z;
        float winding;
        float tu, tv;
    };
};

#endif /*VOODOO_META_HPP*/
