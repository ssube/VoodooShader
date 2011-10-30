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

#include "VSPass.hpp"

#include "Version.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "ILogger.hpp"
#include "ITechnique.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{
    VSPass::VSPass(ITechnique * pTechnique, CGpass pCgPass) : 
        m_Technique(pTechnique), m_CgPass(pCgPass)
    {
        this->m_Core = m_Technique->GetCore();

        const char * passName = cgGetPassName(this->m_CgPass);

        if (passName)
        {
            this->m_Name = passName;
        } else {
            this->m_Name = String::Format(L"pass_%p", this->m_CgPass);
        }
    }

    /**
     *
     */
    VSPass::~VSPass(void)
    {
        m_Target = nullptr;

        // ! @todo Check if passes need to unload themselves from the adapter on dest. m_Core->GetAdapter()->UnloadPass(this);
    }

    uint32_t VSPass::AddRef() const
    {
        return ++m_Refs;
    }

    uint32_t VSPass::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    String VSPass::ToString(void) const
    {
        String name = m_Technique->ToString() + L"::" + m_Name;
        return name;
    };

    ICore * VSPass::GetCore(void) const
    {
        return m_Core;
    }

    ITexture * VSPass::GetTarget(void) const
    {
        return m_Target.get();
    }

    CGprogram VSPass::GetProgram(ProgramStage stage) const
    {
        switch (stage)
        {
        case PS_Vertex:
            return m_VertexProgram;
        case PS_Fragment:
            return m_FragmentProgram;
        case PS_Geometry:
            return m_GeometryProgram;
        case PS_Domain:
            return m_DomainProgram;
        case PS_Hull:
            return m_HullProgram;
        case PS_Unknown:
        default:
            return nullptr;
        }
    }

    ITechnique * VSPass::GetTechnique(void) const
    {
        return m_Technique;
    }

    CGpass VSPass::GetCgPass(void) const
    {
        return m_CgPass;
    }

    void VSPass::Link(void)
    {
        this->m_VertexProgram = cgGetPassProgram(this->m_CgPass, CG_VERTEX_DOMAIN);
        this->m_FragmentProgram = cgGetPassProgram(this->m_CgPass, CG_FRAGMENT_DOMAIN);
        this->m_GeometryProgram = cgGetPassProgram(this->m_CgPass, CG_GEOMETRY_DOMAIN);
        this->m_DomainProgram = cgGetPassProgram(this->m_CgPass, CG_TESSELLATION_CONTROL_DOMAIN);
        this->m_HullProgram = cgGetPassProgram(this->m_CgPass, CG_TESSELLATION_EVALUATION_DOMAIN);

        this->m_Target = nullptr;

        CGannotation targetAnnotation = cgGetNamedPassAnnotation(this->m_CgPass, "target");

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

                    this->m_Target = m_Core->GetStageTexture(TS_Pass);
                }
            } else {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    L"IPass %s has annotation \"target\" of invalid type.",
                    this->ToString().GetData()
                );

                this->m_Target = m_Core->GetStageTexture(TS_Pass);
            }
        } else {
            m_Core->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                L"IPass %s has no target annotation.",
                this->ToString().GetData()
            );

            this->m_Target = m_Core->GetStageTexture(TS_Pass);
        }


        // Load the programs
        IAdapter * adapter = m_Core->GetAdapter();

        if (!adapter)
        {
            m_Core->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                L"No adapter found, pass %s must be explicitly loaded later.",
                this->ToString().GetData()
            );
        }
        else
        {
            if (!adapter->LoadPass(this))
            {
                m_Core->GetLogger()->Log
                (
                    LL_Error,
                    VOODOO_CORE_NAME,
                    L"Failed to load pass %s.",
                    this->ToString().GetData()
                );
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Info,
                    VOODOO_CORE_NAME,
                    L"Successfully loaded pass %s.",
                    this->ToString().GetData()
                );
            }
        }
    }
}
