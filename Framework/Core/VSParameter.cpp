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

#include "VSParameter.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSParameter
    DeclareDebugCache();

    VSParameter::VSParameter(_Pre_notnull_ ICore * const pCore, _In_ const String & name, _In_ const ParameterType type) :
         m_Refs(0), m_Core(pCore), m_Shader(nullptr), m_Virtual(true), m_Type(type)
    {
        m_Core->GetLogger()->LogMessage
        (
            LL_CoreDebug, VOODOO_CORE_NAME,
            Format("Creating virtual parameter '%1%' from core %2% of type %3%.") << name << m_Core << type
        );

        CGcontext context = m_Core->GetCgContext();

        if (!context || !cgIsContext(context))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create parameter (core has no context)."), m_Core);
        }

        m_Param = cgCreateParameter(context, Converter::ToCGType(m_Type));

        ZeroMemory(m_Valuefloat, sizeof(float) * 16);

        AddThisToDebugCache();
    }

    VSParameter::VSParameter(_In_ IShader * const pShader, _In_ CGparameter const pParam) :
        m_Refs(0), m_Core(pShader->GetCore()), m_Shader(pShader), m_Virtual(false), m_Param(pParam)
    {
        m_Type = Converter::ToParameterType(cgGetParameterType(m_Param));
        m_Name = m_Shader->GetName() + VSTR(":") + cgGetParameterName(m_Param);

        memset(m_Valuefloat, 0, sizeof(float) * 16);

        AddThisToDebugCache();
    }

    VOODOO_METHODTYPE VSParameter::~VSParameter()
    {
        RemoveThisFromDebugCache();

        if (m_Virtual && cgIsParameter(m_Param))
        {
            cgDestroyParameter(m_Param);
        }
    }

    uint32_t VOODOO_METHODTYPE VSParameter::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSParameter::Release() CONST
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

    bool VOODOO_METHODTYPE VSParameter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSParameter;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            }
            else if (clsid == IID_IParameter)
            {
                *ppOut = static_cast<const IParameter*>(this);
            }
            else if (clsid == CLSID_VSParameter)
            {
                *ppOut = static_cast<const VSParameter*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSParameter::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return Format("VSParameter(%1%)") << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSParameter::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    String VOODOO_METHODTYPE VSParameter::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Name;
    }

    ParameterType VOODOO_METHODTYPE VSParameter::GetType() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Type;
    }

    bool VOODOO_METHODTYPE VSParameter::IsVirtual() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Virtual;
    }

    bool VOODOO_METHODTYPE VSParameter::AttachParameter(IParameter * const pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!pParam)
        {
            return false;
        }
        else if (!m_Virtual)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Cannot attach parameter '%1%' to non-virtual parameter '%2%'.") << pParam << this
            );
            return false;
        }

        cgConnectParameter(m_Param, pParam->GetCgParameter());

        return true;
    }

    bool VOODOO_METHODTYPE VSParameter::DetachParameter()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        cgDisconnectParameter(m_Param);

        return true;
    }

    uint32_t VOODOO_METHODTYPE VSParameter::GetComponents() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return Converter::ToComponents(m_Type);
    }

    ITexture * VOODOO_METHODTYPE VSParameter::GetTexture() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_ValueTexture.get();
    }

    void VOODOO_METHODTYPE VSParameter::SetTexture(ITexture * pTexture)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        m_ValueTexture = pTexture;

        if (pTexture)
        {
            m_Core->GetAdapter()->ConnectTexture(this, pTexture);
        }
    }

    _Ret_count_c_(16) float * const VOODOO_METHODTYPE VSParameter::GetScalar()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Valuefloat;
    }

    void VOODOO_METHODTYPE VSParameter::SetScalar(const uint32_t count, float * pValues)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (pValues && count > 0)
        {
            memcpy(m_Valuefloat, pValues, min(16, count) * sizeof(float));
        }
    }

    IShader * const VOODOO_METHODTYPE VSParameter::GetShader() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Shader;
    }

    CGparameter VOODOO_METHODTYPE VSParameter::GetCgParameter() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Param;
    }
}
