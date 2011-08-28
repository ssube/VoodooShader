#include "Pass.hpp"

#include "IAdapter.hpp"
#include "Core.hpp"
#include "ILogger.hpp"
#include "Technique.hpp"
#include "Texture.hpp"

namespace VoodooShader
{
    PassRef Pass::Create(TechniqueRef parent, CGpass cgPass)
    {
        PassRef pass;
        new Pass(pass, parent, cgPass);
        pass->Link();
        return pass;
    }

    Pass::Pass(PassRef & self, TechniqueRef parent, CGpass cgPass)
            : m_Technique(parent), m_CgPass(cgPass)
    {
        self.reset(this);

        this->m_Core = parent->GetCore();

        const char * passName = cgGetPassName(this->m_CgPass);
        if (passName)
        {
            this->m_Name = passName;
        }
        else
        {
            char nameBuffer[16];
            _itoa_s((int)(&this->m_CgPass), nameBuffer, 16, 16);

            this->m_Name = "pass_";
            this->m_Name += nameBuffer;
        }
    }

    Pass::~Pass()
    {
        m_Target = nullptr;

        m_Core->GetAdapter()->UnloadPass(this);
    }

    String Pass::GetName()
    {
        String name = m_Technique.lock()->GetName();
        name += "::";
        name += m_Name;
        return name;
    };

    Core * Pass::GetCore()
    {
        return m_Core;
    }

    TextureRef Pass::GetTarget()
    {
        return m_Target;
    }

    CGprogram Pass::GetProgram(ProgramStage stage)
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

    TechniquePtr Pass::GetTechnique()
    {
        return m_Technique;
    }

    CGpass Pass::GetCgPass()
    {
        return m_CgPass;
    }

    void Pass::Link()
    {
        this->m_VertexProgram   = cgGetPassProgram(this->m_CgPass, CG_VERTEX_DOMAIN);
        this->m_FragmentProgram = cgGetPassProgram(this->m_CgPass, CG_FRAGMENT_DOMAIN);
        this->m_GeometryProgram = cgGetPassProgram(this->m_CgPass, CG_GEOMETRY_DOMAIN);
        this->m_DomainProgram   = cgGetPassProgram(this->m_CgPass, CG_TESSELLATION_CONTROL_DOMAIN);
        this->m_HullProgram     = cgGetPassProgram(this->m_CgPass, CG_TESSELLATION_EVALUATION_DOMAIN);

        this->m_Target = nullptr;
        CGannotation targetAnnotation = cgGetNamedPassAnnotation(this->m_CgPass, "target");
        if (cgIsAnnotation(targetAnnotation))
        {
            if (cgGetAnnotationType(targetAnnotation) == CG_STRING)
            {
                const char * targetName = cgGetStringAnnotationValue(targetAnnotation);

                this->m_Target = m_Core->GetTexture(targetName);

                if (!this->m_Target.get())
                {
                    m_Core->GetLogger()->Log
                    (
                        LL_Warning, VOODOO_CORE_NAME, "Pass %s cannot find target %s.",
                        this->GetName().c_str(), targetName
                    );

                    this->m_Target = m_Core->GetStageTexture(TS_Pass);
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning, VOODOO_CORE_NAME, "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
                );

                this->m_Target = m_Core->GetStageTexture(TS_Pass);
            }
        }
        else
        {
            m_Core->GetLogger()->Log
            (
                LL_Debug, VOODOO_CORE_NAME, "Pass %s has no target annotation.",
                this->GetName().c_str()
            );

            this->m_Target = m_Core->GetStageTexture(TS_Pass);
        }

        // Load the programs
        IAdapterRef adapter = m_Core->GetAdapter();

        if (!adapter)
        {
            m_Core->GetLogger()->Log
            (
                LL_Warning, VOODOO_CORE_NAME, "No adapter found, pass %s must be explicitly loaded later.",
                this->GetName().c_str()
            );
        }
        else
        {
            if (!adapter->LoadPass(PassRef(this)))
            {
                m_Core->GetLogger()->Log
                (
                    LL_Error, VOODOO_CORE_NAME, "Failed to load pass %s.",
                    this->GetName().c_str()
                );
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Info, VOODOO_CORE_NAME, "Successfully loaded pass %s.",
                    this->GetName().c_str()
                );
            }
        }
    }
}
