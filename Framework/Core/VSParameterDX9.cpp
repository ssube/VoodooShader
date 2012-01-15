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

#include "VSParameterDX9.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSParameterDX9
    DeclareDebugCache();

    VSParameterDX9::VSParameterDX9(_Pre_notnull_ IEffect * const pEffect, _In_ LPD3DXEFFECT pDXEffect = nullptr, _In_ D3DXHANDLE pParamHandle = nullptr) :
        m_Refs(0), m_Effect(pEffect), m_DXEffect(pDXEffect), m_DXHandle(pParamHandle)
    {
        if (!pEffect)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create parameter with no effect."), nullptr);
        }

        m_Core = m_Effect->GetCore();

        if (!m_DXEffect)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create parameter with no hardware effect."), m_Core);
        }
        else if (!m_DXHandle)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create parameter with no hardware handle."), m_Core);
        }

        D3DXPARAMETER_DESC desc;
        ZeroMemory(&desc, sizeof(D3DXPARAMETER_DESC));
        if (FAILED(m_DXEffect->GetParameterDesc(m_DXHandle, &desc)))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to retrieve parameter description."), m_Core);
        }

        m_Name = desc.Name;
        m_Desc.Type = (ParameterType)desc.Type;
        m_Desc.Columns = desc.Columns;
        m_Desc.Rows = desc.Rows;
        m_Desc.Elements = desc.Elements;

        AddThisToDebugCache();
    }

    VSParameterDX9::VSParameterDX9(_In_ ICore * pCore, const String & name, ParameterDesc desc) :
        m_Refs(0), m_Core(pCore), m_Effect(nullptr), m_Name(name), m_Desc(desc), m_DXEffect(nullptr), m_DXHandle(nullptr)
    {
        AddThisToDebugCache();
    }

    VSParameterDX9::~VSParameterDX9()
    {
        RemoveThisFromDebugCache();
    }

    uint32_t VOODOO_METHODTYPE VSParameterDX9::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSParameterDX9::Release() CONST
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

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const IObject ** ppOut) CONST
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
            else if (refid == IID_IParameter)
            {
                *ppOut = static_cast<const IParameter*>(this);
            }
            else if (refid == CLSID_VSParameterDX9)
            {
                *ppOut = static_cast<const VSParameterDX9*>(this);
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

    String VOODOO_METHODTYPE VSParameterDX9::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return Format("VSParameter(%1%)") << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSParameterDX9::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Core;
    }

    String VOODOO_METHODTYPE VSParameterDX9::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Name;
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::GetProperty(const Uuid propid, _In_ Variant * pValue) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pValue) VSFERR_INVALIDPARAMS;

        if (propid == PropIds::D3DX9Handle)
        {
            pValue->Type = UT_PVoid;
            pValue->VPVoid = (PVOID)m_DXHandle;
            return VSF_OK;
        } 
        else if (propid == PropIds::D3DX9Effect)
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

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::SetProperty(const Uuid propid, Variant * pValue)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (propid == PropIds::D3DX9Handle || propid == PropIds::D3DX9Effect)
        {
            return VSFERR_INVALIDPARAMS;
        }

        m_Properties[propid] = (*pValue);
        return VSF_OK;
    }

    VOODOO_METHODDEF_(ParameterDesc, VSParameterDX9::GetDesc)() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Desc;
    }

    VOODOO_METHODDEF(VSParameterDX9::GetBool)(bool * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;
        if (m_Desc.Type != PT_Bool) return VSFERR_INVALIDCALL;

        if (m_DXEffect && m_DXHandle)
        {
            BOOL rv = 0;
            if (SUCCEEDED(m_DXEffect->GetBool(m_DXHandle, &rv)))
            {
                m_VBool = (rv != 0);
            }
            else
            {
                return VSFERR_APIERROR;
            }
        }

        (*pVal) = m_VBool;

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::GetFloat)(float * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;
        if (m_Desc.Type != PT_Float) return VSFERR_INVALIDCALL;

        if (m_DXEffect && m_DXHandle)
        {
            float tVal;
            if (SUCCEEDED(m_DXEffect->GetFloat(m_DXHandle, &tVal)))
            {
                m_VFloat.X = tVal;
            }
            else
            {
                return VSFERR_APIERROR;
            }
        }
        
        (*pVal) = m_VFloat.X;

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::GetInt)(int32_t * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;
        if (m_Desc.Type != PT_Int) return VSFERR_INVALIDCALL;

        if (m_DXEffect && m_DXHandle && FAILED(m_DXEffect->GetInt(m_DXHandle, &m_VInt)))
        {
            return VSFERR_APIERROR;
        }

        (*pVal) = m_VInt;

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::GetString)(String * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;
        if (m_Desc.Type != PT_String) return VSFERR_INVALIDCALL;

        if (m_DXEffect && m_DXHandle)
        {
            LPCSTR rv = nullptr;
            if (SUCCEEDED(m_DXEffect->GetString(m_DXHandle, &rv)) && rv)
            {
                m_VString = String(rv);
            }
            else
            {
                return VSFERR_APIERROR;
            }
        }

        (*pVal) = m_VString;

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::GetTexture)(ITexture ** ppVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!ppVal) return VSFERR_INVALIDPARAMS;
        if (m_Desc.Type < PT_Texture || m_Desc.Type > PT_TextureCube) return VSFERR_INVALIDCALL;

        (*ppVal) = m_Texture.get();

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::GetVector)(Float4 * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;
        if (m_Desc.Type != PT_Float && m_Desc.Type != PT_Bool && m_Desc.Type != PT_Int) return VSFERR_INVALIDCALL;

        if (m_DXEffect && m_DXHandle)
        {
            D3DXVECTOR4 rv;
            if (SUCCEEDED(m_DXEffect->GetVector(m_DXHandle, &rv)))
            {
                m_VFloat.X = rv.x;
                m_VFloat.Y = rv.y;
                m_VFloat.Z = rv.z;
                m_VFloat.W = rv.w;
            }
            else
            {
                return VSFERR_APIERROR;
            }
        }

        (*pVal) = m_VFloat;

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::SetBool)(bool val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (m_Desc.Type != PT_Bool) return VSFERR_INVALIDCALL;

        m_VBool = val;

        ParameterList::iterator child = m_Attached.begin();
        while (child != m_Attached.end())
        {
            (*child)->SetBool(val);
        }

        if (m_DXEffect && m_DXHandle && FAILED(m_DXEffect->SetBool(m_DXHandle, val)))
        {
            return VSFERR_APIERROR;
        }

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::SetFloat)(float val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (m_Desc.Type != PT_Float) return VSFERR_INVALIDCALL;

        m_VFloat.X = val;

        ParameterList::iterator child = m_Attached.begin();
        while (child != m_Attached.end())
        {
            (*child)->SetFloat(val);
        }

        if (m_DXEffect && m_DXHandle && FAILED(m_DXEffect->SetFloat(m_DXHandle, val)))
        {
            return VSFERR_APIERROR;
        }

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::SetInt)(int32_t val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (m_Desc.Type != PT_Int) return VSFERR_INVALIDCALL;

        m_VInt = val;

        ParameterList::iterator child = m_Attached.begin();
        while (child != m_Attached.end())
        {
            (*child)->SetInt(val);
        }

        if (m_DXEffect && m_DXHandle && FAILED(m_DXEffect->SetInt(m_DXHandle, val)))
        {
            return VSFERR_APIERROR;
        }

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::SetString)(const String & val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (m_Desc.Type != PT_String) return VSFERR_INVALIDCALL;

        m_VString = val;

        ParameterList::iterator child = m_Attached.begin();
        while (child != m_Attached.end())
        {
            (*child)->SetString(val);
        }

        if (m_DXEffect && m_DXHandle)
        {
            std::string vstr = val.ToStringA();
            if (FAILED(m_DXEffect->SetString(m_DXHandle, vstr.c_str())))
            {
                return VSFERR_APIERROR;
            }
        }

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::SetTexture)(ITexture * pVal)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (m_Desc.Type < PT_Texture || m_Desc.Type > PT_TextureCube) return VSFERR_INVALIDCALL;

        m_Texture = pVal;

        ParameterList::iterator child = m_Attached.begin();
        while (child != m_Attached.end())
        {
            (*child)->SetTexture(pVal);
        }

        if (m_DXEffect && m_DXHandle)
        {
            return m_Core->GetAdapter()->BindTexture(this, pVal);
        }

        return VSF_OK;
    }

    VOODOO_METHODDEF(VSParameterDX9::SetVector)(Float4 val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (m_Desc.Type != PT_Float) return VSFERR_INVALIDCALL;

        m_VFloat = val;

        ParameterList::iterator child = m_Attached.begin();
        while (child != m_Attached.end())
        {
            (*child)->SetVector(val);
        }

        if (m_DXEffect && m_DXHandle)
        {
            D3DXVECTOR4 sv;
            sv.x = val.X;
            sv.y = val.Y;
            sv.z = val.Z;
            sv.w = val.W;

            if (FAILED(m_DXEffect->SetVector(m_DXHandle, &sv)))
            {
                return VSFERR_APIERROR;
            }
        }

        return VSF_OK;
    }

    bool VOODOO_METHODTYPE VSParameterDX9::IsVirtual() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return (m_DXEffect && m_DXHandle);
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::AttachParameter(IParameter * const pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pParam)
        {
            return VSFERR_INVALIDPARAMS;
        }

        m_Attached.push_back(pParam);

        return VSF_OK;
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::DetachParameter(IParameter * const pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        std::remove_if(m_Attached.begin(), m_Attached.end(), [&pParam](IParameterRef param){return param.get() == pParam;});

        return VSF_OK;
    }

    IEffect * VOODOO_METHODTYPE VSParameterDX9::GetEffect() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Effect;
    }

    void VOODOO_METHODTYPE VSParameterDX9::Link()
    {
        // Handle textures 
        if (m_Desc.Type >= PT_Texture && m_Desc.Type <= PT_TextureCube)
        {
            this->LinkNewTexture();

            D3DXHANDLE texAnnot = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texture");
            if (texAnnot)
            {
                LPCSTR texName = nullptr;
                if (SUCCEEDED(m_DXEffect->GetString(texAnnot, &texName)))
                {
                    m_Texture = m_Core->GetTexture(texName);
                    if (m_Texture)
                    {
                        m_Core->GetAdapter()->BindTexture(this, m_Texture.get());
                    }
                }
                else
                {
                    m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, Format("Texture parameter %1% has no source texture annotation.") << m_Name);
                }
            }
        }

        // Handle linkage
        D3DXHANDLE sourceAnnot = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_source");
        if (sourceAnnot)
        {
            LPCSTR sourceName = NULL;
            if (SUCCEEDED(m_DXEffect->GetString(sourceAnnot, &sourceName)))
            {
                IParameter * sourceParam = m_Core->GetParameter(sourceName, m_Desc);
                if (sourceParam)
                {
                    sourceParam->AttachParameter(this);
                }
            }
            else
            {
                m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                    Format("Unable to get source annotation for parameter %1%.") << m_Name);
            }
        }
    }

    void VOODOO_METHODTYPE VSParameterDX9::LinkNewTexture()
    {
        if (m_Desc.Type < PT_Texture || m_Desc.Type > PT_TextureCube)
        {
            return;
        }

        D3DXHANDLE newA = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texcreate");
        if (!newA)
        {
            return;
        }
        else
        {
            BOOL newFlag = FALSE;
            if (FAILED(m_DXEffect->GetBool(newA, &newFlag)) || newFlag != TRUE)
            {
                return;
            }
        }

        D3DXHANDLE nameA = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texture");
        D3DXHANDLE dimA  = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texdim");
        D3DXHANDLE mipA  = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texmip");
        D3DXHANDLE rttA  = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texrtt");
        D3DXHANDLE fmtA  = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texfmt");
        if (!nameA || !dimA || !fmtA)
        {
            m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Texture parameter %1% has texture create flag set but is missing required parameters.") << m_Name);
            return;
        }

        LPCSTR nameStr = NULL;
        if (FAILED(m_DXEffect->GetString(nameA, &nameStr)))
        {
            m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Unable to get texture name from parameter %1%.") << m_Name);
            return;
        }

        DECLARE_AND_ZERO(D3DXVECTOR4, dimVec);
        if (FAILED(m_DXEffect->GetVector(dimA, &dimVec)))
        {
            m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Unable to get texture dimensions from parameter %1%.") << m_Name);
            return;
        }

        UInt3 dim = {(uint32_t)dimVec.x, (uint32_t)dimVec.y, (uint32_t)dimVec.z};

        LPCSTR fmtStr = NULL;
        if (FAILED(m_DXEffect->GetString(fmtA, &fmtStr)))
        {
            m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Unable to get texture format from parameter %1%.") << m_Name);
            return;
        }

        TextureFormat fmt = Converter::ToTextureFormat(String(fmtStr).GetData());
        if (fmt == TF_Unknown)
        {
            m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Unknown texture format on parameter %1%.") << m_Name);
            return;
        }            

        BOOL mipB = FALSE;
        m_DXEffect->GetBool(mipA, &mipB);
        bool mip = (mipB == TRUE);

        BOOL rttB = TRUE;
        m_DXEffect->GetBool(rttA, &rttB);
        bool rtt = (rttB == TRUE);

        TextureDesc desc = {dim, mip, rtt, fmt};

        m_Core->GetLogger()->LogMessage(LL_CoreInfo, VOODOO_CORE_NAME, 
            Format("Creating texture %1% as %2% for parameter %3%.") << nameStr << desc << m_Name);

        ITexture * pTex = m_Core->CreateTexture(nameStr, desc);
        if (!pTex)
        {
            m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Unable to create texture for parameter %1%.") << m_Name);
            return;
        }
        else
        {
            m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Successfully created texture for parameter %1%.") << m_Name);
        }
    }
}
