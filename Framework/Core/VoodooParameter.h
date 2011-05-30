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
 * @struct IVoodooParameter
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooParameter
 * @par GUID
 *    {1d3d7f8c-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief 
 *    Connects a shader input variable to a set of data.
 * 
 * A parameter contains an underlying Cg parameter (may be effect-level or virtual) and the data it
 * currently contains. Voodoo fully supports float and sampler parameter types (up to 4x4 float
 * matrices and 3D samplers), and has basic support for user-defined structs.
 * 
 * Voodoo uses two primary types of parameters (virtual and effect) to handle data. Virtual
 * parameters exist only in the runtime, but may have effect parameters bound to them (data is
 * propogated downward automatically, making for easy global params).
 * 
 * All parameters have a parent IVoodooCore, but virtual parameters have no parent IVoodooShader.
 * Effect parameters do have a shader, and share the shader's life span.
 * 
 * @todo Support struct parameters fully and add array handling.
 * 
 * @sa @ref page_params
 * @sa @ref page_params_sec_virtual
 **/

/**
 * @class CVoodooParameter
 * 
 * @par Interface
 *    @ref IVoodooParameter
 * @par IID/CLSID
 *    @ref CLSID_VoodooParameter
 * @par GUID
 *    {1d3d7fa7-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    noncreatable
 * @par Version
 *    0.3
 *    
 * @brief 
 *    Data container for shader inputs.
 */
class ATL_NO_VTABLE CVoodooParameter :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CVoodooParameter, &CLSID_VoodooParameter>,
    public IDispatchImpl<IVoodooParameter, &IID_IVoodooParameter, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
    CVoodooParameter();
    virtual ~CVoodooParameter();
    static IVoodooParameter * Create(IVoodooCore * pCore, BSTR pName, ParameterType Type);
    static IVoodooParameter * Create(IVoodooShader * pShader, CGparameter Parameter);

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPARAMETER)

DECLARE_NOT_AGGREGATABLE(CVoodooParameter)

BEGIN_COM_MAP(CVoodooParameter)
    COM_INTERFACE_ENTRY(IVoodooParameter)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooParameter
public:
    virtual HRESULT STDMETHODCALLTYPE get_Name(LPBSTR pName);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    virtual HRESULT STDMETHODCALLTYPE get_Type(DWORD *pType);
    virtual HRESULT STDMETHODCALLTYPE get_Virtual(VARIANT_BOOL *pVirtual);
    virtual HRESULT STDMETHODCALLTYPE AttachParameter(IVoodooParameter *pParameter);
    virtual HRESULT STDMETHODCALLTYPE get_Components(int *pComponents);
    virtual HRESULT STDMETHODCALLTYPE get_SamplerValue(IVoodooTexture **ppTexture);
    virtual HRESULT STDMETHODCALLTYPE put_SamplerValue(IVoodooTexture *pTexture);
    virtual HRESULT STDMETHODCALLTYPE get_ScalarValue(SAFEARRAY * *ppData);
    virtual HRESULT STDMETHODCALLTYPE put_ScalarValue(SAFEARRAY * pData);
    virtual HRESULT STDMETHODCALLTYPE get_Shader(IVoodooShader **ppShader);
    virtual HRESULT STDMETHODCALLTYPE get_CgParameter(VARIANT *ppCgParameter);

private:
    HRESULT STDMETHODCALLTYPE LinkParameter();
    HRESULT STDMETHODCALLTYPE LinkSampler();
    HRESULT STDMETHODCALLTYPE CreateSamplerTexture();

    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    IVoodooShader * m_Shader;
    ParameterType m_Type;
    VARIANT_BOOL m_Virtual;
    CComPtr<IVoodooTexture> m_Texture;
    CComSafeArray<float> m_Data;
    CGparameter m_Parameter;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooParameter), CVoodooParameter)
