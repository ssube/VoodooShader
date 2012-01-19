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

#include "VSParameterDX9.hpp"
#include "VSTechniqueDX9.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        #define VOODOO_DEBUG_TYPE VSEffectDX9
        DeclareDebugCache();

        VSEffectDX9::VSEffectDX9(IFile * pFile) :
            m_Refs(0) 
        {
            if (!pFile)
            {
                Throw(VOODOO_CORE_NAME, VSTR("Unable to create effect without file."), nullptr);
            }

            m_Core = pFile->GetCore();

            // Cache core objects
            ILoggerRef  logger  = m_Core->GetLogger();

            if (!logger)
            {
                Throw(VOODOO_CORE_NAME, VSTR("Unable to create effect without logger."), m_Core);
            }

            // Get the D3D9 device from the adapter
            IDirect3DDevice9 * device = nullptr;
            Variant deviceVar;
            ZeroMemory(&deviceVar, sizeof(Variant));

            if (SUCCEEDED(adapter->GetProperty(PropIds::D3D9Device, &deviceVar)) && deviceVar.Type == UT_PVoid && deviceVar.VPVoid)
            {
                device = (IDirect3DDevice9*)deviceVar.VPVoid;
                device->AddRef();
            }
            else
            {
                Throw(VOODOO_CORE_NAME, VSTR("Unable to retrieve device from adapter."), m_Core);
            }

            // Build the flags
            DWORD d3dxflags = 0;
            if ((flags & CF_AvoidFlow) > 0)     d3dxflags |= D3DXSHADER_AVOID_FLOW_CONTROL;
            if ((flags & CF_PreferFlow) > 0)    d3dxflags |= D3DXSHADER_PREFER_FLOW_CONTROL;
            if ((flags & CF_Debug) > 0)         d3dxflags |= D3DXSHADER_DEBUG;
            if ((flags & CF_NoOpt) > 0)         d3dxflags |= D3DXSHADER_SKIPOPTIMIZATION;

            // Output/buffer
            LPD3DXBUFFER errors = NULL;

            HRESULT hr = D3DXCreateEffectFromFile(device, pFile->GetPath().GetData(), NULL, NULL, d3dxflags, NULL, &m_DXEffect, &errors);
            if (FAILED(hr))
            {
                logger->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    Format("Error compiling effect from file '%1%'. Errors:\n%2%") << 
                    pFile->GetPath() << (LPCSTR)errors->GetBufferPointer());

                Throw(VOODOO_CORE_NAME, VSTR("Unable to compile effect."), m_Core);
            }

            // Get desc
            D3DXEFFECT_DESC desc;
            ZeroMemory(&desc, sizeof(D3DXEFFECT_DESC));
            if (FAILED(m_DXEffect->GetDesc(&desc)))
            {
                Throw(VOODOO_CORE_NAME, VSTR("Failed to retrieve effect description."), m_Core);
            }

            // Get parameters
            for (UINT paramIndex = 0; paramIndex < desc.Parameters; ++paramIndex)
            {
                D3DXHANDLE paramHandle = m_DXEffect->GetParameter(NULL, paramIndex);
                if (!paramHandle)
                {
                    break;
                }

                boost::intrusive_ptr<VSParameterDX9> parameter = new VSParameterDX9(this, m_DXEffect, paramHandle);
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

                try
                {
                    VSTechniqueDX9 * technique = new VSTechniqueDX9(this, m_DXEffect, techHandle);
                    m_Techniques.push_back(technique);
                    if (!m_DefaultTechnique)
                    {
                        m_DefaultTechnique = technique;
                    }
                }
                catch (Exception & exc)
                {
                    D3DXTECHNIQUE_DESC techdesc;
                    if (SUCCEEDED(m_DXEffect->GetTechniqueDesc(techHandle, &techdesc)) && techdesc.Name)
                    {
                        logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, Format("Failed to create technique %1%: %2%") << techdesc.Name << exc.strwhat());
                    }
                    else
                    {
                        logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, Format("Failed to create technique %1%: %2%") << techIndex << exc.strwhat());
                    }
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

        VOODOO_METHODDEF(VSEffectDX9::QueryInterface)(_In_ Uuid refid, _Deref_out_opt_ CONST IObject ** ppOut)
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
                else if (refid == IID_IEffect)
                {
                    *ppOut = static_cast<const IEffect*>(this);
                }
                else if (refid == CLSID_VSEffectDX9)
                {
                    *ppOut = static_cast<const VSEffectDX9*>(this);
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
            return Format(VSTR("VSEffectDX9(%1%)")) << m_Name;
        }

        ICore * VOODOO_METHODTYPE VSEffectDX9::GetCore() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return m_Core;
        }

        String VOODOO_METHODTYPE VSEffectDX9::GetName() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return m_Name;
        }

        VoodooResult VOODOO_METHODTYPE VSEffectDX9::GetProperty(const Uuid propid, _In_ Variant * pValue) CONST
        {
            if (!pValue) VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3DX9Effect)
            {
                pValue->Type = UT_PVoid;
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
            }

            return VSFERR_INVALIDCALL;
        }

        VoodooResult VOODOO_METHODTYPE VSEffectDX9::SetProperty(const Uuid propid, Variant * pValue)
        {
            if (propid == PropIds::D3DX9Effect)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Properties[propid] = (*pValue);
            return VSF_OK;
        }

        uint32_t VOODOO_METHODTYPE VSEffectDX9::GetTechniqueCount() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return m_Techniques.size();
        }

        ITechnique * VOODOO_METHODTYPE VSEffectDX9::GetTechnique(const uint32_t index) CONST
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

        ITechnique * VOODOO_METHODTYPE VSEffectDX9::GetTechniqueByName(const String & name) CONST
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

        VoodooResult VOODOO_METHODTYPE VSEffectDX9::SetDefaultTechnique(ITechnique * pTechnique)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pTechnique)
            {
                m_Core->GetLogger()->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, VSTR("Cannot set null technique as default."));
                return VSFERR_INVALIDPARAMS;
            }
            else if (pTechnique->GetEffect() != this)
            {
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_CoreError, VOODOO_CORE_NAME,
                    Format(VSTR("Technique %1% cannot be set as default (technique originated from another effect)."))
                    << pTechnique
                );
                return VSFERR_INVALIDPARAMS;
            }

            m_DefaultTechnique = pTechnique;
            return VSF_OK;
        }

        uint32_t VOODOO_METHODTYPE VSEffectDX9::GetParameterCount() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Parameters.size();
        }

        IParameter * VOODOO_METHODTYPE VSEffectDX9::GetParameter(_In_ const uint32_t index) CONST
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

        IParameter * VOODOO_METHODTYPE VSEffectDX9::GetParameterByName(const String & name) CONST
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
