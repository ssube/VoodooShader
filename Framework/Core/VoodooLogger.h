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

#include "Common.hpp"

using namespace ATL;

/**
 * @struct IVoodooLogger
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooLogger
 * @par GUID
 *    {1d3d7f8f-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Logs data to a file or other storage mechanism, possibly providing formatting or handling
 *    more advanced logging and error handling.
 * 
 * Abstracts file writing to add formatting and filter support, as well as noting loaded modules
 * or other data.
 * 
 * Logger implementations may vary widely, from plaintext (as the core implementation) to Xml or
 * even logging into a database. Loggers may also support various error and crash handling
 * features, from direct-to-disk writing to journaled database inserts.
 **/

/**
 * @class CVoodooLogger
 * 
 * @par Interface
 *    @ref IVoodooLogger
 * @par IID/CLSID
 *    @ref CLSID_VoodooLogger
 * @par GUID
 *    {1d3d7fac-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    none
 * @par Version
 *    1.0
 *    
 * @brief 
 *    Plaintext logger with basic formatting.
 *    
 * Logs provided data to a text file with timestamps and simple formatting of messages. Does not
 * perform any extended formatting and uses the extension ".log". Logs are not compatible with the 
 * log viewer. Logs may be truncated in the event of a crash.
 */
class ATL_NO_VTABLE CVoodooLogger :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooLogger, &CLSID_VoodooLogger>,
	public IDispatchImpl<IVoodooLogger, &IID_IVoodooLogger, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooLogger();
    virtual ~CVoodooLogger();
    static IVoodooLogger * Create(IVoodooCore * pCore);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOLOGGER)

DECLARE_NOT_AGGREGATABLE(CVoodooLogger)

BEGIN_COM_MAP(CVoodooLogger)
	COM_INTERFACE_ENTRY(IVoodooLogger)
    COM_INTERFACE_ENTRY(IVoodooPlugin)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooLogger
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE Open(BSTR pFilename, VARIANT_BOOL Append);
    virtual HRESULT STDMETHODCALLTYPE Close(void);
    virtual HRESULT STDMETHODCALLTYPE Dump(void);
    virtual HRESULT STDMETHODCALLTYPE get_LogLevel(DWORD *pLevel);
    virtual HRESULT STDMETHODCALLTYPE put_LogLevel(DWORD Level);
    virtual HRESULT STDMETHODCALLTYPE LogModule(VersionStruct Module);
    virtual HRESULT STDMETHODCALLTYPE Log(DWORD Level, BSTR pModule, BSTR pMessage);
    virtual HRESULT STDMETHODCALLTYPE get_BufferSize(int *pSize);
    virtual HRESULT STDMETHODCALLTYPE put_BufferSize(int Size);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    LogLevel m_Level;
    CStdioFile m_File;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooLogger), CVoodooLogger)
