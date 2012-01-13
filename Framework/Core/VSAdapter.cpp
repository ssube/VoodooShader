/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

#include "VSAdapter.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSAdapter
    DeclareDebugCache();

    VSAdapter::VSAdapter(ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { 
        AddThisToDebugCache();
    }

    VSAdapter::~VSAdapter()
    {
        RemoveThisFromDebugCache();
    }

    uint32_t VOODOO_METHODTYPE VSAdapter::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSAdapter::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return m_Refs;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const IObject ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            return VSFERR_INVALIDPARAMS;
        }
        else
        {
            if (refid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            }
            else if (refid == IID_IAdapter)
            {
                *ppOut = static_cast<const IAdapter*>(this);
            }
            else if (refid == CLSID_VSAdapter)
            {
                *ppOut = static_cast<const VSAdapter*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return VSFERR_INVALIDUUID;
            }

            (*ppOut)->AddRef();
            return VSF_OK;
        }
    }

    String VOODOO_METHODTYPE VSAdapter::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return VSTR("VSAdapter()");
    }

    ICore * VOODOO_METHODTYPE VSAdapter::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    String VOODOO_METHODTYPE VSAdapter::GetName() CONST
    {
        return VSTR("Null");
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::GetProperty(const Uuid propid, Variant * pValue) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(propid);
        UNREFERENCED_PARAMETER(pValue);

        return VSFERR_INVALIDPROPERTY;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::SetProperty(const Uuid propid, Variant * pValue)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(propid);
        UNREFERENCED_PARAMETER(pValue);

        return VSFERR_INVALIDCALL;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::SetEffect(IEffect * const pEffect)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pEffect);

        return VSFERR_INVALIDCALL;
    }

    IEffect * VOODOO_METHODTYPE VSAdapter::GetEffect() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return nullptr;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::ResetEffect()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSFERR_INVALIDCALL;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::SetPass(IPass * const pPass)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pPass);

        return VSFERR_INVALIDCALL;
    }

    IPass * VOODOO_METHODTYPE VSAdapter::GetPass() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return nullptr;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::ResetPass()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return VSFERR_INVALIDCALL;
    }

    ITexture * VOODOO_METHODTYPE VSAdapter::CreateTexture(const String & name, const TextureDesc pDesc)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(pDesc);

        return nullptr;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::LoadTexture(IImage * const pFile, const TextureRegion pRegion, ITexture * const pTexture)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pFile);
        UNREFERENCED_PARAMETER(pRegion);
        UNREFERENCED_PARAMETER(pTexture);

        return VSFERR_INVALIDCALL;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::BindTexture(IParameter * const pParam, ITexture * const pTexture)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pParam);
        UNREFERENCED_PARAMETER(pTexture);

        return VSFERR_INVALIDCALL;
    }

    VoodooResult VOODOO_METHODTYPE VSAdapter::DrawGeometry(const uint32_t offset, const uint32_t count, void * const pData, const VertexFlags flags)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(offset);
        UNREFERENCED_PARAMETER(count);
        UNREFERENCED_PARAMETER(pData);
        UNREFERENCED_PARAMETER(flags);

        return VSFERR_INVALIDCALL;
    }
}
