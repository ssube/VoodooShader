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

#include "VSPass.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSPass
    DeclareDebugCache();

    VOODOO_METHODTYPE VSPass::VSPass(ITechnique * pTechnique, CGpass pCgPass) :
        m_Refs(0), m_Technique(pTechnique), m_CgPass(pCgPass)
    {
        if (!m_Technique)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Cannot create a pass with no parent technique."), nullptr);
        }
        else if (!cgIsPass(pCgPass))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Cannot create a pass with no Cg pass."), nullptr);
        }

        m_Core = m_Technique->GetCore();

        const char * passName = cgGetPassName(m_CgPass);

        m_Name = m_Technique->GetName() + VSTR(":");
        if (passName)
        {
            m_Name += passName;
        }
        else
        {
            m_Name += Format(VSTR("pass_%p")) << m_CgPass;
        }

        ++this->m_Refs;
        this->Link();
        --this->m_Refs;

        AddThisToDebugCache();
    }

    VOODOO_METHODTYPE VSPass::~VSPass()
    {
        RemoveThisFromDebugCache();

        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (m_Core)
        {
            IAdapter * pAdapter = m_Core->GetAdapter();
            if (pAdapter)
            {
                pAdapter->UnloadPass(this);
            }
        }
    }

    uint32_t VOODOO_METHODTYPE VSPass::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSPass::Release() CONST
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

    bool VOODOO_METHODTYPE VSPass::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSPass;
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
            else if (clsid == IID_IPass)
            {
                *ppOut = static_cast<const IPass*>(this);
            }
            else if (clsid == CLSID_VSPass)
            {
                *ppOut = static_cast<const VSPass*>(this);
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

    String VOODOO_METHODTYPE VSPass::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return Format(VSTR("VSPass(%1%)")) << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSPass::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    String VOODOO_METHODTYPE VSPass::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Name;
    }

    ITexture * VOODOO_METHODTYPE VSPass::GetTarget(uint32_t index) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (index < 4)
        {
            return m_Target[index].get();
        }
        else
        {
            return nullptr;
        }
    }

    bool VOODOO_METHODTYPE VSPass::SetTarget(uint32_t index, ITexture * pTarget)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (index < 4)
        {
            m_Target[index] = pTarget;
            return true;
        }
        else
        {
            return false;
        }
    }

    CGprogram VOODOO_METHODTYPE VSPass::GetProgram(ProgramStage stage) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
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

    ITechnique * VOODOO_METHODTYPE VSPass::GetTechnique() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Technique;
    }

    CGpass VOODOO_METHODTYPE VSPass::GetCgPass() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_CgPass;
    }

    void VSPass::Link()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        m_VertexProgram = cgGetPassProgram(m_CgPass, CG_VERTEX_DOMAIN);
        m_FragmentProgram = cgGetPassProgram(m_CgPass, CG_FRAGMENT_DOMAIN);
        m_GeometryProgram = cgGetPassProgram(m_CgPass, CG_GEOMETRY_DOMAIN);
        m_DomainProgram = cgGetPassProgram(m_CgPass, CG_TESSELLATION_CONTROL_DOMAIN);
        m_HullProgram = cgGetPassProgram(m_CgPass, CG_TESSELLATION_EVALUATION_DOMAIN);
        
        char targetAnnotationName[] = "target0";

        for (uint8_t i = 0; i < 4; ++i)
        {
            targetAnnotationName[6] = '0' + i;

            CGannotation targetAnnotation = cgGetNamedPassAnnotation(m_CgPass, targetAnnotationName);

            if (cgIsAnnotation(targetAnnotation))
            {
                if (cgGetAnnotationType(targetAnnotation) == CG_STRING)
                {
                    const char *targetName = cgGetStringAnnotationValue(targetAnnotation);

                    if (!(m_Target[i] = m_Core->GetTexture(targetName)))
                    {
                        m_Core->GetLogger()->LogMessage
                        (
                            LL_CoreWarning, VOODOO_CORE_NAME,
                            Format(VSTR("%1% cannot find target '%2%'.")) << this << targetName 
                        );
                    }
                }
                else
                {
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_CoreWarning, VOODOO_CORE_NAME,
                        Format(VSTR("%1% has target annotation of invalid type.")) << this
                    );
                }
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    Format(VSTR("%1% has no target annotation.")) << this
                );
            }
        }

        // Load the programs
        IAdapter * adapter = m_Core->GetAdapter();

        if (!adapter)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME,
                Format(VSTR("No adapter found, %1% must be explicitly loaded later.")) << this
            );
        }
        else
        {
            if (!adapter->LoadPass(this))
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreError, VOODOO_CORE_NAME,
                    Format(VSTR("Failed to load %1%.")) << this
                );
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreInfo, VOODOO_CORE_NAME,
                    Format(VSTR("Successfully loaded %1%.")) << this
                );
            }
        }
    }
}
