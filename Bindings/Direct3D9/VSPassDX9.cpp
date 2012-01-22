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

#include "VSPassDX9.hpp"
// Voodoo D3D9
#include "VSTechniqueDX9.hpp"
#include "VSEffectDX9.hpp"
#include "VSBindingDX9.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        #define VOODOO_DEBUG_TYPE VSPassDX9
        DeclareDebugCache();

        VSPassDX9::VSPassDX9(VSTechniqueDX9 * pTechnique, UINT passId) :
            m_Refs(0), m_Technique(pTechnique),  m_DXPassId(passId)
        {
            if (!m_Technique)
            {
                Throw(VOODOO_CORE_NAME, VSTR("Cannot create a pass with no parent technique."), nullptr);
            } 

            m_Core = m_Technique->GetCore();

            AddThisToDebugCache();
        }

        VSPassDX9::~VSPassDX9()
        {
            RemoveThisFromDebugCache();
        }

        uint32_t VOODOO_METHODTYPE VSPassDX9::AddRef() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE VSPassDX9::Release() CONST
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

        VoodooResult VOODOO_METHODTYPE VSPassDX9::QueryInterface(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut)
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
                    *ppOut = static_cast<IObject*>(this);
                }
                else if (refid == IID_IPass)
                {
                    *ppOut = static_cast<IPass*>(this);
                }
                else if (refid == CLSID_VSPassDX9)
                {
                    *ppOut = static_cast<VSPassDX9*>(this);
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

        String VOODOO_METHODTYPE VSPassDX9::ToString() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return StringFormat(VSTR("VSPass(%1%)")) << m_Name;
        }

        ICore * VOODOO_METHODTYPE VSPassDX9::GetCore() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Core;
        }

        String VOODOO_METHODTYPE VSPassDX9::GetName() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Name;
        }

        VoodooResult VOODOO_METHODTYPE VSPassDX9::GetProperty(const Uuid propid, _In_ Variant * pValue) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pValue) VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3DX9PassId)
            {
                (*pValue) = CreateVariant(uint32_t(m_DXPassId));
                return VSF_OK;
            }
            else
            {
                PropertyMap::const_iterator property = m_Properties.find(propid);
                if (property != m_Properties.end())
                {
                    CopyMemory(pValue, &property->second, sizeof(Variant));
                    return VSF_OK;
                }
            }

            return VSFERR_INVALIDCALL;
        }

        VoodooResult VOODOO_METHODTYPE VSPassDX9::SetProperty(const Uuid propid, _In_ Variant * pValue)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (propid == PropIds::D3DX9Handle || propid == PropIds::D3DX9Effect || propid == PropIds::D3DX9PassId)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Properties[propid] = (*pValue);
            return VSF_OK;
        }

        VOODOO_METHODDEF(VSPassDX9::Bind)()
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            
            boost::intrusive_ptr<VSEffectDX9> effect = m_Technique->m_Effect;
            if (effect != effect->m_Binding->m_BoundEffect)
            {
                return VSFERR_INVALIDCALL;
            }
            else if (effect->m_Binding->m_BoundPass)
            {
                return VSFERR_INVALIDCALL;
            }

            if (SUCCEEDED(effect->m_DXEffect->BeginPass(m_DXPassId)))
            {
                effect->m_Binding->m_BoundPass = this;
                return VSF_OK;
            }
            else
            {
                return VSF_FAIL;
            }
        }

        VOODOO_METHODDEF(VSPassDX9::Reset)()
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            boost::intrusive_ptr<VSEffectDX9> effect = m_Technique->m_Effect;
            if (effect->m_Binding->m_BoundPass != this)
            {
                return VSFERR_INVALIDCALL;
            }

            if (SUCCEEDED(effect->m_DXEffect->EndPass()))
            {
                effect->m_Binding->m_BoundPass = nullptr;
                return VSF_OK;
            }
            else
            {
                return VSF_FAIL;
            }
        }

        ITexture * VOODOO_METHODTYPE VSPassDX9::GetTarget(const uint32_t index) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (index < 4)
            {
                return m_Targets[index].get();
            }
            else
            {
                return nullptr;
            }
        }

        VoodooResult VOODOO_METHODTYPE VSPassDX9::SetTarget(const uint32_t index, ITexture * pTarget)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (index < 4)
            {
                m_Targets[index] = pTarget;
                return VSF_OK;
            }
            else
            {
                return VSFERR_INVALIDPARAMS;
            }
        }

        VoodooResult VOODOO_METHODTYPE VSPassDX9::GetShader(const ShaderStage stage, _In_ Variant * pShader) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            UNREFERENCED_PARAMETER(stage);
            UNREFERENCED_PARAMETER(pShader);

            return VSFERR_NOTIMPLEMENTED;
        }

        ITechnique * VOODOO_METHODTYPE VSPassDX9::GetTechnique() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Technique;
        }
    }
}
