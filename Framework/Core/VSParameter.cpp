/**
 * This file is part of the Voodoo IShader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 */
#include "VSParameter.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include "ICore.hpp"
#include "ILogger.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{
    VSParameter::VSParameter(_In_ ICore * pCore, _In_ String name, _In_ ParameterType type) : 
         m_Refs(0), m_Core(pCore), m_Name(name), m_Type(type), m_Shader(nullptr), m_Virtual(true)
    {
        m_Core->GetLogger()->Log
        (
            LL_Debug,
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

    VSParameter::VSParameter(_In_ IShader * pShader, _In_ CGparameter pParam) :
        m_Refs(0), m_Core(pShader->GetCore()), m_Shader(pShader), m_Param(pParam), m_Virtual(false)
    {
        m_Type = Converter::ToParameterType(cgGetParameterType(m_Param));
        m_Name = cgGetParameterName(m_Param);

        memset(m_Valuefloat, 0, sizeof(float) * 16);
    }

    VSParameter::~VSParameter(void)
    {
        m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"Destroying parameter %s.", m_Name.GetData());

        if (m_Virtual && cgIsParameter(m_Param))
        {
            cgDestroyParameter(m_Param);
        }
    }

    uint32_t VSParameter::AddRef() const
    {
        return ++m_Refs;
    }

    uint32_t VSParameter::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    String VSParameter::ToString(void) const
    {
        String name;

        if (m_Shader)
        {
            name = this->m_Shader->ToString();
        }

        name += L":";
        name += m_Name;

        return name;
    }

    ICore * VSParameter::GetCore(void) const
    {
        return m_Core;
    }

    ParameterType VSParameter::GetType(void) const
    {
        return m_Type;
    }

    bool VSParameter::IsVirtual(void) const
    {
        return m_Virtual;
    }

    bool VSParameter::AttachParameter(IParameter * pParam)
    {
        if (!this->m_Virtual)
        {
            m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, L"Cannot attach to a non-virtual parameter (%s to %s).", pParam->ToString().GetData(), this->ToString().GetData());
            return false;
        }

        cgConnectParameter(this->m_Param, pParam->GetCgParameter());

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

    void VSParameter::SetTexture(ITexture* Texture)
    {
        m_ValueTexture = Texture;
    }

    float * const VSParameter::GetScalar(void)
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
        return this->m_Param;
    }
}
