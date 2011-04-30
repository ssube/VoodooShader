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
 * @struct IVoodooAdapter
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooAdapter
 * @par GUID
 *    {1d3d7f8d-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Ties the Voodoo core to a specific target application and graphics API.
 *    
 * Provides various implementation details that vary by program, as well as handling most startup
 * details. 
 * 
 * Adapters must meet a specific set of requirements for behavior of each method. Any difference
 * may cause the adapter to generate errors or be incompatible with Voodoo.
 * 
 * @sa @ref page_adapters
 **/

/**
 * @class CVoodooAdapter
 * 
 * @par Interface
 *    @ref IVoodooShader
 * @par IID/CLSID
 *    @ref CLSID_VoodooShader
 * @par GUID
 *    {1d3d7faa-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    none
 * @par Version
 *    0.3
 *    
 * @brief 
 *    Null implementation of the adapter interface (provides all methods but performs no actions).
 *    
 * This adapter does no rendering, nor does it tie to (or require) any graphics API. It returns
 * VSFERR_NULLIMPL for all method calls.
 */
class ATL_NO_VTABLE CVoodooAdapter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooAdapter, &CLSID_VoodooAdapter>,
	public IDispatchImpl<IVoodooAdapter, &IID_IVoodooAdapter, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
	CVoodooAdapter();
    virtual ~CVoodooAdapter();
    static IVoodooAdapter * Create(IVoodooCore * pCore);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOADAPTER)

DECLARE_NOT_AGGREGATABLE(CVoodooAdapter)

BEGIN_COM_MAP(CVoodooAdapter)
	COM_INTERFACE_ENTRY(IVoodooAdapter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooAdapter
public:
    virtual HRESULT STDMETHODCALLTYPE get_Name(LPBSTR pName);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE LoadPass(IVoodooPass *pPass);
    virtual HRESULT STDMETHODCALLTYPE UnloadPass(IVoodooPass *pPass);
    virtual HRESULT STDMETHODCALLTYPE get_Pass(IVoodooPass **ppPass);
    virtual HRESULT STDMETHODCALLTYPE put_Pass(IVoodooPass *pPass);
    virtual HRESULT STDMETHODCALLTYPE get_Target(IVoodooTexture **ppTarget);
    virtual HRESULT STDMETHODCALLTYPE put_Target(IVoodooTexture *pTarget);
    virtual HRESULT STDMETHODCALLTYPE get_Light(int Index, LightStruct *pLight);
    virtual HRESULT STDMETHODCALLTYPE put_Light(int Index, LightStruct Light);
    virtual HRESULT STDMETHODCALLTYPE SetProperty(BSTR pName, VARIANT *pData);
    virtual HRESULT STDMETHODCALLTYPE DrawGeometry(int Vertexes, SAFEARRAY *pVertexData);
    virtual HRESULT STDMETHODCALLTYPE ApplyParameter(IVoodooParameter *pParameter);
    virtual HRESULT STDMETHODCALLTYPE DrawShader(IVoodooShader *pShader);
    virtual HRESULT STDMETHODCALLTYPE CreateTexture(BSTR pName, TextureDesc Description, IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE LoadTexture(IVoodooImage *pImage, TextureRegion Region, IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE ConnectTexture(IVoodooParameter *pParameter, IVoodooTexture *pTexture);
    virtual HRESULT STDMETHODCALLTYPE HandleError(VARIANT pContext, DWORD Error, IVoodooCore *pCore);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooAdapter), CVoodooAdapter)
