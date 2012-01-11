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

#include "VSTechniqueDX9.hpp"
#include "VSEffectDX9.hpp"
#include "VSPassDX9.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSTechniqueDX9
    DeclareDebugCache();

    VSTechniqueDX9::VSTechniqueDX9(VSEffectDX9 * pEffect, LPD3DXEFFECT pDXEffect, D3DXHANDLE pTechHandle) :
        m_Refs(0), m_Effect(pEffect), m_DXEffect(pDXEffect), m_DXHandle(pTechHandle)
    {
        if (!m_Effect)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create technique with no effect."), nullptr);
        }

        m_Core = m_Effect->GetCore();

        if (!m_DXEffect)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create technique with no hardware effect."), nullptr);
        }
        else if (!m_DXHandle)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create technique with no hardware handle."), nullptr);
        }

        D3DXTECHNIQUE_DESC desc;
        ZeroMemory(&desc, sizeof(D3DXTECHNIQUE_DESC));
        if (FAILED(pDXEffect->GetTechniqueDesc(pTechHandle, &desc)))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to get technique description."), m_Core);
        }

        m_Name = desc.Name;

        if (FAILED(m_DXEffect->ValidateTechnique(m_DXHandle)))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to validate technique."), m_Core);
        }

        for (UINT passIndex = 0; passIndex < desc.Passes; ++passIndex)
        {
            D3DXHANDLE passHandle = m_DXEffect->GetPass(m_DXHandle, passIndex++);
            VSPassDX9 * pass = new VSPassDX9(this, m_DXEffect, passHandle);
            m_Passes.push_back(pass);
        }

        AddThisToDebugCache();
    }

    VSTechniqueDX9::~VSTechniqueDX9()
    {
        RemoveThisFromDebugCache();

        m_Passes.clear();
    }

    uint32_t VOODOO_METHODTYPE VSTechniqueDX9::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSTechniqueDX9::Release() CONST
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

    VoodooResult VOODOO_METHODTYPE VSTechniqueDX9::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const IObject ** ppOut) CONST
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
            else if (refid == IID_ITechnique)
            {
                *ppOut = static_cast<const ITechnique*>(this);
            }
            else if (refid == CLSID_VSTechniqueDX9)
            {
                *ppOut = static_cast<const VSTechniqueDX9*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return false;
            }

            (*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSTechniqueDX9::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return Format(VSTR("VSTechnique(%1%)")) << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSTechniqueDX9::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    String VOODOO_METHODTYPE VSTechniqueDX9::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Name;
    }

    VoodooResult VOODOO_METHODTYPE VSTechniqueDX9::GetProperty(const String & name, _In_ Variant * pValue) CONST
    {
        if (!pValue) VSFERR_INVALIDPARAMS;

        if (name.Compare(VSTR("D3DX9HANDLE")))
        {
            pValue->Type = UT_PVoid;
            pValue->VPVoid = (PVOID)m_DXHandle;
            return VSF_OK;
        } 
        else if (name.Compare(VSTR("D3DX9EFFECT")))
        {
            pValue->Type = UT_PVoid;
            pValue->VPVoid = (PVOID)m_DXEffect;
            return VSF_OK;
        }
        else
        {
            VariantMap::const_iterator property = m_Properties.find(name);
            if (property != m_Properties.end())
            {
                CopyMemory(pValue, &property->second, sizeof(Variant));
                return VSF_OK;
            }
        }

        return VSFERR_INVALIDCALL;
    }

    VoodooResult VOODOO_METHODTYPE VSTechniqueDX9::SetProperty(const String & name, const Variant & value)
    {
        if (name.Compare(VSTR("D3DX9HANDLE")) || name.Compare(VSTR("D3DX9EFFECT")))
        {
            return VSFERR_INVALIDPARAMS;
        }

        m_Properties[name] = value;
        return VSF_OK;
    }

    IPass * VOODOO_METHODTYPE VSTechniqueDX9::GetPass(const uint32_t index) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (index < m_Passes.size())
        {
            return m_Passes[index].get();
        }
        else
        {
            return nullptr;
        }
    }

    uint32_t VOODOO_METHODTYPE VSTechniqueDX9::GetPassCount() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Passes.size();
    }

    IEffect * VOODOO_METHODTYPE VSTechniqueDX9::GetEffect() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Effect;
    }
}
