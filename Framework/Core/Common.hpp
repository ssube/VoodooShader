
#pragma once

namespace VoodooShader
{
    // HRESULTs. General are < 0x1000
    // General
    const HRESULT E_NONAME      = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0001);
    const HRESULT E_NOCORE      = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0002);
    const HRESULT E_DUPNAME     = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0003);
    const HRESULT E_NOT_FOUND   = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0004);
    const HRESULT E_BADCLSID    = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0100);
    // IVoodooCore
    const HRESULT E_CFGNOTFOUND = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0011);
    const HRESULT E_INVALIDCFG  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0012);
    const HRESULT E_MSXMLERROR  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0013);

    const HRESULT E_BADADPCLSID = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0101);
    const HRESULT E_BADFSCLSID  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0102);
    const HRESULT E_BADHSCLSID  = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0103);
    const HRESULT E_BADLOGCLSID = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0104);

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
        LPCSTR Name;
        UINT Major;
        UINT Minor;
        ULONG Patch;
        ULONG Rev;
        BOOL Debug;
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
}