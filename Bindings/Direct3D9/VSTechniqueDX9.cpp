/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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
#include "D3D9_Version.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        #define VOODOO_DEBUG_TYPE VSTechniqueDX9
        DeclareDebugCache();

        VSTechniqueDX9::VSTechniqueDX9(_In_ VSEffectDX9 * pEffect, _In_ D3DXHANDLE pTechHandle) :
            m_Refs(0), m_Effect(pEffect), m_Handle(pTechHandle)
        {
            if (!m_Effect)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create technique with no effect."), nullptr);
            }

            m_Core = m_Effect->GetCore();

            if (!m_Handle)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create technique with no hardware handle."), nullptr);
            }

            D3DXTECHNIQUE_DESC desc;
            ZeroMemory(&desc, sizeof(D3DXTECHNIQUE_DESC));
            if (FAILED(m_Effect->m_Handle->GetTechniqueDesc(m_Handle, &desc)))
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to get technique description."), m_Core);
            }

            m_Name = desc.Name;

            if (FAILED(m_Effect->m_Handle->ValidateTechnique(m_Handle)))
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to validate technique."), m_Core);
            }

            for (UINT passIndex = 0; passIndex < desc.Passes; ++passIndex)
            {
                D3DXHANDLE passHandle = m_Effect->m_Handle->GetPass(m_Handle, passIndex);
                VSPassDX9Ref pass(new VSPassDX9(this, passIndex));
                m_Passes.push_back(pass);

                // Get the targets
                uint32_t targetIndex = 0;
                char targetName[] = "vs_target0";
                while (targetIndex < 4)
                {
                    char targetIndexChar = '0' + char(targetIndex);
                    targetName[9] = targetIndexChar;

                    D3DXHANDLE annotation = m_Effect->m_Handle->GetAnnotationByName(passHandle, targetName);
                    if (!annotation)
                    {
                        break;
                    }

                    LPCSTR annotationValue = NULL;
                    if (FAILED(m_Effect->m_Handle->GetString(annotation, &annotationValue)) || !annotationValue)
                    {
                        break;
                    }

                    pass->SetTarget(targetIndex, m_Core->GetTexture(annotationValue));

                    ++targetIndex;
                }
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

        VoodooResult VOODOO_METHODTYPE VSTechniqueDX9::QueryInterface(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut)
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
                else if (refid == IID_ITechnique)
                {
                    *ppOut = static_cast<ITechnique*>(this);
                }
                else if (refid == CLSID_VSTechniqueDX9)
                {
                    *ppOut = static_cast<VSTechniqueDX9*>(this);
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

        String VOODOO_METHODTYPE VSTechniqueDX9::ToString() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return StringFormat(VSTR("VSTechnique(%1%)")) << m_Name;
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

        VoodooResult VOODOO_METHODTYPE VSTechniqueDX9::GetProperty(_In_ CONST Uuid propid, _Out_ Variant * pValue) CONST
        {
            if (!pValue) VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3DX9Handle)
            {
                pValue->Type = VSUT_PVoid;
                pValue->VPVoid = (PVOID)m_Handle;
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
				else
				{
					*pValue = CreateVariant();
				}
            }

            return VSFERR_INVALIDCALL;
        }

        VoodooResult VOODOO_METHODTYPE VSTechniqueDX9::SetProperty(_In_ CONST Uuid propid, _In_ Variant * pValue)
        {
            if (propid == PropIds::D3DX9Handle || propid == PropIds::D3DX9Effect)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Properties[propid] = (*pValue);
            return VSF_OK;
        }

        IPass * VOODOO_METHODTYPE VSTechniqueDX9::GetPass(_In_ CONST uint32_t index) CONST
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
}
