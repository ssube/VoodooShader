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
 * @file VoodooFramework.hpp
 * 
 * This is the public-facing header for the Voodoo Shader Framework (Voodoo/Core module).
 * 
 * It is the only file which needs included to use or define Voodoo classes in other modules,
 * and should be included by all Voodoo-compatible code. This file includes the version and general
 * type headers.
 * 
 * You may, if necessary, include VoodooSupport.hpp to provide a small set of helper functions for
 * use with Voodoo.
 **/

#ifdef __cplusplus

#define COM_NO_WINDOWS_H
#include "objbase.h"
#include "windows.h"
#include "oaidl.h"

/* GUID implement macro (to prevent linking reqs) */
#define IMPLEMENT_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    extern const GUID name;\
    const DECLSPEC_SELECTANY GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

/* Library LIBID */
IMPLEMENT_GUID(LIBID_Voodoo_Core,      0x1d3d7f85, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);

/* Interface IIDs */
IMPLEMENT_GUID(IID_IVoodooCore,        0x1d3d7f86, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooParser,      0x1d3d7f87, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooShader,      0x1d3d7f88, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooTechnique,   0x1d3d7f89, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooPass,        0x1d3d7f8a, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooTexture,     0x1d3d7f8b, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooParameter,   0x1d3d7f8c, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooAdapter,     0x1d3d7f8d, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooHookSystem,  0x1d3d7f8e, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooLogger,      0x1d3d7f8f, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooFileSystem,  0x1d3d7f90, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooFile,        0x1d3d7f91, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooImage,       0x1d3d7f92, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(IID_IVoodooPlugin,      0x1d3d7f93, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);

/* CoClass CLSIDs */
IMPLEMENT_GUID(CLSID_VoodooCore,       0x1d3d7fa3, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooParser,     0x1d3d7fa4, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooShader,     0x1d3d7fa5, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooTechnique,  0x1d3d7fa6, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooPass,       0x1d3d7fa7, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooTexture,    0x1d3d7fa8, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooParameter,  0x1d3d7fa9, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooAdapter,    0x1d3d7faa, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooHookSystem, 0x1d3d7fab, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooLogger,     0x1d3d7fac, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooFileSystem, 0x1d3d7fad, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooFile,       0x1d3d7fae, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);
IMPLEMENT_GUID(CLSID_VoodooImage,      0x1d3d7faf, 0x6f32, 0x11e0, 0x8a, 0xc0, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x00);

#ifndef DECLSPEC_UUID
#if _MSC_VER >= 1100
#define DECLSPEC_UUID(x)    __declspec(uuid(x))
#else
#define DECLSPEC_UUID(x)
#endif
#endif

interface DECLSPEC_UUID("1d3d7f86-6f32-11e0-8ac0-005056c00000") IVoodooCore;
interface DECLSPEC_UUID("1d3d7f87-6f32-11e0-8ac0-005056c00000") IVoodooParser;
interface DECLSPEC_UUID("1d3d7f88-6f32-11e0-8ac0-005056c00000") IVoodooShader;
interface DECLSPEC_UUID("1d3d7f89-6f32-11e0-8ac0-005056c00000") IVoodooTechnique;
interface DECLSPEC_UUID("1d3d7f8a-6f32-11e0-8ac0-005056c00000") IVoodooPass;
interface DECLSPEC_UUID("1d3d7f8b-6f32-11e0-8ac0-005056c00000") IVoodooTexture;
interface DECLSPEC_UUID("1d3d7f8c-6f32-11e0-8ac0-005056c00000") IVoodooParameter;
interface DECLSPEC_UUID("1d3d7f8d-6f32-11e0-8ac0-005056c00000") IVoodooAdapter;
interface DECLSPEC_UUID("1d3d7f8e-6f32-11e0-8ac0-005056c00000") IVoodooHookSystem;
interface DECLSPEC_UUID("1d3d7f8f-6f32-11e0-8ac0-005056c00000") IVoodooLogger;
interface DECLSPEC_UUID("1d3d7f90-6f32-11e0-8ac0-005056c00000") IVoodooFileSystem;
interface DECLSPEC_UUID("1d3d7f91-6f32-11e0-8ac0-005056c00000") IVoodooFile;
interface DECLSPEC_UUID("1d3d7f92-6f32-11e0-8ac0-005056c00000") IVoodooImage;
interface DECLSPEC_UUID("1d3d7f93-6f32-11e0-8ac0-005056c00000") IVoodooPlugin;

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
_COM_SMARTPTR_TYPEDEF(IVoodooPlugin,        __uuidof(IVoodooPlugin));
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
typedef interface IVoodooPlugin     IVoodooPlugin;

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

/* Voodoo/Core interfaces */
#undef INTERFACE
#define INTERFACE IVoodooCore

DECLARE_INTERFACE_IID_(IVoodooCore, IDispatch, "1d3d7f86-6f32-11e0-8ac0-005056c00000")
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
    STDMETHOD(get_Config)(THIS_ IUnknown ** ppConfig) PURE;
    STDMETHOD(get_CgContext)(THIS_ VARIANT * pCgContext) PURE;
    STDMETHOD(put_CgContext)(THIS_ VARIANT CgContext) PURE;
    STDMETHOD(CreateShader)(THIS_ IVoodooFile * pFile, SAFEARRAY * pArgs, IVoodooShader ** ppShader) PURE;
    STDMETHOD(CreateParameter)(THIS_ BSTR pName, EnumType Type, IVoodooParameter ** ppParameter) PURE;
    STDMETHOD(CreateTexture)(THIS_ BSTR pName, TextureDesc Desc, VARIANT Data, IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(GetParameter)(THIS_ BSTR pName, IVoodooParameter ** ppParameter) PURE;
    STDMETHOD(GetTexture)(THIS_ BSTR pName, IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(RemoveParameter)(THIS_ BSTR pName) PURE;
    STDMETHOD(RemoveTexture)(THIS_ BSTR pName) PURE;
    STDMETHOD(GetStageTexture)(THIS_ EnumType Stage, IVoodooTexture ** ppTexture) PURE;
    STDMETHOD(SetStageTexture)(THIS_ EnumType Stage, IVoodooTexture * pTexture) PURE;
};

typedef struct IVoodooCore *LPVOODOOCORE, *PVOODOOCORE;

#undef INTERFACE
#define INTERFACE IVoodooParser

DECLARE_INTERFACE_IID_(IVoodooParser, IDispatch, "1d3d7f87-6f32-11e0-8ac0-005056c00000")
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
    STDMETHOD(Add)(THIS_ BSTR pName, BSTR pValue, EnumType System) PURE;
    STDMETHOD(Remove)(THIS_ BSTR pName) PURE;
    STDMETHOD(Parse)(THIS_ BSTR pString, EnumType Flags, LPBSTR pParsed) PURE;
};

typedef struct IVoodooParser *LPVOODOOPARSER, *PVOODOOPARSER;

#undef INTERFACE
#define INTERFACE IVoodooPlugin

DECLARE_INTERFACE_IID_(IVoodooPlugin, IDispatch, "1d3d7f93-6f32-11e0-8ac0-005056c00000")
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
    /* IVoodooPlugin */
    STDMETHOD(Initialize)(THIS_ IVoodooCore * pCore) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
};

