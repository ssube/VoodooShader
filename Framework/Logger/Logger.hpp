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

#include <afx.h>

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#define _ATL_ATTRIBUTES
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define COM_NO_WINDOWS_H

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlplus.h>

using namespace ATL;

#include <iostream>
#include <stdio.h>

using std::wcout;

//#include "Core_i.h"
#include "VoodooFramework.h"

[module(dll, name="Voodoo_XmlLogger")];
//#import "Debug\\Voodoo_Core.tlb" embedded_idl("emitidl")
[emitidl];

/**
 * Provides a simple implementation of ILogger, printing a neatly-formatted Xml log. These
 * logs can be opened with the standard log viewer and support all features. 
 * 
 * @note In debug builds, this logger also prints all messages to cout. If a debugger is
 *    attached, warnings and errors are printed to its output window.
 * 
 * @addtogroup VoodooXmlLogger Voodoo/XmlLogger
 * @{
 */

/**
 * Xml logger class, writes neatly formatted logs for use with the log viewer.
 * Supports all Voodoo ILogger features and debug logging (with no buffer).
 * 
 * @warning If the logger isn't destroyed properly, the main tag of the log file won't be closed 
 *    and the log won't be valid.
 */
[
    coclass, default(IVoodooLogger),
    vi_progid("VoodooXmlLogger.Logger"), progid("VoodooXmlLogger.Logger.1"),
    uuid("1d3d7fb0-6f32-11e0-8ac0-005056c00000")
]
class XmlLogger
    : public IVoodooLogger
{
public:
    /**
     * Default constructor, opens a log file with the given name and mode.
     *
     * @param core The core to bind this logger to.
     */
    XmlLogger();

    /** 
     * Default destructor, flushes and closes the log file (if open).
     */
    virtual ~XmlLogger();

    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE Open(BSTR pFilename, VARIANT_BOOL Append);
    virtual HRESULT STDMETHODCALLTYPE Close(void);
    virtual HRESULT STDMETHODCALLTYPE Dump(void);
    virtual HRESULT STDMETHODCALLTYPE get_LogLevel(DWORD *pLevel);
    virtual HRESULT STDMETHODCALLTYPE put_LogLevel(DWORD Level);
    virtual HRESULT STDMETHODCALLTYPE LogModule(VersionStruct Module);
    virtual HRESULT STDMETHODCALLTYPE Log(DWORD Level, BSTR pModule, BSTR pFormat, SAFEARRAY * ppArgs);
    virtual HRESULT STDMETHODCALLTYPE LogList(DWORD Level, BSTR pModule, BSTR pFormat, VARIANT pList);
    virtual HRESULT STDMETHODCALLTYPE get_BufferSize(int *pSize);
    virtual HRESULT STDMETHODCALLTYPE put_BufferSize(int Size);

private:
    /**
     * Formats a timestamp for the log. The timestamp will have the 
     * form <code>HHMMSS</code>. Leading zeros are guaranteed to be
     * present, so the timestamp length is 6 chars.
     *
     * @note If the system time cannot be retrieved, an equal-length error 
     *       stamp of <code>000000</code> will be returned.
     */
    CStringW STDMETHODCALLTYPE LogTime();

    /**
     * Formats a date for the log. The date will have the form 
     * <code>YYYYMMDD</code>. Leading zeros are guaranteed to be present,
     * so the date length is 8 chars.
     *
     * @note If the system time cannot be retrieved, an equal-length error 
     *       stamp of <code>00000000</code> will be returned.
     */
    CStringW STDMETHODCALLTYPE LogDate();

    /**
     * Formats the system's current tick count. The stamp will have the
     * form <code>xxxxxxxxx</code>, with a potentially varying length. This
     * records ticks, usually ms since system start.
     */
    CStringW STDMETHODCALLTYPE LogTicks();

    inline void DebugLog(LPCWSTR pMsg)
    {
#ifdef _DEBUG
        OutputDebugString(pMsg);
        wcout << pMsg;
#endif
    };

    ULONG m_Refrs;
    IVoodooCore * m_Core;
    LogLevel m_LogLevel;
    CStdioFile m_LogFile;
};
/**
 * @}
 **/
