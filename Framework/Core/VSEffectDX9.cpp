/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

#include "VSEffectDX9.hpp"

#include "VSParameterDX9.hpp"
#include "VSTechniqueDX9.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSEffectDX9
    DeclareDebugCache();

    VSEffectDX9::VSEffectDX9(_In_ IFile * pFile, CompileFlags flags) :
        m_Refs(0) 
    {
        if (!pFile)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create effect without file."), nullptr);
        }

        m_Core = pFile->GetCore();

        // Cache core objects
        ILoggerRef  logger  = m_Core->GetLogger();
        IAdapterRef adapter = m_Core->GetAdapter();

        if (!logger)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create effect without logger."), m_Core);
        }
        else if (!adapter)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create effect without adapter."), m_Core);
        }

        // Get the D3D9 device from the adapter
        IDirect3DDevice9 * device = nullptr;
        Variant deviceVar;
        if (SUCCEEDED(adapter->GetProperty(VSTR("IDirect3DDevice9"), &deviceVar)) && deviceVar.Type == UT_PVoid && deviceVar.VPVoid)
        {
            device = (IDirect3DDevice9*)deviceVar.VPVoid;
            device->AddRef();
        }
        else
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to retrieve device from adapter."), m_Core);
        }

        // Build the flags
        DWORD d3dxflags = 0;
        if ((flags & CF_AvoidFlow) > 0)     d3dxflags |= D3DXSHADER_AVOID_FLOW_CONTROL;
        if ((flags & CF_PreferFlow) > 0)    d3dxflags |= D3DXSHADER_PREFER_FLOW_CONTROL;
        if ((flags & CF_Debug) > 0)         d3dxflags |= D3DXSHADER_DEBUG;
        if ((flags & CF_NoOpt) > 0)         d3dxflags |= D3DXSHADER_SKIPOPTIMIZATION;

        // Output/buffer
        LPD3DXBUFFER errors = NULL;

        HRESULT hr = D3DXCreateEffectFromFile(device, pFile->GetPath().GetData(), NULL, NULL, d3dxflags, NULL, &m_DXEffect, &errors);
        if (FAILED(hr))
        {
            logger->LogMessage(LL_CoreError, VOODOO_CORE_NAME, 
                Format("Error compiling effect from file '%1%'. Errors:\n%2%") << 
                pFile->GetPath() << (LPCSTR)errors->GetBufferPointer());

            Throw(VOODOO_CORE_NAME, VSTR("Unable to compile effect."), m_Core);
        }

        // Get desc
        D3DXEFFECT_DESC desc;
        ZeroMemory(&desc, sizeof(D3DXEFFECT_DESC));
        if (FAILED(m_DXEffect->GetDesc(&desc)))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Failed to retrieve effect description."), m_Core);
        }

        // Get parameters
        for (UINT paramIndex = 0; paramIndex < desc.Parameters; ++paramIndex)
        {
            D3DXHANDLE paramHandle = m_DXEffect->GetParameter(NULL, paramIndex);
            if (!paramHandle)
            {
                break;
            }
            boost::intrusive_ptr<VSParameterDX9> parameter = new VSParameterDX9(this, m_DXEffect, paramHandle);
            m_DXEffect->m_Parameters.push_back(parameter);
        }

        // Get techniques
        for (UINT techIndex = 0; techIndex < desc.Techniques; ++techIndex)
        {
            D3DXHANDLE techHandle = m_DXEffect->GetTechnique(techIndex++);
            if (!techHandle)
            {
                break;
            }

            try
            {
                VSTechniqueDX9 * technique = new VSTechniqueDX9(this, m_DXEffect, techHandle);
                m_DXEffect->m_Techniques.push_back(technique);
            }
            catch (Exception & exc)
            {
                //logger->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, Format("Failed to create technique %1%.") <<
            }
        } 

        AddThisToDebugCache();
    }

    VSShader::~VSShader()
    {
        RemoveThisFromDebugCache();

        m_DefaultTechnique = nullptr;
        m_Techniques.clear();
        m_Parameters.clear();
    }

    uint32_t VOODOO_METHODTYPE VSShader::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSShader::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return m_Refs;
        }
    }

    bool VOODOO_METHODTYPE VSShader::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSShader;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            }
            else if (clsid == IID_IShader)
            {
                *ppOut = static_cast<const IShader*>(this);
            }
            else if (clsid == CLSID_VSShader)
            {
                *ppOut = static_cast<const VSShader*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSShader::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return Format(VSTR("VSShader(%1%)")) << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSShader::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    String VOODOO_METHODTYPE VSShader::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Name;
    }

    uint32_t VOODOO_METHODTYPE VSShader::GetTechniqueCount() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Techniques.size();
    }

    ITechnique * VOODOO_METHODTYPE VSShader::GetTechnique(const uint32_t index) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (index < m_Techniques.size())
        {
            return m_Techniques[index].get();
        }
        else
        {
            return nullptr;
        }
    }

    ITechnique * VOODOO_METHODTYPE VSShader::GetDefaultTechnique() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_DefaultTechnique.get();
    }

    bool VOODOO_METHODTYPE VSShader::SetDefaultTechnique(ITechnique * pTechnique)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (pTechnique != nullptr)
        {
            if (pTechnique->GetShader() == this)
            {
                m_DefaultTechnique = pTechnique;
                return true;
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreError, VOODOO_CORE_NAME,
                    Format(VSTR("Technique %1% cannot be set as default (technique originated from another shader)."))
                    << pTechnique
                );
            }
        }

        return false;
    }

    uint32_t VOODOO_METHODTYPE VSShader::GetParameterCount() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Parameters.size();
    }

    IParameter * VOODOO_METHODTYPE VSShader::GetParameter(_In_ const uint32_t index) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (index < m_Parameters.size())
        {
            return m_Parameters[index].get();
        }
        else
        {
            return nullptr;
        }
    }

    CGeffect VOODOO_METHODTYPE VSShader::GetCgEffect() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_CgEffect;
    }

    void VSShader::Link()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        // Make sure it's a valid effect
        if (!cgIsEffect(m_CgEffect))
        {
            return;
        }

        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(m_CgEffect);

        while (cgIsParameter(cParam))
        {
            try
            {
                IParameterRef pParam = new VSParameter(this, cParam);

                this->LinkParameter(pParam.get());

                m_Parameters.push_back(pParam);
            }
            catch (const std::exception & exc)
            {
                const char * name = cgGetParameterName(cParam);

                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    Format(VSTR("Error creating linking parameter %1%: %2%")) << name << exc.what()
                );
            }

            cParam = cgGetNextParameter(cParam);
        }

        this->SetupTechniques();
    }

    void VSShader::LinkParameter(IParameter * pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        IParameterRef param = pParam;

        // Cache basic data for future use
        ParameterType type = param->GetType();
        CGparameter cgparam = param->GetCgParameter();

        // Check if it has a global link annotation
        CGannotation globalAnnotation = cgGetNamedParameterAnnotation(cgparam, "source");

        if (cgIsAnnotation(globalAnnotation) && cgGetAnnotationType(globalAnnotation) == CG_STRING)
        {
            const char * globalName = cgGetStringAnnotationValue(globalAnnotation);

            if (globalName != nullptr && strlen(globalName) > 0)
            {
                IParameterRef globalParam = m_Core->GetParameter(globalName, type);

                if (globalParam)
                {
                    globalParam->AttachParameter(param.get());
                }
                else
                {
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_CoreWarning, VOODOO_CORE_NAME,
                        Format(VSTR("Unable to find global param '%1%' for parameter '%2%'.")) << globalName << param
                    );
                }
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreWarning, VOODOO_CORE_NAME,
                    Format(VSTR("Unable to read global annotation for parameter '%1%'.")) << param
                );
            }

            return;
        }

        // If it's not linked to a global, it doesn't need linked unless it is a sampler.
        if (Converter::ToParameterCategory(type) == PC_Sampler)
        {
            this->LinkSampler(param.get());
        }
    }

    void VSShader::LinkSampler(IParameter * pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        IParameterRef param = pParam;

        CGparameter cgparam = param->GetCgParameter();
        // Link to a texture
        CGannotation textureAnnotation = cgGetNamedParameterAnnotation(cgparam, "texture");

        if (!cgIsAnnotation(textureAnnotation) || cgGetAnnotationType(textureAnnotation) != CG_STRING)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME,
                Format(VSTR("Could not retrieve texture annotation for parameter '%1%'.")) << param
            );

            return;
        }

        const char * textureName = cgGetStringAnnotationValue(textureAnnotation);

        if (textureName == nullptr || strlen(textureName) == 0)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME,
                Format(VSTR("Could not retrieve texture name for parameter '%1%'.")) << param
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
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME,
                Format(VSTR("Could not find texture '%1%' for parameter '%1%', attempting to load."))  << textureName << param
            );

            this->CreateParameterTexture(param.get());
        }
    }

    void VSShader::CreateParameterTexture(_In_ IParameter * pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        IParameterRef param = pParam;

        CGparameter parameter = param->GetCgParameter();

        if (!cgIsParameter(parameter))
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreError, VOODOO_CORE_NAME,
                VSTR("Could not create parameter texture for unknown parameter.")
            );

            return;
        }

        CGannotation atexName = cgGetNamedParameterAnnotation(parameter, "texture");

        if (!cgIsAnnotation(atexName) || cgGetAnnotationType(atexName) != CG_STRING)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreError, VOODOO_CORE_NAME,
                Format(VSTR("Invalid or missing texture name for parameter '%1%'.")) << param
            );

            return;
        }

        String texName;

        if (cgGetAnnotationType(atexName) != CG_STRING)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreError, VOODOO_CORE_NAME,
                Format(VSTR("Invalid texture name annotation type in '%1%'.")) << param
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
                m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("1-dimensional texture size found."));

                texRegion.Size.X = texSize[0];
                texRegion.Size.Y = texRegion.Size.Z = 1;
                break;
            case CG_INT2:
                m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME,VSTR("2-dimensional texture size found."));

                texRegion.Size.X = texSize[0];
                texRegion.Size.Y = texSize[1];
                texRegion.Size.Z = 1;
                break;
            case CG_INT3:
                m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("3-dimensional texture size found."));

                texRegion.Size.X = texSize[0];
                texRegion.Size.Y = texSize[1];
                texRegion.Size.Z = texSize[2];
                break;
            case CG_INT4:
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreError,
                    VOODOO_CORE_NAME,
                    VSTR("4-dimensional texture size found (creating quantum texture).")
                );
            default:
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreError,
                    VOODOO_CORE_NAME,
                    Format(VSTR("Invalid texture size annotation type for parameter '%1%'.")) << param
                );

                return;
            }
        }
        else
        {
            m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("No texture size found."));
            texRegion.Size.X = texRegion.Size.Y = texRegion.Size.Z = 0;
        }

        if (cgIsAnnotation(atexOrigin))
        {
            CGtype originType = cgGetAnnotationType(atexOrigin);

            int outCount;
            const int * texOrigin = cgGetIntAnnotationValues(atexSize, &outCount);

            switch (originType)
            {
            case CG_INT:
                m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("1-dimensional texture origin found."));

                texRegion.Origin.X = texOrigin[0];
                texRegion.Origin.Y = texRegion.Size.Z = 1;
                break;
            case CG_INT2:
                m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("2-dimensional texture origin found."));

                texRegion.Origin.X = texOrigin[0];
                texRegion.Origin.Y = texOrigin[1];
                texRegion.Origin.Z = 1;
                break;
            case CG_INT3:
                m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("3-dimensional texture origin found."));

                texRegion.Origin.X = texOrigin[0];
                texRegion.Origin.Y = texOrigin[1];
                texRegion.Origin.Z = texOrigin[2];
                break;
            default:
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreError,
                    VOODOO_CORE_NAME,
                    Format(VSTR("Invalid texture size annotation type for parameter '%1%'.")) << param
                );

                return;
            }
        }
        else
        {
            m_Core->GetLogger()->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("No texture origin found."));
            texRegion.Origin.X = texRegion.Origin.Y = texRegion.Origin.Z = 0;
        }

        {
            int outCount = 0;
            const CGbool * boolVals = cgGetBoolAnnotationValues(atexMips, &outCount);
            if (boolVals && outCount > 0)
            {
                texRegion.Mipmaps = boolVals[0] == 1;
            }
            else
            {
                texRegion.Mipmaps = false;
            }
        }

        {
            int outCount = 0;
            const CGbool * boolVals = cgGetBoolAnnotationValues(atexTarget, &outCount);
            if (boolVals && outCount > 0)
            {
                texRegion.RenderTarget = boolVals[0] == 1;
            }
            else
            {
                texRegion.RenderTarget = false;
            }
        }

        if (cgGetAnnotationType(atexFormat) != CG_STRING)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreError, VOODOO_CORE_NAME,
                Format(VSTR("Invalid texture format annotation type in '%1%'.")) << param
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
        ITextureRef texture = m_Core->CreateTexture(texName, (TextureDesc)texRegion);

        IFile * texFile = m_Core->GetFileSystem()->GetFile(texName);
        if (texFile)
        {
            IImage * texImage = texFile->OpenImage();
            if (texImage)
            {
                adapter->LoadTexture(texImage, texRegion, texture.get());

                if (!texture)
                {
                    m_Core->GetLogger()->LogMessage
                    (
                        LL_CoreWarning, VOODOO_CORE_NAME, 
                        Format(VSTR("Adapter was unable to load texture from file '%1%'.")) << texFile
                    );
                }
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreWarning, VOODOO_CORE_NAME, 
                    Format(VSTR("File '%1%' is not a valid image.")) << texFile
                );
            }
        }
        else
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME, 
                Format(VSTR("Unable to find texture file '%1%'.")) << texName
            );
        }

        param->SetTexture(texture.get());
    }

    void VSShader::SetupTechniques()
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        CGtechnique cTech = cgGetFirstTechnique(m_CgEffect);

        while (cgIsTechnique(cTech))
        {
            CGbool valid = cgValidateTechnique(cTech);

            if (valid == CG_TRUE)
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    Format(VSTR("Validated technique '%1%'.")) << cgGetTechniqueName(cTech)
                );

                // Insert the technique into the map
                ITechniqueRef tech = new VSTechnique(this, cTech);

                m_Techniques.push_back(tech);

                // The first valid technique is the default one
                if (!m_DefaultTechnique)
                {
                    m_DefaultTechnique = tech;
                }
            }
            else
            {
                m_Core->GetLogger()->LogMessage
                (
                    LL_CoreWarning, VOODOO_CORE_NAME,
                    Format(VSTR("Technique '%1%' failed to validate.")) << cgGetTechniqueName(cTech)
                );
            }

            cTech = cgGetNextTechnique(cTech);
        }
    }
}
