#include "Shader.hpp"

#include "IAdapter.hpp"
#include "Converter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "IFilesystem.hpp"
#include "ILogger.hpp"

namespace VoodooShader
{
    Shader::Shader(Core * parent, String filename, const char ** args)
        : mCore(parent), mName(filename), mDefaultTechnique()
    {
        CGcontext context = mCore->GetCgContext();

        if ( !context || !cgIsContext(context) )
        {
            throw std::exception("Unable to create parameter (core has no context).");
        }

        IFileRef fileref = mCore->GetFileSystem()->GetFile(filename);

        if ( !fileref.get() )
        {
            throw std::exception("Unable to find file.");
        }

        this->mEffect = cgCreateEffectFromFile
        (
            context, 
            fileref->GetPath().c_str(), 
            args
        );

        if ( !cgIsEffect(this->mEffect) )
        {
            throw std::exception("Failed to create shader.");
        } else {
            cgSetEffectName(this->mEffect, this->mName.c_str());
        }
    }

    Shader::~Shader()
    {
        mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Destroying shader %s.", mName.c_str());

        mDefaultTechnique = NULL;
        mTechniques.clear();
        mParameters.clear();

        if ( cgIsEffect(mEffect) )
        {
            cgDestroyEffect(mEffect);
        }
    }

    TechniqueRef Shader::GetDefaultTechnique()
    {
        return this->mDefaultTechnique;
    }

    size_t Shader::GetParameterCount()
    {
        return mParameters.size();
    }

    ParameterRef Shader::GetParameter(_In_ size_t index)
    {
        if ( index < mParameters.size() )
        {
            return mParameters[index];
        } else {
            return ParameterRef();
        }
    }

    ParameterRef Shader::GetParameter(_In_ String name)
    {
        String fullname = this->GetName() + ":" + name;

        for ( ParameterVector::iterator iparam = mParameters.begin(); iparam != mParameters.end(); ++iparam )
        {
            ParameterRef param = (*iparam);
            if ( param->GetName() == fullname )
            {
                return param;
            }
        }

        return ParameterRef();
    }

    String Shader::GetName()
    {
        return mName;
    }

    Core * Shader::GetCore()
    {
        return mCore;
    }

    void Shader::Link()
    {
        // Make sure it's a valid effect
        if ( !cgIsEffect(mEffect) )
        {
            Throw(VOODOO_CORE_NAME, "Invalid effect.", mCore);
        }

        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(this->mEffect);

        while ( cgIsParameter(cParam) )
        {
            ParameterRef param(new Parameter(this, cParam));

            this->LinkParameter(param);

            this->mParameters.push_back(param);

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
                ParameterRef globalParam = mCore->GetParameter(globalName, type);
                if ( globalParam.get() )
                {
                    globalParam->Attach(param);
                } else {
                    mCore->GetLogger()->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME, 
                        "Unable to find global param %s for parameter %s.",
                        globalName, param->GetName().c_str()
                    );
                }
            } else {
                mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Could not retrieve texture name for parameter %s.",
                param->GetName().c_str()
            );

