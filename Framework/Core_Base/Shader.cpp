#include "Shader.hpp"

#include "Converter.hpp"

namespace VoodooShader
{
    Shader::Shader()
        : m_Refrs(0), m_Core(NULL), m_Shader(NULL), m_DefaultTechnique(NULL)
    { }

    Shader::~Shader()
    {
        //m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Destroying shader %s.", m_Name.c_str());

        m_DefaultTechnique = NULL;
        m_Techniques.RemoveAll();
        m_Parameters.RemoveAll();

        if ( cgIsEffect(m_Shader) )
        {
            cgDestroyEffect(m_Shader);
        }
    }

    Shader * Shader::Create(IVoodooCore * pCore, IVoodooFile * pFile)
    {
        if ( pCore == NULL || pFile == NULL )
        {
            return NULL;
        }

        Shader * shader = new Shader();

        pFile->get_Path(&shader->m_Name);

        CGcontext context = NULL; 
        pCore->get_CgContext((void**)&context);

        if ( context == NULL || !cgIsContext(context) )
        {
            delete shader;
            return NULL;
        }

        CW2A filename(shader->m_Name);
        shader->m_Shader = cgCreateEffectFromFile
        (
            context, 
            filename, 
            NULL
        );

        if ( !cgIsEffect(shader->m_Shader) )
        {
            delete shader;
            return NULL;
        } else {
            cgSetEffectName(shader->m_Shader, filename);
            return shader;
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
        return (++m_Refrs);
    }

    ULONG Shader::Release()
    {
        --m_Refrs;
        if ( m_Refrs == 0 )
        {
            delete this;
            return 0;
        } else {
            return m_Refrs;
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
        } else if ( Number < (UINT)m_Parameters.GetSize() ) {
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
            *ppCgShader = m_Shader;
            return S_OK;
        }
    }

    HRESULT Shader::Link()
    {
        // Make sure it's a valid effect
        if ( !cgIsEffect(m_Shader) )
        {
            return E_BADTHING;
        }

        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(m_Shader);

        while ( cgIsParameter(cParam) )
        {
            IVoodooParameter * iparameter = NULL;

            ParameterCategory pc = Converter::ToParameterCategory(cgGetParameterType(cParam));
            if ( pc == PC_Float || pc == PC_Matrix )
            {
                Scalar * parameter = Scalar::Create(this, cParam);
                parameter->QueryInterface(IID_VoodooParameter, &iparameter);
            } else if ( pc == PC_Sampler ) {
                Sampler * parameter = Sampler::Create(this, cParam);
                parameter->QueryInterface(IID_VoodooParameter, &iparameter);
            }

            m_Parameters.Add(iparameter);

            this->LinkParameter(iparameter);

            cParam = cgGetNextParameter(cParam);
        }

        this->SetupTechniques();
    }

    void Shader::LinkParameter(IVoodooParameter * param)
    {
        // Cache basic data for future use 
        ParameterType type;
        param->GetType(&type);
        CGparameter cgparam;
        param->GetCgParameter(&cgparam);

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
                /*m_Core->GetLogger()->Log
                (
                    LL_Debug,
                    VOODOO_CORE_NAME,
                    "Validated technique %s.",
                    cgGetTechniqueName(cTech)
                );*/

                // Insert the technique into the map
                Technique * tech = Technique::Create(this, cTech);
                tech->Link();

                IVoodooTechnique * itech = NULL;
                tech->QueryInterface(IID_VoodooTechnique, (void**)&itech);
                m_Techniques.Add(itech);

                // The first valid technique is the default one
                if ( m_DefaultTechnique == NULL )
                {
                    m_DefaultTechnique = itech;
                }
            } else {
                /*m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Technique failed to validate: %s.",
                    cgGetTechniqueName(cTech)
                );*/
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
