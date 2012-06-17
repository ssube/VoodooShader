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

#include "VSEffectDX9.hpp"
// Voodoo D3D9
#include "VSBindingDX9.hpp"
#include "VSParameterDX9.hpp"
#include "VSTechniqueDX9.hpp"
#include "D3D9_Version.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        #define VOODOO_DEBUG_TYPE VSEffectDX9
        DeclareDebugCache();

        VSEffectDX9::VSEffectDX9(_In_ VSBindingDX9 * pBinding, LPD3DXEFFECT pEffect) :
            m_Refs(0), m_Binding(pBinding), m_DXEffect(pEffect)
        {
            if (!m_Binding)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create effect with no binding."), nullptr);
            }

            m_Core = m_Binding->m_Core;
            if (!m_Core)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create effect from binding with no core."), nullptr);
            }

            // Cache core objects
            LoggerRef logger  = m_Core->GetLogger();

            if (!logger)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create effect without logger."), m_Core);
            }

            // Get desc
            D3DXEFFECT_DESC desc;
            ZeroMemory(&desc, sizeof(D3DXEFFECT_DESC));
            if (FAILED(m_DXEffect->GetDesc(&desc)))
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Failed to retrieve effect description."), m_Core);
            }

            // Get parameters
            for (UINT paramIndex = 0; paramIndex < desc.Parameters; ++paramIndex)
            {
                D3DXHANDLE paramHandle = m_DXEffect->GetParameter(NULL, paramIndex);
                if (!paramHandle)
                {
                    break;
                }

                boost::intrusive_ptr<VSParameterDX9> parameter = new VSParameterDX9(this, paramHandle);
                m_Parameters.push_back(parameter);

                parameter->Link();
            }

            // Get techniques
            for (UINT techIndex = 0; techIndex < desc.Techniques; ++techIndex)
            {
                D3DXHANDLE techHandle = m_DXEffect->GetTechnique(techIndex++);
                if (!techHandle)
                {
                    break;
                }

                D3DXTECHNIQUE_DESC techdesc;
                ZeroMemory(&techdesc, sizeof(D3DXTECHNIQUE_DESC));
                m_DXEffect->GetTechniqueDesc(techHandle, &techdesc);
                String techName(techdesc.Name);

                try
                {
					VSTechniqueDX9Ref technique(new VSTechniqueDX9(this, techHandle));
                    m_Techniques.push_back(technique);
                    if (!m_DefaultTechnique)
                    {
                        m_DefaultTechnique = technique;
                    }
                }
                catch (Exception & exc)
                {
                    logger->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Failed to create technique %1%: %2%") << techName << exc.strwhat());
                }
            } 

            AddThisToDebugCache();
        }

        VSEffectDX9::~VSEffectDX9()
        {
            RemoveThisFromDebugCache();

            m_DefaultTechnique = nullptr;
            m_Techniques.clear();
            m_Parameters.clear();
        }

        uint32_t VOODOO_METHODTYPE VSEffectDX9::AddRef() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE VSEffectDX9::Release() CONST
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

        VOODOO_METHODDEF(VSEffectDX9::QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut)
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
                else if (refid == IID_IEffect)
                {
                    *ppOut = static_cast<IEffect*>(this);
                }
                else if (refid == CLSID_VSEffectDX9)
                {
                    *ppOut = static_cast<VSEffectDX9*>(this);
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

        String VOODOO_METHODTYPE VSEffectDX9::ToString() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return VSTR("VSEffectDX9()");
        }

        ICore * VOODOO_METHODTYPE VSEffectDX9::GetCore() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return m_Core;
        }

        String VOODOO_METHODTYPE VSEffectDX9::GetName() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            //return m_DXEffect->m_Name;
            return VSTR("Unnamed");
        }

        VoodooResult VOODOO_METHODTYPE VSEffectDX9::GetProperty(_In_ CONST Uuid propid, _Out_ Variant * pValue) CONST
        {
            if (!pValue) VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3DX9Effect)
            {
                pValue->Type = VSUT_PVoid;
                pValue->VPVoid = (PVOID)m_DXEffect;
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

        VoodooResult VOODOO_METHODTYPE VSEffectDX9::SetProperty(_In_ CONST Uuid propid, _In_ Variant * pValue)
        {
            if (propid == PropIds::D3DX9Effect)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Properties[propid] = (*pValue);
            return VSF_OK;
        }

        ITechnique * VOODOO_METHODTYPE VSEffectDX9::Bind(_In_ CONST bool clean)
        {
            if (m_Binding->m_BoundEffect && m_Binding->m_BoundEffect != this) return nullptr;

            UINT passes = 0;
            DWORD flags = 0;
            if (clean)
            {
                flags = D3DXFX_DONOTSAVESTATE;

                m_Binding->PushState();
                m_Binding->ResetState();
            }

            if (SUCCEEDED(m_DXEffect->Begin(&passes, flags)))
            {
                m_Binding->m_BoundEffect = this;
                return m_DefaultTechnique.get();
            }
            else
            {
                if (clean) m_Binding->PopState();

                return nullptr;
            }
        }

        VOODOO_METHODDEF(VSEffectDX9::Reset)()
        {
            if (m_Binding->m_BoundEffect != this) return VSFERR_INVALIDCALL;

            if (SUCCEEDED(m_DXEffect->End()))
            {
                m_Binding->PopState();
                m_Binding->m_BoundEffect = nullptr;
                return VSF_OK;
            }
            else
            {
                return VSF_FAIL;
            }
        }

        uint32_t VOODOO_METHODTYPE VSEffectDX9::GetTechniqueCount() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return m_Techniques.size();
        }

        ITechnique * VOODOO_METHODTYPE VSEffectDX9::GetTechnique(_In_ CONST uint32_t index) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            if (index < m_Techniques.size())
            {
                return m_Techniques[index].get();
            }
            else
            {
                return nullptr;
            }
        }

        ITechnique * VOODOO_METHODTYPE VSEffectDX9::GetTechniqueByName(_In_ CONST String & name) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            TechniqueVector::const_iterator tech = m_Techniques.begin();
            while (tech != m_Techniques.end())
            {
                if ((*tech)->GetName() == name) return (*tech).get();
            }
            return nullptr;
        }

        ITechnique * VOODOO_METHODTYPE VSEffectDX9::GetDefaultTechnique() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return m_DefaultTechnique.get();
        }

        VoodooResult VOODOO_METHODTYPE VSEffectDX9::SetDefaultTechnique(_In_ ITechnique * pTechnique)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pTechnique)
            {
                m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_D3D9_NAME, VSTR("Cannot set null technique as default."));
                return VSFERR_INVALIDPARAMS;
            }
            else if (pTechnique->GetEffect() != this)
            {
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_PlugError, VOODOO_D3D9_NAME,
                    StringFormat(VSTR("Technique %1% cannot be set as default (technique originated from another effect)."))
                    << pTechnique
                );
                return VSFERR_INVALIDPARAMS;
            }

            VSTechniqueDX9 * pTechDX = nullptr;
            if (FAILED(pTechnique->QueryInterface(CLSID_VSTechniqueDX9, (IObject**)&pTechDX)) || pTechDX == nullptr)
            {                
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_PlugError, VOODOO_D3D9_NAME,
                    StringFormat(VSTR("Technique %1% is not a valid technique from this binding.")) << pTechnique
                );
                return VSFERR_INVALIDPARAMS;
            }

            if (SUCCEEDED(m_DXEffect->SetTechnique(pTechDX->m_DXHandle)))
            {
                m_DefaultTechnique = pTechnique;
                return VSF_OK;
            }
            else
            {
                return VSFERR_APIERROR;
            }
        }

        uint32_t VOODOO_METHODTYPE VSEffectDX9::GetParameterCount() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Parameters.size();
        }

        IParameter * VOODOO_METHODTYPE VSEffectDX9::GetParameter(_In_ CONST uint32_t index) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (index < m_Parameters.size())
            {
                return m_Parameters[index].get();
            }
            else
            {
                return nullptr;
            }
        }

        IParameter * VOODOO_METHODTYPE VSEffectDX9::GetParameterByName(_In_ CONST String & name) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            ParameterVector::const_iterator param = m_Parameters.begin();
            while (param != m_Parameters.end())
            {
                if ((*param)->GetName() == name) return (*param).get();
            }
            return nullptr;
        }
    }
}
