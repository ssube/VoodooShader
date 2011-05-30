

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat May 28 13:12:11 2011
 */
/* Compiler settings for Core.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: none
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Core_i_h__
#define __Core_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IVoodooCore_FWD_DEFINED__
#define __IVoodooCore_FWD_DEFINED__
typedef interface IVoodooCore IVoodooCore;
#endif     /* __IVoodooCore_FWD_DEFINED__ */


#ifndef __IVoodooParser_FWD_DEFINED__
#define __IVoodooParser_FWD_DEFINED__
typedef interface IVoodooParser IVoodooParser;
#endif     /* __IVoodooParser_FWD_DEFINED__ */


#ifndef __IVoodooPlugin_FWD_DEFINED__
#define __IVoodooPlugin_FWD_DEFINED__
typedef interface IVoodooPlugin IVoodooPlugin;
#endif     /* __IVoodooPlugin_FWD_DEFINED__ */


#ifndef __IVoodooShader_FWD_DEFINED__
#define __IVoodooShader_FWD_DEFINED__
typedef interface IVoodooShader IVoodooShader;
#endif     /* __IVoodooShader_FWD_DEFINED__ */


#ifndef __IVoodooTechnique_FWD_DEFINED__
#define __IVoodooTechnique_FWD_DEFINED__
typedef interface IVoodooTechnique IVoodooTechnique;
#endif     /* __IVoodooTechnique_FWD_DEFINED__ */


#ifndef __IVoodooPass_FWD_DEFINED__
#define __IVoodooPass_FWD_DEFINED__
typedef interface IVoodooPass IVoodooPass;
#endif     /* __IVoodooPass_FWD_DEFINED__ */


#ifndef __IVoodooTexture_FWD_DEFINED__
#define __IVoodooTexture_FWD_DEFINED__
typedef interface IVoodooTexture IVoodooTexture;
#endif     /* __IVoodooTexture_FWD_DEFINED__ */


#ifndef __IVoodooParameter_FWD_DEFINED__
#define __IVoodooParameter_FWD_DEFINED__
typedef interface IVoodooParameter IVoodooParameter;
#endif     /* __IVoodooParameter_FWD_DEFINED__ */


#ifndef __IVoodooAdapter_FWD_DEFINED__
#define __IVoodooAdapter_FWD_DEFINED__
typedef interface IVoodooAdapter IVoodooAdapter;
#endif     /* __IVoodooAdapter_FWD_DEFINED__ */


#ifndef __IVoodooHookSystem_FWD_DEFINED__
#define __IVoodooHookSystem_FWD_DEFINED__
typedef interface IVoodooHookSystem IVoodooHookSystem;
#endif     /* __IVoodooHookSystem_FWD_DEFINED__ */


#ifndef __IVoodooLogger_FWD_DEFINED__
#define __IVoodooLogger_FWD_DEFINED__
typedef interface IVoodooLogger IVoodooLogger;
#endif     /* __IVoodooLogger_FWD_DEFINED__ */


#ifndef __IVoodooFileSystem_FWD_DEFINED__
#define __IVoodooFileSystem_FWD_DEFINED__
typedef interface IVoodooFileSystem IVoodooFileSystem;
#endif     /* __IVoodooFileSystem_FWD_DEFINED__ */


#ifndef __IVoodooFile_FWD_DEFINED__
#define __IVoodooFile_FWD_DEFINED__
typedef interface IVoodooFile IVoodooFile;
#endif     /* __IVoodooFile_FWD_DEFINED__ */


#ifndef __IVoodooImage_FWD_DEFINED__
#define __IVoodooImage_FWD_DEFINED__
typedef interface IVoodooImage IVoodooImage;
#endif     /* __IVoodooImage_FWD_DEFINED__ */


#ifndef __IComponentRegistrar_FWD_DEFINED__
#define __IComponentRegistrar_FWD_DEFINED__
typedef interface IComponentRegistrar IComponentRegistrar;
#endif     /* __IComponentRegistrar_FWD_DEFINED__ */


#ifndef __CompReg_FWD_DEFINED__
#define __CompReg_FWD_DEFINED__

#ifdef __cplusplus
typedef class CompReg CompReg;
#else
typedef struct CompReg CompReg;
#endif /* __cplusplus */

#endif     /* __CompReg_FWD_DEFINED__ */


#ifndef __VoodooCore_FWD_DEFINED__
#define __VoodooCore_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooCore VoodooCore;
#else
typedef struct VoodooCore VoodooCore;
#endif /* __cplusplus */

#endif     /* __VoodooCore_FWD_DEFINED__ */


#ifndef __VoodooParser_FWD_DEFINED__
#define __VoodooParser_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooParser VoodooParser;
#else
typedef struct VoodooParser VoodooParser;
#endif /* __cplusplus */

#endif     /* __VoodooParser_FWD_DEFINED__ */


#ifndef __VoodooShader_FWD_DEFINED__
#define __VoodooShader_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooShader VoodooShader;
#else
typedef struct VoodooShader VoodooShader;
#endif /* __cplusplus */

#endif     /* __VoodooShader_FWD_DEFINED__ */


#ifndef __VoodooTechnique_FWD_DEFINED__
#define __VoodooTechnique_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooTechnique VoodooTechnique;
#else
typedef struct VoodooTechnique VoodooTechnique;
#endif /* __cplusplus */

#endif     /* __VoodooTechnique_FWD_DEFINED__ */


#ifndef __VoodooPass_FWD_DEFINED__
#define __VoodooPass_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooPass VoodooPass;
#else
typedef struct VoodooPass VoodooPass;
#endif /* __cplusplus */

#endif     /* __VoodooPass_FWD_DEFINED__ */


#ifndef __VoodooTexture_FWD_DEFINED__
#define __VoodooTexture_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooTexture VoodooTexture;
#else
typedef struct VoodooTexture VoodooTexture;
#endif /* __cplusplus */

#endif     /* __VoodooTexture_FWD_DEFINED__ */


#ifndef __VoodooParameter_FWD_DEFINED__
#define __VoodooParameter_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooParameter VoodooParameter;
#else
typedef struct VoodooParameter VoodooParameter;
#endif /* __cplusplus */

#endif     /* __VoodooParameter_FWD_DEFINED__ */


#ifndef __VoodooAdapter_FWD_DEFINED__
#define __VoodooAdapter_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooAdapter VoodooAdapter;
#else
typedef struct VoodooAdapter VoodooAdapter;
#endif /* __cplusplus */

#endif     /* __VoodooAdapter_FWD_DEFINED__ */


#ifndef __VoodooHookSystem_FWD_DEFINED__
#define __VoodooHookSystem_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooHookSystem VoodooHookSystem;
#else
typedef struct VoodooHookSystem VoodooHookSystem;
#endif /* __cplusplus */

#endif     /* __VoodooHookSystem_FWD_DEFINED__ */


#ifndef __VoodooLogger_FWD_DEFINED__
#define __VoodooLogger_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooLogger VoodooLogger;
#else
typedef struct VoodooLogger VoodooLogger;
#endif /* __cplusplus */

#endif     /* __VoodooLogger_FWD_DEFINED__ */


#ifndef __VoodooFileSystem_FWD_DEFINED__
#define __VoodooFileSystem_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooFileSystem VoodooFileSystem;
#else
typedef struct VoodooFileSystem VoodooFileSystem;
#endif /* __cplusplus */

#endif     /* __VoodooFileSystem_FWD_DEFINED__ */


#ifndef __VoodooFile_FWD_DEFINED__
#define __VoodooFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooFile VoodooFile;
#else
typedef struct VoodooFile VoodooFile;
#endif /* __cplusplus */

#endif     /* __VoodooFile_FWD_DEFINED__ */


#ifndef __VoodooImage_FWD_DEFINED__
#define __VoodooImage_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoodooImage VoodooImage;
#else
typedef struct VoodooImage VoodooImage;
#endif /* __cplusplus */

