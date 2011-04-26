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
 * @struct IVoodooParser
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooParser
 * @par GUID
 *    {1d3d7f87-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Relatively simple variable parser (find and replace with a few other features).
 *    
 * The parser provides a simple system for using variables within Voodoo strings and paths. Most
 * major string are run through this before being used.
 * 
 * A number of potentially useful features, including temporary variables and specialized rules for
 * path parsing, are available.
 * 
 * @sa @ref page_varsyntax
 * @sa @ref ParseFlags
 **/

/**
 * String/string map, functioning as a dictionary for variables. Keys are variable name.
 */
typedef CMap<CComBSTR, CComBSTR, CComBSTR, CComBSTR> Dictionary;

/**
 * @class CVoodooParser
 * 
 * @par Interface
 *    @ref IVoodooParser
 * @par IID/CLSID
 *    @ref CLSID_VoodooParser
 * @par GUID
 *    {1d3d7fa4-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    none
 * @par Version
 *    0.3
 *    
 * @brief 
 *    Primary parser implementation.
 *    
 * This object may be created externally (@tt CoCreateInstance) but will not feature logging in
 * that case. When created within Voodoo/Core (automatically performed by CVoodooCore on init), it
 * will be linked to an IVoodooLogger and can dump debug information on parsing.
 */
class ATL_NO_VTABLE CVoodooParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooParser, &CLSID_VoodooParser>,
	public IDispatchImpl<IVoodooParser, &IID_IVoodooParser, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
	CVoodooParser();
    virtual ~CVoodooParser();
    static IVoodooParser * Create(IVoodooCore * pCore);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPARSER)

DECLARE_NOT_AGGREGATABLE(CVoodooParser)

BEGIN_COM_MAP(CVoodooParser)
	COM_INTERFACE_ENTRY(IVoodooParser)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooParser
public:
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddVariable( 
        /* [in] */ BSTR pName,
        /* [in] */ BSTR pValue,
        /* [in] */ DWORD Type);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveVariable( 
        /* [in] */ BSTR pName);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Parse( 
        /* [in] */ BSTR pString,
        /* [defaultvalue][in] */ DWORD Flags,
        /* [retval][out] */ LPBSTR pParsed);

private:
    HRESULT STDMETHODCALLTYPE ParseRaw(LPBSTR input, DWORD Flags, INT Depth, Dictionary * State);

    static const INT VarMaxDepth = 8;
    static const WCHAR VarDelimStart = '(';
    static const WCHAR VarDelimEnd   = ')';
    static const WCHAR VarDelimPre   = '$';

    ULONG m_Refrs;
    IVoodooCore * m_Core;
    CComPtr<IVoodooLogger> m_Logger;
    Dictionary m_Variables;
    Dictionary m_SysVariables;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooParser), CVoodooParser)
