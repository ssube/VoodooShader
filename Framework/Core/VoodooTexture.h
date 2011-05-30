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
 * @struct IVoodooTexture
 * 
 * @par IID/CLSID
 *    @ref IID_IVoodooTexture
 * @par GUID
 *    {1d3d7f8b-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    object, dual, nonextensible, pointer_default(unique)
 * 
 * @brief
 *    Represents a texture or surface used as a shaped buffer for color data. 
 *    
 * Voodoo textures MAY be of 1 to 3 dimensions (pixel strips, traditional textures and volume
 * textures). No support is available at this time for cube maps in the texture, but they can be
 * emulated in a shader or handled as flat textures. Mip-mapping and other filter modes are
 * handled internally by the Cg runtime.
 * 
 * A texture may not map precisely to an API texture object. Voodoo textures may, for example,
 * correspond to OpenGL textures, pixel buffers or render buffers, or DirectX textures, 
 * volume textures or surfaces. Some textures MAY be able to be used as render targets, and in
 * some cases the backbuffer MAY be represented as a texture. The precise mapping and properties 
 * are left to the adapter, and so texture objects MUST not be used between different adapters.
 **/

/**
 * @class CVoodooTexture
 * 
 * @par Interface
 *    @ref IVoodooTexture
 * @par IID/CLSID
 *    @ref CLSID_VoodooTexture
 * @par GUID
 *    {1d3d7fa8-6f32-11e0-8ac0-005056c00000}
 * @par Attributes
 *    noncreatable
 * @par Version
 *    0.3
 *    
 * Core implementation of the texture interface. API-neutral, provides standard extended data (when
 * provided by the API creator).
 */
class ATL_NO_VTABLE CVoodooTexture :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CVoodooTexture, &CLSID_VoodooTexture>,
    public IDispatchImpl<IVoodooTexture, &IID_IVoodooTexture, &LIBID_Voodoo_Core, /*wMajor =*/ 0, /*wMinor =*/ 3>
{
public:
    CVoodooTexture();
    virtual ~CVoodooTexture();
    static IVoodooTexture * Create(IVoodooCore * pCore, BSTR pName, TextureDesc Desc, VARIANT pData);

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOTEXTURE)

DECLARE_NOT_AGGREGATABLE(CVoodooTexture)

BEGIN_COM_MAP(CVoodooTexture)
    COM_INTERFACE_ENTRY(IVoodooTexture)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooTexture
public:
    /**
     * Get the name this texture was created with (used by the shader linker and most other 
     * references).
     * 
     * @param pName Returned string. This will be allocated by SysAllocString, copying the name.
     */
    virtual HRESULT STDMETHODCALLTYPE get_Name(LPBSTR pName);

    /**
     * Get the core this texture was created from.
     */
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);

    /**
     * Get the data stored in this texture. The variant's type depends on value given when the
     * texture was created (it is typically @tt VT_UNKNOWN for DirectX and @tt VT_UINT for OpenGL).
     */
    virtual HRESULT STDMETHODCALLTYPE get_Data(VARIANT *ppData);

    virtual HRESULT STDMETHODCALLTYPE get_Desc(TextureDesc *pDesc);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    TextureDesc m_Desc;
    VARIANT m_Data;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooTexture), CVoodooTexture)
