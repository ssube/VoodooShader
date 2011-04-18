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

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_ATTRIBUTES
//#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

using namespace ATL;

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
    DEFINE_GUID(IID_VoodooTarget,       0xB2286B91, 0xAF4F, 0x4330, 0xA7, 0xAC, 0x7E, 0xCB, 0x90, 0x55, 0xBA, 0x17);
    DEFINE_GUID(IID_VoodooTechnique,    0x1BD418B4, 0x7D6C, 0x4CFB, 0x81, 0x38, 0x33, 0x9D, 0x26, 0xAC, 0xB3, 0x78);
    DEFINE_GUID(IID_VoodooTexture,      0x0F33959B, 0x242E, 0x4282, 0x85, 0x10, 0xBE, 0x6C, 0x99, 0x97, 0xF4, 0x64);
    DEFINE_GUID(IID_VoodooShader,       0x7FFF4B7A, 0xA831, 0x4D98, 0xA3, 0xDD, 0xF5, 0xDA, 0x59, 0x2B, 0xE7, 0x48);

    /**
     * Standard interfaces.
     */
    interface DECLSPEC_UUID("76073706-54E6-457D-B891-52A80410BEEF") IVoodooAdapter;
    //interface DECLSPEC_UUID("94158AA0-87AD-4F99-89F2-7E7B8E9D5CDB") IVoodooConverter;
    interface DECLSPEC_UUID("31420F71-1050-45D9-838F-8735BBB8EDEA") IVoodooCore;
    interface DECLSPEC_UUID("73926B89-5899-4E58-A714-851812188F15") IVoodooFile;
    interface DECLSPEC_UUID("229B5CF6-99BF-4250-9088-128FE299767E") IVoodooFileSystem;
    //interface DECLSPEC_UUID("4939253A-71F0-44F5-94ED-711B62E6B4DB") IVoodooFsManager;
    interface DECLSPEC_UUID("38C848CD-915A-49EB-92A4-A463053C62A8") IVoodooHookSystem;
    interface DECLSPEC_UUID("7EF1C1F0-A474-4115-BF5A-441C83474536") IVoodooImage;
    interface DECLSPEC_UUID("41482ECA-13DA-4D12-B254-F7E55D4DC28D") IVoodooLogger;
    //interface DECLSPEC_UUID("F11D985D-5865-4646-8CBB-44ECDDEE371E") IVoodooMatManager;
    interface DECLSPEC_UUID("AC697824-E290-43FB-9C06-CCFB0D8BF68E") IVoodooObject;
    interface DECLSPEC_UUID("FC21372A-6EA6-4CA5-811C-5ED4835973B7") IVoodooParameter;
    interface DECLSPEC_UUID("41F3AEE5-0F0E-4BF4-9512-DC290E37EB7B") IVoodooParser;
    interface DECLSPEC_UUID("C8BEBCB5-B62F-48B8-82CE-802220B5E930") IVoodooPass;
    interface DECLSPEC_UUID("67D488C5-A57D-4B8D-B48B-7295608B924A") IVoodooTargeted;
    interface DECLSPEC_UUID("B2286B91-AF4F-4330-A7AC-7ECB9055BA17") IVoodooTarget;
    interface DECLSPEC_UUID("1BD418B4-7D6C-4CFB-8138-339D26ACB378") IVoodooTechnique;
    interface DECLSPEC_UUID("0F33959B-242E-4282-8510-BE6C9997F464") IVoodooTexture;
    interface DECLSPEC_UUID("7FFF4B7A-A831-4D98-A3DD-F5DA592BE748") IVoodooShader;

    typedef IVoodooAdapter      *LPVOODOOADAPTER,       *PVOODOOADAPTER;
    //typedef IVoodooConverter    *LPVOODOOCONVERTER,     *PVOODOOCONVERTER;
    typedef IVoodooCore         *LPVOODOOCORE,          *PVOODOOCORE;
    typedef IVoodooFile         *LPVOODOOFILE,          *PVOODOOFILE;
    typedef IVoodooFileSystem   *LPVOODOOFILESYSTEM,    *PVOODOOFILESYSTEM;
    //typedef IVoodooFsManager    *LPVOODOOFSMANAGER,     *PVOODOOFSMANAGER;
    typedef IVoodooHookSystem   *LPVOODOOHOOKSYSTEM,    *LPVOODOOHOOKSYSTEM;
    typedef IVoodooImage        *LPVOODOOIMAGE,         *LPVOODOOIMAGE;
    typedef IVoodooLogger       *LPVOODOOLOGGER,        *LPVOODOOLOGGER;
    //typedef IVoodooMatManager   *LPVOODOOMATMANAGER,    *LPVOODOOMATMANAGER;
    typedef IVoodooParameter    *LPVOODOOPARAMETER,     *LPVOODOOPARAMETER;
    typedef IVoodooParser       *LPVOODOOPARSER,        *LPVOODOOPARSER;
    typedef IVoodooPass         *LPVOODOOPASS,          *LPVOODOOPASS;
    typedef IVoodooTechnique    *LPVOODOOTECHNIQUE,     *LPVOODOOTECHNIQUE;
    typedef IVoodooTexture      *LPVOODOOTEXTURE,       *LPVOODOOTEXTURE;
    typedef IVoodooShader       *LPVOODOOSHADER,        *PVOODOOSHADER;

    [
        object, pointer_default(unique),
        uuid("AC697824-E290-43FB-9C06-CCFB0D8BF68E"),
        helpstring("Named Object Interface")
    ]
    __interface IVoodooObject
        : IUnknown
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("67D488C5-A57D-4B8D-B48B-7295608B924A"),
        helpstring("Targeted Interface")
    ]
    __interface IVoodooTargeted
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooTargeted
        STDMETHOD(GetTarget)([out, retval] IVoodooTarget ** ppTexture) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("B2286B91-AF4F-4330-A7AC-7ECB9055BA17"),
        helpstring("Target Interface")
    ]
    __interface IVoodooTarget
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooTarget
        STDMETHOD(GetRenderTarget)([out, retval] void ** ppTarget) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("76073706-54E6-457D-B891-52A80410BEEF"),
        helpstring("Adapter Interface")
    ]
    __interface IVoodooAdapter
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooAdapter
        STDMETHOD(LoadPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(UnloadPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(BindPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(UnbindPass)([in] IVoodooPass * pPass) PURE;
        STDMETHOD(BindTarget)([in] IVoodooTarget * pTarget) PURE;
        STDMETHOD(UnbindTarget)([in] IVoodooTarget * pTarget) PURE;
        STDMETHOD(DrawGeometry)([in] UINT Vertexes, [in, optional] Vertex * pVertexData) PURE;
        STDMETHOD(ApplyParameter)([in] IVoodooParameter * pParameter) PURE;
        STDMETHOD(DrawShader)([in] IVoodooShader * pShader) PURE;
        STDMETHOD(CreateTexture)([in] BSTR pName, [in] TextureDesc Description, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(LoadTexture)([in] IVoodooImage * pImage, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(ConnectTexture)([in] IVoodooParameter * pParameter, [in] IVoodooTexture * pTexture) PURE;
        STDMETHOD(HandleError)([in, optional] void * pContext, [in] DWORD Error, [in, optional] IVoodooCore * pCore) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("31420F71-1050-45D9-838F-8735BBB8EDEA"),
        helpstring("Core Interface")
    ]
    __interface IVoodooCore
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)([in] REFIID riid, [out] void** ppvObj) PURE;
        STDMETHOD_(ULONG, AddRef)() PURE;
        STDMETHOD_(ULONG, Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooCore
        STDMETHOD(Initialize)([in, optional] BSTR pConfig) PURE;
        [propget, id(7)] STDMETHOD(Parser)([out, retval] IVoodooParser ** pParser) PURE;
        [propget, id(8)] STDMETHOD(HookSystem)([out, retval] IVoodooHookSystem ** ppHookManager) PURE;
        [propget, id(9)] STDMETHOD(FileSystem)([out, retval] IVoodooFileSystem ** ppFileSystem) PURE;
        [propget, id(10)] STDMETHOD(Adapter)([out, retval] IVoodooAdapter ** ppAdapter) PURE;
        [propget, id(11)] STDMETHOD(Logger)([out, retval] IVoodooLogger ** ppLogger) PURE;
        [propget, id(12)] STDMETHOD(Config)([out, retval] IUnknown ** ppConfig) PURE;

        [propget, id(13)] STDMETHOD(CgContext)([out, retval] void ** ppCgContext) PURE;
        [propput, id(13)] STDMETHOD(CgContext)([in] void * pCgContext) PURE;

        STDMETHOD(CreateShader)([in] IVoodooFile * pFile, [out, retval] IVoodooShader ** ppShader) PURE;
        STDMETHOD(CreateParameter)([in] BSTR pName, [in] ParameterType Type, [out, retval] IVoodooParameter ** ppParameter) PURE;
        STDMETHOD(CreateTexture)(BSTR pName, IVoodooTarget * pData, IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(GetParameter)([in] BSTR pName, [out, retval] IVoodooParameter ** ppParameter) PURE;
        STDMETHOD(GetTexture)([in] BSTR pName, [out, retval] IVoodooTexture ** ppTexture) PURE;
        STDMETHOD(RemoveTexture)([in] BSTR pName) PURE;

        [propget, id(21)] STDMETHOD(StageTexture)([in] TextureType Stage, [out, retval] IVoodooTexture ** ppTexture) PURE;
        [propput, id(21)] STDMETHOD(StageTexture)([in] TextureType Stage, [in] IVoodooTexture * pTexture) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("73926B89-5899-4E58-A714-851812188F15"),
        helpstring("File Interface")
    ]
    __interface IVoodooFile
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooFile
        STDMETHOD(GetPath)([in, out] UINT * Length, [out, optional] LPBSTR pPath) PURE;
        STDMETHOD(Open)([in] FileOpenMode Mode) PURE;
        STDMETHOD(Close)() PURE;
        STDMETHOD(Read)([in, out] UINT * Length, [out, size_is(Length)] void * ppBuffer) PURE;
        STDMETHOD(Write)([in] UINT Length, [in, size_is(Length)] void * pBuffer) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("229B5CF6-99BF-4250-9088-128FE299767E"),
        helpstring("FileSystem Interface")
    ]
    __interface IVoodooFileSystem
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooFileSystem
        STDMETHOD(AddDirectory)([in] BSTR pPath) PURE;
        STDMETHOD(RemoveDirectory)([in] BSTR pPath) PURE;
        STDMETHOD(FindFile)([in] BSTR pPath, [out, retval] IVoodooFile ** ppFile) PURE;
        STDMETHOD(FindImage)([in] BSTR pPath, [out, retval] IVoodooImage ** ppImage) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("38C848CD-915A-49EB-92A4-A463053C62A8"),
        helpstring("Hook System Interface")
    ]
    __interface IVoodooHookSystem
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooHookSystem
        STDMETHOD(CreateHook)([in] BSTR pName, [in] void * pSource, [in] void * pDest) PURE;
        STDMETHOD(RemoveHook)([in] BSTR pName) PURE;
        STDMETHOD(RemoveAllHooks)() PURE;
    };

    [
        object, pointer_default(unique),
        uuid("7EF1C1F0-A474-4115-BF5A-441C83474536"),
        helpstring("Image File Interface")
    ]
    __interface IVoodooImage
        : IVoodooFile
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooFile
        STDMETHOD(GetPath)([in, out] UINT * Length, [out, optional] LPBSTR pPath) PURE;
        STDMETHOD(Open)([in] FileOpenMode Mode) PURE;
        STDMETHOD(Close)() PURE;
        STDMETHOD(Read)([in, out] UINT * Length, [out, size_is(Length)] void * ppBuffer) PURE;
        STDMETHOD(Write)([in] UINT Length, [in, size_is(Length)] void * pBuffer) PURE;

        // IVoodooImage
        STDMETHOD(GetDesc)([out, retval] TextureDesc * pDesc) PURE;
        STDMETHOD(GetData)([in] TextureRegion Region, [out, retval] void * ppBuffer) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("41482ECA-13DA-4D12-B254-F7E55D4DC28D"),
        helpstring("Logger Interface")
    ]
    __interface IVoodooLogger
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooLogger
        STDMETHOD(Open)([in] BSTR pFilename, [in] BOOL Append) PURE;
        STDMETHOD(Close)() PURE;
        STDMETHOD(Dump)() PURE;
        [propget, id(4)] STDMETHOD(GetLogLevel)([out, retval] LogLevel * pLevel) PURE;
        [propput, id(4)] STDMETHOD(SetLogLevel)([in] LogLevel * Level) PURE;
        //STDMETHOD(Stamp)([in] StampType Stamp, [in, out] UINT * Length, [out, retval, size_is(Length)] void * pBuffer) PURE;
        STDMETHOD(LogModule)([in] Version Module) PURE;
        [vararg] STDMETHOD(Log)([in] LogLevel Level, [in] BSTR pModule, [in] BSTR pFormat, [in, satype(VARIANT)] SAFEARRAY * pArgs) PURE;
        [propget, id(9)] STDMETHOD(GetBufferSize)([out, retval] UINT * pSize) PURE;
        [propput, id(9)] STDMETHOD(SetBufferSize)([in] UINT Size) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("FC21372A-6EA6-4CA5-811C-5ED4835973B7"),
        helpstring("Parameter Interface")
    ]
    __interface IVoodooParameter
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooParameter
        STDMETHOD(GetCgParameter)([out, retval] void ** ppCgParameter) PURE;
        STDMETHOD(GetParameterType)([out, retval] ParameterType * pType) PURE;
        STDMETHOD(AttachParameter)([in] IVoodooParameter * pParameter) PURE;
        STDMETHOD(ForceUpdate)() PURE;

        STDMETHOD(SetValue)([in, optional] IVoodooTexture * pTexture) PURE;
        STDMETHOD(GetValue)([out, retval] IVoodooTexture ** ppTexture) PURE;

