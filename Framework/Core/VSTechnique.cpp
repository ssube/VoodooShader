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

#include "VSTechnique.hpp"

#include "VSPass.hpp"

namespace VoodooShader
{
    VOODOO_METHODTYPE VSTechnique::VSTechnique(IShader * pShader, CGtechnique pCgTech) :
        m_Refs(0), m_Shader(pShader), m_CgTechnique(pCgTech), m_Target(nullptr)
    {
        if (!pShader)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create technique with no shader."), nullptr);
        }
        else if (!cgIsTechnique(pCgTech))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create technique with no Cg technique."), pShader->GetCore());
        }

        m_Core = m_Shader->GetCore();

        const char * techName = cgGetTechniqueName(m_CgTechnique);

        m_Name = m_Shader->GetName() + VSTR(":");
        if (techName)
        {
            m_Name += techName;
        }
        else
        {
            m_Name += Format(VSTR("tech_%p")) << m_CgTechnique;
        }

        ++this->m_Refs;
        this->Link();
        --this->m_Refs;
    }

    VOODOO_METHODTYPE VSTechnique::~VSTechnique()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        m_Target = nullptr;
        m_Passes.clear();
    }

    uint32_t VOODOO_METHODTYPE VSTechnique::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSTechnique::Release() CONST
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

    bool VOODOO_METHODTYPE VSTechnique::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = IID_ITechnique;
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
            else if (clsid == IID_ITechnique)
            {
                *ppOut = static_cast<const ITechnique*>(this);
            }
            else if (clsid == CLSID_VSTechnique)
            {
                *ppOut = static_cast<const VSTechnique*>(this);
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

    String VOODOO_METHODTYPE VSTechnique::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return Format(VSTR("VSTechnique(%1%)")) << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSTechnique::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    String VOODOO_METHODTYPE VSTechnique::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Name;
    }

    IPass * VOODOO_METHODTYPE VSTechnique::GetPass(const uint32_t index) CONST
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

    ITexture * VOODOO_METHODTYPE VSTechnique::GetTarget() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Target.get();
    }

    uint32_t VOODOO_METHODTYPE VSTechnique::GetPassCount() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Passes.size();
    }

    IShader * VOODOO_METHODTYPE VSTechnique::GetShader() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Shader;
    }

    CGtechnique VOODOO_METHODTYPE VSTechnique::GetCgTechnique() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_CgTechnique;
    }

    void VSTechnique::Link()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(m_CgTechnique, "target");

        if (cgIsAnnotation(targetAnnotation))
        {
            if (cgGetAnnotationType(targetAnnotation) == CG_STRING)
            {

                const char *targetName = cgGetStringAnnotationValue(targetAnnotation);

                m_Target = m_Core->GetTexture(targetName);

                if (!m_Target)
                {
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_CoreWarning, VOODOO_CORE_NAME,
                        Format(VSTR("Technique %1% cannot find target %2%.")) << this << targetName
                    );

                    m_Target = m_Core->GetStageTexture(TS_Shader);
                }
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreWarning, VOODOO_CORE_NAME,
                    Format(VSTR("Technique %1% has target annotation of invalid type.")) << this
                );

                m_Target = m_Core->GetStageTexture(TS_Shader);
            }
        }
        else
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreDebug, VOODOO_CORE_NAME,
                Format(VSTR("Technique %1% has no target annotation.")) << this
            );

            m_Target = m_Core->GetStageTexture(TS_Shader);
        }

        m_Passes.clear();

        CGpass cPass = cgGetFirstPass(m_CgTechnique);

        while (cgIsPass(cPass))
        {
            // Insert the pass into the vector
            try
            {
                IPassRef pass = new VSPass(this, cPass);

                m_Passes.push_back(pass);
            }
            catch (const std::exception & exc)
            {
                const char * name = cgGetPassName(cPass);

                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    Format(VSTR("Error linking pass %1%: %2%")) << name << exc.what()
                );
            }

            cPass = cgGetNextPass(cPass);
        }
    }
}
