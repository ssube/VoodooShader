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
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

typedef DWORD EnumType;

typedef enum TextureFormat      TextureFormat;
typedef enum ParameterType      ParameterType;
typedef enum ParameterCategory  ParameterCategory;
typedef enum ProgramStage       ProgramStage;
typedef enum TextureStage       TextureStage;
typedef enum FileOpenMode       FileOpenMode;
typedef enum LogLevel           LogLevel;
typedef enum ParseFlags         ParseFlags;
typedef enum VariableType       VariableType;

typedef struct InitParams       InitParams;
typedef struct TextureDesc      TextureDesc;
typedef struct TextureRegion    TextureRegion;
typedef struct VersionStruct    VersionStruct;
typedef struct float2           float2;
typedef struct float3           float3;
typedef struct float4           float4;
typedef struct VertexStruct     VertexStruct;
typedef struct LightStruct      LightStruct;
typedef struct FunctionPtr      FunctionPtr;

enum TextureFormat
{	
    TF_Unknown	    = 0,
    TF_RGB5	        = 0x1,
    TF_RGB8	        = 0x3,
    TF_RGBA8	    = 0x4,
    TF_RGB10A2	    = 0x5,
    TF_RGBA16F	    = 0x10,
    TF_RGBA32F	    = 0x11,
    TF_D16	        = 0x40,
    TF_D32	        = 0x41,
    TF_Max	        = 0x7fffffff
};

enum ParameterType
{	
    PT_Unknown	    = 0,
    PT_Float1	    = 0x11,
    PT_Float2	    = 0x12,
    PT_Float3	    = 0x13,
    PT_Float4	    = 0x14,
    PT_Float1x1	    = 0x11,
    PT_Float1x2	    = 0x12,
    PT_Float1x3	    = 0x13,
    PT_Float1x4	    = 0x14,
    PT_Float2x1	    = 0x21,
    PT_Float2x2	    = 0x22,
    PT_Float2x3	    = 0x23,
    PT_Float2x4	    = 0x24,
    PT_Float3x1	    = 0x31,
    PT_Float3x2	    = 0x32,
    PT_Float3x3	    = 0x33,
    PT_Float3x4	    = 0x34,
    PT_Float4x1	    = 0x41,
    PT_Float4x2	    = 0x42,
    PT_Float4x3	    = 0x43,
    PT_Float4x4	    = 0x44,
    PT_Sampler1D	= 0x101,
    PT_Sampler2D	= 0x102,
    PT_Sampler3D	= 0x103,
    PT_Struct	    = 0x1000,
    PT_Max	        = 0x7fffffff
};

enum ParameterCategory
{	
    PC_Unknown	    = 0,
    PC_Float	    = 0x1,
    PC_Sampler	    = 0x2,
    PC_Struct	    = 0x4,
    PC_Max	        = 0x7fffffff
};

enum ProgramStage
{	
    PS_Unknown	    = 0,
    PS_Vertex	    = 0x1,
    PS_Fragment	    = 0x2,
    PS_Geometry	    = 0x3,
    PS_Domain	    = 0x4,
    PS_Hull	        = 0x5,
    PS_Max	        = 0x7fffffff
};

enum TextureStage
{	
    TS_Unknown	    = 0,
    TS_Shader	    = 0x1,
    TS_Pass	        = 0x2,
    TS_Max	        = 0x7fffffff
};

enum FileOpenMode
{	
    FM_Unknown	    = 0,
    FM_Read	        = 0x1,
    FM_Write	    = 0x2,
    FM_Max	        = 0x7fffffff
};

enum LogLevel
{	
    LL_Unknown	    = 0,
    LL_Debug	    = 0x1,
    LL_Info	        = 0x2,
    LL_Warning	    = 0x4,
    LL_Error	    = 0x8,
    LL_External	    = 0x10,
    LL_Plugin       = 0x20,
    LL_Module	    = 0x40,
    LL_Framework	= 0x80,
    LL_Internal	    = 0x82,
    LL_Initial	    = 0xfe,
    LL_All	        = 0xff,
    LL_Max	        = 0x7fffffff
};

enum ParseFlags
{	
    PF_None	        = 0,
    PF_SingleSlash	= 0x1,
    PF_SlashOnly	= 0x2,
    PF_BackslashOnly= 0x4,
    PF_Lowercase	= 0x10,
    PF_Uppercase	= 0x20,
    PF_VarName	    = 0x100,
    PF_Max	        = 0x7fffffff
};

enum VariableType
{	
    VT_Normal	    = 0,
    VT_State	    = 0x1,
    VT_System	    = 0x10,
    VT_Max	        = 0x7fffffff
} ;

struct InitParams
{
    BSTR GlobalRoot;
    BSTR LocalRoot;
    BSTR RunRoot;
    BSTR Target;
    BSTR Loader;
    BSTR Config;
};

struct TextureDesc
{
    int Width;
    int Height;
    int Depth;
    VARIANT_BOOL Mipmaps;
    VARIANT_BOOL RenderTarget;
    TextureFormat Format;
};

struct TextureRegion
{
    int Width;
    int Height;
    int Depth;
    VARIANT_BOOL Mipmaps;
    VARIANT_BOOL RenderTarget;
    TextureFormat Format;
    int OffX;
    int OffY;
    int OffZ;
};

struct VersionStruct
{
    BSTR Name;
    int Major;
    int Minor;
    long Patch;
    long Rev;
    VARIANT_BOOL Debug;
};

struct float2
{
    union
    {
        float X, Y;
        float R, G;
        float U, V;
    };
};

struct float3
{
    union
    {
        float X, Y, Z;
        float R, G, B;
        float U, V, S;
    };
};

struct float4
{
    union
    {
        float X, Y, Z, W;
        float R, G, B, A;
        float U, V, S, T;
    };
};

struct VertexStruct
{
    float3 Position;
    float Winding;
    float2 TexCoord;
};

struct LightStruct
{
    int Type;
    float4 Diffuse;
    float4 Specular;
    float4 Ambient;
    float3 Position;
    float3 Direction;
    float Range;
    float Falloff;
    float3 Attenuation;
    float Theta;
    float Phi;
};

struct FunctionPtr
{
    unsigned __int64 Address;
};