#endif     /* __VoodooImage_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Core_0000_0000 */
/* [local] */ 

typedef /* [v1_enum] */ 
enum TextureFormat
    {    TF_Unknown    = 0,
    TF_RGB5    = 0x1,
    TF_RGB5A1    = 0x2,
    TF_RGB8    = 0x3,
    TF_RGBA8    = 0x4,
    TF_RGB10A2    = 0x5,
    TF_RGBA16F    = 0x10,
    TF_RGBA32F    = 0x11,
    TF_D16    = 0x40,
    TF_D32    = 0x41,
    TF_Max    = 0x7fffffff
    }     TextureFormat;

typedef /* [v1_enum] */ 
enum ParameterType
    {    PT_Unknown    = 0,
    PT_Float1    = 0x11,
    PT_Float2    = 0x12,
    PT_Float3    = 0x13,
    PT_Float4    = 0x14,
    PT_Float1x1    = 0x11,
    PT_Float1x2    = 0x12,
    PT_Float1x3    = 0x13,
    PT_Float1x4    = 0x14,
    PT_Float2x1    = 0x21,
    PT_Float2x2    = 0x22,
    PT_Float2x3    = 0x23,
    PT_Float2x4    = 0x24,
    PT_Float3x1    = 0x31,
    PT_Float3x2    = 0x32,
    PT_Float3x3    = 0x33,
    PT_Float3x4    = 0x34,
    PT_Float4x1    = 0x41,
    PT_Float4x2    = 0x42,
    PT_Float4x3    = 0x43,
    PT_Float4x4    = 0x44,
    PT_Sampler1D    = 0x101,
    PT_Sampler2D    = 0x102,
    PT_Sampler3D    = 0x103,
    PT_Struct    = 0x1000,
    PT_Max    = 0x7fffffff
    }     ParameterType;

typedef /* [v1_enum] */ 
enum ParameterCategory
    {    PC_Unknown    = 0,
    PC_Float    = 0x1,
    PC_Sampler    = 0x2,
    PC_Struct    = 0x4,
    PC_Max    = 0x7fffffff
    }     ParameterCategory;

typedef /* [v1_enum] */ 
enum ProgramStage
    {    PS_Unknown    = 0,
    PS_Vertex    = 0x1,
    PS_Fragment    = 0x2,
    PS_Geometry    = 0x3,
    PS_Domain    = 0x4,
    PS_Hull    = 0x5,
    PS_Max    = 0x7fffffff
    }     ProgramStage;

typedef /* [v1_enum] */ 
enum TextureStage
    {    TS_Unknown    = 0,
    TS_Shader    = 0x1,
    TS_Pass    = 0x2,
    TS_Max    = 0x7fffffff
    }     TextureStage;

typedef /* [v1_enum] */ 
enum FileOpenMode
    {    FM_Unknown    = 0,
    FM_Read    = 0x1,
    FM_Write    = 0x2,
    FM_Max    = 0x7fffffff
    }     FileOpenMode;

typedef /* [v1_enum] */ 
enum LogLevel
    {    LL_Unknown    = 0,
    LL_Debug    = 0x1,
    LL_Info    = 0x2,
    LL_Warning    = 0x4,
    LL_Error    = 0x8,
    LL_External    = 0x10,
    LL_Plugin    = 0x20,
    LL_Module    = 0x40,
    LL_Framework    = 0x80,
    LL_Internal    = 0x82,
    LL_Initial    = 0xfe,
    LL_All    = 0xff,
    LL_Max    = 0x7fffffff
    }     LogLevel;

typedef /* [v1_enum] */ 
enum ParseFlags
    {    PF_None    = 0,
    PF_SingleSlash    = 0x1,
    PF_SlashOnly    = 0x2,
    PF_BackslashOnly    = 0x4,
    PF_Lowercase    = 0x10,
    PF_Uppercase    = 0x20,
    PF_VarName    = 0x100,
    PF_Max    = 0x7fffffff
    }     ParseFlags;

typedef /* [v1_enum] */ 
enum VariableType
    {    VT_Normal    = 0,
    VT_State    = 0x1,
    VT_System    = 0x10,
    VT_Max    = 0x7fffffff
    }     VariableType;

typedef /* [helpstring] */ struct InitParams
    {
    BSTR GlobalRoot;
    BSTR LocalRoot;
    BSTR RunRoot;
    BSTR Target;
    BSTR Loader;
    BSTR Config;
    }     InitParams;

typedef /* [helpstring] */ struct TextureDesc
    {
    int Width;
    int Height;
    int Depth;
    VARIANT_BOOL Mipmaps;
    VARIANT_BOOL RenderTarget;
    TextureFormat Format;
    }     TextureDesc;

typedef /* [helpstring] */ struct TextureRegion
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
    }     TextureRegion;

typedef /* [helpstring] */ struct VersionStruct
    {
    BSTR Name;
    int Major;
    int Minor;
    long Patch;
    long Rev;
    VARIANT_BOOL Debug;
    }     VersionStruct;

typedef /* [helpstring] */ struct float2
    {
    float X;
    float Y;
    }     float2;

typedef /* [helpstring] */ struct float3
    {
    float X;
    float Y;
    float Z;
    }     float3;

typedef /* [helpstring] */ struct float4
    {
    float X;
    float Y;
    float Z;
    float W;
    }     float4;

typedef /* [helpstring] */ struct VertexStruct
    {
    float3 Position;
    float Winding;
    float2 TexCoord;
    }     VertexStruct;

typedef /* [helpstring] */ struct LightStruct
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
    }     LightStruct;

typedef /* [helpstring] */ struct FunctionPtr
    {
    unsigned __int64 Address;
    }     FunctionPtr;

typedef DWORD EnumType;

















extern RPC_IF_HANDLE __MIDL_itf_Core_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Core_0000_0000_v0_0_s_ifspec;

#ifndef __IVoodooCore_INTERFACE_DEFINED__
#define __IVoodooCore_INTERFACE_DEFINED__

