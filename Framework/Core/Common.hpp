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

#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <msxml2.h>
#include <atlsafe.h>

#include <Cg/Cg.h>

#include "VoodooVersion.hpp"

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
#define IS_LIB_HR(hr)   (( hr & 0x00008000 ) == 0x00000000 )
#define IS_IFACE_HR(hr) (( hr & 0x00008000 ) == 0x00008000 )

// IVoodoo*
extern const HRESULT VSF_OK;
extern const HRESULT VSF_FAIL;

extern const HRESULT VSFOK_NULL_IMPL;
extern const HRESULT VSFOK_NOT_FOUND;

extern const HRESULT VSFERR_BAD_CLSID;
extern const HRESULT VSFERR_DUP_NAME;
extern const HRESULT VSFERR_LINKER_ERROR;

extern const HRESULT VSFERR_NOT_LINKED;
extern const HRESULT VSFERR_NOT_FOUND;

extern const HRESULT VSFERR_INVALID_CFG;
extern const HRESULT VSFERR_INVALID_CG;
extern const HRESULT VSFERR_INVALID_TECH;
extern const HRESULT VSFERR_INVALID_ARG;

extern const HRESULT VSFERR_NO_CORE;
extern const HRESULT VSFERR_NO_PARSER;
extern const HRESULT VSFERR_NO_SHADER;
extern const HRESULT VSFERR_NO_TECHNIQUE;
extern const HRESULT VSFERR_NO_PASS;
extern const HRESULT VSFERR_NO_TEXTURE;
extern const HRESULT VSFERR_NO_PARAMETER;
extern const HRESULT VSFERR_NO_ADAPTER;
extern const HRESULT VSFERR_NO_HOOKSYSTEM;
extern const HRESULT VSFERR_NO_LOGGER;
extern const HRESULT VSFERR_NO_FILESYSTEM;
extern const HRESULT VSFERR_NO_FILE;
extern const HRESULT VSFERR_NO_IMAGE;
extern const HRESULT VSFERR_NO_PROGRAM;

// IVoodooCore
extern const HRESULT VSFERR_NOT_INIT;

// IVoodooLogger
extern const HRESULT VSFERR_FILE_ERROR;

// IVoodooParser
extern const HRESULT VSFERR_IS_SYSVAR;

// Creates an interface to a string-format class ID. The ID may be in registry form or a ProgID.
HRESULT WINAPI InstanceFromString(_In_ BSTR lpStr, _In_ REFIID iid, _In_ void ** pp);
CGparameter WINAPI CreateVirtualParameter(IVoodooCore * pCore, ParameterType Type);

inline HRESULT WINAPI LogFormat(IVoodooLogger * pLogger, DWORD Level, LPWSTR pModule, LPWSTR pMsg, ...)
{
    if ( pLogger == NULL || pModule == NULL || pMsg == NULL ) return VSFERR_INVALID_ARG;

    CStringW fmtMsg;

    va_list args;
    va_start(args, pModule);
    fmtMsg.FormatV(pMsg, args);
    va_end(args);

    CComBSTR fmtBSTR = fmtMsg;
    return pLogger->Log(Level, pModule, fmtBSTR, NULL);
}

CGtype WINAPI ToCgType(ParameterType Type);
ParameterType WINAPI ToParameterType(CGtype Type);
ParameterCategory WINAPI ToParameterCategory(ParameterType Type);
TextureFormat WINAPI ToTextureFormat(BSTR pString);

/*
#ifdef _DEBUG
CPair<long, int> VoodooMemMgr_MemBlock;
CMap<void*, void*, VoodooMemMgr_MemBlock, VoodooMemMgr_MemBlock> VoodooMemMgr_Memory;

int VoodooMemMgr(int allocType, void *userData, size_t size, int blockType, long requestNumber, const unsigned char *filename, int lineNumber);
void VoodooMemMgr_Report();
#endif
*/
