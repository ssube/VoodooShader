#include "Shader.hpp"

#include "IAdapter.hpp"
#include "Converter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "IFilesystem.hpp"
#include "ILogger.hpp"
#include "Parameter.hpp"
#include "Technique.hpp"

namespace VoodooShader
{
    Shader::Shader(Core * pParent, String Path, const char ** ppArgs)
        : m_Name(Path), m_Core(pParent), m_DefaultTechnique(nullptr)
    {
        CGcontext context = m_Core->GetCgContext();

        if ( !context || !cgIsContext(context) )
        {
            throw std::exception("Unable to create parameter (core has no context).");
        }

        this->m_CgEffect = cgCreateEffectFromFile
        (
            context, 
            Path.c_str(), 
            ppArgs
        );

        if ( !cgIsEffect(this->m_CgEffect) )
        {
            throw std::exception("Failed to create shader.");
        } else {
            cgSetEffectName(this->m_CgEffect, this->m_Name.c_str());
        }
    }

    Shader::~Shader()
    {
        m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Destroying shader %s.", m_Name.c_str());

        m_DefaultTechnique = nullptr;
        m_Techniques.clear();
        m_Parameters.clear();

        if ( cgIsEffect(m_CgEffect) )
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
        if ( Index < m_Techniques.size() )
        {
            return m_Techniques[Index];
        } else {
            return nullptr;
        }
    }

    TechniqueRef Shader::GetDefaultTechnique()
    {
        return m_DefaultTechnique;
    }

    void Shader::SetDefaultTechnique(TechniqueRef Technique)
    {
        m_DefaultTechnique = Technique;
    }

    size_t Shader::GetParameterCount()
    {
        return m_Parameters.size();
    }

    ParameterRef Shader::GetParameter(_In_ size_t Index)
    {
        if ( Index < m_Parameters.size() )
        {
            return m_Parameters[Index];
        } else {
            return nullptr;
        }
    }

    void Shader::Link()
    {
        // Make sure it's a valid effect
        if ( !cgIsEffect(m_CgEffect) )
        {
            Throw(VOODOO_CORE_NAME, "Invalid effect.", m_Core);
        }

        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(this->m_CgEffect);

        while ( cgIsParameter(cParam) )
        {
            ParameterRef param(new Parameter(this, cParam));

            this->LinkParameter(param);

            this->m_Parameters.push_back(param);

            cParam = cgGetNextParameter(cParam);
        }

        this->SetupTechniques();
    }

