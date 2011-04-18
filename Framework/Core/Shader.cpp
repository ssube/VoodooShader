#include "Shader.hpp"

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
        //mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Destroying shader %s.", mName.c_str());

        mDefaultTechnique = NULL;
        mTechniques.clear();
        mParameters.clear();

        if ( cgIsEffect(mEffect) )
        {
            cgDestroyEffect(mEffect);
        }
    }

    HRESULT Core::QueryInterface(REFIID iid, void ** pp)
    {
        if ( pp == NULL )
        {
            return E_POINTER;
        } else if ( iid == IID_IUnknown || iid == IID_VoodooObject || iid == IID_VoodooShader ) {
            *pp = this;
            return S_OK;
        } else {
            *pp = NULL;
            return E_NOINTERFACE;
        }
    }

    ULONG Core::AddRef()
    {
        return (++mRefrs);
    }

    ULONG Core::Release()
    {
        --mRefrs;
        if ( mRefrs == 0 )
        {
            delete this;
            return 0;
        } else {
            return mRefrs;
        }
    }

    HRESULT Core::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        } else {
            return mName.CopyTo(pName);
        }
    }

    HRESULT Core::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppCore = mCore;
            return S_OK;
        }
    }

    UINT get_TechniqueCount()
    {
        return mTechniques.size();
    }

    HRESULT GetTechnique(UINT Number, IVoodooTechnique ** ppTechnique)
    {
        if ( ppTechnique == NULL )
        {
            return E_INVALIDARG;
        } else if ( Number < mTechniques.size() ) {
            *ppTechnique = mTechniques[Number];
            return S_OK;
        } else {
            return E_INVALIDARG;
        }
    }

    HRESULT Shader::get_DefaultTechnique(IVoodooTechnique ** ppTechnique)
    {
        if ( ppTechnique == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppTechnique = mDefaultTechnique;
            return S_OK;
        }
    }

    HRESULT Shader::put_DefaultTechnique(IVoodooTechnique * ppTechnique)
    {
        mDefaultTechnique = ppTechnique;
        return S_OK;
    }

    UINT Shader::get_ParameterCount()
    {
        return mParameters.size();
    }

    HRESULT Shader::GetParameter(UINT Number, IVoodooParameter ** ppParameter)
    {
        if ( ppParameter == NULL )
        {
            return E_INVALIDARG;
        } else if ( Number < mParameters.size() ) {
            *ppParameter = mParameters[Number];
            return S_OK;
        } else {
            return E_INVALIDARG;
        }
    }

    HRESULT Shader::GetCgShader(void ** ppCgShader)
    {
        if ( ppCgShader == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppCgShader = mCgShader;
            return S_OK;
        }
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
            texName = mCore->GetParser()->ParseString(texName);
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

}
