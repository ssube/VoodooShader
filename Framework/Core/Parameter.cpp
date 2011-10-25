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
#include "IParameter.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include "ICore.hpp"
#include "ILogger.hpp"
#include "IShader.hpp"

namespace VoodooShader
{
    IParameter::IParameter(_In_ ICore * pCore, _In_ String name, _In_ ParameterType type) : 
         m_Refs(0), m_Core(pCore), m_Name(name), m_Type(type), m_Shader(nullptr), m_Virtual(true)
    {
        m_Core->GetLogger()->Log
        (
            LL_Debug,
            VOODOO_CORE_NAME,
            "Creating a virtual parameter (%s, core %p) of type %s.",
            name.GetData(), m_Core, Converter::ToString(type)
        );

        CGcontext context = m_Core->GetCgContext();

        if (!context || !cgIsContext(context))
        {
            throw std::exception("Unable to create parameter (core has no context).");
        }

        m_Param = cgCreateParameter(context, Converter::ToCGType(m_Type));

        memset(m_Valuefloat, 0, sizeof(float) * 16);
    }

    IParameter::IParameter(_In_ IShader * pShader, _In_ CGparameter pParam) :
        m_Refs(0), m_Core(pShader->GetCore()), m_Shader(pShader), m_Param(pParam), m_Virtual(false)
    {
        m_Type = Converter::ToParameterType(cgGetParameterType(m_Param));
        m_Name = cgGetParameterName(m_Param);

        memset(m_Valuefloat, 0, sizeof(float) * 16);
    }

    IParameter::~IParameter(void)
    {
        m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Destroying parameter %s.", m_Name.GetData());

        if (m_Virtual && cgIsParameter(m_Param))
        {
            cgDestroyParameter(m_Param);
        }
    }

    int32_t IParameter::AddRef() const
    {
        return ++m_Refs;
    }

    int32_t IParameter::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    String IParameter::ToString(void) const
    {
        String name;

        if (m_Shader)
        {
            name = this->m_Shader->ToString();
        }

        name += ":" + m_Name;

        return name;
    }

    ICore * IParameter::GetCore(void) const
    {
        return m_Core;
    }

    ParameterType IParameter::GetType(void) const
    {
        return m_Type;
    }

    bool IParameter::IsVirtual(void) const
    {
        return m_Virtual;
    }

    bool IParameter::AttachParameter(IParameter * pParam)
    {
        if (!this->m_Virtual)
        {
            //Throw(VOODOO_CORE_NAME, "Cannot attach to a non-virtual parameter.", m_Core);
            return false;
        }

        cgConnectParameter(this->m_Param, pParam->GetCgParameter());
    }

    int IParameter::GetComponents(void) const
    {
        return Converter::ToComponents(m_Type);
    }

    ITexture * const IParameter::GetTexture(void) const
    {
        return m_ValueTexture.get();
    }

    void IParameter::SetTexture(ITexture* Texture)
    {
        m_ValueTexture = Texture;
    }

    float * const IParameter::GetScalar(void)
    {
        return m_Valuefloat;
    }

    void IParameter::SetScalar(int Count, _In_count_(Count) float *Values)
    {
        if (Values)
        {
            memcpy(m_Valuefloat, Values, min(16, Count) * sizeof(float));
        }
    }

    IShader * const IParameter::GetShader(void) const
    {
        return m_Shader.get();
    }

    CGparameter IParameter::GetCgParameter(void) const
    {
        return this->m_Param;
    }
}
