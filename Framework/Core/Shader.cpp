#include "Shader.hpp"

#include "IAdapter.hpp"
#include "Converter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "ILogger.hpp"
#include "Parameter.hpp"
#include "Parser.hpp"
#include "Technique.hpp"

namespace VoodooShader
{
    ShaderRef Shader::Create(Core * parent, String Path, const char ** ppArgs /* = nullptr */)
    {
        ShaderRef shader;
        new Shader(shader, parent, Path, ppArgs);
        return shader;
    }

    Shader::Shader(ShaderRef self, Core * pParent, String Path, const char ** ppArgs)
            : m_Name(Path), m_Core(pParent), m_DefaultTechnique(nullptr)
    {
        self.reset(this);

        CGcontext context = m_Core->GetCgContext();

        if (!context || !cgIsContext(context))
        {
            throw std::exception("Unable to create parameter (core has no context).");
        }

        this->m_CgEffect = cgCreateEffectFromFile
                           (
                               context,
                               Path.c_str(),
                               ppArgs
                           );

        if (!cgIsEffect(this->m_CgEffect))
        {
            throw std::exception("Failed to create shader.");
        }
        else
        {
            cgSetEffectName(this->m_CgEffect, this->m_Name.c_str());
        }

        this->Link(self);
    }

    Shader::~Shader()
    {
        m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Destroying shader %s.", m_Name.c_str());

        m_DefaultTechnique = nullptr;
        m_Techniques.clear();
        m_Parameters.clear();

        if (cgIsEffect(m_CgEffect))
        {
            cgDestroyEffect(m_CgEffect);
        }
    }

    String Shader::GetName()
    {
        return m_Name;
    }

    Core * Shader::GetCore()
    {
        return m_Core;
    }

    size_t Shader::GetTechniqueCount()
    {
        return m_Techniques.size();
    }

    TechniqueRef Shader::GetTechnique(size_t Index)
    {
        if (Index < m_Techniques.size())
        {
            return m_Techniques[Index];
        }
        else
        {
            return nullptr;
        }
    }

    TechniqueRef Shader::GetDefaultTechnique()
    {
        return m_DefaultTechnique;
    }

