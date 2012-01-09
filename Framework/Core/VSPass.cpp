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

#include "VSProgram.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSPass
    DeclareDebugCache();

    VOODOO_METHODTYPE VSPass::VSPass(ITechnique * pTechnique, pugi::xml_node passNode) :
        m_Refs(0), m_Technique(pTechnique), m_Node(passNode)
    {
        if (!m_Technique)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Cannot create a pass with no parent technique."), nullptr);
        } 
        else if (!passNode) 
        {
            Throw(VOODOO_CORE_NAME, VSTR("Cannot create a pass with no definition node."), nullptr);
        }

        m_Core = m_Technique->GetCore();

        // Parse the XML
        pugi::xml_attribute xName = passNode.attribute(VSTR("name"));
        m_Name = xName.value();

        AddThisToDebugCache();
    }

    VOODOO_METHODTYPE VSPass::~VSPass()
    {
        RemoveThisFromDebugCache();

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

    VoodooResult VOODOO_METHODTYPE VSPass::QueryInterface(_In_ Uuid clsid, _Deref_out_opt_ const IObject ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            return VSFERROR_INVALIDPARAMS;
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

            (*ppOut)->AddRef();
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
        if (index < m_Targets.size())
        {
            return m_Targets[index].get();
        }
        else
        {
            return nullptr;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSPass::SetTarget(uint32_t index, ITexture * pTarget)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (index < m_Targets.size())
        {
            m_Targets[index] = pTarget;
            return true;
        }
        else
        {
            return false;
        }
    }

    IProgram * VOODOO_METHODTYPE VSPass::GetProgram(ProgramStage stage) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        switch (stage)
        {
        case PS_Vertex:
            return m_VertexShader.get();
        case PS_Pixel:
            return m_PixelShader.get();
        case PS_Geometry:
            return m_GeometryShader.get();
        case PS_Domain:
            return m_DomainShader.get();
        case PS_Hull:
            return m_HullShader.get();
        case PS_Compute:
            return m_ComputeShader.get();
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

    VoodooResult VOODOO_METHODTYPE VSPass::Compile(const CompileFlags flags)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        pugi::xml_node xTarget = m_Node.child(VSTR("Target"));
        while (xTarget)
        {
            String targetName = xTarget.value();
            if (!targetName.IsEmpty())
            {
                ITexture * pTarget = m_Core->GetTexture(targetName);
                if (pTarget)
                {
                    m_Targets.push_back(pTarget);
                }
                else
                {
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_CoreWarning, VOODOO_CORE_NAME,
                        Format(VSTR("%1% cannot find target '%2%'.")) << this << targetName 
                    );
                }
            }

            xTarget = xTarget.next_sibling();
        }

        for (ProgramStage stage = PS_Vertex; stage <= PS_Compute; stage = static_cast<ProgramStage>(stage + 1))
        {
            const wchar_t * name = Converter::ToString(stage);
            try
            {
                IProgramRef tempProgram = new VSProgram(this, m_Node.child(name));
                VoodooResult vr = tempProgram->Compile(flags);

                if (VSUCCESS(vr))
                {
                    switch (stage)
                    {
                    case PS_Vertex:
                        m_VertexShader = tempProgram; break;
                    case PS_Pixel:
                        m_PixelShader = tempProgram; break;
                    case PS_Geometry:
                        m_GeometryShader = tempProgram; break;
                    case PS_Domain:
                        m_DomainShader = tempProgram; break;
                    case PS_Hull:
                        m_HullShader = tempProgram; break;
                    case PS_Compute:
                        m_ComputeShader = tempProgram; break;
                    }
                }
            }
            catch (Exception & exc)
            {
                m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                    Format("Error compiling program: %1%") << exc.strwhat());
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
