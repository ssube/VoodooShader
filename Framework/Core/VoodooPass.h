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
 * @struct IVoodooPass
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooPass
 * @par GUID
 *    {1d3d7f8a-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Contains a single set of programs, each operating on a different stage of the render 
 *    pipeline and intended to function simultaneously.
 * 
 * A pass contains programs for one or more render stages and may contain other render states. 
 * These are all set and reset with the pass.
 * 
 * The available render states can manipulate a number of things (blending modes, lighting, etc).
 * See the Cg docs for a full list and API support.
 * 
 * @note Valid stages vary by underlying API and version. In OpenGL and DirectX 9 and earlier,
 *     stages may be left to the fixed-function pipeline by not specifying a program for that 
 *     stage. Later APIs require the vertex and pixel shaders to be specified in each pass and may
 *     make additional stages available. Voodoo internally supports all stages up to OpenGL 4.0/
 *     DirectX 11.
 * 
 * @sa @ref page_shaders_sec_pass
 **/

/**
 * @class CVoodooPass
 * 
 * @par Interface
 *    @ref IVoodooPass
 * @par IID/CLSID
 *    @ref CLSID_VoodooPass
 * @par GUID
 *    {1d3d7fa7-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    noncreatable
 * @par Version
 *    0.3
 *    
 * @brief 
 *    Bottom-level shader object, contains a set of programs and states.
 */
class ATL_NO_VTABLE CVoodooPass :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooPass, &CLSID_VoodooPass>,
	public IDispatchImpl<IVoodooPass, &IID_IVoodooPass, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
	CVoodooPass();
    virtual ~CVoodooPass();
    static IVoodooPass * Create(IVoodooTechnique * pTechnique, CGpass Pass);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPASS)

DECLARE_NOT_AGGREGATABLE(CVoodooPass)

BEGIN_COM_MAP(CVoodooPass)
	COM_INTERFACE_ENTRY(IVoodooPass)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooPass
public:
    virtual HRESULT STDMETHODCALLTYPE get_Name(LPBSTR pName);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE get_Target(IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE get_Shader(IVoodooShader **ppShader);
    virtual HRESULT STDMETHODCALLTYPE get_Technique(IVoodooTechnique **ppTechnique);
    virtual HRESULT STDMETHODCALLTYPE GetCgProgram(DWORD Stage, VARIANT *ppProgram);
    virtual HRESULT STDMETHODCALLTYPE get_CgPass(VARIANT *ppPass);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    IVoodooTexture * m_Target;
    IVoodooShader * m_Shader;
    IVoodooTechnique * m_Technique;
    CGpass m_Pass;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooPass), CVoodooPass)