    void Shader::SetDefaultTechnique(TechniqueRef Technique)
    {
        if (Technique.get() != nullptr)
        {
            if (Technique->GetShader().lock().get() == this)
            {
                m_DefaultTechnique = Technique;
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Error, VOODOO_CORE_NAME,
                    "Technique %s cannot be set as default for shader %s (not technique for this shader).",
                    Technique->GetName().c_str(), this->GetName().c_str()
                );
            }
        }
    }

    size_t Shader::GetParameterCount()
    {
        return m_Parameters.size();
    }

    ParameterRef Shader::GetParameter(_In_ size_t Index)
    {
        if (Index < m_Parameters.size())
        {
            return m_Parameters[Index];
        }
        else
        {
            return nullptr;
        }
    }

    void Shader::Link(ShaderRef self)
    {
        // Make sure it's a valid effect
        if (!cgIsEffect(m_CgEffect))
        {
            Throw(VOODOO_CORE_NAME, "Invalid effect.", m_Core);
        }

        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(this->m_CgEffect);

        while (cgIsParameter(cParam))
        {
            ParameterRef param(new Parameter(this, cParam));

            this->LinkParameter(param);

            this->m_Parameters.push_back(param);

            cParam = cgGetNextParameter(cParam);
        }

        this->SetupTechniques(self);
    }

    void Shader::LinkParameter(ParameterRef param)
    {
        // Cache basic data for future use
        ParameterType type = param->GetType();
        CGparameter cgparam = param->GetCgParameter();

        // Check if it has a global link annotation
        CGannotation globalAnnotation = cgGetNamedParameterAnnotation(cgparam, "parameter");

        if (cgIsAnnotation(globalAnnotation) && cgGetAnnotationType(globalAnnotation) == CG_STRING)
        {
            const char * globalName = cgGetStringAnnotationValue(globalAnnotation);
            if (globalName != NULL && strlen(globalName) > 0)
            {
                ParameterRef globalParam = m_Core->GetParameter(globalName, type);
                if (globalParam.get())
                {
                    globalParam->AttachParameter(param);
                }
                else
                {
                    m_Core->GetLogger()->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Unable to find global param %s for parameter %s.",
                        globalName, param->GetName().c_str()
                    );
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Unable to read global annotation for parameter %s.",
                    param->GetName().c_str()
                );
            }

            return;
        }

        // If it's not linked to a global, it doesn't need linked unless it is
        // a sampler.
        if (Converter::ToParameterCategory(type) == PC_Sampler)
        {
            this->LinkSampler(param);
        }
    }

    void Shader::LinkSampler(ParameterRef param)
    {
        CGparameter cgparam = param->GetCgParameter();

        // Link to a texture
        CGannotation textureAnnotation = cgGetNamedParameterAnnotation(cgparam, "texture");

        if (!cgIsAnnotation(textureAnnotation) || cgGetAnnotationType(textureAnnotation) != CG_STRING)
        {
            m_Core->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Could not retrieve texture annotation for parameter %s.",
                param->GetName().c_str()
            );

            return;
        }

        const char * textureName = cgGetStringAnnotationValue(textureAnnotation);
        if (textureName == NULL || strlen(textureName) == 0)
        {
            m_Core->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Could not retrieve texture name for parameter %s.",
                param->GetName().c_str()
            );

            return;
        }

        // Try to get the texture first, otherwise pass to adapter
        TextureRef texture = m_Core->GetTexture(textureName);

        if (texture.get())
        {
            m_Core->GetAdapter()->ConnectTexture(param, texture);
        }
        else
        {
            m_Core->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Could not find texture %s for parameter %s, attempting to load.",
                textureName, param->GetName().c_str()
            );

            this->CreateParameterTexture(param);
        }
    }

    void Shader::CreateParameterTexture(_In_ ParameterRef param)
    {
        CGparameter parameter = param->GetCgParameter();

        if (!cgIsParameter(parameter))
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Could not create parameter texture for unknown parameter."
            );

            return;
        }

        CGannotation atexName = cgGetNamedParameterAnnotation(parameter, "texture");
        if (!cgIsAnnotation(atexName) || cgGetAnnotationType(atexName) != CG_STRING)
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid or missing texture name for parameter %s.",
                param->GetName().c_str()
            );

            return;
        }

        String texName;
        if (cgGetAnnotationType(atexName) != CG_STRING)
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture name annotation type in %s.",
                param->GetName().c_str()
            );

            return;
        }
        else
        {
            texName = cgGetStringAnnotationValue(atexName);
            texName = m_Core->GetParser()->Parse(texName);
        }

        // Check for a valid texture file
        TextureRegion texRegion;
        /**
         * @todo Load texture region info from the annotations.
         */

        TextureRef tex = m_Core->GetAdapter()->LoadTexture(texName, texRegion);
        if (tex.get())
        {
            param->SetTexture(tex);
            return;
        }

        // No file, make blank texture
        CGannotation atexSize   = cgGetNamedParameterAnnotation(parameter, "size");
        CGannotation atexFormat = cgGetNamedParameterAnnotation(parameter, "format");
        //CGannotation atexColor  = cgGetNamedParameterAnnotation(parameter, "color");

        CGtype texSizeType = cgGetAnnotationType(atexSize);
        TextureDesc texDesc;

        if (texSizeType ==  CG_INT1)
        {
            m_Core->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "1-dimensional texture size found."
            );

            int outCount;
            const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
            texDesc.Width = texDims[0];
            texDesc.Height = texDesc.Depth = 1;
        }
        else if (texSizeType == CG_INT2)
        {
            m_Core->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "2-dimensional texture size found."
            );

            int outCount;
            const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
            texDesc.Width = texDims[0];
            texDesc.Height = texDims[1];
            texDesc.Depth = 1;
        }
        else if (texSizeType == CG_INT3)
        {
            m_Core->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "3-dimensional texture size found."
            );

            int outCount;
            const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
            texDesc.Width = texDims[0];
            texDesc.Height = texDims[1];
            texDesc.Depth = texDims[2];
        }
        else if (texSizeType == CG_INT4)
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "4-dimensional texture size found. Creating quantum texture... (invalid type on %s).",
                param->GetName().c_str()
            );
        }
        else
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture size annotation type for parameter %s.",
                param->GetName().c_str()
            );
        }

        texDesc.Mipmaps = false;

        if (cgGetAnnotationType(atexFormat) != CG_STRING)
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture format annotation type in %s.",
                param->GetName().c_str()
            );

            return;
        }
        else
        {
            const char * formatString = cgGetStringAnnotationValue(atexFormat);
            texDesc.Format = Converter::ToTextureFormat(formatString);
        }

        IAdapterRef adapter = m_Core->GetAdapter();
        TextureRef texture = adapter->CreateTexture(texName, texDesc);
        param->SetTexture(texture);
    }


    void Shader::SetupTechniques(ShaderRef self)
    {
        CGtechnique cTech = cgGetFirstTechnique(m_CgEffect);
        while (cgIsTechnique(cTech))
        {
            CGbool valid = cgValidateTechnique(cTech);

            if (valid == CG_TRUE)
            {
                m_Core->GetLogger()->Log
                (
                    LL_Debug,
                    VOODOO_CORE_NAME,
                    "Validated technique %s.",
                    cgGetTechniqueName(cTech)
                );

                // Insert the technique into the map
                TechniqueRef tech = Technique::Create(self, cTech);

                //std::string techName = cgGetTechniqueName(cTech);
                //m_Techniques[techName] = tech;
                m_Techniques.push_back(tech);

                // The first valid technique is the default one
                if (!m_DefaultTechnique.get())
                {
                    this->m_DefaultTechnique = tech;
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Technique failed to validate: %s.",
                    cgGetTechniqueName(cTech)
                );
            }

            cTech = cgGetNextTechnique(cTech);
        }
    }

    CGeffect Shader::GetCgEffect()
    {
        return m_CgEffect;
    }
}