/* interface IVoodooCore */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooCore;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f86-6f32-11e0-8ac0-005056c00000")
    IVoodooCore : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ const InitParams pParams) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Parser( 
            /* [retval][out] */ IVoodooParser **ppParser) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HookSystem( 
            /* [retval][out] */ IVoodooHookSystem **ppHookSystem) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FileSystem( 
            /* [retval][out] */ IVoodooFileSystem **ppFileSystem) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Adapter( 
            /* [retval][out] */ IVoodooAdapter **ppAdapter) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Logger( 
            /* [retval][out] */ IVoodooLogger **ppLogger) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Config( 
            /* [retval][out] */ IUnknown **ppConfig) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CgContext( 
            /* [retval][out] */ VARIANT *pCgContext) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_CgContext( 
            /* [in] */ VARIANT CgContext) = 0;
        
        virtual /* [id][vararg] */ HRESULT STDMETHODCALLTYPE CreateShader( 
            /* [in] */ IVoodooFile *pFile,
            /* [in] */ SAFEARRAY * pArgs,
            /* [retval][out] */ IVoodooShader **ppShader) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateParameter( 
            /* [in] */ BSTR pName,
            /* [in] */ EnumType Type,
            /* [retval][out] */ IVoodooParameter **ppParameter) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateTexture( 
            /* [in] */ BSTR pName,
            /* [in] */ TextureDesc Desc,
            /* [in] */ VARIANT Data,
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetParameter( 
            /* [in] */ BSTR pName,
            /* [retval][out] */ IVoodooParameter **ppParameter) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTexture( 
            /* [in] */ BSTR pName,
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveParameter( 
            /* [in] */ BSTR pName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveTexture( 
            /* [in] */ BSTR pName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetStageTexture( 
            /* [in] */ EnumType Stage,
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetStageTexture( 
            /* [in] */ EnumType Stage,
            /* [in] */ IVoodooTexture *pTexture) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooCoreVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooCore * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooCore * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooCore * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooCore * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooCore * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooCore * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooCore * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IVoodooCore * This,
            /* [in] */ const InitParams pParams);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Parser )( 
            IVoodooCore * This,
            /* [retval][out] */ IVoodooParser **ppParser);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HookSystem )( 
            IVoodooCore * This,
            /* [retval][out] */ IVoodooHookSystem **ppHookSystem);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileSystem )( 
            IVoodooCore * This,
            /* [retval][out] */ IVoodooFileSystem **ppFileSystem);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Adapter )( 
            IVoodooCore * This,
            /* [retval][out] */ IVoodooAdapter **ppAdapter);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Logger )( 
            IVoodooCore * This,
            /* [retval][out] */ IVoodooLogger **ppLogger);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Config )( 
            IVoodooCore * This,
            /* [retval][out] */ IUnknown **ppConfig);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CgContext )( 
            IVoodooCore * This,
            /* [retval][out] */ VARIANT *pCgContext);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CgContext )( 
            IVoodooCore * This,
            /* [in] */ VARIANT CgContext);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE *CreateShader )( 
            IVoodooCore * This,
            /* [in] */ IVoodooFile *pFile,
            /* [in] */ SAFEARRAY * pArgs,
            /* [retval][out] */ IVoodooShader **ppShader);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateParameter )( 
            IVoodooCore * This,
            /* [in] */ BSTR pName,
            /* [in] */ EnumType Type,
            /* [retval][out] */ IVoodooParameter **ppParameter);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateTexture )( 
            IVoodooCore * This,
            /* [in] */ BSTR pName,
            /* [in] */ TextureDesc Desc,
            /* [in] */ VARIANT Data,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetParameter )( 
            IVoodooCore * This,
            /* [in] */ BSTR pName,
            /* [retval][out] */ IVoodooParameter **ppParameter);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTexture )( 
            IVoodooCore * This,
            /* [in] */ BSTR pName,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveParameter )( 
            IVoodooCore * This,
            /* [in] */ BSTR pName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveTexture )( 
            IVoodooCore * This,
            /* [in] */ BSTR pName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetStageTexture )( 
            IVoodooCore * This,
            /* [in] */ EnumType Stage,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetStageTexture )( 
            IVoodooCore * This,
            /* [in] */ EnumType Stage,
            /* [in] */ IVoodooTexture *pTexture);
        
        END_INTERFACE
    } IVoodooCoreVtbl;

    interface IVoodooCore
    {
        CONST_VTBL struct IVoodooCoreVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooCore_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooCore_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooCore_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooCore_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooCore_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooCore_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooCore_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooCore_Initialize(This,pParams)    \
    ( (This)->lpVtbl -> Initialize(This,pParams) ) 

#define IVoodooCore_get_Parser(This,ppParser)    \
    ( (This)->lpVtbl -> get_Parser(This,ppParser) ) 

#define IVoodooCore_get_HookSystem(This,ppHookSystem)    \
    ( (This)->lpVtbl -> get_HookSystem(This,ppHookSystem) ) 

#define IVoodooCore_get_FileSystem(This,ppFileSystem)    \
    ( (This)->lpVtbl -> get_FileSystem(This,ppFileSystem) ) 

#define IVoodooCore_get_Adapter(This,ppAdapter)    \
    ( (This)->lpVtbl -> get_Adapter(This,ppAdapter) ) 

#define IVoodooCore_get_Logger(This,ppLogger)    \
    ( (This)->lpVtbl -> get_Logger(This,ppLogger) ) 

#define IVoodooCore_get_Config(This,ppConfig)    \
    ( (This)->lpVtbl -> get_Config(This,ppConfig) ) 

#define IVoodooCore_get_CgContext(This,pCgContext)    \
    ( (This)->lpVtbl -> get_CgContext(This,pCgContext) ) 

#define IVoodooCore_put_CgContext(This,CgContext)    \
    ( (This)->lpVtbl -> put_CgContext(This,CgContext) ) 

#define IVoodooCore_CreateShader(This,pFile,pArgs,ppShader)    \
    ( (This)->lpVtbl -> CreateShader(This,pFile,pArgs,ppShader) ) 

#define IVoodooCore_CreateParameter(This,pName,Type,ppParameter)    \
    ( (This)->lpVtbl -> CreateParameter(This,pName,Type,ppParameter) ) 

#define IVoodooCore_CreateTexture(This,pName,Desc,Data,ppTexture)    \
    ( (This)->lpVtbl -> CreateTexture(This,pName,Desc,Data,ppTexture) ) 

#define IVoodooCore_GetParameter(This,pName,ppParameter)    \
    ( (This)->lpVtbl -> GetParameter(This,pName,ppParameter) ) 

#define IVoodooCore_GetTexture(This,pName,ppTexture)    \
    ( (This)->lpVtbl -> GetTexture(This,pName,ppTexture) ) 

#define IVoodooCore_RemoveParameter(This,pName)    \
    ( (This)->lpVtbl -> RemoveParameter(This,pName) ) 

#define IVoodooCore_RemoveTexture(This,pName)    \
    ( (This)->lpVtbl -> RemoveTexture(This,pName) ) 

#define IVoodooCore_GetStageTexture(This,Stage,ppTexture)    \
    ( (This)->lpVtbl -> GetStageTexture(This,Stage,ppTexture) ) 

#define IVoodooCore_SetStageTexture(This,Stage,pTexture)    \
    ( (This)->lpVtbl -> SetStageTexture(This,Stage,pTexture) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooCore_INTERFACE_DEFINED__ */


#ifndef __IVoodooParser_INTERFACE_DEFINED__
#define __IVoodooParser_INTERFACE_DEFINED__

/* interface IVoodooParser */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f87-6f32-11e0-8ac0-005056c00000")
    IVoodooParser : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR pName,
            /* [in] */ BSTR pValue,
            /* [defaultvalue][in] */ EnumType System = VT_Normal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ BSTR pName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Parse( 
            /* [in] */ BSTR pString,
            /* [defaultvalue][in] */ EnumType Flags,
            /* [retval][out] */ LPBSTR pParsed) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooParser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooParser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooParser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooParser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooParser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IVoodooParser * This,
            /* [in] */ BSTR pName,
            /* [in] */ BSTR pValue,
            /* [defaultvalue][in] */ EnumType System);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IVoodooParser * This,
            /* [in] */ BSTR pName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Parse )( 
            IVoodooParser * This,
            /* [in] */ BSTR pString,
            /* [defaultvalue][in] */ EnumType Flags,
            /* [retval][out] */ LPBSTR pParsed);
        
        END_INTERFACE
    } IVoodooParserVtbl;

    interface IVoodooParser
    {
        CONST_VTBL struct IVoodooParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooParser_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooParser_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooParser_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooParser_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooParser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooParser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooParser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooParser_Add(This,pName,pValue,System)    \
    ( (This)->lpVtbl -> Add(This,pName,pValue,System) ) 

#define IVoodooParser_Remove(This,pName)    \
    ( (This)->lpVtbl -> Remove(This,pName) ) 

#define IVoodooParser_Parse(This,pString,Flags,pParsed)    \
    ( (This)->lpVtbl -> Parse(This,pString,Flags,pParsed) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooParser_INTERFACE_DEFINED__ */


#ifndef __IVoodooPlugin_INTERFACE_DEFINED__
#define __IVoodooPlugin_INTERFACE_DEFINED__

/* interface IVoodooPlugin */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f93-6f32-11e0-8ac0-005056c00000")
    IVoodooPlugin : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IVoodooCore *pCore) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
            /* [retval][out] */ IVoodooCore **ppCore) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooPlugin * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooPlugin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooPlugin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooPlugin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooPlugin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooPlugin * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IVoodooPlugin * This,
            /* [in] */ IVoodooCore *pCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooPlugin * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        END_INTERFACE
    } IVoodooPluginVtbl;

    interface IVoodooPlugin
    {
        CONST_VTBL struct IVoodooPluginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooPlugin_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooPlugin_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooPlugin_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooPlugin_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooPlugin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooPlugin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooPlugin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooPlugin_Initialize(This,pCore)    \
    ( (This)->lpVtbl -> Initialize(This,pCore) ) 

#define IVoodooPlugin_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooPlugin_INTERFACE_DEFINED__ */


#ifndef __IVoodooShader_INTERFACE_DEFINED__
#define __IVoodooShader_INTERFACE_DEFINED__

/* interface IVoodooShader */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooShader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f88-6f32-11e0-8ac0-005056c00000")
    IVoodooShader : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ LPBSTR ppName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
            /* [retval][out] */ IVoodooCore **ppCore) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TechniqueCount( 
            /* [retval][out] */ int *pCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTechnique( 
            /* [in] */ int Number,
            /* [retval][out] */ IVoodooTechnique **ppTechnique) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DefaultTechnique( 
            /* [retval][out] */ IVoodooTechnique **ppTechnique) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DefaultTechnique( 
            /* [in] */ IVoodooTechnique *pTechnique) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ParameterCount( 
            /* [retval][out] */ int *pCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetParameter( 
            /* [in] */ int Number,
            /* [retval][out] */ IVoodooParameter **ppParameter) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CgShader( 
            /* [retval][out] */ VARIANT *pCgShader) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooShaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooShader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooShader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooShader * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooShader * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooShader * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooShader * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooShader * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IVoodooShader * This,
            /* [retval][out] */ LPBSTR ppName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooShader * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TechniqueCount )( 
            IVoodooShader * This,
            /* [retval][out] */ int *pCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTechnique )( 
            IVoodooShader * This,
            /* [in] */ int Number,
            /* [retval][out] */ IVoodooTechnique **ppTechnique);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultTechnique )( 
            IVoodooShader * This,
            /* [retval][out] */ IVoodooTechnique **ppTechnique);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DefaultTechnique )( 
            IVoodooShader * This,
            /* [in] */ IVoodooTechnique *pTechnique);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParameterCount )( 
            IVoodooShader * This,
            /* [retval][out] */ int *pCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetParameter )( 
            IVoodooShader * This,
            /* [in] */ int Number,
            /* [retval][out] */ IVoodooParameter **ppParameter);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CgShader )( 
            IVoodooShader * This,
            /* [retval][out] */ VARIANT *pCgShader);
        
        END_INTERFACE
    } IVoodooShaderVtbl;

    interface IVoodooShader
    {
        CONST_VTBL struct IVoodooShaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooShader_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooShader_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooShader_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooShader_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooShader_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooShader_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooShader_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooShader_get_Name(This,ppName)    \
    ( (This)->lpVtbl -> get_Name(This,ppName) ) 

#define IVoodooShader_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 

#define IVoodooShader_get_TechniqueCount(This,pCount)    \
    ( (This)->lpVtbl -> get_TechniqueCount(This,pCount) ) 

#define IVoodooShader_GetTechnique(This,Number,ppTechnique)    \
    ( (This)->lpVtbl -> GetTechnique(This,Number,ppTechnique) ) 

#define IVoodooShader_get_DefaultTechnique(This,ppTechnique)    \
    ( (This)->lpVtbl -> get_DefaultTechnique(This,ppTechnique) ) 

#define IVoodooShader_put_DefaultTechnique(This,pTechnique)    \
    ( (This)->lpVtbl -> put_DefaultTechnique(This,pTechnique) ) 

#define IVoodooShader_get_ParameterCount(This,pCount)    \
    ( (This)->lpVtbl -> get_ParameterCount(This,pCount) ) 

#define IVoodooShader_GetParameter(This,Number,ppParameter)    \
    ( (This)->lpVtbl -> GetParameter(This,Number,ppParameter) ) 

#define IVoodooShader_get_CgShader(This,pCgShader)    \
    ( (This)->lpVtbl -> get_CgShader(This,pCgShader) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooShader_INTERFACE_DEFINED__ */


#ifndef __IVoodooTechnique_INTERFACE_DEFINED__
#define __IVoodooTechnique_INTERFACE_DEFINED__

/* interface IVoodooTechnique */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooTechnique;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f89-6f32-11e0-8ac0-005056c00000")
    IVoodooTechnique : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ LPBSTR ppName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
            /* [retval][out] */ IVoodooCore **ppCore) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Target( 
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PassCount( 
            /* [retval][out] */ int *pCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPass( 
            /* [in] */ int Number,
            /* [retval][out] */ IVoodooPass **ppPass) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Shader( 
            /* [retval][out] */ IVoodooShader **ppShader) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CgTechnique( 
            /* [retval][out] */ VARIANT *pCgTechnique) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooTechniqueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooTechnique * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooTechnique * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooTechnique * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooTechnique * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooTechnique * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooTechnique * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooTechnique * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IVoodooTechnique * This,
            /* [retval][out] */ LPBSTR ppName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooTechnique * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Target )( 
            IVoodooTechnique * This,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PassCount )( 
            IVoodooTechnique * This,
            /* [retval][out] */ int *pCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPass )( 
            IVoodooTechnique * This,
            /* [in] */ int Number,
            /* [retval][out] */ IVoodooPass **ppPass);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Shader )( 
            IVoodooTechnique * This,
            /* [retval][out] */ IVoodooShader **ppShader);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CgTechnique )( 
            IVoodooTechnique * This,
            /* [retval][out] */ VARIANT *pCgTechnique);
        
        END_INTERFACE
    } IVoodooTechniqueVtbl;

    interface IVoodooTechnique
    {
        CONST_VTBL struct IVoodooTechniqueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooTechnique_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooTechnique_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooTechnique_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooTechnique_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooTechnique_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooTechnique_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooTechnique_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooTechnique_get_Name(This,ppName)    \
    ( (This)->lpVtbl -> get_Name(This,ppName) ) 

#define IVoodooTechnique_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 

#define IVoodooTechnique_get_Target(This,ppTexture)    \
    ( (This)->lpVtbl -> get_Target(This,ppTexture) ) 

#define IVoodooTechnique_get_PassCount(This,pCount)    \
    ( (This)->lpVtbl -> get_PassCount(This,pCount) ) 

#define IVoodooTechnique_GetPass(This,Number,ppPass)    \
    ( (This)->lpVtbl -> GetPass(This,Number,ppPass) ) 

#define IVoodooTechnique_get_Shader(This,ppShader)    \
    ( (This)->lpVtbl -> get_Shader(This,ppShader) ) 

#define IVoodooTechnique_get_CgTechnique(This,pCgTechnique)    \
    ( (This)->lpVtbl -> get_CgTechnique(This,pCgTechnique) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooTechnique_INTERFACE_DEFINED__ */


#ifndef __IVoodooPass_INTERFACE_DEFINED__
#define __IVoodooPass_INTERFACE_DEFINED__

/* interface IVoodooPass */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooPass;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f8a-6f32-11e0-8ac0-005056c00000")
    IVoodooPass : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ LPBSTR ppName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
            /* [retval][out] */ IVoodooCore **ppCore) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Target( 
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Shader( 
            /* [retval][out] */ IVoodooShader **ppShader) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Technique( 
            /* [retval][out] */ IVoodooTechnique **ppTechnique) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCgProgram( 
            /* [in] */ EnumType Stage,
            /* [retval][out] */ VARIANT *pProgram) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CgPass( 
            /* [retval][out] */ VARIANT *pCgPass) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooPassVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooPass * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooPass * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooPass * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooPass * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooPass * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooPass * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooPass * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IVoodooPass * This,
            /* [retval][out] */ LPBSTR ppName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooPass * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Target )( 
            IVoodooPass * This,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Shader )( 
            IVoodooPass * This,
            /* [retval][out] */ IVoodooShader **ppShader);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Technique )( 
            IVoodooPass * This,
            /* [retval][out] */ IVoodooTechnique **ppTechnique);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCgProgram )( 
            IVoodooPass * This,
            /* [in] */ EnumType Stage,
            /* [retval][out] */ VARIANT *pProgram);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CgPass )( 
            IVoodooPass * This,
            /* [retval][out] */ VARIANT *pCgPass);
        
        END_INTERFACE
    } IVoodooPassVtbl;

    interface IVoodooPass
    {
        CONST_VTBL struct IVoodooPassVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooPass_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooPass_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooPass_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooPass_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooPass_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooPass_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooPass_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooPass_get_Name(This,ppName)    \
    ( (This)->lpVtbl -> get_Name(This,ppName) ) 

#define IVoodooPass_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 

#define IVoodooPass_get_Target(This,ppTexture)    \
    ( (This)->lpVtbl -> get_Target(This,ppTexture) ) 

#define IVoodooPass_get_Shader(This,ppShader)    \
    ( (This)->lpVtbl -> get_Shader(This,ppShader) ) 

#define IVoodooPass_get_Technique(This,ppTechnique)    \
    ( (This)->lpVtbl -> get_Technique(This,ppTechnique) ) 

#define IVoodooPass_GetCgProgram(This,Stage,pProgram)    \
    ( (This)->lpVtbl -> GetCgProgram(This,Stage,pProgram) ) 

#define IVoodooPass_get_CgPass(This,pCgPass)    \
    ( (This)->lpVtbl -> get_CgPass(This,pCgPass) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooPass_INTERFACE_DEFINED__ */


#ifndef __IVoodooTexture_INTERFACE_DEFINED__
#define __IVoodooTexture_INTERFACE_DEFINED__

/* interface IVoodooTexture */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooTexture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f8b-6f32-11e0-8ac0-005056c00000")
    IVoodooTexture : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ LPBSTR ppName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
            /* [retval][out] */ IVoodooCore **ppCore) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
            /* [retval][out] */ VARIANT *pData) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Desc( 
            /* [retval][out] */ TextureDesc *pDesc) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooTextureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooTexture * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooTexture * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooTexture * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooTexture * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooTexture * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooTexture * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooTexture * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IVoodooTexture * This,
            /* [retval][out] */ LPBSTR ppName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooTexture * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            IVoodooTexture * This,
            /* [retval][out] */ VARIANT *pData);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Desc )( 
            IVoodooTexture * This,
            /* [retval][out] */ TextureDesc *pDesc);
        
        END_INTERFACE
    } IVoodooTextureVtbl;

    interface IVoodooTexture
    {
        CONST_VTBL struct IVoodooTextureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooTexture_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooTexture_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooTexture_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooTexture_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooTexture_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooTexture_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooTexture_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooTexture_get_Name(This,ppName)    \
    ( (This)->lpVtbl -> get_Name(This,ppName) ) 

#define IVoodooTexture_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 

#define IVoodooTexture_get_Data(This,pData)    \
    ( (This)->lpVtbl -> get_Data(This,pData) ) 

#define IVoodooTexture_get_Desc(This,pDesc)    \
    ( (This)->lpVtbl -> get_Desc(This,pDesc) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooTexture_INTERFACE_DEFINED__ */


#ifndef __IVoodooParameter_INTERFACE_DEFINED__
#define __IVoodooParameter_INTERFACE_DEFINED__

/* interface IVoodooParameter */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooParameter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f8c-6f32-11e0-8ac0-005056c00000")
    IVoodooParameter : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ LPBSTR ppName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
            /* [retval][out] */ IVoodooCore **ppCore) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ EnumType *pType) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Virtual( 
            /* [retval][out] */ VARIANT_BOOL *pVirtual) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AttachParameter( 
            /* [in] */ IVoodooParameter *pParameter) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Components( 
            /* [retval][out] */ int *pComponents) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SamplerValue( 
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_SamplerValue( 
            /* [in] */ IVoodooTexture *pTexture) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ScalarValue( 
            /* [retval][out] */ SAFEARRAY * *ppData) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ScalarValue( 
            /* [in] */ SAFEARRAY * pData) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Shader( 
            /* [retval][out] */ IVoodooShader **ppShader) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CgParameter( 
            /* [retval][out] */ VARIANT *pCgParameter) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooParameterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooParameter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooParameter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooParameter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooParameter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooParameter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooParameter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooParameter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IVoodooParameter * This,
            /* [retval][out] */ LPBSTR ppName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooParameter * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IVoodooParameter * This,
            /* [retval][out] */ EnumType *pType);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Virtual )( 
            IVoodooParameter * This,
            /* [retval][out] */ VARIANT_BOOL *pVirtual);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AttachParameter )( 
            IVoodooParameter * This,
            /* [in] */ IVoodooParameter *pParameter);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Components )( 
            IVoodooParameter * This,
            /* [retval][out] */ int *pComponents);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SamplerValue )( 
            IVoodooParameter * This,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SamplerValue )( 
            IVoodooParameter * This,
            /* [in] */ IVoodooTexture *pTexture);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ScalarValue )( 
            IVoodooParameter * This,
            /* [retval][out] */ SAFEARRAY * *ppData);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ScalarValue )( 
            IVoodooParameter * This,
            /* [in] */ SAFEARRAY * pData);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Shader )( 
            IVoodooParameter * This,
            /* [retval][out] */ IVoodooShader **ppShader);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CgParameter )( 
            IVoodooParameter * This,
            /* [retval][out] */ VARIANT *pCgParameter);
        
        END_INTERFACE
    } IVoodooParameterVtbl;

    interface IVoodooParameter
    {
        CONST_VTBL struct IVoodooParameterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooParameter_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooParameter_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooParameter_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooParameter_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooParameter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooParameter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooParameter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooParameter_get_Name(This,ppName)    \
    ( (This)->lpVtbl -> get_Name(This,ppName) ) 

#define IVoodooParameter_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 

#define IVoodooParameter_get_Type(This,pType)    \
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IVoodooParameter_get_Virtual(This,pVirtual)    \
    ( (This)->lpVtbl -> get_Virtual(This,pVirtual) ) 

#define IVoodooParameter_AttachParameter(This,pParameter)    \
    ( (This)->lpVtbl -> AttachParameter(This,pParameter) ) 

#define IVoodooParameter_get_Components(This,pComponents)    \
    ( (This)->lpVtbl -> get_Components(This,pComponents) ) 

#define IVoodooParameter_get_SamplerValue(This,ppTexture)    \
    ( (This)->lpVtbl -> get_SamplerValue(This,ppTexture) ) 

#define IVoodooParameter_put_SamplerValue(This,pTexture)    \
    ( (This)->lpVtbl -> put_SamplerValue(This,pTexture) ) 

#define IVoodooParameter_get_ScalarValue(This,ppData)    \
    ( (This)->lpVtbl -> get_ScalarValue(This,ppData) ) 

#define IVoodooParameter_put_ScalarValue(This,pData)    \
    ( (This)->lpVtbl -> put_ScalarValue(This,pData) ) 

#define IVoodooParameter_get_Shader(This,ppShader)    \
    ( (This)->lpVtbl -> get_Shader(This,ppShader) ) 

#define IVoodooParameter_get_CgParameter(This,pCgParameter)    \
    ( (This)->lpVtbl -> get_CgParameter(This,pCgParameter) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooParameter_INTERFACE_DEFINED__ */


#ifndef __IVoodooAdapter_INTERFACE_DEFINED__
#define __IVoodooAdapter_INTERFACE_DEFINED__

/* interface IVoodooAdapter */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooAdapter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f8d-6f32-11e0-8ac0-005056c00000")
    IVoodooAdapter : public IVoodooPlugin
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LoadPass( 
            /* [in] */ IVoodooPass *pPass) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnloadPass( 
            /* [in] */ IVoodooPass *pPass) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Pass( 
            /* [retval][out] */ IVoodooPass **ppPass) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Pass( 
            /* [in] */ IVoodooPass *pPass) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Target( 
            /* [retval][out] */ IVoodooTexture **ppTarget) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Target( 
            /* [in] */ IVoodooTexture *pTarget) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateTexture( 
            /* [in] */ BSTR pName,
            /* [in] */ TextureDesc Description,
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LoadTexture( 
            /* [in] */ IVoodooImage *pImage,
            /* [in] */ TextureRegion Region,
            /* [retval][out] */ IVoodooTexture **ppTexture) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DrawGeometry( 
            /* [in] */ int Vertexes,
            /* [in] */ SAFEARRAY * pVertexData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DrawShader( 
            /* [in] */ IVoodooShader *pShader) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ApplyParameter( 
            /* [in] */ IVoodooParameter *pParameter) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [in] */ BSTR pName,
            /* [out][in] */ VARIANT *pData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ConnectTexture( 
            /* [in] */ IVoodooParameter *pParameter,
            /* [in] */ IVoodooTexture *pTexture) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE HandleError( 
            /* [in] */ VARIANT pContext,
            /* [in] */ EnumType Error,
            /* [in] */ IVoodooCore *pCore) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooAdapterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooAdapter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooAdapter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooAdapter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooAdapter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooAdapter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooAdapter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooAdapter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooCore *pCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooAdapter * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LoadPass )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooPass *pPass);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnloadPass )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooPass *pPass);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Pass )( 
            IVoodooAdapter * This,
            /* [retval][out] */ IVoodooPass **ppPass);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Pass )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooPass *pPass);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Target )( 
            IVoodooAdapter * This,
            /* [retval][out] */ IVoodooTexture **ppTarget);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Target )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooTexture *pTarget);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateTexture )( 
            IVoodooAdapter * This,
            /* [in] */ BSTR pName,
            /* [in] */ TextureDesc Description,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LoadTexture )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooImage *pImage,
            /* [in] */ TextureRegion Region,
            /* [retval][out] */ IVoodooTexture **ppTexture);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DrawGeometry )( 
            IVoodooAdapter * This,
            /* [in] */ int Vertexes,
            /* [in] */ SAFEARRAY * pVertexData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DrawShader )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooShader *pShader);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ApplyParameter )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooParameter *pParameter);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetProperty )( 
            IVoodooAdapter * This,
            /* [in] */ BSTR pName,
            /* [out][in] */ VARIANT *pData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ConnectTexture )( 
            IVoodooAdapter * This,
            /* [in] */ IVoodooParameter *pParameter,
            /* [in] */ IVoodooTexture *pTexture);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *HandleError )( 
            IVoodooAdapter * This,
            /* [in] */ VARIANT pContext,
            /* [in] */ EnumType Error,
            /* [in] */ IVoodooCore *pCore);
        
        END_INTERFACE
    } IVoodooAdapterVtbl;

    interface IVoodooAdapter
    {
        CONST_VTBL struct IVoodooAdapterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooAdapter_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooAdapter_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooAdapter_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooAdapter_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooAdapter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooAdapter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooAdapter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooAdapter_Initialize(This,pCore)    \
    ( (This)->lpVtbl -> Initialize(This,pCore) ) 

#define IVoodooAdapter_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 


#define IVoodooAdapter_LoadPass(This,pPass)    \
    ( (This)->lpVtbl -> LoadPass(This,pPass) ) 

#define IVoodooAdapter_UnloadPass(This,pPass)    \
    ( (This)->lpVtbl -> UnloadPass(This,pPass) ) 

#define IVoodooAdapter_get_Pass(This,ppPass)    \
    ( (This)->lpVtbl -> get_Pass(This,ppPass) ) 

#define IVoodooAdapter_put_Pass(This,pPass)    \
    ( (This)->lpVtbl -> put_Pass(This,pPass) ) 

#define IVoodooAdapter_get_Target(This,ppTarget)    \
    ( (This)->lpVtbl -> get_Target(This,ppTarget) ) 

#define IVoodooAdapter_put_Target(This,pTarget)    \
    ( (This)->lpVtbl -> put_Target(This,pTarget) ) 

#define IVoodooAdapter_CreateTexture(This,pName,Description,ppTexture)    \
    ( (This)->lpVtbl -> CreateTexture(This,pName,Description,ppTexture) ) 

#define IVoodooAdapter_LoadTexture(This,pImage,Region,ppTexture)    \
    ( (This)->lpVtbl -> LoadTexture(This,pImage,Region,ppTexture) ) 

#define IVoodooAdapter_DrawGeometry(This,Vertexes,pVertexData)    \
    ( (This)->lpVtbl -> DrawGeometry(This,Vertexes,pVertexData) ) 

#define IVoodooAdapter_DrawShader(This,pShader)    \
    ( (This)->lpVtbl -> DrawShader(This,pShader) ) 

#define IVoodooAdapter_ApplyParameter(This,pParameter)    \
    ( (This)->lpVtbl -> ApplyParameter(This,pParameter) ) 

#define IVoodooAdapter_SetProperty(This,pName,pData)    \
    ( (This)->lpVtbl -> SetProperty(This,pName,pData) ) 

#define IVoodooAdapter_ConnectTexture(This,pParameter,pTexture)    \
    ( (This)->lpVtbl -> ConnectTexture(This,pParameter,pTexture) ) 

#define IVoodooAdapter_HandleError(This,pContext,Error,pCore)    \
    ( (This)->lpVtbl -> HandleError(This,pContext,Error,pCore) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooAdapter_INTERFACE_DEFINED__ */


#ifndef __IVoodooHookSystem_INTERFACE_DEFINED__
#define __IVoodooHookSystem_INTERFACE_DEFINED__

/* interface IVoodooHookSystem */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooHookSystem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f8e-6f32-11e0-8ac0-005056c00000")
    IVoodooHookSystem : public IVoodooPlugin
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR pName,
            /* [in] */ FunctionPtr pSource,
            /* [in] */ FunctionPtr pDest) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ BSTR pName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooHookSystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooHookSystem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooHookSystem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooHookSystem * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooHookSystem * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooHookSystem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooHookSystem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooHookSystem * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IVoodooHookSystem * This,
            /* [in] */ IVoodooCore *pCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooHookSystem * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IVoodooHookSystem * This,
            /* [in] */ BSTR pName,
            /* [in] */ FunctionPtr pSource,
            /* [in] */ FunctionPtr pDest);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IVoodooHookSystem * This,
            /* [in] */ BSTR pName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IVoodooHookSystem * This);
        
        END_INTERFACE
    } IVoodooHookSystemVtbl;

    interface IVoodooHookSystem
    {
        CONST_VTBL struct IVoodooHookSystemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooHookSystem_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooHookSystem_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooHookSystem_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooHookSystem_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooHookSystem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooHookSystem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooHookSystem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooHookSystem_Initialize(This,pCore)    \
    ( (This)->lpVtbl -> Initialize(This,pCore) ) 

#define IVoodooHookSystem_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 


#define IVoodooHookSystem_Add(This,pName,pSource,pDest)    \
    ( (This)->lpVtbl -> Add(This,pName,pSource,pDest) ) 

#define IVoodooHookSystem_Remove(This,pName)    \
    ( (This)->lpVtbl -> Remove(This,pName) ) 

#define IVoodooHookSystem_RemoveAll(This)    \
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooHookSystem_INTERFACE_DEFINED__ */


#ifndef __IVoodooLogger_INTERFACE_DEFINED__
#define __IVoodooLogger_INTERFACE_DEFINED__

/* interface IVoodooLogger */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooLogger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f8f-6f32-11e0-8ac0-005056c00000")
    IVoodooLogger : public IVoodooPlugin
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR pFilename,
            /* [in] */ VARIANT_BOOL Append) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Dump( void) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LogLevel( 
            /* [retval][out] */ EnumType *pLevel) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LogLevel( 
            /* [in] */ EnumType Level) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LogModule( 
            /* [in] */ VersionStruct Module) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Log( 
            /* [in] */ EnumType Level,
            /* [in] */ BSTR pModule,
            /* [in] */ BSTR pMessage) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BufferSize( 
            /* [retval][out] */ int *pSize) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BufferSize( 
            /* [in] */ int Size) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooLoggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooLogger * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooLogger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooLogger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooLogger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooLogger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooLogger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooLogger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IVoodooLogger * This,
            /* [in] */ IVoodooCore *pCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooLogger * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IVoodooLogger * This,
            /* [in] */ BSTR pFilename,
            /* [in] */ VARIANT_BOOL Append);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IVoodooLogger * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Dump )( 
            IVoodooLogger * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LogLevel )( 
            IVoodooLogger * This,
            /* [retval][out] */ EnumType *pLevel);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LogLevel )( 
            IVoodooLogger * This,
            /* [in] */ EnumType Level);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LogModule )( 
            IVoodooLogger * This,
            /* [in] */ VersionStruct Module);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Log )( 
            IVoodooLogger * This,
            /* [in] */ EnumType Level,
            /* [in] */ BSTR pModule,
            /* [in] */ BSTR pMessage);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BufferSize )( 
            IVoodooLogger * This,
            /* [retval][out] */ int *pSize);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BufferSize )( 
            IVoodooLogger * This,
            /* [in] */ int Size);
        
        END_INTERFACE
    } IVoodooLoggerVtbl;

    interface IVoodooLogger
    {
        CONST_VTBL struct IVoodooLoggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooLogger_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooLogger_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooLogger_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooLogger_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooLogger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooLogger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooLogger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooLogger_Initialize(This,pCore)    \
    ( (This)->lpVtbl -> Initialize(This,pCore) ) 

#define IVoodooLogger_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 


#define IVoodooLogger_Open(This,pFilename,Append)    \
    ( (This)->lpVtbl -> Open(This,pFilename,Append) ) 

#define IVoodooLogger_Close(This)    \
    ( (This)->lpVtbl -> Close(This) ) 

#define IVoodooLogger_Dump(This)    \
    ( (This)->lpVtbl -> Dump(This) ) 

#define IVoodooLogger_get_LogLevel(This,pLevel)    \
    ( (This)->lpVtbl -> get_LogLevel(This,pLevel) ) 

#define IVoodooLogger_put_LogLevel(This,Level)    \
    ( (This)->lpVtbl -> put_LogLevel(This,Level) ) 

#define IVoodooLogger_LogModule(This,Module)    \
    ( (This)->lpVtbl -> LogModule(This,Module) ) 

#define IVoodooLogger_Log(This,Level,pModule,pMessage)    \
    ( (This)->lpVtbl -> Log(This,Level,pModule,pMessage) ) 

#define IVoodooLogger_get_BufferSize(This,pSize)    \
    ( (This)->lpVtbl -> get_BufferSize(This,pSize) ) 

#define IVoodooLogger_put_BufferSize(This,Size)    \
    ( (This)->lpVtbl -> put_BufferSize(This,Size) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooLogger_INTERFACE_DEFINED__ */


#ifndef __IVoodooFileSystem_INTERFACE_DEFINED__
#define __IVoodooFileSystem_INTERFACE_DEFINED__

/* interface IVoodooFileSystem */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooFileSystem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f90-6f32-11e0-8ac0-005056c00000")
    IVoodooFileSystem : public IVoodooPlugin
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddDirectory( 
            /* [in] */ BSTR pPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveDirectory( 
            /* [in] */ BSTR pPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindFile( 
            /* [in] */ BSTR pPath,
            /* [retval][out] */ IVoodooFile **ppFile) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindImage( 
            /* [in] */ BSTR pPath,
            /* [retval][out] */ IVoodooImage **ppImage) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooFileSystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooFileSystem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooFileSystem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooFileSystem * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooFileSystem * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooFileSystem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooFileSystem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooFileSystem * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IVoodooFileSystem * This,
            /* [in] */ IVoodooCore *pCore);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Core )( 
            IVoodooFileSystem * This,
            /* [retval][out] */ IVoodooCore **ppCore);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddDirectory )( 
            IVoodooFileSystem * This,
            /* [in] */ BSTR pPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveDirectory )( 
            IVoodooFileSystem * This,
            /* [in] */ BSTR pPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindFile )( 
            IVoodooFileSystem * This,
            /* [in] */ BSTR pPath,
            /* [retval][out] */ IVoodooFile **ppFile);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindImage )( 
            IVoodooFileSystem * This,
            /* [in] */ BSTR pPath,
            /* [retval][out] */ IVoodooImage **ppImage);
        
        END_INTERFACE
    } IVoodooFileSystemVtbl;

    interface IVoodooFileSystem
    {
        CONST_VTBL struct IVoodooFileSystemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooFileSystem_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooFileSystem_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooFileSystem_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooFileSystem_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooFileSystem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooFileSystem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooFileSystem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooFileSystem_Initialize(This,pCore)    \
    ( (This)->lpVtbl -> Initialize(This,pCore) ) 

#define IVoodooFileSystem_get_Core(This,ppCore)    \
    ( (This)->lpVtbl -> get_Core(This,ppCore) ) 


#define IVoodooFileSystem_AddDirectory(This,pPath)    \
    ( (This)->lpVtbl -> AddDirectory(This,pPath) ) 

#define IVoodooFileSystem_RemoveDirectory(This,pPath)    \
    ( (This)->lpVtbl -> RemoveDirectory(This,pPath) ) 

#define IVoodooFileSystem_FindFile(This,pPath,ppFile)    \
    ( (This)->lpVtbl -> FindFile(This,pPath,ppFile) ) 

#define IVoodooFileSystem_FindImage(This,pPath,ppImage)    \
    ( (This)->lpVtbl -> FindImage(This,pPath,ppImage) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooFileSystem_INTERFACE_DEFINED__ */


#ifndef __IVoodooFile_INTERFACE_DEFINED__
#define __IVoodooFile_INTERFACE_DEFINED__

/* interface IVoodooFile */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f91-6f32-11e0-8ac0-005056c00000")
    IVoodooFile : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ LPBSTR pPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ EnumType Mode) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [out][in] */ int *Length,
            /* [size_is][out] */ byte Buffer[  ]) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ int Length,
            /* [size_is][in] */ byte Buffer[  ]) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooFile * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IVoodooFile * This,
            /* [retval][out] */ LPBSTR pPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IVoodooFile * This,
            /* [in] */ EnumType Mode);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IVoodooFile * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Read )( 
            IVoodooFile * This,
            /* [out][in] */ int *Length,
            /* [size_is][out] */ byte Buffer[  ]);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Write )( 
            IVoodooFile * This,
            /* [in] */ int Length,
            /* [size_is][in] */ byte Buffer[  ]);
        
        END_INTERFACE
    } IVoodooFileVtbl;

    interface IVoodooFile
    {
        CONST_VTBL struct IVoodooFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooFile_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooFile_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooFile_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooFile_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooFile_get_Path(This,pPath)    \
    ( (This)->lpVtbl -> get_Path(This,pPath) ) 

#define IVoodooFile_Open(This,Mode)    \
    ( (This)->lpVtbl -> Open(This,Mode) ) 

#define IVoodooFile_Close(This)    \
    ( (This)->lpVtbl -> Close(This) ) 

#define IVoodooFile_Read(This,Length,Buffer)    \
    ( (This)->lpVtbl -> Read(This,Length,Buffer) ) 

#define IVoodooFile_Write(This,Length,Buffer)    \
    ( (This)->lpVtbl -> Write(This,Length,Buffer) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooFile_INTERFACE_DEFINED__ */


#ifndef __IVoodooImage_INTERFACE_DEFINED__
#define __IVoodooImage_INTERFACE_DEFINED__

/* interface IVoodooImage */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoodooImage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d3d7f92-6f32-11e0-8ac0-005056c00000")
    IVoodooImage : public IVoodooFile
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Desc( 
            /* [retval][out] */ TextureDesc *pDesc) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
            /* [in] */ TextureRegion Region,
            /* [out][in] */ int *pSize,
            /* [size_is][retval][out] */ byte ppBuffer[  ]) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IVoodooImageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoodooImage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoodooImage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoodooImage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoodooImage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoodooImage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoodooImage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoodooImage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IVoodooImage * This,
            /* [retval][out] */ LPBSTR pPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Open )( 
            IVoodooImage * This,
            /* [in] */ EnumType Mode);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IVoodooImage * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Read )( 
            IVoodooImage * This,
            /* [out][in] */ int *Length,
            /* [size_is][out] */ byte Buffer[  ]);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Write )( 
            IVoodooImage * This,
            /* [in] */ int Length,
            /* [size_is][in] */ byte Buffer[  ]);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Desc )( 
            IVoodooImage * This,
            /* [retval][out] */ TextureDesc *pDesc);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            IVoodooImage * This,
            /* [in] */ TextureRegion Region,
            /* [out][in] */ int *pSize,
            /* [size_is][retval][out] */ byte ppBuffer[  ]);
        
        END_INTERFACE
    } IVoodooImageVtbl;

    interface IVoodooImage
    {
        CONST_VTBL struct IVoodooImageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoodooImage_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoodooImage_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoodooImage_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoodooImage_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoodooImage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoodooImage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoodooImage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoodooImage_get_Path(This,pPath)    \
    ( (This)->lpVtbl -> get_Path(This,pPath) ) 

#define IVoodooImage_Open(This,Mode)    \
    ( (This)->lpVtbl -> Open(This,Mode) ) 

#define IVoodooImage_Close(This)    \
    ( (This)->lpVtbl -> Close(This) ) 

#define IVoodooImage_Read(This,Length,Buffer)    \
    ( (This)->lpVtbl -> Read(This,Length,Buffer) ) 

#define IVoodooImage_Write(This,Length,Buffer)    \
    ( (This)->lpVtbl -> Write(This,Length,Buffer) ) 


#define IVoodooImage_get_Desc(This,pDesc)    \
    ( (This)->lpVtbl -> get_Desc(This,pDesc) ) 

#define IVoodooImage_get_Data(This,Region,pSize,ppBuffer)    \
    ( (This)->lpVtbl -> get_Data(This,Region,pSize,ppBuffer) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IVoodooImage_INTERFACE_DEFINED__ */


#ifndef __IComponentRegistrar_INTERFACE_DEFINED__
#define __IComponentRegistrar_INTERFACE_DEFINED__

/* interface IComponentRegistrar */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IComponentRegistrar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a817e7a2-43fa-11d0-9e44-00aa00b6770a")
    IComponentRegistrar : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ BSTR bstrPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetComponents( 
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
    };
    
#else     /* C style interface */

    typedef struct IComponentRegistrarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComponentRegistrar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComponentRegistrar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComponentRegistrar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComponentRegistrar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComponentRegistrar * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Attach )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetComponents )( 
            IComponentRegistrar * This,
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        END_INTERFACE
    } IComponentRegistrarVtbl;

    interface IComponentRegistrar
    {
        CONST_VTBL struct IComponentRegistrarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComponentRegistrar_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComponentRegistrar_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComponentRegistrar_Release(This)    \
    ( (This)->lpVtbl -> Release(This) ) 


#define IComponentRegistrar_GetTypeInfoCount(This,pctinfo)    \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IComponentRegistrar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IComponentRegistrar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IComponentRegistrar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IComponentRegistrar_Attach(This,bstrPath)    \
    ( (This)->lpVtbl -> Attach(This,bstrPath) ) 

#define IComponentRegistrar_RegisterAll(This)    \
    ( (This)->lpVtbl -> RegisterAll(This) ) 

#define IComponentRegistrar_UnregisterAll(This)    \
    ( (This)->lpVtbl -> UnregisterAll(This) ) 

#define IComponentRegistrar_GetComponents(This,pbstrCLSIDs,pbstrDescriptions)    \
    ( (This)->lpVtbl -> GetComponents(This,pbstrCLSIDs,pbstrDescriptions) ) 

#define IComponentRegistrar_RegisterComponent(This,bstrCLSID)    \
    ( (This)->lpVtbl -> RegisterComponent(This,bstrCLSID) ) 

#define IComponentRegistrar_UnregisterComponent(This,bstrCLSID)    \
    ( (This)->lpVtbl -> UnregisterComponent(This,bstrCLSID) ) 

#endif /* COBJMACROS */


#endif     /* C style interface */




#endif     /* __IComponentRegistrar_INTERFACE_DEFINED__ */



#ifndef __Voodoo_Core_LIBRARY_DEFINED__
#define __Voodoo_Core_LIBRARY_DEFINED__

/* library Voodoo_Core */
/* [custom][version][uuid] */ 





















EXTERN_C const IID LIBID_Voodoo_Core;

EXTERN_C const CLSID CLSID_CompReg;

#ifdef __cplusplus

class DECLSPEC_UUID("5F3D58FE-A84D-4E15-9AAA-878EAC2E4BED")
CompReg;
#endif

EXTERN_C const CLSID CLSID_VoodooCore;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fa3-6f32-11e0-8ac0-005056c00000")
VoodooCore;
#endif

EXTERN_C const CLSID CLSID_VoodooParser;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fa4-6f32-11e0-8ac0-005056c00000")
VoodooParser;
#endif

EXTERN_C const CLSID CLSID_VoodooShader;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fa5-6f32-11e0-8ac0-005056c00000")
VoodooShader;
#endif

EXTERN_C const CLSID CLSID_VoodooTechnique;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fa6-6f32-11e0-8ac0-005056c00000")
VoodooTechnique;
#endif

EXTERN_C const CLSID CLSID_VoodooPass;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fa7-6f32-11e0-8ac0-005056c00000")
VoodooPass;
#endif

EXTERN_C const CLSID CLSID_VoodooTexture;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fa8-6f32-11e0-8ac0-005056c00000")
VoodooTexture;
#endif

EXTERN_C const CLSID CLSID_VoodooParameter;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fa9-6f32-11e0-8ac0-005056c00000")
VoodooParameter;
#endif

EXTERN_C const CLSID CLSID_VoodooAdapter;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7faa-6f32-11e0-8ac0-005056c00000")
VoodooAdapter;
#endif

EXTERN_C const CLSID CLSID_VoodooHookSystem;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fab-6f32-11e0-8ac0-005056c00000")
VoodooHookSystem;
#endif

EXTERN_C const CLSID CLSID_VoodooLogger;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fac-6f32-11e0-8ac0-005056c00000")
VoodooLogger;
#endif

EXTERN_C const CLSID CLSID_VoodooFileSystem;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fad-6f32-11e0-8ac0-005056c00000")
VoodooFileSystem;
#endif

EXTERN_C const CLSID CLSID_VoodooFile;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7fae-6f32-11e0-8ac0-005056c00000")
VoodooFile;
#endif

EXTERN_C const CLSID CLSID_VoodooImage;

#ifdef __cplusplus

class DECLSPEC_UUID("1d3d7faf-6f32-11e0-8ac0-005056c00000")
VoodooImage;
#endif
#endif /* __Voodoo_Core_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


