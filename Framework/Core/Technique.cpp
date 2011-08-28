
#include "Technique.hpp"

#include "IAdapter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "ILogger.hpp"
#include "Pass.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace VoodooShader
{
    TechniqueRef Technique::Create(ShaderRef parent, CGtechnique cgTech)
    {
        TechniqueRef tech;
        new Technique(tech, parent, cgTech);
        tech->Link(tech);
        return tech;
    }

    Technique::Technique(TechniqueRef & self, ShaderRef parent, CGtechnique cgTech)
            : m_Shader(parent), m_Core(parent->GetCore()), m_CgTechnique(cgTech)
    {
        self.reset(this);

        const char * techName = cgGetTechniqueName(this->m_CgTechnique);
        if (techName)
        {
            this->m_Name = techName;
        }
        else
        {
            char nameBuffer[16];
            sprintf_s(nameBuffer, "tech_%p", m_CgTechnique);
            this->m_Name = nameBuffer;
        }
    }

    Technique::~Technique()
    {
        m_Target = NULL;
        m_Passes.clear();
    }

    Core * Technique::GetCore()
    {
        return m_Core;
    }

    String Technique::GetName()
    {
        String name = m_Shader.lock()->GetName();
        name += "::";
        name += m_Name;
        return name;
    };

    PassRef Technique::GetPass(size_t index)
    {
        if (index < this->m_Passes.size())
        {
            return this->m_Passes[index];
        }
        else
        {
            Throw(VOODOO_CORE_NAME, "Voodoo Core: Invalid pass index (> pass count).", m_Core);
        }
    }

    TextureRef Technique::GetTarget()
    {
        return m_Target;
    }

    size_t Technique::GetPassCount()
    {
        return this->m_Passes.size();
    }

    ShaderPtr Technique::GetShader()
    {
        return m_Shader;
    }

    CGtechnique Technique::GetCgTechnique()
    {
        return m_CgTechnique;
    }

    void Technique::Link(TechniqueRef self)
    {
        this->m_Target = TextureRef();

        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(this->m_CgTechnique, "target");

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
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Pass %s cannot find target %s.",
                        this->GetName().c_str(), targetName
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
                    "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
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
                "Pass %s has no target annotation.",
                this->GetName().c_str()
            );

            this->m_Target = m_Core->GetStageTexture(TS_Shader);
        }


        this->m_Passes.clear();

        CGpass cPass = cgGetFirstPass(m_CgTechnique);

        while (cgIsPass(cPass))
        {
            // Insert the pass into the vector
            PassRef pass(Pass::Create(self, cPass));

            m_Passes.push_back(pass);

            cPass = cgGetNextPass(cPass);
        }
    }
}
