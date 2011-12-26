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

#include "VSTechnique.hpp"

#include "VSPass.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "ILogger.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"

#include "Exception.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    VOODOO_METHODTYPE VSTechnique::VSTechnique(IShader * pShader, CGtechnique pCgTech) :
        m_Refs(0), m_Shader(pShader), m_Core(pShader->GetCore()), m_CgTechnique(pCgTech), m_Target(nullptr)
    {
        const char * techName = cgGetTechniqueName(m_CgTechnique);

        m_Name = m_Shader->GetName() + VSTR("::");
        if (techName)
        {
            m_Name += techName;
        } else {
            m_Name += String::Format(VSTR("tech_%p"), m_CgTechnique);
        }

        ++this->m_Refs;
        this->Link();
        --this->m_Refs;
    }

    VOODOO_METHODTYPE VSTechnique::~VSTechnique()
    {
        m_Target = nullptr;
        m_Passes.clear();
    }

    uint32_t VOODOO_METHODTYPE VSTechnique::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSTechnique::Release() CONST
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VOODOO_METHODTYPE VSTechnique::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = IID_ITechnique;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_ITechnique) {
                *ppOut = static_cast<const ITechnique*>(this);
            } else if (clsid == CLSID_VSTechnique) {
                *ppOut = static_cast<const VSTechnique*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSTechnique::ToString() CONST
    {
        return String::Format(VSTR("VSTechnique(") VPFVSTR VSTR(")"), m_Name.GetData());
    }

    ICore * VOODOO_METHODTYPE VSTechnique::GetCore() CONST
    {
        return m_Core;
    }

    String VOODOO_METHODTYPE VSTechnique::GetName() CONST
    {
        return m_Name;
    }

    IPass * VOODOO_METHODTYPE VSTechnique::GetPass(const uint32_t index) CONST
    {
        if (index < m_Passes.size())
        {
            return m_Passes[index].get();
        } else {
            return nullptr;
        }
    }

    ITexture * VOODOO_METHODTYPE VSTechnique::GetTarget() CONST
    {
        return m_Target.get();
    }

    uint32_t VOODOO_METHODTYPE VSTechnique::GetPassCount() CONST
    {
        return m_Passes.size();
    }

    IShader * VOODOO_METHODTYPE VSTechnique::GetShader() CONST
    {
        return m_Shader;
    }

    CGtechnique VOODOO_METHODTYPE VSTechnique::GetCgTechnique() CONST
    {
        return m_CgTechnique;
    }

    void VSTechnique::Link()
    {
        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(m_CgTechnique, "target");

        if (cgIsAnnotation(targetAnnotation))
        {
            if (cgGetAnnotationType(targetAnnotation) == CG_STRING)
            {

                const char *targetName = cgGetStringAnnotationValue(targetAnnotation);

                m_Target = m_Core->GetTexture(targetName);

                if (!m_Target.get())
                {
                    m_Core->GetLogger()->Log
                    (
                        LL_CoreWarn,
                        VOODOO_CORE_NAME,
                        L"Technique %s cannot find target %S.",
                        this->ToString().GetData(),
                        targetName
                    );

                    m_Target = m_Core->GetStageTexture(TS_Shader);
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_CoreWarn,
                    VOODOO_CORE_NAME,
                    L"Technique %s has target annotation of invalid type.",
                    this->ToString().GetData()
                );

                m_Target = m_Core->GetStageTexture(TS_Shader);
            }
        }
        else
        {
            m_Core->GetLogger()->Log
            (
                LL_CoreDebug,
                VOODOO_CORE_NAME,
                L"Technique %s has no target annotation.",
                this->ToString().GetData()
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
            } catch (const std::exception & exc) {
                const char * name = cgGetPassName(cPass);

                m_Core->GetLogger()->Log
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    L"Error linking pass '%S': %S", name, exc.what()
                );
            }

            cPass = cgGetNextPass(cPass);
        }
    }
}