#undef INTERFACE
#define INTERFACE IVoodooShader

DECLARE_INTERFACE_IID_(IVoodooShader, IDispatch, "1d3d7f88-6f32-11e0-8ac0-005056c00000")
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

DECLARE_INTERFACE_IID_(IVoodooTechnique, IDispatch, "1d3d7f89-6f32-11e0-8ac0-005056c00000")
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

DECLARE_INTERFACE_IID_(IVoodooPass, IDispatch, "1d3d7f8a-6f32-11e0-8ac0-005056c00000")
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

DECLARE_INTERFACE_IID_(IVoodooTexture, IDispatch, "1d3d7f8b-6f32-11e0-8ac0-005056c00000")
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
};

typedef struct IVoodooTexture *LPVOODOOTEXTURE, *PVOODOOTEXTURE;

#undef INTERFACE
#define INTERFACE IVoodooParameter

DECLARE_INTERFACE_IID_(IVoodooParameter, IDispatch, "1d3d7f8c-6f32-11e0-8ac0-005056c00000")
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

DECLARE_INTERFACE_IID_(IVoodooAdapter, IVoodooPlugin, "1d3d7f8d-6f32-11e0-8ac0-005056c00000")
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
    /* IVoodooPlugin */
    STDMETHOD(Initialize)(THIS_ IVoodooCore * pCore) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    /* IVoodooAdapter */
    STDMETHOD(LoadPass)(THIS_ IVoodooPass *pPass) PURE;
    STDMETHOD(UnloadPass)(THIS_ IVoodooPass *pPass) PURE;
    STDMETHOD(get_Pass)(THIS_ IVoodooPass **ppPass) PURE;
    STDMETHOD(put_Pass)(THIS_ IVoodooPass *pPass) PURE;
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

