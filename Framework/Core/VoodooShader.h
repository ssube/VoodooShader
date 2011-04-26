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
 *    {1d3d7f88-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Top-level shader object, encapsulates an entire effect tree.
 *    
 * Provides the shader loader and linker, as well as access to the generated tree of effect
 * components.
 * 
 * Each shader consists of techniques and parameters. Techniques are validated during loading and
 * not added if they cannot run on the current hardware. All effect-level parameters with 
 * compatible types are added and linked as appropriate.
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
 *    {1d3d7fa5-6f32-11e0-8ac0-005056c00000}
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
    virtual HRESULT STDMETHODCALLTYPE get_Name(LPBSTR pName);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE get_TechniqueCount(INT * pCount);
    virtual HRESULT STDMETHODCALLTYPE GetTechnique(INT Number, IVoodooTechnique **ppTechnique);
    virtual HRESULT STDMETHODCALLTYPE get_DefaultTechnique(IVoodooTechnique **ppTechnique);
    virtual HRESULT STDMETHODCALLTYPE put_DefaultTechnique(IVoodooTechnique *pTechnique);
    virtual HRESULT STDMETHODCALLTYPE get_ParameterCount(INT *ppCount);
    virtual HRESULT STDMETHODCALLTYPE GetParameter(INT Number, IVoodooParameter **ppParameter);
    virtual HRESULT STDMETHODCALLTYPE get_CgShader(VARIANT *ppCgShader);

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
