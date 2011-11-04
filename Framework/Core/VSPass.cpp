/**
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

#include "VSPass.hpp"

#include "Exception.hpp"
#include "Version.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "ILogger.hpp"
#include "ITechnique.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{
    VSPass::VSPass(ITechnique * pTechnique, CGpass pCgPass) :
        m_Refs(0), m_Technique(pTechnique), m_CgPass(pCgPass)
    {
        if (!m_Technique)
        {
            Throw(VOODOO_CORE_NAME, L"Cannot create a pass with no technique.", m_Core);
        }

        m_Core = m_Technique->GetCore();

        const char * passName = cgGetPassName(m_CgPass);

        m_Name = m_Technique->GetName() + L"::";
        if (passName)
        {
            m_Name += passName;
        } else {
            m_Name += String::Format(L"pass_%p", m_CgPass);
        }
    }

    VSPass::~VSPass(void)
    {
        if (m_Core)
        {
            IAdapter * pAdapter = m_Core->GetAdapter();
            if (pAdapter)
            {
                pAdapter->UnloadPass(this);
            }
        }

        m_Target = nullptr;
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

    bool VSPass::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSPass;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IPass) {
                *ppOut = static_cast<const IPass*>(this);
            } else if (clsid == CLSID_VSPass) {
                *ppOut = static_cast<const VSPass*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VSPass::ToString(void) const
    {
        return String::Format(L"VSPass(%s)", m_Name.GetData());
    }

    ICore * VSPass::GetCore(void) const
    {
        return m_Core;
    }

    String VSPass::GetName() const
    {
        return m_Name;
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
        m_VertexProgram = cgGetPassProgram(m_CgPass, CG_VERTEX_DOMAIN);
        m_FragmentProgram = cgGetPassProgram(m_CgPass, CG_FRAGMENT_DOMAIN);
        m_GeometryProgram = cgGetPassProgram(m_CgPass, CG_GEOMETRY_DOMAIN);
        m_DomainProgram = cgGetPassProgram(m_CgPass, CG_TESSELLATION_CONTROL_DOMAIN);
        m_HullProgram = cgGetPassProgram(m_CgPass, CG_TESSELLATION_EVALUATION_DOMAIN);

        m_Target = nullptr;

        CGannotation targetAnnotation = cgGetNamedPassAnnotation(m_CgPass, "target");

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
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        L"IPass %s cannot find target %s.",
                        this->ToString().GetData(),
                        targetName
                    );

                    m_Target = m_Core->GetStageTexture(TS_Pass);
                }
            } else {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    L"IPass %s has annotation \"target\" of invalid type.",
                    this->ToString().GetData()
                );

                m_Target = m_Core->GetStageTexture(TS_Pass);
            }
        } else {
            m_Core->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                L"IPass %s has no target annotation.",
                this->ToString().GetData()
            );

            m_Target = m_Core->GetStageTexture(TS_Pass);
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