DECLARE_INTERFACE_IID_(IVoodooHookSystem, IVoodooPlugin, "1d3d7f8e-6f32-11e0-8ac0-005056c00000")
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
    /* IVoodooPlugin */
    STDMETHOD(Initialize)(THIS_ IVoodooCore * pCore) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    /* IVoodooHookSystem */
    STDMETHOD(Add)(THIS_ BSTR pName, FunctionPtr pSource, FunctionPtr pDest) PURE;
    STDMETHOD(Remove)(THIS_ BSTR pName) PURE;
    STDMETHOD(RemoveAll)(THIS) PURE;
};

typedef struct IVoodooHookSystem *LPVOODOOHOOKSYSTEM, *PVOODOOHOOKSYSTEM;

#undef INTERFACE
#define INTERFACE IVoodoooLogger

DECLARE_INTERFACE_IID_(IVoodooLogger, IVoodooPlugin, "1d3d7f8f-6f32-11e0-8ac0-005056c00000")
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
    /* IVoodooPlugin */
    STDMETHOD(Initialize)(THIS_ IVoodooCore * pCore) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    /* IVoodoooLogger */
    STDMETHOD(Open)(THIS_ BSTR pFilename, VARIANT_BOOL Append) PURE;
    STDMETHOD(Close)(THIS) PURE;
    STDMETHOD(Dump)(THIS) PURE;
    STDMETHOD(get_LogLevel)(THIS_ EnumType * pLevel) PURE;
    STDMETHOD(put_LogLevel)(THIS_ EnumType Level) PURE;
    STDMETHOD(LogModule)(THIS_ VersionStruct Module) PURE;
    STDMETHOD(Log)(THIS_ EnumType Level, BSTR pModule, BSTR pMessage) PURE;
    STDMETHOD(get_BufferSize)(THIS_ int * pSize) PURE;
    STDMETHOD(put_BufferSize)(THIS_ int Size) PURE;
};

typedef IVoodooLogger *LPVOODOOLOGGER, *PVOODOOLOGGER;

#undef INTERFACE
#define INTERFACE IVoodooFileSystem

DECLARE_INTERFACE_IID_(IVoodooFileSystem, IVoodooPlugin, "1d3d7f90-6f32-11e0-8ac0-005056c00000")
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
    /* IVoodooPlugin */
    STDMETHOD(Initialize)(THIS_ IVoodooCore * pCore) PURE;
    STDMETHOD(get_Core)(THIS_ IVoodooCore ** ppCore) PURE;
    /* IVoodooFileSystem */
    STDMETHOD(AddDirectory)(THIS_ BSTR pPath) PURE;
    STDMETHOD(RemoveDirectory)(THIS_ BSTR pPath) PURE;
    STDMETHOD(FindFile)(THIS_ BSTR pPath, IVoodooFile ** ppFile) PURE;
    STDMETHOD(FindImage)(THIS_ BSTR pPath, IVoodooImage ** ppImage) PURE;
};

typedef struct IVoodooFileSystem *LPVOODOOFILESYSTEM, *PVOODOOFILESYSTEM;

#undef INTERFACE
#define INTERFACE IVoodooFile

DECLARE_INTERFACE_IID_(IVoodooFile, IDispatch, "1d3d7f91-6f32-11e0-8ac0-005056c00000")
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

