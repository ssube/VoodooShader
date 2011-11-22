/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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

#include "VSShader.hpp"

#include "VSParameter.hpp"
#include "VSTechnique.hpp"
#include "VSTexture.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "IFilesystem.hpp"
#include "ILogger.hpp"
#include "IParser.hpp"
#include "ITexture.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    //! @todo PREfast fails if the input core here has an _In_ annotation. This needs fixed.
    VSShader::VSShader(_Pre_notnull_ ICore * const pCore, _In_ const String & path, _In_opt_ const char ** ppArgs) :
        m_Refs(0), m_Core(pCore), m_Name(path), m_DefaultTechnique(nullptr)
    {
        CGcontext context = m_Core->GetCgContext();

        if (!context || !cgIsContext(context))
        {
            Throw(VOODOO_CORE_NAME, L"Unable to create parameter (core has no context).", m_Core);
        }

        int32_t len = m_Name.ToCharStr(0, nullptr);
        std::vector<char> buffer(len);
        path.ToCharStr(len, &buffer[0]);

        m_CgEffect = cgCreateEffectFromFile(context, &buffer[0], ppArgs);

        if (!cgIsEffect(m_CgEffect))
        {
            Throw(VOODOO_CORE_NAME, L"Failed to create shader.", m_Core);
        }
        else
        {
            cgSetEffectName(m_CgEffect, &buffer[0]);
        }

        this->Link();
    }

    VSShader::~VSShader(void)
    {
        m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"Destroying shader %s.", m_Name.GetData());

        m_DefaultTechnique = nullptr;
        m_Techniques.clear();
        m_Parameters.clear();

        if (cgIsEffect(m_CgEffect))
        {
            cgDestroyEffect(m_CgEffect);
        }
    }

    uint32_t VSShader::AddRef() const
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VSShader::Release() const
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VSShader::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSShader;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IShader) {
                *ppOut = static_cast<const IShader*>(this);
            } else if (clsid == CLSID_VSShader) {
                *ppOut = static_cast<const VSShader*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VSShader::ToString(void) const
    {
        return String::Format(L"VSShader(%s)", m_Name.GetData());
    }

    ICore * VSShader::GetCore(void) const
    {
        return m_Core;
    }

    String VSShader::GetName() const
    {
        return m_Name;
    }

    const uint32_t VSShader::GetTechniqueCount(void) const
    {
        return m_Techniques.size();
    }

    ITechnique * VSShader::GetTechnique(const uint32_t index) const
    {
        if (index < m_Techniques.size())
        {
            return m_Techniques[index].get();
        }
        else
        {
            return nullptr;
        }
    }

    ITechnique * VSShader::GetDefaultTechnique(void) const
    {
        return m_DefaultTechnique.get();
    }

    bool VSShader::SetDefaultTechnique(ITechnique * pTechnique)
    {
        if (pTechnique != nullptr)
        {
            if (pTechnique->GetShader() == this)
            {
                m_DefaultTechnique = pTechnique;
                return true;
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Error,
                    VOODOO_CORE_NAME,
                    L"Technique %s cannot be set as default for shader %s (not technique for this shader).",
                    pTechnique->ToString().GetData(), this->ToString().GetData()
                );
            }
        }

        return false;
    }

    const uint32_t VSShader::GetParameterCount(void) const
    {
        return m_Parameters.size();
    }

    IParameter * VSShader::GetParameter(_In_ const uint32_t index) const
    {
        if (index < m_Parameters.size())
        {
            return m_Parameters[index].get();
        }
        else
        {
            return nullptr;
        }
    }

    CGeffect VSShader::GetCgEffect(void) const
    {
        return m_CgEffect;
    }

    void VSShader::Link()
    {
        // Make sure it's a valid effect
        if (!cgIsEffect(m_CgEffect))
        {
            return;
        }

        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(m_CgEffect);

        while (cgIsParameter(cParam))
        {
            IParameter * pParam = new VSParameter(this, cParam);

            this->LinkParameter(pParam);

            m_Parameters.push_back(pParam);

            cParam = cgGetNextParameter(cParam);
        }

        this->SetupTechniques();
    }

    void VSShader::LinkParameter(IParameter * pParam)
    {
        // Cache basic data for future use
        ParameterType type = pParam->GetType();
        CGparameter cgparam = pParam->GetCgParameter();

        // Check if it has a global link annotation
        CGannotation globalAnnotation = cgGetNamedParameterAnnotation(cgparam, "parameter");

        if (cgIsAnnotation(globalAnnotation) && cgGetAnnotationType(globalAnnotation) == CG_STRING)
        {
            const char * globalName = cgGetStringAnnotationValue(globalAnnotation);

            if (globalName != nullptr && strlen(globalName) > 0)
            {
                IParameterRef globalParam = m_Core->GetParameter(globalName, type);

                if (globalParam)
                {
                    globalParam->AttachParameter(pParam);
                }
                else
                {
                    m_Core->GetLogger()->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        L"Unable to find global param %s for parameter %s.",
                        globalName, pParam->ToString().GetData()
                    );
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    L"Unable to read global annotation for parameter %s.",
                    pParam->ToString().GetData()
                );
            }

            return;
        }

        // If it's not linked to a global, it doesn't need linked unless it is a sampler.
        if (Converter::ToParameterCategory(type) == PC_Sampler)
        {
            this->LinkSampler(pParam);
        }
    }

    void VSShader::LinkSampler(IParameter * pParam)
    {
        IParameterRef param = pParam;

        CGparameter cgparam = param->GetCgParameter();
        // Link to a texture
        CGannotation textureAnnotation = cgGetNamedParameterAnnotation(cgparam, "texture");

        if (!cgIsAnnotation(textureAnnotation) || cgGetAnnotationType(textureAnnotation) != CG_STRING)
        {
            m_Core->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                L"Could not retrieve texture annotation for parameter %s.",
                param->ToString().GetData()
            );

            return;
        }

        const char * textureName = cgGetStringAnnotationValue(textureAnnotation);

        if (textureName == nullptr || strlen(textureName) == 0)
        {
            m_Core->GetLogger()->Log
                (
                LL_Warning,
                VOODOO_CORE_NAME,
                L"Could not retrieve texture name for parameter %s.",
                param->ToString().GetData()
                );

            return;
        }

        // Try to get the texture first, otherwise pass to adapter
        ITextureRef texture = m_Core->GetTexture(textureName);

        if (texture)
        {
            m_Core->GetAdapter()->ConnectTexture(param.get(), texture.get());
        }
        else
        {
            m_Core->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                L"Could not find texture %s for parameter %s, attempting to load.",
                textureName,
                param->ToString().GetData()
            );

            this->CreateParameterTexture(param.get());
        }
    }

    void VSShader::CreateParameterTexture(_In_ IParameter * pParam)
    {
        IParameterRef param = pParam;

        CGparameter parameter = param->GetCgParameter();

        if (!cgIsParameter(parameter))
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                L"Could not create parameter texture for unknown parameter."
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
                L"Invalid or missing texture name for parameter %s.",
                param->ToString().GetData()
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
                L"Invalid texture name annotation type in %s.",
                param->ToString().GetData()
            );

            return;
        }
        else
        {
            String rawname = String(cgGetStringAnnotationValue(atexName));
            texName = m_Core->GetParser()->Parse(rawname);
        }

        // Check for a valid texture file
        TextureRegion texRegion;
        ZeroMemory(&texRegion, sizeof(TextureRegion));

        /* @todo Load texture region info from the annotations. */
        CGannotation atexSize = cgGetNamedParameterAnnotation(parameter, "size");
        CGannotation atexOrigin = cgGetNamedParameterAnnotation(parameter, "origin");
        CGannotation atexFormat = cgGetNamedParameterAnnotation(parameter, "format");
        CGannotation atexMips = cgGetNamedParameterAnnotation(parameter, "mipmaps");
        CGannotation atexTarget = cgGetNamedParameterAnnotation(parameter, "target");

        // CGannotation atexColor = cgGetNamedParameterAnnotation(parameter, "color");
        if (cgIsAnnotation(atexSize))
        {
            CGtype sizeType = cgGetAnnotationType(atexSize);

            int outCount;
            const int * texSize = cgGetIntAnnotationValues(atexSize, &outCount);

            switch (sizeType)
            {
            case CG_INT:
                {
                    m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"1-dimensional texture size found.");

                    texRegion.Size.X = texSize[0];
                    texRegion.Size.Y = texRegion.Size.Z = 1;
                }
            case CG_INT2:
                {
                    m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"2-dimensional texture size found.");


                    texRegion.Size.X = texSize[0];
                    texRegion.Size.Y = texSize[1];
                    texRegion.Size.Z = 1;
                }
            case CG_INT3:
                {
                    m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"3-dimensional texture size found.");

                    texRegion.Size.X = texSize[0];
                    texRegion.Size.Y = texSize[1];
                    texRegion.Size.Z = texSize[2];
                }
            case CG_INT4:
                {
                    m_Core->GetLogger()->Log
                        (
                        LL_Error,
                        VOODOO_CORE_NAME,
                        L"4-dimensional texture size found. Creating quantum texture... (invalid type on %s).",
                        param->ToString().GetData()
                        );
                }
            default:
                {
                    m_Core->GetLogger()->Log
                        (
                        LL_Error,
                        VOODOO_CORE_NAME,
                        L"Invalid texture size annotation type for parameter %s.",
                        param->ToString().GetData()
                        );
                }
            }
        } else {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"No texture size found.");
            texRegion.Size.X = texRegion.Size.Y = texRegion.Size.Z = (uint32_t)-1;
        }

        if (cgIsAnnotation(atexOrigin))
        {
            CGtype originType = cgGetAnnotationType(atexOrigin);

            int outCount;
            const int * texOrigin = cgGetIntAnnotationValues(atexSize, &outCount);

            switch (originType)
            {
            case CG_INT:
                {
                    m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"1-dimensional texture origin found.");

                    texRegion.Origin.X = texOrigin[0];
                    texRegion.Origin.Y = texRegion.Size.Z = 1;
                }
            case CG_INT2:
                {
                    m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"2-dimensional texture origin found.");


                    texRegion.Origin.X = texOrigin[0];
                    texRegion.Origin.Y = texOrigin[1];
                    texRegion.Origin.Z = 1;
                }
            case CG_INT3:
                {
                    m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"3-dimensional texture origin found.");

                    texRegion.Origin.X = texOrigin[0];
                    texRegion.Origin.Y = texOrigin[1];
                    texRegion.Origin.Z = texOrigin[2];
                }
            default:
                {
                    m_Core->GetLogger()->Log
                        (
                        LL_Error,
                        VOODOO_CORE_NAME,
                        L"Invalid texture size annotation type for parameter %s.",
                        param->ToString().GetData()
                        );
                }
            }
        } else {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, L"No texture origin found.");
            texRegion.Origin.X = texRegion.Origin.Y = texRegion.Origin.Z = (uint32_t)-1;
        }

        {
            int outCount = 0;
            const CGbool * boolVals = cgGetBoolAnnotationValues(atexMips, &outCount);
            if (boolVals && outCount > 0)
            {
                texRegion.Mipmaps = boolVals[0] == 1;
            } else {
                texRegion.Mipmaps = false;
            }
        }

        {
            int outCount = 0;
            const CGbool * boolVals = cgGetBoolAnnotationValues(atexTarget, &outCount);
            if (boolVals && outCount > 0)
            {
                texRegion.RenderTarget = boolVals[0] == 1;
            } else {
                texRegion.RenderTarget = false;
            }
        }

        if (cgGetAnnotationType(atexFormat) != CG_STRING)
        {
            m_Core->GetLogger()->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                L"Invalid texture format annotation type in %s.",
                param->ToString().GetData()
            );

            return;
        }
        else
        {
            String formatString = cgGetStringAnnotationValue(atexFormat);

            texRegion.Format = Converter::ToTextureFormat(formatString.GetData());
        }

        // Attempt to load the texture from a file
        IAdapterRef adapter = m_Core->GetAdapter();
        ITextureRef texture = nullptr;

        IFile * texFile = m_Core->GetFileSystem()->FindFile(texName);
        if (texFile)
        {
            IImage * texImage = texFile->OpenImage();
            if (texImage)
            {
                texture = new VSTexture(texFile->GetPath(), nullptr);
                adapter->LoadTexture(texImage, &texRegion, texture.get());

                if (!texture)
                {
                    m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, L"Adapter was unable to load texture from file '%s'.", texFile->GetPath().GetData());
                }
            } else {
                m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, L"File '%s' is not a valid image.", texFile->GetPath().GetData());
            }
        } else {
            m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, L"Unable to find texture file '%s'.", texName.GetData());
        }

        // No file, make blank texture
        if (!texture)
        {
            texture = new VSTexture(texName, nullptr);
            adapter->CreateTexture(texName, (TextureDesc*)&texRegion, texture.get());
        }

        param->SetTexture(texture.get());
    }

    void VSShader::SetupTechniques()
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
                    L"Validated technique %s.",
                    cgGetTechniqueName(cTech)
                );

                // Insert the technique into the map
                ITechniqueRef tech = new VSTechnique(this, cTech);

                m_Techniques.push_back(tech);

                // The first valid technique is the default one
                if (!m_DefaultTechnique.get())
                {
                    m_DefaultTechnique = tech;
                }
            }
            else
            {
                m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    L"ITechnique failed to validate: %s.",
                    cgGetTechniqueName(cTech)
                );
            }

            cTech = cgGetNextTechnique(cTech);
        }
    }
}
