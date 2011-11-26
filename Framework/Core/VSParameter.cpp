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

#include "VSParameter.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "ILogger.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    VSParameter::VSParameter(_Pre_notnull_ ICore * const pCore, _In_ const String & name, _In_ const ParameterType type) :
         m_Refs(0), m_Core(pCore), m_Shader(nullptr), m_Virtual(true), m_Type(type)
    {
        m_Core->GetLogger()->Log
        (
            LL_CoreDebug,
            VOODOO_CORE_NAME,
            L"Creating a virtual parameter (%s, core %p) of type %s.",
            name.GetData(), m_Core, Converter::ToString(type)
        );

        CGcontext context = m_Core->GetCgContext();

        if (!context || !cgIsContext(context))
        {
            //! @todo Get rid of this throw
            Throw(VOODOO_CORE_NAME, L"Unable to create parameter (core has no context).", m_Core);
        }

        m_Param = cgCreateParameter(context, Converter::ToCGType(m_Type));

        memset(m_Valuefloat, 0, sizeof(float) * 16);
    }

    VSParameter::VSParameter(_In_ IShader * const pShader, _In_ CGparameter const pParam) :
        m_Refs(0), m_Core(pShader->GetCore()), m_Shader(pShader), m_Virtual(false), m_Param(pParam)
    {
        m_Type = Converter::ToParameterType(cgGetParameterType(m_Param));
        m_Name = m_Shader->GetName() + L"::" + cgGetParameterName(m_Param);

        memset(m_Valuefloat, 0, sizeof(float) * 16);
    }

    VOODOO_METHODCALLTYPE VSParameter::~VSParameter()
    {
        m_Core->GetLogger()->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Destroying parameter %s.", m_Name.GetData());

        if (m_Virtual && cgIsParameter(m_Param))
        {
            cgDestroyParameter(m_Param);
        }
    }

    uint32_t VOODOO_METHODCALLTYPE VSParameter::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODCALLTYPE VSParameter::Release() CONST
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VOODOO_METHODCALLTYPE VSParameter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSParameter;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IParameter) {
                *ppOut = static_cast<const IParameter*>(this);
            } else if (clsid == CLSID_VSParameter) {
                *ppOut = static_cast<const VSParameter*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODCALLTYPE VSParameter::ToString() CONST
    {
        return String::Format(L"VSParameter(%s)", m_Name.GetData());
    }

    ICore * VOODOO_METHODCALLTYPE VSParameter::GetCore() CONST
    {
        return m_Core;
    }

    String VOODOO_METHODCALLTYPE VSParameter::GetName() CONST
    {
        return m_Name;
    }

    ParameterType VOODOO_METHODCALLTYPE VSParameter::GetType() CONST
    {
        return m_Type;
    }

    bool VOODOO_METHODCALLTYPE VSParameter::IsVirtual() CONST
    {
        return m_Virtual;
    }

    bool VOODOO_METHODCALLTYPE VSParameter::AttachParameter(IParameter * const pParam)
    {
        if (!pParam)
        {
            return false;
        } else if (!m_Virtual)
        {
            m_Core->GetLogger()->Log(LL_CoreWarn, VOODOO_CORE_NAME, L"Cannot attach to a non-virtual parameter (%s to %s).", pParam->ToString().GetData(), this->ToString().GetData());
            return false;
        }

        cgConnectParameter(m_Param, pParam->GetCgParameter());

        return true;
    }

    bool VOODOO_METHODCALLTYPE VSParameter::DetachParameter()
    {
        cgDisconnectParameter(m_Param);

        return true;
    }

    const uint32_t VOODOO_METHODCALLTYPE VSParameter::GetComponents() CONST
    {
        return Converter::ToComponents(m_Type);
    }

    ITexture * VOODOO_METHODCALLTYPE VSParameter::GetTexture() CONST
    {
        return m_ValueTexture.get();
    }

    void VOODOO_METHODCALLTYPE VSParameter::SetTexture(ITexture * pTexture)
    {
        m_ValueTexture = pTexture;

        if (pTexture)
        {
            m_Core->GetAdapter()->ConnectTexture(this, pTexture);
        }
    }

    _Ret_count_c_(16) float * const VOODOO_METHODCALLTYPE VSParameter::GetScalar() 
    {
        return m_Valuefloat;
    }

    void VOODOO_METHODCALLTYPE VSParameter::SetScalar(const uint32_t count, float * pValues)
    {
        if (pValues && count > 0)
        {
            memcpy(m_Valuefloat, pValues, min(16, count) * sizeof(float));
        }
    }

    IShader * const VOODOO_METHODCALLTYPE VSParameter::GetShader() CONST
    {
        return m_Shader.get();
    }

    CGparameter VOODOO_METHODCALLTYPE VSParameter::GetCgParameter() CONST
    {
        return m_Param;
    }
}
