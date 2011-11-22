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

    VSParameter::~VSParameter(void)
    {
        m_Core->GetLogger()->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Destroying parameter %s.", m_Name.GetData());

        if (m_Virtual && cgIsParameter(m_Param))
        {
            cgDestroyParameter(m_Param);
        }
    }

    uint32_t VSParameter::AddRef() const
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VSParameter::Release() const
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VSParameter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
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

    String VSParameter::ToString(void) const
    {
        return String::Format(L"VSParameter(%s)", m_Name.GetData());
    }

    ICore * VSParameter::GetCore(void) const
    {
        return m_Core;
    }

    String VSParameter::GetName() const
    {
        return m_Name;
    }

    ParameterType VSParameter::GetType(void) const
    {
        return m_Type;
    }

    bool VSParameter::IsVirtual(void) const
    {
        return m_Virtual;
    }

    bool VSParameter::AttachParameter(IParameter * const pParam)
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

    bool VSParameter::DetachParameter()
    {
        cgDisconnectParameter(m_Param);

        return true;
    }

    const uint32_t VSParameter::GetComponents(void) const
    {
        return Converter::ToComponents(m_Type);
    }

    ITexture * VSParameter::GetTexture(void) const
    {
        return m_ValueTexture.get();
    }

    void VSParameter::SetTexture(ITexture * pTexture)
    {
        m_ValueTexture = pTexture;

        if (pTexture)
        {
            m_Core->GetAdapter()->ConnectTexture(this, pTexture);
        }
    }

    _Ret_count_c_(16) float * const VSParameter::GetScalar(void) throw()
    {
        return m_Valuefloat;
    }

    void VSParameter::SetScalar(const uint32_t count, float * pValues)
    {
        if (pValues && count > 0)
        {
            memcpy(m_Valuefloat, pValues, min(16, count) * sizeof(float));
        }
    }

    IShader * const VSParameter::GetShader(void) const
    {
        return m_Shader.get();
    }

    CGparameter VSParameter::GetCgParameter(void) const
    {
        return m_Param;
    }
}
