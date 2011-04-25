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

/** 
 * This file is the public-facing header for the Voodoo Shader Framework (Voodoo/Core module).
 * 
 * It is the only file which needs included to use or define Voodoo classes in other modules,
 * and should be included by all Voodoo-compatible code.
 **/

#define COM_NO_WINDOWS_H
#include <objbase.h>
#include <windows.h>

/* Interface IIDs */
DEFINE_GUID(IID_IVoodooCore,        0x6D183074, 0x9127, 0x4615, 0x83, 0x66, 0x02, 0x58, 0x7B, 0xAF, 0xE3, 0x8C);
DEFINE_GUID(IID_IVoodooParser,      0xB375BC6B, 0x5A6C, 0x4B8B, 0x88, 0x8B, 0xA9, 0x37, 0xBA, 0x38, 0x14, 0xFD);
DEFINE_GUID(IID_IVoodooShader,      0x8C26CEBA, 0x45AD, 0x4382, 0xBE, 0xA9, 0xED, 0xBB, 0x2F, 0x44, 0x6B, 0x21);
DEFINE_GUID(IID_IVoodooTechnique,   0xB0D5E12B, 0x2A58, 0x4AF3, 0xBD, 0x0A, 0xA4, 0x1A, 0x39, 0x68, 0xAB, 0x36);
DEFINE_GUID(IID_IVoodooPass,        0x7C231D9E, 0x146B, 0x40CF, 0x9F, 0x39, 0xA4, 0xD3, 0x51, 0x3D, 0xD7, 0xF7);
DEFINE_GUID(IID_IVoodooTexture,     0x164A33FD, 0xE80D, 0x4666, 0xAE, 0x93, 0x29, 0x8B, 0x33, 0x02, 0x09, 0x86);
DEFINE_GUID(IID_IVoodooParameter,   0x6AB220A5, 0x4579, 0x441E, 0x90, 0x60, 0x4A, 0xF7, 0x53, 0x5D, 0x66, 0x01);
DEFINE_GUID(IID_IVoodooAdapter,     0x59C2FE67, 0xA7EC, 0x48B2, 0x98, 0x2E, 0x97, 0x07, 0xA3, 0x84, 0x02, 0x59);
DEFINE_GUID(IID_IVoodooHookSystem,  0x4E25F9D4, 0x87D0, 0x4CE8, 0x84, 0xC0, 0xB7, 0x22, 0x25, 0x33, 0x6D, 0xA3);
DEFINE_GUID(IID_IVoodooLogger,      0xD26A5521, 0x3E3D, 0x4BD9, 0x8A, 0x75, 0x1A, 0xD5, 0x01, 0x4D, 0x24, 0x79);
DEFINE_GUID(IID_IVoodooFileSystem,  0x2ADFD66B, 0x2C4F, 0x4172, 0x82, 0x3F, 0x47, 0xD4, 0x80, 0xBF, 0x85, 0x23);
DEFINE_GUID(IID_IVoodooFile,        0x35068CA2, 0xB06B, 0x48DF, 0xAD, 0xAB, 0xBC, 0x1F, 0xDA, 0x16, 0x3C, 0x1F);
DEFINE_GUID(IID_IVoodooImage,       0x8B74AE58, 0x98A2, 0x4597, 0xB9, 0x63, 0x46, 0x05, 0xF0, 0x3A, 0x86, 0xD3);

#ifndef DECLSPEC_UUID
#if _MSC_VER >= 1100
#define DECLSPEC_UUID(x)    __declspec(uuid(x))
#else
#define DECLSPEC_UUID(x)
#endif

