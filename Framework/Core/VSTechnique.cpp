/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
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

#include "VSTechnique.hpp"

#include "Exception.hpp"
#include "Version.hpp"

#include "VSPass.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "ILogger.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{
    VSTechnique::VSTechnique(IShader * pShader, CGtechnique pCgTech) :
        m_Shader(pShader), m_Core(pShader->GetCore()), m_CgTechnique(pCgTech), m_Target(nullptr)
    {
        const char * techName = cgGetTechniqueName(this->m_CgTechnique);

        this->m_Name = m_Shader->ToString() + L"::";
        if (techName)
        {
            this->m_Name += techName;
        } else {
            this->m_Name += String::Format(L"tech_%p", m_CgTechnique);
        }
    }

    VSTechnique::~VSTechnique(void)
    {
        m_Target = nullptr;
        m_Passes.clear();
    }

    uint32_t VSTechnique::AddRef() const
    {
        return ++m_Refs;
    }

    uint32_t VSTechnique::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    String VSTechnique::ToString(void) const
    {
        return String::Format(L"VSTechnique(%s)", m_Name.GetData());
    }

    ICore * VSTechnique::GetCore(void) const
    {
        return m_Core;
    }

    IPass * VSTechnique::GetPass(const uint32_t index) const
    {
        if (index < this->m_Passes.size())
        {
            return this->m_Passes[index].get();
        } else {
            return nullptr;
        }
    }

    ITexture * VSTechnique::GetTarget(void) const
    {
        return m_Target.get();
    }

    const uint32_t VSTechnique::GetPassCount(void) const
    {
        return this->m_Passes.size();
    }

    IShader * VSTechnique::GetShader(void) const
    {
        return m_Shader;
    }

    CGtechnique VSTechnique::GetCgTechnique(void) const
    {
        return m_CgTechnique;
    }

    void VSTechnique::Link()
    {
        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(this->m_CgTechnique, "target");

        if (cgIsAnnotation(targetAnnotation))
        {
            if (cgGetAnnotationType(targetAnnotation) == CG_STRING)
            {

                const char *targetName = cgGetStringAnnotationValue(targetAnnotation);

                this->m_Target = m_Core->GetTexture(targetName);

                if (!this->m_Target.get())
                {
                    m_Core->GetLogger()->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        L"IPass %s cannot find target %s.",
                        this->ToString().GetData(),
                        targetName
                    );

                    this->m_Target = m_Core->GetStageTexture(TS_Shader);
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    L"IPass %s has annotation \"target\" of invalid type.",
                    this->ToString().GetData()
                );

                this->m_Target = m_Core->GetStageTexture(TS_Shader);
            }
        }
        else
        {
            m_Core->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                L"IPass %s has no target annotation.",
                this->ToString().GetData()
            );

            this->m_Target = m_Core->GetStageTexture(TS_Shader);
        }

        this->m_Passes.clear();

        CGpass cPass = cgGetFirstPass(m_CgTechnique);

        while (cgIsPass(cPass))
        {
            // Insert the pass into the vector
            IPass * pass = new VSPass(this, cPass);

            m_Passes.push_back(pass);

            cPass = cgGetNextPass(cPass);
        }
    }
}
