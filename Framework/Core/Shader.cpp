#include "Shader.hpp"

namespace VoodooShader
{
    Shader::Shader(Core * parent, String filename, const char ** args)
        : m_Core(parent), m_Name(filename), m_DefaultTechnique()
    {
        CGcontext context = m_Core->GetCgContext();

        if ( !context || !cgIsContext(context) )
        {
            throw std::exception("Unable to create parameter (core has no context).");
        }

        IFileRef fileref = m_Core->GetFileSystem()->GetFile(filename);

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
            cgSetEffectName(this->mEffect, this->m_Name.c_str());
        }
    }

    Shader::~Shader()
    {
        //m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Destroying shader %s.", m_Name.c_str());

        m_DefaultTechnique = NULL;
        m_Techniques.clear();
        m_Parameters.clear();

        if ( cgIsEffect(mEffect) )
        {
            cgDestroyEffect(mEffect);
        }
    }

    HRESULT Shader::QueryInterface(REFIID iid, void ** pp)
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

    ULONG Shader::AddRef()
    {
        return (++mRefrs);
    }

    ULONG Shader::Release()
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

    HRESULT Shader::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        } else {
            return m_Name.CopyTo(pName);
        }
    }

    HRESULT Shader::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppCore = m_Core;
            return S_OK;
        }
    }

    UINT Shader::get_TechniqueCount()
    {
        return (UINT)m_Techniques.GetSize();
    }

    HRESULT Shader::GetTechnique(UINT Number, IVoodooTechnique ** ppTechnique)
    {
        if ( ppTechnique == NULL )
        {
            return E_INVALIDARG;
        } else if ( Number < (UINT)m_Techniques.GetSize() ) {
            CComPtr<IVoodooTechnique> tech = m_Techniques.GetAt(Number);
            *ppTechnique = tech;
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
            *ppTechnique = m_DefaultTechnique;
            return S_OK;
        }
    }

    HRESULT Shader::put_DefaultTechnique(IVoodooTechnique * ppTechnique)
    {
        m_DefaultTechnique = ppTechnique;
        return S_OK;
    }

    UINT Shader::get_ParameterCount()
    {
        return m_Parameters.GetSize();
    }

    HRESULT Shader::GetParameter(UINT Number, IVoodooParameter ** ppParameter)
    {
        if ( ppParameter == NULL )
        {
            return E_INVALIDARG;
        } else if ( Number < m_Parameters.GetSize() ) {
            *ppParameter = m_Parameters[Number];
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
            *ppCgShader = m_CgShader;
            return S_OK;
        }
    }

    void Shader::Link()
    {
        // Make sure it's a valid effect
        if ( !cgIsEffect(mEffect) )
        {
            Throw(VOODOO_CORE_NAME, "Invalid effect.", m_Core);
        }

        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(this->mEffect);

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
                    globalParam->Attach(param);
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

    void Shader::CreateParameterTexture( _In_ ParameterRef param )
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
            texName = m_Core->GetParser()->ParseString(texName);
        }

        // Check for a valid texture file
        IImageRef texFile = m_Core->GetFileSystem()->GetFile(texName);
        if ( texFile.get() )
        {
            TextureRef tex = m_Core->GetAdapter()->CreateTexture(texFile);
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
        CGtechnique cTech = cgGetFirstTechnique(mEffect);
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

    void Shader::SetDefaultTechnique(String name)
    {
        TechniqueRef tech = this->GetTechnique(name);
        if ( tech.get() )
        {
            this->m_DefaultTechnique = tech;
        } else {
            Throw(VOODOO_CORE_NAME, "Technique not found in shader.", m_Core);
        }
    }

    TechniqueRef Shader::GetTechnique(String name)
    {
        TechniqueMap::iterator tech = m_Techniques.find(name);
        if ( tech != m_Techniques.end() )
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