interface DECLSPEC_UUID("6D183074-9127-4615-8366-02587BAFE38C") IVoodooCore;
interface DECLSPEC_UUID("B375BC6B-5A6C-4B8B-888B-A937BA3814FD") IVoodooParser;
interface DECLSPEC_UUID("8C26CEBA-45AD-4382-BEA9-EDBB2F446B21") IVoodooShader;
interface DECLSPEC_UUID("B0D5E12B-2A58-4AF3-BD0A-A41A3968AB36") IVoodooTechnique;
interface DECLSPEC_UUID("7C231D9E-146B-40CF-9F39-A4D3513DD7F7") IVoodooPass;
interface DECLSPEC_UUID("164A33FD-E80D-4666-AE93-298B33020986") IVoodooTexture;
interface DECLSPEC_UUID("6AB220A5-4579-441E-9060-4AF7535D6601") IVoodooParameter;
interface DECLSPEC_UUID("59C2FE67-A7EC-48B2-982E-9707A3840259") IVoodooAdapter;
interface DECLSPEC_UUID("4E25F9D4-87D0-4CE8-84C0-B72225336DA3") IVoodooHookSystem;
interface DECLSPEC_UUID("D26A5521-3E3D-4BD9-8A75-1AD5014D2479") IVoodooLogger;
interface DECLSPEC_UUID("2ADFD66B-2C4F-4172-823F-47D480BF8523") IVoodooFileSystem;
interface DECLSPEC_UUID("8B74AE58-98A2-4597-B963-4605F03A86D3") IVoodooFile;
interface DECLSPEC_UUID("8B74AE58-98A2-4597-B963-4605F03A86D3") IVoodooImage;

#if defined(_COM_SMARTPTR_TYPEDEF)
_COM_SMARTPTR_TYPEDEF(IVoodooCore,          __uuidof(IVoodooCore));
_COM_SMARTPTR_TYPEDEF(IVoodooParser,        __uuidof(IVoodooParser));
_COM_SMARTPTR_TYPEDEF(IVoodooShader,        __uuidof(IVoodooShader));
_COM_SMARTPTR_TYPEDEF(IVoodooTechnique,     __uuidof(IVoodooTechnique));
_COM_SMARTPTR_TYPEDEF(IVoodooPass,          __uuidof(IVoodooPass));
_COM_SMARTPTR_TYPEDEF(IVoodooTexture,       __uuidof(IVoodooTexture));
_COM_SMARTPTR_TYPEDEF(IVoodooParameter,     __uuidof(IVoodooParameter));
_COM_SMARTPTR_TYPEDEF(IVoodooAdapter,       __uuidof(IVoodooAdapter));
_COM_SMARTPTR_TYPEDEF(IVoodooHookSystem,    __uuidof(IVoodooHookSystem));
_COM_SMARTPTR_TYPEDEF(IVoodooLogger,        __uuidof(IVoodooLogger));
_COM_SMARTPTR_TYPEDEF(IVoodooFileSystem,    __uuidof(IVoodooFileSystem));
_COM_SMARTPTR_TYPEDEF(IVoodooFile,          __uuidof(IVoodooFile));
_COM_SMARTPTR_TYPEDEF(IVoodooImage,         __uuidof(IVoodooImage));
#endif /*_COM_SMARTPTR_TYPEDEF*/

typedef interface IVoodooCore       IVoodooCore;
typedef interface IVoodooParser     IVoodooParser;
typedef interface IVoodooShader     IVoodooShader;
typedef interface IVoodooTechnique  IVoodooTechnique;
typedef interface IVoodooPass       IVoodooPass;
typedef interface IVoodooTexture    IVoodooTexture;
typedef interface IVoodooParameter  IVoodooParameter;
typedef interface IVoodooAdapter    IVoodooAdapter;
typedef interface IVoodooHookSystem IVoodooHookSystem;
typedef interface IVoodooLogger     IVoodooLogger;
typedef interface IVoodooFileSystem IVoodooFileSystem;
typedef interface IVoodooFile       IVoodooFile;
typedef interface IVoodooImage      IVoodooImage;

/* Voodoo types */
#include "VoodooTypes.hpp"

/* Cg types */
#ifndef CG_VERSION_NUM
typedef void * CGcontext;
typedef void * CGprogram;
typedef void * CGparameter;
typedef void * CGeffect;
typedef void * CGtechnique;
typedef void * CGpass;
#endif