            return;
        }

        // Try to get the texture first, otherwise pass to adapter
        TextureRef texture = mCore->GetTexture(textureName);
                
        if ( texture.get() )
        {
            mCore->GetAdapter()->ConnectTexture(param, texture);
        } else {
            mCore->GetLogger()->Log
            (
                LL_Warning,
                 VOODOO_CORE_NAME,
                "Could not find texture %s for parameter %s, attempting to load.",
                textureName, param->GetName().c_str()
            );

            this->CreateParameterTexture(param);
        }
    }

    void Shader::CreateParameterTexture( _In_ ParameterRef param )
    {
        CGparameter parameter = param->GetCgParameter();

        if ( !cgIsParameter(parameter) )
        {
            mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture name annotation type in %s.",
                param->GetName().c_str()
            );

            return;
        } else {
            texName = cgGetStringAnnotationValue(atexName);
            texName = mCore->GetParser()->Parse(texName);
        }

        // Check for a valid texture file
        IImageRef texFile = mCore->GetFileSystem()->GetFile(texName);
        if ( texFile.get() )
        {
            TextureRef tex = mCore->GetAdapter()->CreateTexture(texFile);
            param->Set(tex);
            return;
        }

        // No file, make blank texture
        CGannotation atexSize   = cgGetNamedParameterAnnotation(parameter, "size");
        CGannotation atexFormat = cgGetNamedParameterAnnotation(parameter, "format");
        CGannotation atexColor  = cgGetNamedParameterAnnotation(parameter, "color");

        CGtype texSizeType = cgGetAnnotationType(atexSize);
        TextureDesc texDesc;

        if ( texSizeType ==  CG_INT1 )
        {
            mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "4-dimensional texture size found. Creating quantum texture... (invalid type on %s).",
                param->GetName().c_str()
            );        
        } else {
            mCore->GetLogger()->Log
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
            mCore->GetLogger()->Log
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

        IAdapterRef adapter = mCore->GetAdapter();
        TextureRef texture = adapter->CreateTexture(texName, texDesc);
        param->Set(texture);
    }


    void Shader::SetupTechniques()
    {
        CGtechnique cTech = cgGetFirstTechnique(mEffect);
        while ( cgIsTechnique(cTech) )
        {
            CGbool valid = cgValidateTechnique(cTech);

            if ( valid == CG_TRUE )
            {
                mCore->GetLogger()->Log
                (
                    LL_Debug,
                    VOODOO_CORE_NAME,
                    "Validated technique %s.",
                    cgGetTechniqueName(cTech)
                );

                // Insert the technique into the map
                TechniqueRef tech(new Technique(this, cTech));

                const char * techName = cgGetTechniqueName(cTech);
                mTechniques[techName] = tech;

                // The first valid technique is the default one
                if ( !mDefaultTechnique.get() )
                {
                    this->mDefaultTechnique = tech;
                }

                tech->Link();
            } else {
                mCore->GetLogger()->Log
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

    void Shader::SetDefaultTechnique(String name)
    {
        TechniqueRef tech = this->GetTechnique(name);
        if ( tech.get() )
        {
            this->mDefaultTechnique = tech;
        } else {
            Throw(VOODOO_CORE_NAME, "Technique not found in shader.", mCore);
        }
    }

    TechniqueRef Shader::GetTechnique(String name)
    {
        TechniqueMap::iterator tech = mTechniques.find(name);
        if ( tech != mTechniques.end() )
        {
            return tech->second;
        } else {
            return TechniqueRef();
        }
    }

    CGeffect Shader::GetCgEffect()
    {
        return mEffect;
    }

    Technique::Technique(Shader * parent, CGtechnique cgTech)
        : mParent(parent), mTechnique(cgTech)
    {
        this->mCore = this->mParent->GetCore();

        const char * techName = cgGetTechniqueName(this->mTechnique);
        if ( techName )
        {
            this->mName = techName;
        } else {
            char nameBuffer[16];
            sprintf_s(nameBuffer, "tech_%p", mTechnique);
            this->mName = nameBuffer;
        }
    }

    Technique::~Technique()
    {
        mTarget = NULL;
        mPasses.clear();
    }

    Core * Technique::GetCore()
    {
        return mCore;
    }

    String Technique::GetName()
    {
        String name = mParent->GetName();
        name += "::";
        name += mName;
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

    Pass::Pass(Technique * parent, CGpass cgPass)
        : mParent(parent), mPass(cgPass)
    {
        this->mCore = this->mParent->GetCore();

        const char * passName = cgGetPassName(this->mPass);
        if ( passName )
        {
            this->mName = passName;
        } else {
            char nameBuffer[16];
            _itoa_s((int)(&this->mPass), nameBuffer, 16, 16);

            this->mName = "pass_";
            this->mName += nameBuffer;
        }
    }

    Pass::~Pass()
    {
        mTarget = NULL;

        mCore->GetAdapter()->UnloadPass(this);
    }

    String Pass::GetName()
    {
        String name = mParent->GetName();
        name += "::";
        name += mName;
        return name;
    };

    Core * Pass::GetCore()
    {
        return mCore;
    }

    TextureRef Pass::GetTarget()
    {
        return mTarget;
    }

    CGprogram Pass::GetProgram(ProgramStage stage)
    {
        switch ( stage )
        {
        case PS_Vertex:
            return mVertexProgram;
        case PS_Fragment:
            return mFragmentProgram;
        case PS_Geometry:
            return mGeometryProgram;
        case PS_Domain:
        case PS_Hull:
        case PS_Unknown:
        default:
            return NULL;
        }
    }

    CGpass Pass::GetCgPass()
    {
        return mPass;
    }

    void Pass::Link()
    {
        this->mVertexProgram   = cgGetPassProgram(this->mPass, CG_VERTEX_DOMAIN  );
        this->mFragmentProgram = cgGetPassProgram(this->mPass, CG_FRAGMENT_DOMAIN);
        this->mGeometryProgram = cgGetPassProgram(this->mPass, CG_GEOMETRY_DOMAIN);

        this->mTarget = TextureRef();
        CGannotation targetAnnotation = cgGetNamedPassAnnotation(this->mPass, "target");
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

                    this->mTarget = mCore->GetTexture(TT_PassTarget);
                }
            } else {
                mCore->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
                );

                this->mTarget = mCore->GetTexture(TT_PassTarget);
            }
        } else {
            mCore->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.", 
                this->GetName().c_str()
            );

            this->mTarget = mCore->GetTexture(TT_PassTarget);
        }

        // Load the programs
        IAdapterRef adapter = mCore->GetAdapter();

        if ( !adapter )
        {
            mCore->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "No adapter found, pass %s must be explicitly loaded later.", 
                this->GetName().c_str()
            );
        } else {
            if ( !adapter->LoadPass(this) )
            {
                mCore->GetLogger()->Log(LL_Error, VOODOO_CORE_NAME, "Failed to load pass %s.", this->GetName().c_str());
            } else {
                mCore->GetLogger()->Log(LL_Info, VOODOO_CORE_NAME, "Successfully loaded pass %s.", this->GetName().c_str());
            }
        }
    }
}
