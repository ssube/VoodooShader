/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */

#include "VSTexture.hpp"

namespace VoodooShader
{
    VSTexture::VSTexture(ICore * pCore, String name, _In_ const TextureDesc * pDesc, void * pTexture) :
        m_Refs(0), m_Core(pCore), m_Name(name), m_Data(pTexture)
    {
        if (pDesc)
        {
            m_Desc = *pDesc;
        }
    }

    VSTexture::~VSTexture()
    { }

    uint32_t VOODOO_METHODCALLTYPE VSTexture::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODCALLTYPE VSTexture::Release() CONST
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VOODOO_METHODCALLTYPE VSTexture::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = IID_ITexture;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_ITexture) {
                *ppOut = static_cast<const ITexture*>(this);
            } else if (clsid == CLSID_VSTexture) {
                *ppOut = static_cast<const VSTexture*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODCALLTYPE VSTexture::ToString() CONST
    {
        return String::Format(L"VSTexture(%s)", m_Name.GetData());
    }

    ICore * VOODOO_METHODCALLTYPE VSTexture::GetCore() CONST
    {
        return m_Core;
    }

    String VOODOO_METHODCALLTYPE VSTexture::GetName() CONST
    {
        return m_Name;
    }

    void * VOODOO_METHODCALLTYPE VSTexture::GetData() CONST
    {
        return m_Data;
    }

    void * VOODOO_METHODCALLTYPE VSTexture::SetData(_In_ void * pData) 
    {
        void * oldData = m_Data;
        m_Data = pData;
        return oldData;
    }

    const TextureDesc * VOODOO_METHODCALLTYPE VSTexture::GetDesc() CONST
    {
        return &m_Desc;
    }
}