/* Voodoo version info */
#include "VoodooVersion.hpp"
#ifndef VOODOO_VERSION
#define VOODOO_VERSION VOODOO_GLOBAL_VERSION_REV
#endif

#undef INTERFACE
#define INTERFACE IVoodooCore

DECLARE_INTERFACE_(IVoodooCore, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooCore */
    STDMETHOD(Initialize)(THIS_ const InitParams pParams) PURE;
    STDMETHOD(get_Parser)(THIS_ IVoodooParser ** ppParser) PURE;
    STDMETHOD(get_HookSystem)(THIS_ IVoodooHookSystem ** ppHookSystem) PURE;
    STDMETHOD(get_FileSystem)(THIS_ IVoodooFileSystem ** ppFileSystem) PURE;
    STDMETHOD(get_Adapter)(THIS_ IVoodooAdapter ** ppAdapter) PURE;
    STDMETHOD(get_Logger)(THIS_ IVoodooLogger ** ppLogger) PURE;
    STDMETHOD(get_Config)(THIS_ IXMLDOMDocument ** ppConfig) PURE;
    STDMETHOD(get_CgContext)(THIS_ VARIANT * pCgContext) PURE;
    STDMETHOD(put_CgContext)THIS_ (VARIANT CgContext) PURE;
    STDMETHOD(CreateShader)(THIS_ IVoodooFile * pFile, SAFEARRAY * pArgs, IVoodooShader ** ppShader) PURE;
    STDMETHOD(CreateParameter)(THIS_ BSTR pName, EnumType Type, IVoodooParameter ** ppParameter) PURE;
    STDMETHOD(CreateTexture)(THIS_ BSTR pName, VARIANT Data, IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(GetParameter)(THIS_ BSTR pName, IVoodooParameter ** ppParameter) PURE;
    STDMETHOD(GetTexture)(THIS_ BSTR pName, IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(RemoveTexture)(THIS_ BSTR pName) PURE;
    STDMETHOD(GetStageTexture)(THIS_ EnumType Stage, IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(SetStageTexture)(THIS_ EnumType Stage, IVoodooTexture * pTexture) PURE;
};

typedef struct IVoodooCore *LPVOODOOCORE, *PVOODOOCORE;

#undef INTERFACE
#define INTERFACE IVoodooParser

DECLARE_INTERFACE_(IVoodooParser, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooParser */
    STDMETHOD(AddVariable)(THIS_ BSTR pName, BSTR pValue, EnumType System = VT_Normal) PURE;
    STDMETHOD(RemoveVariable)(THIS_ BSTR pName) PURE;
    STDMETHOD(Parse)(THIS_ BSTR pString, EnumType Flags, LPBSTR pParsed) PURE;
};

typedef struct IVoodooParser *LPVOODOOPARSER, *PVOODOOPARSER;

#undef INTERFACE
#define INTERFACE IVoodooShader

DECLARE_INTERFACE_(IVoodooShader, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooShader */
    STDMETHOD(get_Name)(THIS_ LPBSTR ppName) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    STDMETHOD(get_TechniqueCount)(THIS_ int * pCount) PURE;
    STDMETHOD(GetTechnique)(THIS_ int Number, IVoodooTechnique ** ppTechnique) PURE;
    STDMETHOD(get_DefaultTechnique)(THIS_ IVoodooTechnique ** ppTechnique) PURE;
    STDMETHOD(put_DefaultTechnique)(THIS_ IVoodooTechnique * pTechnique) PURE;
    STDMETHOD(get_ParameterCount)(THIS_ int * pCount) PURE;
    STDMETHOD(GetParameter)(THIS_ int Number, IVoodooParameter ** ppParameter) PURE;
    STDMETHOD(get_CgShader)(THIS_ VARIANT * pCgShader) PURE;
};

typedef struct IVoodooShader *LPVOODOOSHADER, *PVOODOOSHADER;

#undef INTERFACE
#define INTERFACE IVoodooTechnique

DECLARE_INTERFACE_(IVoodooTechnique, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooTechnique */
    STDMETHOD(get_Name)(THIS_ LPBSTR ppName) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    STDMETHOD(get_Target)(THIS_ IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(get_PassCount)(THIS_ int * pCount) PURE;
    STDMETHOD(GetPass)(THIS_ int Number, IVoodooPass ** ppPass) PURE;
    STDMETHOD(get_Shader)(THIS_ IVoodooShader ** ppShader) PURE;
    STDMETHOD(get_CgTechnique)(THIS_ VARIANT * pCgTechnique) PURE;
};

typedef struct IVoodooTechnique *LPVOODOOTECHNIQUE, *PVOODOOTECHNIQUE;

#undef INTERFACE
#define INTERFACE IVoodooPass

DECLARE_INTERFACE_(IVoodooPass, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooPass */
    STDMETHOD(get_Name)(THIS_ LPBSTR ppName) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    STDMETHOD(get_Target)(THIS_ IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(get_Shader)(THIS_ IVoodooShader ** ppShader) PURE;
    STDMETHOD(get_Technique)(THIS_ IVoodooTechnique ** ppTechnique) PURE;
    STDMETHOD(GetCgProgram)(THIS_ EnumType Stage, VARIANT * pProgram) PURE;
    STDMETHOD(get_CgPass)(THIS_ VARIANT * pCgPass) PURE;
};

typedef struct IVoodooPass *LPVOODOOPASS, *PVOODOOPASS;

#undef INTERFACE
#define INTERFACE IVoodooTexture

DECLARE_INTERFACE_(IVoodooTexture, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooTexture */
    STDMETHOD(get_Name)(THIS_ LPBSTR ppName) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    STDMETHOD(get_Data)(THIS_ VARIANT * pData) PURE;
}

typedef struct IVoodooTexture *LPVOODOOTEXTURE, *PVOODOOTEXTURE;

#undef INTERFACE
#define INTERFACE IVoodooParameter

DECLARE_INTERFACE_(IVoodooParameter, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooParameter */
    STDMETHOD(get_Name)(THIS_ LPBSTR ppName) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    STDMETHOD(get_Type)(THIS_ EnumType * pType) PURE;
    STDMETHOD(get_Virtual)(THIS_ boolean * pVirtual) PURE;
    STDMETHOD(AttachParameter)(THIS_ IVoodooParameter * pParameter) PURE;
    STDMETHOD(get_Components)(THIS_ int * pComponents) PURE;
    STDMETHOD(get_SamplerValue)(THIS_ IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(put_SamplerValue)(THIS_ IVoodooTexture  *pTexture) PURE;
    STDMETHOD(get_ScalarValue)(THIS_ SAFEARRAY ** ppData) PURE;
    STDMETHOD(put_ScalarValue)(THIS_ SAFEARRAY * pData) PURE;
    STDMETHOD(get_Shader)(THIS_ IVoodooShader ** ppShader) PURE;
    STDMETHOD(get_CgParameter)(THIS_ VARIANT * pCgParameter) PURE;
};

typedef struct IVoodooParameter *LPVOODOOPARAMETER, *PVOODOOPARAMETER;

#undef INTERFACE
#define INTERFACE IVoodooAdapter

DECLARE_INTERFACE_(IVoodooAdapter, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooAdapter */
    STDMETHOD(get_Core)(THIS_ IVoodooCore **ppCore) PURE;
    STDMETHOD(LoadPass)(THIS_ IVoodooPass *pPass) PURE;
    STDMETHOD(UnloadPass)(THIS_ IVoodooPass *pPass) PURE;
    STDMETHOD(get_Pass)(THIS_ IVoodooPass **ppPass) PURE;
    STDMETHOD(put_Pass(THIS_ IVoodooPass *pPass) PURE;
    STDMETHOD(get_Target)(THIS_ IVoodooTexture **ppTarget) PURE;
    STDMETHOD(put_Target)(THIS_ IVoodooTexture *pTarget) PURE;
    STDMETHOD(CreateTexture)(THIS_ BSTR pName,  TextureDesc Description,  IVoodooTexture **ppTexture) PURE;
    STDMETHOD(LoadTexture)(THIS_ IVoodooImage *pImage,  TextureRegion Region,  IVoodooTexture **ppTexture) PURE;
    STDMETHOD(DrawGeometry)(THIS_ int Vertexes, SAFEARRAY * pVertexData) PURE;
    STDMETHOD(DrawShader)(THIS_ IVoodooShader *pShader) PURE;
    STDMETHOD(ApplyParameter)(THIS_ IVoodooParameter *pParameter) PURE;
    STDMETHOD(SetProperty)(THIS_ BSTR pName, VARIANT *pData) PURE;
    STDMETHOD(ConnectTexture)(THIS_ IVoodooParameter *pParameter, IVoodooTexture *pTexture) PURE;
    STDMETHOD(HandleError)(THIS_ VARIANT pContext, EnumType Error, IVoodooCore *pCore) PURE;
};

typedef struct IVoodooAdapter *LPVOODOOADAPTER, *PVOODOOADAPTER;

#undef INTERFACE
#define INTERFACE IVoodooHookSystem

DECLARE_INTERFACE_(IVoodooHookSystem, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooHookSystem */
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    STDMETHOD(Add)(THIS_ BSTR pName, FunctionPtr pSource, FunctionPtr pDest) PURE;
    STDMETHOD(Remove)(THIS_ BSTR pName) PURE;
    STDMETHOD(RemoveAll)(THIS) PURE;
};

typedef struct IVoodooHookSystem *LPVOODOOHOOKSYSTEM, *PVOODOOHOOKSYSTEM;

#undef INTERFACE
#define INTERFACE IVoodoooLogger

DECLARE_INTERFACE_(IVoodoooLogger, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodoooLogger */
    STDMETHOD(get_Core)(THIS_  IVoodooCore ** ppCore) PURE;
    STDMETHOD(Open)(THIS_ BSTR pFilename, boolean Append) PURE;
    STDMETHOD(Close)(THIS) PURE;
    STDMETHOD(Dump)(THIS) PURE;
    STDMETHOD(get_LogLevel)(THIS_ EnumType * pLevel) PURE;
    STDMETHOD(put_LogLevel)(THIS_ EnumType Level) PURE;
    STDMETHOD(LogModule)(THIS_ VersionStruct Module) PURE;
    STDMETHOD(Log)(THIS_ EnumType Level, BSTR pModule, BSTR pFormat, SAFEARRAY * pArgs) PURE;
    STDMETHOD(LogList)(THIS_ EnumType Level, BSTR pModule, BSTR pFormat, VARIANT pList) PURE;
    STDMETHOD(get_BufferSize)(THIS_ int * pSize) PURE;
    STDMETHOD(put_BufferSize)(THIS_ int Size) PURE;
};

typedef struct IVoodooLogger *LPVOODOOLOGGER, *PVOODOOLOGGER;

#undef INTERFACE
#define INTERFACE IVoodooFileSystem

DECLARE_INTERFACE_(IVoodooFileSystem, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooFileSystem */
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    STDMETHOD(AddDirectory)(THIS_ BSTR pPath) PURE;
    STDMETHOD(RemoveDirectory)(THIS_ BSTR pPath) PURE;
    STDMETHOD(FindFile)(THIS_ BSTR pPath, IVoodooFile ** ppFile) PURE;
    STDMETHOD(FindImage)(THIS_ BSTR pPath, IVoodooImage ** ppImage) PURE;
};

typedef struct IVoodooFileSystem *LPVOODOOFILESYSTEM, *PVOODOOFILESYSTEM;

#undef INTERFACE
#define INTERFACE IVoodooFile

DECLARE_INTERFACE_(IVoodooFile, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooFile */
    STDMETHOD(get_Path)(THIS_ LPBSTR pPath) PURE;
    STDMETHOD(Open)(THIS_ EnumType Mode) PURE;
    STDMETHOD(Close)(THIS) PURE;
    STDMETHOD(Read)(THIS_ int * Length, byte Buffer[]) PURE;
    STDMETHOD(Write)(THIS_ int Length, byte Buffer[]) PURE;
};

typedef struct IVoodooFile *LPVOODOOFILE, *PVOODOOFILE;

#undef INTERFACE
#define INTERFACE IVoodooImage

DECLARE_INTERFACE_(IVoodooImage, IDispatch)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void ** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /* IDispatch */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT * pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
    STDMETHOD(Invoke)(THIS_ DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) PURE;
    /* IVoodooFile */
    STDMETHOD(get_Path)(THIS_ LPBSTR pPath) PURE;
    STDMETHOD(Open)(THIS_ EnumType Mode) PURE;
    STDMETHOD(Close)(THIS) PURE;
    STDMETHOD(Read)(THIS_ int * Length, byte Buffer[]) PURE;
    STDMETHOD(Write)(THIS_ int Length, byte Buffer[]) PURE;
    /* IVoodooImage */
    STDMETHOD(get_Desc)(THIS_ TextureDesc *pDesc) PURE;
    STDMETHOD(get_Data)(THIS_ TextureRegion Region, int * pSize, byte ppBuffer[]) PURE;
};

typedef struct IVoodooImage *LPVOODOOIMAGE, *PVOODOOIMAGE;

#undef INTERFACE

/* Voodoo HRESULTS */
#define _FACVSF FACILITY_ITF

// General Voodoo HRESULT macros
#define MAKE_VSF_OK(code)     MAKE_HRESULT(SEVERITY_SUCCESS, _FACVSF, code)
#define MAKE_VSF_ERR(code)    MAKE_HRESULT(SEVERITY_ERROR, _FACVSF, code)

// Library/interface specific HRESULT macros
#define MAKE_VSF_LOK(code)    MAKE_HRESULT(SEVERITY_SUCCESS, _FACVSF, code | 0x8000 )
#define MAKE_VSF_IOK(code)    MAKE_HRESULT(SEVERITY_SUCCESS, _FACVSF, code ^ 0x8000 )
#define MAKE_VSF_LERR(code)   MAKE_HRESULT(SEVERITY_ERROR, _FACVSF, code | 0x8000 )
#define MAKE_VSF_IERR(code)   MAKE_HRESULT(SEVERITY_ERROR, _FACVSF, code ^ 0x8000 )

// Library/interface HRESULT get macros
#define IS_LIB_HR(hr)   (  hr & 0x00008000 )
#define IS_IFACE_HR(hr) ( ~hr & 0x00008000 )

// IVoodoo*
#define VSF_OK                  S_OK
#define VSFERR_BADTHING         MAKE_VSF_LERR(0x0001)
#define VSFERR_INVALIDCFG       MAKE_VSF_LERR(0x0002)
#define VSFERR_BADCLSID         MAKE_VSF_LERR(0x0003)
#define VSFERR_DUPNAME          MAKE_VSF_LERR(0x0004)
#define VSFERR_NULLIMPL         MAKE_VSF_LERR(0x0005)
#define VSFERR_NOT_FOUND        MAKE_VSF_LERR(0x0006)
#define VSFOK_NOT_FOUND         MAKE_VSF_LOK(0x0006)

// IVoodooCore
#define VSFERR_NOTINIT          MAKE_VSF_IERR(0x0001)

// IVoodooLogger
#define VSFERR_FILEERROR        MAKE_VSF_IERR(0x0001)

// IVoodooPass
#define VSFERR_NOPROGRAM        MAKE_VSF_IERR(0x0001)

// IVoodooParser
#define VSFERR_ISSYSVAR         MAKE_VSF_IERR(0x0001)