//         STDMETHOD(SetValue)([in] float X) PURE;
//         STDMETHOD(SetValue)([in] float X, [in] float Y) PURE;
//         STDMETHOD(SetValue)([in] float X, [in] float Y, [in] float Z) PURE;
//         STDMETHOD(SetValue)([in] float X, [in] float Y, [in] float Z, [in] float W) PURE;
//         STDMETHOD(SetValue)([in, size_is(pData + sizeof(float)*16)] float * pData) PURE;
// 
//         STDMETHOD(GetValue)([out] float * pX) PURE;
//         STDMETHOD(GetValue)([out] float * pX, [out] float * pY) PURE;
//         STDMETHOD(GetValue)([out] float * pX, [out] float * pY, [out] float * pZ) PURE;
//         STDMETHOD(GetValue)([out] float * pX, [out] float * pY, [out] float * pZ, [out] float * pW) PURE;
//         STDMETHOD(GetValue)([out] float * pData) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("41F3AEE5-0F0E-4BF4-9512-DC290E37EB7B"),
        helpstring("Variable Parser Interface")
    ]
    __interface IVoodooParser
        : IUnknown
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooParser
        STDMETHOD(ToLower)([in, out] LPBSTR pString) PURE;
        STDMETHOD(ToUpper)([in, out] LPBSTR pString) PURE;
        STDMETHOD(AddVariable)([in] BSTR pName, [in] BSTR pValue, [in] BOOL System) PURE;
        STDMETHOD(RemoveVariable)([in] BSTR pName) PURE;
        STDMETHOD(Parse)([in] BSTR pInput, [out, retval] LPBSTR pOutString) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("C8BEBCB5-B62F-48B8-82CE-802220B5E930"),
        helpstring("Pass Interface")
    ]
    __interface IVoodooPass
        : IVoodooTargeted
    {
        STDMETHOD(GetCgProgram)([in] ProgramStage Stage, [out, retval] void ** ppProgram) PURE;
        STDMETHOD(GetCgPass)([out, retval] void ** ppPass) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("1BD418B4-7D6C-4CFB-8138-339D26ACB378"),
        helpstring("Technique Interface")
    ]
    __interface IVoodooTechnique
        : IVoodooTargeted
    {
        STDMETHOD_(UINT, GetPassCount)() PURE;
        STDMETHOD(GetPass)([in] UINT Number, [out, retval] IVoodooPass ** ppPass) PURE;
        STDMETHOD(GetCgTechnique)([out, retval] void ** ppPass) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("0F33959B-242E-4282-8510-BE6C9997F464"),
        helpstring("Texture Interface")
    ]
    __interface IVoodooTexture
        : IVoodooTarget
    {
        STDMETHOD(GetData)([out, retval] void ** ppData) PURE;
    };

    [
        object, pointer_default(unique),
        uuid("7FFF4B7A-A831-4D98-A3DD-F5DA592BE748"),
        helpstring("Shader Interface")
    ]
    __interface IVoodooShader
        : IVoodooObject
    {
        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)() PURE;
        STDMETHOD_(ULONG,Release)() PURE;

        // IVoodooObject
        STDMETHOD(GetName)([out, retval] LPBSTR pName) PURE;
        STDMETHOD(GetCore)([out, retval] IVoodooCore ** ppCore) PURE;

        // IVoodooShader
        STDMETHOD_(UINT, GetTechniqueCount)() PURE;
        STDMETHOD(GetTechnique)([in] UINT Number) PURE;
        [propget, id(4)] STDMETHOD(GetDefaultTechnique)([out, retval] IVoodooTechnique ** ppTechnique) PURE;
        [propput, id(4)] STDMETHOD(SetDefaultTechnique)([in] IVoodooTechnique * pTechnique) PURE;
        STDMETHOD_(UINT, GetParameterCount)() PURE;
        STDMETHOD(GetParameter)([in] UINT Number, [out, retval] IVoodooParameter ** ppParameter) PURE;
        STDMETHOD(GetCgShader)([out, retval] void ** ppCgShader) PURE;
    };
}