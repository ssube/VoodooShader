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
 * @struct IVoodooTechnique
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooTechnique
 * @par GUID
 *    {1d3d7f89-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Primary shader object, containing a set of passes. 
 *    
 * Techniques are the main object of a shader. Each technique contains a set of passes, intended to 
 * be run in order and forming the full effect. Techniques may be designed for a particular set of
 * hardware or feature level, and multiple techniques may be provided within a shader. Techniques
 * may have a target, which essentially acts as the output for the whole effect (final gather).
 * 
 * @sa @ref page_shaders_sec_tech
 **/

/**
 * @class CVoodooTechnique
 * 
 * @par Interface
 *    @ref IVoodooTechnique
 * @par IID/CLSID
 *    @ref CLSID_VoodooTechnique
 * @par GUID
 *    {1d3d7fa6-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    noncreatable
 * @par Version
 *    0.3
 *    
 * @brief 
 *    Primary shader object, containing a set of passes.
 *    
 * A single compiled, validated and linked technique. 
 */
class ATL_NO_VTABLE CVoodooTechnique :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooTechnique, &CLSID_VoodooTechnique>,
	public IDispatchImpl<IVoodooTechnique, &IID_IVoodooTechnique, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
	CVoodooTechnique();
    virtual ~CVoodooTechnique();
    static IVoodooTechnique * Create(IVoodooShader * pShader, CGtechnique pTechnique);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOTECHNIQUE)

DECLARE_NOT_AGGREGATABLE(CVoodooTechnique)

BEGIN_COM_MAP(CVoodooTechnique)
	COM_INTERFACE_ENTRY(IVoodooTechnique)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooTechnique
public:
    virtual HRESULT STDMETHODCALLTYPE get_Name(LPBSTR pName);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE get_Target(IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE get_PassCount(INT *ppCount);
    virtual HRESULT STDMETHODCALLTYPE GetPass(INT Number, IVoodooPass **ppPass);
    virtual HRESULT STDMETHODCALLTYPE get_Shader(IVoodooShader **ppShader);
    virtual HRESULT STDMETHODCALLTYPE get_CgTechnique(VARIANT *ppPass);

private:
    HRESULT STDMETHODCALLTYPE LinkTechnique();

    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    IVoodooTexture * m_Target;
    CArray<CComPtr<IVoodooPass>> m_Passes;
    IVoodooShader * m_Shader;
    CGtechnique m_Technique;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooTechnique), CVoodooTechnique)
