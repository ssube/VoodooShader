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
 * @file VoodooSupport.hpp
 * 
 * Provides global support functions to make certain repeated tasks easier (formatted logging, for 
 * example). These functions are intended to be used within any module or plugin and are provided
 * in inline forms to allow that.
 */

#include "VoodooFramework.hpp"

#include <atlstr.h>

/**
 * Formats and logs a message containing the current function, file, line and optionally
 * parameteres.
 *
 * @param pLogger The IVoodooLogger to write to.
 * @param Level The log level to use (a combination of @ref LogLevel flags).
 * @param pModule The source module for this message.
 * @param pMsg The custom message to print (will not be formatted).
 * @return Results of the log call.
 */

#define LOGFUNC(pLogger, Level, pModule, pMsg) \
    LogFormat(pLogger, Level, pModule, VSTR("Function %s (%s:%d). %s."), __FUNCSIG__, __FILE__, __LINE__, pMsg)

/**
 * Writes a formatted message to the given logger. This function uses CString.FormatV internally,
 * so any formatting restrictions on that also apply here (it uses printf syntax). This formats the
 * message use a va_list before crossing module/process boundaries, providing safer memory 
 * handling. This functions essentially identically to the .Net syntax for IVoodooLogger::Log(),
 * taking a series of variable args and performing identical formatting.
 * 
 * @param pLogger The IVoodooLogger to write to (calls <code>pLogger->Log(...)</code>).
 * @param Level The log level to use (a combination of @ref LogLevel flags).
 * @param pModule The source module for this message.
 * @param pMsg The format string to use.
 * @param ... The arguments to use for formatting.
 * @return Results of the log call.
 */
inline HRESULT WINAPI LogFormat(IVoodooLogger * pLogger, DWORD Level, LPTSTR pModule, LPTSTR pMsg, ...)
{
    if ( pLogger == NULL || pModule == NULL || pMsg == NULL ) return VSFERR_INVALID_ARG;

    CString fmtMsg;
    CString module(pModule);

    va_list args;
    va_start(args, pMsg);
    fmtMsg.FormatV(pMsg, args);
    va_end(args);

    CComBSTR fmtBSTR(fmtMsg);
    CComBSTR modBSTR(pModule);

    return pLogger->Log(Level, pModule, fmtBSTR);
}
