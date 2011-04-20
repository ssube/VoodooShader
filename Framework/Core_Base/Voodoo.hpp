/**************************************************************************************************\
 * void file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * void program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * void program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with void program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_ATTRIBUTES
//#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlsafe.h>
#include <atltypes.h>
#include <atlwin.h>

using namespace ATL;

namespace VoodooShader
{
    // HRESULTs.
    // Error
    // General
    const HRESULT E_NONAME      = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0001);
    const HRESULT E_NOCORE      = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0002);
    const HRESULT E_DUPNAME     = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0003);
    const HRESULT E_NOT_FOUND   = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0004);
    const HRESULT E_INVALIDPOS  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0005);
    const HRESULT E_BADCLSID    = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0100);
    const HRESULT E_BADTHING    = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0xFFFF);

    // IVoodooCore
    const HRESULT E_CFGNOTFOUND = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0011);
    const HRESULT E_INVALIDCFG  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0012);
    const HRESULT E_MSXMLERROR  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0013);
    const HRESULT E_BADADPCLSID = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0101);
    const HRESULT E_BADFSCLSID  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0102);
    const HRESULT E_BADHSCLSID  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0103);
    const HRESULT E_BADLOGCLSID = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0104);

    // IVoodooParameter
    const HRESULT E_VIRTUAL     = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0011);
    const HRESULT E_NOTVIRTUAL  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0012);

    // Success
    const HRESULT S_ISCORE      = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0002);
    const HRESULT S_NOT_FOUND   = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x0004);
    
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
        UINT Width;
        UINT Height;
        UINT Depth;
        BOOL Mipmaps;
        TextureFormat Format;
    };

    /**
     * Describes a portion of a texture. void defines a cube region and provides
     * an optional format the region should be set up as.
     */
    struct TextureRegion
    {
        UINT Width;
        UINT Height;
        UINT Depth;
        BOOL Mipmaps;
        UINT OffX;
        UINT OffY;
        UINT OffZ;
        TextureFormat Format;
    };

    /**
     * Describes the precise version of a particular library, including name, 
     * main version, revision and debug status.
     */
    struct Version
    {
        BSTR Name;
        UINT Major;
        UINT Minor;
        ULONG Patch;
        ULONG Rev;
        BOOL Debug;
    };

    /**
     * Generic vertex format for use with IAdapter draw calls. void format is
     * compatible with both OpenGL and DirectX.
     *
     * @note Because of the draw mechanism for adapters, most draws with
     *        user-provided vertexes will not use vertex buffers of any sort. 
     *        void can hurt performance if used heavily, so drawing through 
     *        IAdapter::DrawQuad() should be avoided as much as possible.
     *        Adapters may draw internally, of course, having full control over
     *        the graphics API.
     *
     * @note void vertex format provides a float3 position and float2 texture
     *        coordinate. For compatibility with Direct3D, a RHW value is also
     *        included (the vertex format is D3DFVF_XYZRHW|D3DFVF_TEX1). OpenGL
     *        adapters may ignore void winding value. The members of the vert are
     *        ordered so that <code>&x</code> is a valid float3 with the position
     *        and <code>&tu</code> is a valid float2 with the texture coordinate.
     */
    struct Vertex
    {
        float X, Y, Z;
        float Winding;
        float U, V;
    };

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

    DEFINE_GUID(IID_VoodooAdapter,      0x76073706, 0x54E6, 0x457D, 0xB8, 0x91, 0x52, 0xA8, 0x04, 0x10, 0xBE, 0xEF);
    DEFINE_GUID(IID_VoodooCore,         0x31420F71, 0x1050, 0x45D9, 0x83, 0x8F, 0x87, 0x35, 0xBB, 0xB8, 0xED, 0xEA);
    DEFINE_GUID(IID_VoodooFile,         0x73926B89, 0x5899, 0x4E58, 0xA7, 0x14, 0x85, 0x18, 0x12, 0x18, 0x8F, 0x15);
    DEFINE_GUID(IID_VoodooFileSystem,   0x229B5CF6, 0x99BF, 0x4250, 0x90, 0x88, 0x12, 0x8F, 0xE2, 0x99, 0x76, 0x7E);
    DEFINE_GUID(IID_VoodooHookSystem,   0x38C848CD, 0x915A, 0x49EB, 0x92, 0xA4, 0xA4, 0x63, 0x05, 0x3C, 0x62, 0xA8);
    DEFINE_GUID(IID_VoodooImage,        0x7EF1C1F0, 0xA474, 0x4115, 0xBF, 0x5A, 0x44, 0x1C, 0x83, 0x47, 0x45, 0x36);
    DEFINE_GUID(IID_VoodooLogger,       0x41482ECA, 0x13DA, 0x4D12, 0xB2, 0x54, 0xF7, 0xE5, 0x5D, 0x4D, 0xC2, 0x8D);
    DEFINE_GUID(IID_VoodooObject,       0xAC697824, 0xE290, 0x43FB, 0x9C, 0x06, 0xCC, 0xFB, 0x0D, 0x8B, 0xF6, 0x8E);
    DEFINE_GUID(IID_VoodooParameter,    0xFC21372A, 0x6EA6, 0x4CA5, 0x81, 0x1C, 0x5E, 0xD4, 0x83, 0x59, 0x73, 0xB7);
    DEFINE_GUID(IID_VoodooParser,       0x41F3AEE5, 0x0F0E, 0x4BF4, 0x95, 0x12, 0xDC, 0x29, 0x0E, 0x37, 0xEB, 0x7B);
    DEFINE_GUID(IID_VoodooPass,         0xC8BEBCB5, 0xB62F, 0x48B8, 0x82, 0xCE, 0x80, 0x22, 0x20, 0xB5, 0xE9, 0x30);
    DEFINE_GUID(IID_VoodooTargeted,     0x67D488C5, 0xA57D, 0x4B8D, 0xB4, 0x8B, 0x72, 0x95, 0x60, 0x8B, 0x92, 0x4A);
    //DEFINE_GUID(IID_VoodooTarget,       0xB2286B91, 0xAF4F, 0x4330, 0xA7, 0xAC, 0x7E, 0xCB, 0x90, 0x55, 0xBA, 0x17);
    DEFINE_GUID(IID_VoodooTechnique,    0x1BD418B4, 0x7D6C, 0x4CFB, 0x81, 0x38, 0x33, 0x9D, 0x26, 0xAC, 0xB3, 0x78);
    DEFINE_GUID(IID_VoodooTexture,      0x0F33959B, 0x242E, 0x4282, 0x85, 0x10, 0xBE, 0x6C, 0x99, 0x97, 0xF4, 0x64);
    DEFINE_GUID(IID_VoodooSampler,      0x2a63b473, 0x4dbf, 0x4d8a, 0xa9, 0x7f, 0xb2, 0x85, 0xed, 0xe5, 0x83, 0xc8);
    DEFINE_GUID(IID_VoodooScalar,       0x78bfc69b, 0x0359, 0x4a04, 0x94, 0xdf, 0x41, 0x27, 0x2c, 0x5c, 0xe8, 0x8f);
    DEFINE_GUID(IID_VoodooShader,       0x7FFF4B7A, 0xA831, 0x4D98, 0xA3, 0xDD, 0xF5, 0xDA, 0x59, 0x2B, 0xE7, 0x48);

    /**
     * Standard interfaces.
     */
    interface DECLSPEC_UUID("76073706-54E6-457D-B891-52A80410BEEF") IVoodooAdapter;
    //interface DECLSPEC_UUID("94158AA0-87AD-4F99-89F2-7E7B8E9D5CDB") IVoodooConverter;
    interface DECLSPEC_UUID("31420F71-1050-45D9-838F-8735BBB8EDEA") IVoodooCore;
    interface DECLSPEC_UUID("73926B89-5899-4E58-A714-851812188F15") IVoodooFile;
    interface DECLSPEC_UUID("229B5CF6-99BF-4250-9088-128FE299767E") IVoodooFileSystem;
    interface DECLSPEC_UUID("38C848CD-915A-49EB-92A4-A463053C62A8") IVoodooHookSystem;
    interface DECLSPEC_UUID("7EF1C1F0-A474-4115-BF5A-441C83474536") IVoodooImage;
    interface DECLSPEC_UUID("41482ECA-13DA-4D12-B254-F7E55D4DC28D") IVoodooLogger;
    interface DECLSPEC_UUID("AC697824-E290-43FB-9C06-CCFB0D8BF68E") IVoodooObject;
    interface DECLSPEC_UUID("FC21372A-6EA6-4CA5-811C-5ED4835973B7") IVoodooParameter;
    interface DECLSPEC_UUID("41F3AEE5-0F0E-4BF4-9512-DC290E37EB7B") IVoodooParser;
    interface DECLSPEC_UUID("C8BEBCB5-B62F-48B8-82CE-802220B5E930") IVoodooPass;
    interface DECLSPEC_UUID("67D488C5-A57D-4B8D-B48B-7295608B924A") IVoodooTargeted;
    //interface DECLSPEC_UUID("B2286B91-AF4F-4330-A7AC-7ECB9055BA17") IVoodooTarget;
    interface DECLSPEC_UUID("1BD418B4-7D6C-4CFB-8138-339D26ACB378") IVoodooTechnique;
    interface DECLSPEC_UUID("0F33959B-242E-4282-8510-BE6C9997F464") IVoodooTexture;
    interface DECLSPEC_UUID("2A63B473-4DBF-4D8A-A97F-B285EDE583C8") IVoodooSampler;
    interface DECLSPEC_UUID("78BFC69B-0359-4A04-94DF-41272C5CE88F") IVoodooScalar;
    interface DECLSPEC_UUID("7FFF4B7A-A831-4D98-A3DD-F5DA592BE748") IVoodooShader;

    typedef IVoodooAdapter      *LPVOODOOADAPTER,       *PVOODOOADAPTER;
    //typedef IVoodooConverter    *LPVOODOOCONVERTER,     *PVOODOOCONVERTER;
    typedef IVoodooCore         *LPVOODOOCORE,          *PVOODOOCORE;
    typedef IVoodooFile         *LPVOODOOFILE,          *PVOODOOFILE;
    typedef IVoodooFileSystem   *LPVOODOOFILESYSTEM,    *PVOODOOFILESYSTEM;
    typedef IVoodooHookSystem   *LPVOODOOHOOKSYSTEM,    *PVOODOOHOOKSYSTEM;
    typedef IVoodooImage        *LPVOODOOIMAGE,         *PVOODOOIMAGE;
    typedef IVoodooLogger       *LPVOODOOLOGGER,        *PVOODOOLOGGER;
    typedef IVoodooParameter    *LPVOODOOPARAMETER,     *PVOODOOPARAMETER;
    typedef IVoodooParser       *LPVOODOOPARSER,        *PVOODOOPARSER;
    typedef IVoodooPass         *LPVOODOOPASS,          *PVOODOOPASS;
    typedef IVoodooTechnique    *LPVOODOOTECHNIQUE,     *PVOODOOTECHNIQUE;
    typedef IVoodooTexture      *LPVOODOOTEXTURE,       *PVOODOOTEXTURE;
    typedef IVoodooSampler      *LPVOODOOSAMPLER,       *PVOODOOSAMPLER;
    typedef IVoodooScalar       *LPVOODOOSCALAR,        *PVOODOOSCALAR;
    typedef IVoodooShader       *LPVOODOOSHADER,        *PVOODOOSHADER;

    /*[
        object, pointer_default(unique),
        uuid("AC697824-E290-43FB-9C06-CCFB0D8BF68E"),
        helpstring("Named Object Interface")
    ]
    __interface IVoodooObject
        : IUnknown
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;
    }*/;

    /*[
        object, pointer_default(unique),
        uuid("67D488C5-A57D-4B8D-B48B-7295608B924A"),
        helpstring("Targeted Interface")
    ]
    __interface IVoodooTargeted
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooTargeted
        STDMETHOD(GetTarget)([out, retval] IVoodooTexture ** ppTexture) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("76073706-54E6-457D-B891-52A80410BEEF"),
        helpstring("Adapter Interface")
    ]
    __interface IVoodooAdapter
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooAdapter
        STDMETHOD(LoadPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(UnloadPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(BindPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(UnbindPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(BindTarget)([in] IVoodooTexture * pTarget) PURE;
        STDMETHOD(UnbindTarget)([in] IVoodooTexture * pTarget) PURE;
        STDMETHOD(DrawGeometry)([in] UINT Vertexes, [in] Vertex * pVertexData) PURE;
        STDMETHOD(ApplyParameter)([in] IVoodooParameter * pParameter) PURE;
        STDMETHOD(DrawShader)([in] IVoodooShader * pShader) PURE;
        STDMETHOD(CreateTexture)([in] BSTR pName, [in] TextureDesc Description, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(LoadTexture)([in] IVoodooImage * pImage, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(ConnectTexture)([in] IVoodooParameter * pParameter, [in] IVoodooTexture * pTexture) PURE;
        STDMETHOD(HandleError)([in] void * pContext, [in] DWORD Error, [in] IVoodooCore * pCore) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("31420F71-1050-45D9-838F-8735BBB8EDEA"),
        helpstring("Core Interface")
    ]
    __interface IVoodooCore
        : IVoodooObject
    {
        // IUnknown
        //STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        //STDMETHOD_(ULONG,AddRef)(void) PURE;
        //STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        //STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        //STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooCore
        STDMETHOD(Initialize)([in, optional] BSTR pConfig) PURE;
        [propget, id(2)] STDMETHOD(Parser)([out, retval] IVoodooParser ** pParser) PURE;
        [propget, id(3)] STDMETHOD(HookSystem)([out, retval] IVoodooHookSystem ** ppHookManager) PURE;
        [propget, id(4)] STDMETHOD(FileSystem)([out, retval] IVoodooFileSystem ** ppFileSystem) PURE;
        [propget, id(5)] STDMETHOD(Adapter)([out, retval] IVoodooAdapter ** ppAdapter) PURE;
        [propget, id(6)] STDMETHOD(Logger)([out, retval] IVoodooLogger ** ppLogger) PURE;
        [propget, id(7)] STDMETHOD(Config)([out, retval] IUnknown ** ppConfig) PURE;

        [propget, id(8)] STDMETHOD(CgContext)([out, retval] void ** ppCgContext) PURE;
        [propput, id(8)] STDMETHOD(CgContext)([in] void * pCgContext) PURE;

        STDMETHOD(CreateShader)([in] IVoodooFile * pFile, [out, retval] IVoodooShader ** ppShader) PURE;
        STDMETHOD(CreateParameter)([in] BSTR pName, [in] ParameterType Type, [out, retval] IVoodooParameter ** ppParameter) PURE;
        STDMETHOD(CreateTexture)([in] BSTR pName, [in] void * pData, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(GetParameter)([in] BSTR pName, [out, retval] IVoodooParameter ** ppParameter) PURE;
        STDMETHOD(GetTexture)([in] BSTR pName, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(RemoveTexture)([in] BSTR pName) PURE;

        [propget, id(16)] STDMETHOD(StageTexture)([in] TextureType Stage, [out, retval] IVoodooTexture ** ppTexture) PURE;
        [propput, id(16)] STDMETHOD(StageTexture)([in] TextureType Stage, [in] IVoodooTexture * pTexture) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("73926B89-5899-4E58-A714-851812188F15"),
        helpstring("File Interface")
    ]
    __interface IVoodooFile
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooFile
        [propget, id(6)] STDMETHOD(Path)([out, optional] LPBSTR pPath) PURE;
        STDMETHOD(Open)([in] FileOpenMode Mode) PURE;
        STDMETHOD(Close)() PURE;
        STDMETHOD(Read)([in, out] UINT * Length, [out, size_is(Length)] void * ppBuffer) PURE;
        STDMETHOD(Write)([in] UINT Length, [in, size_is(Length)] void * pBuffer) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("229B5CF6-99BF-4250-9088-128FE299767E"),
        helpstring("FileSystem Interface")
    ]
    __interface IVoodooFileSystem
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooFileSystem
        STDMETHOD(AddDirectory)([in] BSTR pPath) PURE;
        STDMETHOD(RemoveDirectory)([in] BSTR pPath) PURE;
        STDMETHOD(FindFile)([in] BSTR pPath, [out, retval] IVoodooFile ** ppFile) PURE;
        STDMETHOD(FindImage)([in] BSTR pPath, [out, retval] IVoodooImage ** ppImage) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("38C848CD-915A-49EB-92A4-A463053C62A8"),
        helpstring("Hook System Interface")
    ]
    __interface IVoodooHookSystem
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooHookSystem
        STDMETHOD(CreateHook)([in] BSTR pName, [in] void * pSource, [in] void * pDest) PURE;
        STDMETHOD(RemoveHook)([in] BSTR pName) PURE;
        STDMETHOD(RemoveAllHooks)() PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("7EF1C1F0-A474-4115-BF5A-441C83474536"),
        helpstring("Image File Interface")
    ]
    __interface IVoodooImage
        : IVoodooFile
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooFile
        [propget, id(6)] STDMETHOD(Path)([out, optional] LPBSTR pPath) PURE;
        STDMETHOD(Open)([in] FileOpenMode Mode) PURE;
        STDMETHOD(Close)() PURE;
        STDMETHOD(Read)([in, out] UINT * Length, [out, size_is(Length)] void * ppBuffer) PURE;
        STDMETHOD(Write)([in] UINT Length, [in, size_is(Length)] void * pBuffer) PURE;

        // IVoodooImage
        STDMETHOD(GetDesc)([out, retval] TextureDesc * pDesc) PURE;
        STDMETHOD(GetData)([in] TextureRegion Region, [out, retval] void * ppBuffer) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("41482ECA-13DA-4D12-B254-F7E55D4DC28D"),
        helpstring("Logger Interface")
    ]
    __interface IVoodooLogger
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooLogger
        STDMETHOD(Open)([in] BSTR pFilename, [in] BOOL Append) PURE;
        STDMETHOD(Close)() PURE;
        STDMETHOD(Dump)() PURE;
        [propget, id(4)] STDMETHOD(GetLogLevel)([out, retval] LogLevel * pLevel) PURE;
        [propput, id(4)] STDMETHOD(SetLogLevel)([in] LogLevel * Level) PURE;
        STDMETHOD(LogModule)([in] Version Module) PURE;
        [vararg] STDMETHOD(Log)([in] LogLevel Level, [in] BSTR pModule, [in] BSTR pFormat, [in, satype(VARIANT)] SAFEARRAY * pArgs) PURE;
        [propget, id(9)] STDMETHOD(GetBufferSize)([out, retval] UINT * pSize) PURE;
        [propput, id(9)] STDMETHOD(SetBufferSize)([in] UINT Size) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("FC21372A-6EA6-4CA5-811C-5ED4835973B7"),
        helpstring("Parameter Interface")
    ]
    __interface IVoodooParameter
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooParameter
        [propget, id(6)] STDMETHOD_(BOOL, IsVirtual)() PURE;
        STDMETHOD(GetShader)([out, retval] IVoodooShader ** ppShader) PURE;
        STDMETHOD(GetType)([out, retval] ParameterType * pType) PURE;
        STDMETHOD(GetCgParameter)([out, retval] void ** ppCgParameter) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("41F3AEE5-0F0E-4BF4-9512-DC290E37EB7B"),
        helpstring("Variable Parser Interface")
    ]
    __interface IVoodooParser
        : IUnknown
    {
        // IUnknown
        //STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        //STDMETHOD_(ULONG,AddRef)(void) PURE;
        //STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        //STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        //STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooParser
        STDMETHOD(AddVariable)([in] BSTR pName, [in] BSTR pValue, [in] BOOL System) PURE;
        STDMETHOD(RemoveVariable)([in] BSTR pName) PURE;
        STDMETHOD(Parse)([in, out] LPBSTR pString, [in, defaultvalue(PF_None)] ParseFlags Flags) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("C8BEBCB5-B62F-48B8-82CE-802220B5E930"),
        helpstring("Pass Interface")
    ]
    __interface IVoodooPass
        : IVoodooTargeted
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooTargeted
        STDMETHOD(GetTarget)([out, retval] IVoodooTexture ** ppTexture) PURE;

        // IVoodooPass
        STDMETHOD(GetCgProgram)([in] ProgramStage Stage, [out, retval] void ** ppProgram) PURE;
        STDMETHOD(GetCgPass)([out, retval] void ** ppPass) PURE;
        STDMETHOD(GetShader)([out, retval] IVoodooShader ** ppShader) PURE;
        STDMETHOD(GetTechnique)([out, retval] IVoodooTechnique ** ppTechnique) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("2A63B473-4DBF-4D8A-A97F-B285EDE583C8"),
        helpstring("Sampler Parameter Interface")
    ]
    __interface IVoodooSampler
        : IVoodooParameter
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooParameter
        [propget, id(6)] STDMETHOD_(BOOL, IsVirtual)() PURE;
        STDMETHOD(GetShader)([out, retval] IVoodooShader ** ppShader) PURE;
        STDMETHOD(GetType)([out, retval] ParameterType * pType) PURE;
        STDMETHOD(GetCgParameter)([out, retval] void ** ppCgParameter) PURE;

        // IVoodooSampler
        STDMETHOD(AttachParameter)([in] IVoodooSampler * pParameter) PURE;
        [propput, id(11)] STDMETHOD(Texture)([in] IVoodooTexture * pTexture) PURE;
        [propget, id(11)] STDMETHOD(Texture)([out, retval] IVoodooTexture ** ppTexture) PURE;

        [propget, id(13)] STDMETHOD_(UINT, Dimensions)() PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("78BFC69B-0359-4A04-94DF-41272C5CE88F"),
        helpstring("Scalar Parameter Interface")
    ]
    __interface IVoodooScalar
        : IVoodooParameter
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooParameter
        [propget, id(6)] STDMETHOD_(BOOL, IsVirtual)() PURE;
        STDMETHOD(GetShader)([out, retval] IVoodooShader ** ppShader) PURE;
        STDMETHOD(GetType)([out, retval] ParameterType * pType) PURE;
        STDMETHOD(GetCgParameter)([out, retval] void ** ppCgParameter) PURE;

        // IVoodooSampler
        STDMETHOD(AttachParameter)([in] IVoodooScalar * pParameter) PURE;
        [propput, id(11)] STDMETHOD(Value)([in, satype(float)] SAFEARRAY * pData) PURE;
        [propget, id(11)] STDMETHOD(Value)([out, retval, satype(float)] SAFEARRAY ** ppData) PURE;

        [propget, id(13)] STDMETHOD_(UINT, Components)() PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("7FFF4B7A-A831-4D98-A3DD-F5DA592BE748"),
        helpstring("Shader Interface")
    ]
    __interface IVoodooShader
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooShader
        [propget, id(6)] STDMETHOD_(UINT, TechniqueCount)() PURE;
        STDMETHOD(GetTechnique)([in] UINT Number, [out, retval] IVoodooTechnique ** ppTechnique) PURE;
        [propget, id(8)] STDMETHOD(DefaultTechnique)([out, retval] IVoodooTechnique ** ppTechnique) PURE;
        [propput, id(8)] STDMETHOD(DefaultTechnique)([in] IVoodooTechnique * pTechnique) PURE;
        [propget, id(10)] STDMETHOD_(UINT, ParameterCount)() PURE;
        STDMETHOD(GetParameter)([in] UINT Number, [out, retval] IVoodooParameter ** ppParameter) PURE;
        STDMETHOD(GetCgShader)([out, retval] void ** ppCgShader) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("1BD418B4-7D6C-4CFB-8138-339D26ACB378"),
        helpstring("Technique Interface")
    ]
    __interface IVoodooTechnique
        : IVoodooTargeted
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooTargeted
        STDMETHOD(GetTarget)([out, retval] IVoodooTexture ** ppTexture) PURE;

        // IVoodooTechnique
        [propget, id(7)] STDMETHOD_(UINT, PassCount)() PURE;
        STDMETHOD(GetPass)([in] UINT Number, [out, retval] IVoodooPass ** ppPass) PURE;
        STDMETHOD(GetCgTechnique)([out, retval] void ** ppPass) PURE;
        STDMETHOD(GetShader)([out, retval] IVoodooShader ** ppShader) PURE;
    };*/

    /*[
        object, pointer_default(unique),
        uuid("0F33959B-242E-4282-8510-BE6C9997F464"),
        helpstring("Texture Interface")
    ]
    __interface IVoodooTexture
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooTexture
        STDMETHOD(GetData)([out, retval] void ** ppData) PURE;
    };*/
}