    void Shader::LinkParameter(ParameterRef param)
    {
        // Cache basic data for future use 
        ParameterType type = param->GetType();
        CGparameter cgparam = param->GetCgParameter();

        // Check if it has a global link annotation
        CGannotation globalAnnotation = cgGetNamedParameterAnnotation(cgparam, "parameter");

        if ( cgIsAnnotation(globalAnnotation) && cgGetAnnotationType(globalAnnotation) == CG_STRING )
        {
            const char * globalName = cgGetStringAnnotationValue(globalAnnotation);
            if ( globalName != NULL && strlen(globalName) > 0 )
            {
                ParameterRef globalParam = m_Core->GetParameter(globalName, type);
                if ( globalParam.get() )
                {
                    globalParam->AttachParameter(param);
                } else {
                    m_Core->GetLogger()->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME, 
                        "Unable to find global param %s for parameter %s.",
                        globalName, param->GetName().c_str()
                    );
                }
            } else {
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
        if (  Converter::ToParameterCategory(type) == PC_Sampler )
        {
            this->LinkSampler(param);
        }
    }

    void Shader::LinkSampler(ParameterRef param)
    {
        CGparameter cgparam = param->GetCgParameter();

        // Link to a texture
        CGannotation textureAnnotation = cgGetNamedParameterAnnotation(cgparam, "texture");

        if ( !cgIsAnnotation(textureAnnotation) || cgGetAnnotationType(textureAnnotation) != CG_STRING )
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
        if ( textureName == NULL || strlen(textureName) == 0 )
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
                
        if ( texture.get() )
        {
            m_Core->GetAdapter()->ConnectTexture(param, texture);
        } else {
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

        if ( !cgIsParameter(parameter) )
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
        if ( !cgIsAnnotation(atexName) || cgGetAnnotationType(atexName) != CG_STRING )
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
        if ( cgGetAnnotationType(atexName) != CG_STRING )
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture name annotation type in %s.",
                param->GetName().c_str()
            );

            return;
        } else {
            texName = cgGetStringAnnotationValue(atexName);
            texName = m_Core->GetParser()->Parse(texName);
        }

        // Check for a valid texture file
        IImageRef textureImage = nullptr;
        IFileRef texFile = m_Core->GetFileSystem()->FindFile(texName);
        if ( texFile.get() )
        {
            textureImage = texFile->OpenImage();

            if ( textureImage.get() )
            {
                TextureRef tex = m_Core->GetAdapter()->CreateTexture(texName, textureImage);
                param->Set(tex);
                return;
            }
        }

        // No file, make blank texture
        CGannotation atexSize   = cgGetNamedParameterAnnotation(parameter, "size");
        CGannotation atexFormat = cgGetNamedParameterAnnotation(parameter, "format");
        CGannotation atexColor  = cgGetNamedParameterAnnotation(parameter, "color");

        CGtype texSizeType = cgGetAnnotationType(atexSize);
        TextureDesc texDesc;

        if ( texSizeType ==  CG_INT1 )
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
        } else if ( texSizeType == CG_INT2 ) {
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
        } else if ( texSizeType == CG_INT3 ) {
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
        } else if ( texSizeType == CG_INT4 ) {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "4-dimensional texture size found. Creating quantum texture... (invalid type on %s).",
                param->GetName().c_str()
            );        
        } else {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture size annotation type for parameter %s.",
                param->GetName().c_str()
            );
        }

        texDesc.Mipmaps = false;

        if ( cgGetAnnotationType(atexFormat) != CG_STRING )
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture format annotation type in %s.",
                param->GetName().c_str()
            );

            return;
        } else {
            const char * formatString = cgGetStringAnnotationValue(atexFormat);
            texDesc.Format = Converter::ToTextureFormat(formatString);
        }

        IAdapterRef adapter = m_Core->GetAdapter();
        TextureRef texture = adapter->CreateTexture(texName, texDesc);
        param->Set(texture);
    }


    void Shader::SetupTechniques()
    {
        CGtechnique cTech = cgGetFirstTechnique(m_CgEffect);
        while ( cgIsTechnique(cTech) )
        {
            CGbool valid = cgValidateTechnique(cTech);

            if ( valid == CG_TRUE )
            {
                m_Core->GetLogger()->Log
                (
                    LL_Debug,
                    VOODOO_CORE_NAME,
                    "Validated technique %s.",
                    cgGetTechniqueName(cTech)
                );

                // Insert the technique into the map
                TechniqueRef tech(new Technique(this, cTech));

                const char * techName = cgGetTechniqueName(cTech);
                m_Techniques[techName] = tech;

                // The first valid technique is the default one
                if ( !m_DefaultTechnique.get() )
                {
                    this->m_DefaultTechnique = tech;
                }

                tech->Link();
            } else {
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

    void Shader::SetDefaultTechnique(TechniqueRef Technique)
    {
        if ( Technique.get() != nullptr )
        {
            if ( Technique->GetShader().get() == this )
            {
                m_DefaultTechnique = Technique;
            } else {
                m_Core->GetLogger()->Log
                (
                    LL_Error, VOODOO_CORE_NAME, 
                    "Technique %s cannot be set as default for shader %s (not technique for this shader).",
                    Technique->GetName(), this->GetName()
                );
            }
        }
    }

    TechniqueRef Shader::GetTechnique(size_t index)
    {
        if ( m_Techniques.size() < index )
        {
            return m_Techniques[index];
        } else {
            return nullptr;
        }
    }

    CGeffect Shader::GetCgEffect()
    {
        return m_CgEffect;
    }

    Technique::Technique(ShaderPtr parent, CGtechnique cgTech)
        : m_Parent(parent), m_Technique(cgTech)
    {
        this->m_Core = this->m_Parent->GetCore();

        const char * techName = cgGetTechniqueName(this->m_Technique);
        if ( techName )
        {
            this->m_Name = techName;
        } else {
            char nameBuffer[16];
            sprintf_s(nameBuffer, "tech_%p", m_Technique);
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
        String name = m_Parent->GetName();
        name += "::";
        name += m_Name;
        return name;
    };

    PassRef Technique::GetPass(size_t index)
    {
        if ( index < this->mPasses.size() )
        {
            return this->mPasses[index];
        } else {
            Throw(VOODOO_CORE_NAME, "Voodoo Core: Invalid pass index (> pass count).", mCore);
        }
    }

    TextureRef Technique::GetTarget()
    {
        return mTarget;
    }

    size_t Technique::GetPassCount()
    {
        return this->mPasses.size();
    }

    CGtechnique Technique::GetCgTechnique()
    {
        return mTechnique;
    }

    void Technique::Link()
    {
        this->mTarget = TextureRef();

        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(this->mTechnique, "target");

        if ( cgIsAnnotation(targetAnnotation) )
        {
            if ( cgGetAnnotationType(targetAnnotation) == CG_STRING )
            {
                const char * targetName = cgGetStringAnnotationValue(targetAnnotation);

                this->mTarget = mCore->GetTexture(targetName);

                if ( !this->mTarget.get() )
                {
                    mCore->GetLogger()->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Pass %s cannot find target %s.",
                        this->GetName().c_str(), targetName
                    );

                    this->mTarget = mCore->GetTexture(TT_ShaderTarget);
                }
            } else {
                mCore->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
                );

                this->mTarget = mCore->GetTexture(TT_ShaderTarget);
            }
        } else {
            mCore->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.",
                this->GetName().c_str()
            );

            this->mTarget = mCore->GetTexture(TT_ShaderTarget);
        }


        this->mPasses.clear();

        CGpass cPass = cgGetFirstPass(mTechnique);

        while ( cgIsPass(cPass) )
        {
            // Insert the pass into the vector
            PassRef pass(new Pass(this, cPass));

            mPasses.push_back(pass);

            pass->Link();

            cPass = cgGetNextPass(cPass);
        }
    }
}
