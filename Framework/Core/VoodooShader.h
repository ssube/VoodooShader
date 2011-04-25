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
 * @struct IVoodooShader
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooShader
 * @par GUID
 *    {8C26CEBA-45AD-4382-BEA9-EDBB2F446B21}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Top-level shader object, encapsulates an entire effect tree.
 *    
 * Provides the shader loader and linker, as well as access to the generated tree of effect
 * components.
 * 
 * @sa @ref page_shaders
 **/

/**
 * @class CVoodooShader
 * 
 * @par Interface
 *    @ref IVoodooShader
 * @par IID/CLSID
 *    @ref CLSID_VoodooShader
 * @par GUID
 *    {9E6F15CB-5B03-4BD8-9D04-B27D576C3975}
 * @par Attributes
 *    noncreatable
 * @par Version
 *    0.3
 *    
 * @brief 
 *    Top-level shader object, encapsulates an entire effect tree.
 *    
 * Complete shader loader and linker using Cg, created from @ref IVoodooCore::CreateShader() 
 * (the file given is loaded and compiled as a Cg effect). 
 */
class ATL_NO_VTABLE CVoodooShader :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooShader, &CLSID_VoodooShader>,
	public IDispatchImpl<IVoodooShader, &IID_IVoodooShader, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
	CVoodooShader();
    virtual ~CVoodooShader();
    static IVoodooShader * Create(IVoodooCore * pCore, IVoodooFile * pFile);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOSHADER)

DECLARE_NOT_AGGREGATABLE(CVoodooShader)

BEGIN_COM_MAP(CVoodooShader)
	COM_INTERFACE_ENTRY(IVoodooShader)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooShader
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
        /* [retval][out] */ LPBSTR pName);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
        /* [retval][out] */ IVoodooCore **ppCore);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TechniqueCount( 
        /* [retval][out] */ INT * pCount);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTechnique( 
        /* [in] */ INT Number,
        /* [retval][out] */ IVoodooTechnique **ppTechnique);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DefaultTechnique( 
        /* [retval][out] */ IVoodooTechnique **ppTechnique);

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DefaultTechnique( 
        /* [in] */ IVoodooTechnique *pTechnique);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ParameterCount( 
        /* [retval][out] */ INT *ppCount);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetParameter( 
        /* [in] */ INT Number,
        /* [retval][out] */ IVoodooParameter **ppParameter);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE get_CgShader( 
        /* [retval][out] */ VARIANT *ppCgShader);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    CArray<CComPtr<IVoodooTechnique>> m_Techniques;
    CComPtr<IVoodooTechnique> m_Default;
    CArray<CComPtr<IVoodooParameter>> m_Parameters;
    CGeffect m_Effect;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooShader), CVoodooShader)