DECLARE_INTERFACE_IID_(IVoodooImage, IDispatch, "1d3d7f92-6f32-11e0-8ac0-005056c00000")
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
#define MAKE_VSF_LOK(code)    MAKE_HRESULT(SEVERITY_SUCCESS, _FACVSF, code ^ 0x8000 )
#define MAKE_VSF_IOK(code)    MAKE_HRESULT(SEVERITY_SUCCESS, _FACVSF, code | 0x8000 )
#define MAKE_VSF_LERR(code)   MAKE_HRESULT(SEVERITY_ERROR, _FACVSF, code ^ 0x8000 )
#define MAKE_VSF_IERR(code)   MAKE_HRESULT(SEVERITY_ERROR, _FACVSF, code | 0x8000 )

// Library/interface HRESULT get macros
#define IS_LIB_HR(hr)   (  hr & 0x00008000 )
#define IS_IFACE_HR(hr) ( ~hr & 0x00008000 )

// IVoodoo*
#define VSF_OK                  MAKE_VSF_LOK(0x0000)
#define VSF_FAIL                MAKE_VSF_LERR(0x0000)

#define VSFOK_NOT_FOUND         MAKE_VSF_LOK(0x0006)
#define VSFOK_NULL_IMPL         MAKE_VSF_LERR(0x0003)

#define VSFERR_BAD_CLSID        MAKE_VSF_LERR(0x0001)
#define VSFERR_DUP_NAME         MAKE_VSF_LERR(0x0002)
#define VSFERR_LINKER_ERROR     MAKE_VSF_LERR(0x0004)

#define VSFERR_NOT_LINKED       MAKE_VSF_LERR(0x0005)
#define VSFERR_NOT_FOUND        MAKE_VSF_LERR(0x0006)
#define VSFERR_NOT_INIT         MAKE_VSF_LERR(0x0007)

#define VSFERR_INVALID_CFG      MAKE_VSF_LERR(0x0008)
#define VSFERR_INVALID_CG       MAKE_VSF_LERR(0x0009)
#define VSFERR_INVALID_TECH     MAKE_VSF_LERR(0x0010)
#define VSFERR_INVALID_ARG      MAKE_VSF_LERR(0x0011)

#define VSFERR_ALREADY_INIT		MAKE_VSF_LERR(0x0012)

#define VSFERR_NO_CORE          MAKE_VSF_LERR(0x0020)
#define VSFERR_NO_PARSER        MAKE_VSF_LERR(0x0021)
#define VSFERR_NO_SHADER        MAKE_VSF_LERR(0x0022)
#define VSFERR_NO_TECHNIQUE     MAKE_VSF_LERR(0x0023)
#define VSFERR_NO_PASS          MAKE_VSF_LERR(0x0024)
#define VSFERR_NO_TEXTURE       MAKE_VSF_LERR(0x0025)
#define VSFERR_NO_PARAMETER     MAKE_VSF_LERR(0x0026)
#define VSFERR_NO_ADAPTER       MAKE_VSF_LERR(0x0027)
#define VSFERR_NO_HOOKSYSTEM    MAKE_VSF_LERR(0x0028)
#define VSFERR_NO_LOGGER        MAKE_VSF_LERR(0x0029)
#define VSFERR_NO_FILESYSTEM    MAKE_VSF_LERR(0x002A)
#define VSFERR_NO_FILE          MAKE_VSF_LERR(0x002B)
#define VSFERR_NO_IMAGE         MAKE_VSF_LERR(0x002C)
#define VSFERR_NO_PROGRAM       MAKE_VSF_LERR(0x002D)

// IVoodooCore

// IVoodooLogger
#define VSFERR_FILE_ERROR       MAKE_VSF_IERR(0x0001)
#define VSFERR_LOG_ERROR        MAKE_VSF_IERR(0x0003)
#define VSFOK_LOG_LEVEL         MAKE_VSF_IOK(0x0002)

// IVoodooParser
#define VSFERR_IS_SYSVAR        MAKE_VSF_IERR(0x0001)

#endif /*__cplusplus*/
