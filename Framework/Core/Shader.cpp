/**
 * This file is part of the Voodoo IShader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 */

#include "IShader.hpp"

#include "IAdapter.hpp"
#include "Converter.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "ILogger.hpp"
#include "Parameter.hpp"
#include "IParser.hpp"
#include "ITechnique.hpp"

namespace VoodooShader
{
    IShader* IShader::Create(ICore *parent, String Path, const char **ppArgs /* = nullptr */ )
    {
        IShader* shader;

        new IShader(shader, parent, Path, ppArgs);
        return shader;
    }

    IShader::IShader(IShader* self, ICore *pParent, String Path, const char **ppArgs) :
    m_Name(Path),
        m_Core(pParent),
        m_DefaultTechnique(nullptr)
    {
        self.reset(this);

        CGcontext context = m_Core->GetCgContext();

        if (!context || !cgIsContext(context))
        {
            throw std::exception("Unable to create parameter (core has no context).");
        }

        this->m_CgEffect = cgCreateEffectFromFile(context, Path.c_str(), ppArgs);

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

    IShader::~IShader(void)
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

    String IShader::ToString(void)
    {
        return m_Name;
    }

    ICore *IShader::GetCore(void)
    {
        return m_Core;
    }

    size_t IShader::GetTechniqueCount(void)
    {
        return m_Techniques.size();
    }

    ITechnique* IShader::GetTechnique(size_t Index)
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

    ITechnique* IShader::GetDefaultTechnique(void)
    {
        return m_DefaultTechnique;
    }

    void IShader::SetDefaultTechnique(ITechnique* Technique)
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
                    LL_Error,
                    VOODOO_CORE_NAME,
                    "Technique %s cannot be set as default for shader %s (not technique for this shader).",
                    Technique->ToString().c_str(),
                    this->ToString().c_str()
                    );
            }
        }
    }

    size_t IShader::GetParameterCount(void)
    {
        return m_Parameters.size();
    }

    IParameter* IShader::GetParameter(_In_ size_t Index)
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

    void IShader::Link(IShader* self)
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
            IParameter* param(new IParameter(this, cParam));

            this->LinkParameter(param);

            this->m_Parameters.push_back(param);

            cParam = cgGetNextParameter(cParam);
        }

        this->SetupTechniques(self);
    }

    void IShader::LinkParameter(IParameter* param)
    {

        // Cache basic data for future use
        ParameterType type = param->GetType();
        CGparameter cgparam = param->GetCgParameter();
        // Check if it has a global link annotation
        CGannotation globalAnnotation = cgGetNamedParameterAnnotation(cgparam, "parameter");

        if (cgIsAnnotation(globalAnnotation) && cgGetAnnotationType(globalAnnotation) == CG_STRING)
        {
            const char *globalName = cgGetStringAnnotationValue(globalAnnotation);

            if (globalName != nullptr && strlen(globalName) > 0)
            {
                IParameter* globalParam = m_Core->GetParameter(globalName, type);

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
                        globalName,
                        param->ToString().c_str()
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
                    param->ToString().c_str()
                    );
            }

            return;
        }

        // If it's not linked to a global, it doesn't need linked unless it is a sampler.
        if (Converter::ToParameterCategory(type) == PC_Sampler)
        {
            this->LinkSampler(param);
        }
    }

    void IShader::LinkSampler(IParameter* param)
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
                param->ToString().c_str()
                );

            return;
        }

        const char *textureName = cgGetStringAnnotationValue(textureAnnotation);

        if (textureName == nullptr || strlen(textureName) == 0)
        {
            m_Core->GetLogger()->Log
                (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Could not retrieve texture name for parameter %s.",
                param->ToString().c_str()
                );

            return;
        }

        // Try to get the texture first, otherwise pass to adapter
        ITexture* texture = m_Core->GetTexture(textureName);

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
                textureName,
                param->ToString().c_str()
                );

            this->CreateParameterTexture(param);
        }
    }

    void IShader::CreateParameterTexture(_In_ IParameter* param)
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
                param->ToString().c_str()
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
                param->ToString().c_str()
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
        /* @todo Load texture region info from the annotations. */
        ITexture* tex = m_Core->GetAdapter()->LoadTexture(texName, texRegion);

        if (tex.get())
        {
            param->SetTexture(tex);
            return;
        }

        // No file, make blank texture
        CGannotation atexSize = cgGetNamedParameterAnnotation(parameter, "size");
        CGannotation atexFormat = cgGetNamedParameterAnnotation(parameter, "format");
        // CGannotation atexColor = cgGetNamedParameterAnnotation(parameter, "color");
        CGtype texSizeType = cgGetAnnotationType(atexSize);
        TextureDesc texDesc;

        if (texSizeType == CG_INT1)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "1-dimensional texture size found.");

            int outCount;
            const int *texDims = cgGetIntAnnotationValues(atexSize, &outCount);

            texDesc.Width = texDims[0];
            texDesc.Height = texDesc.Depth = 1;
        }
        else if (texSizeType == CG_INT2)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "2-dimensional texture size found.");

            int outCount;
            const int *texDims = cgGetIntAnnotationValues(atexSize, &outCount);

            texDesc.Width = texDims[0];
            texDesc.Height = texDims[1];
            texDesc.Depth = 1;
        }
        else if (texSizeType == CG_INT3)
        {
            m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "3-dimensional texture size found.");

            int outCount;
            const int *texDims = cgGetIntAnnotationValues(atexSize, &outCount);

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
                param->ToString().c_str()
                );
        }
        else
        {
            m_Core->GetLogger()->Log
                (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture size annotation type for parameter %s.",
                param->ToString().c_str()
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
                param->ToString().c_str()
                );

            return;
        }
        else
        {
            const char *formatString = cgGetStringAnnotationValue(atexFormat);

            texDesc.Format = Converter::ToTextureFormat(formatString);
        }

        IAdapter* adapter = m_Core->GetAdapter();
        ITexture* texture = adapter->CreateTexture(texName, texDesc);

        param->SetTexture(texture);
    }

    void IShader::SetupTechniques(IShader* self)
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
                ITechnique* tech = ITechnique::Create(self, cTech);

                // std::string techName = cgGetTechniqueName(cTech);
                // m_Techniques[techName] = tech;
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
                    "ITechnique failed to validate: %s.",
                    cgGetTechniqueName(cTech)
                    );
            }

            cTech = cgGetNextTechnique(cTech);
        }
    }

    CGeffect IShader::GetCgEffect(void)
    {
        return m_CgEffect;
    }
}
