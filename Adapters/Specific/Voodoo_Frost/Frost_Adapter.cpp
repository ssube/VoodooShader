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

#include "Frost_Adapter.hpp"
// Voodoo Frost
#include "Frost_Texture.hpp"
#include "VoodooGL.hpp"
// Voodoo Core
#include "Format.hpp"

#define VOODOO_OGL_HOOK_PARAMS(x) #x, &x, &v##x

namespace VoodooShader
{
    namespace VoodooFrost
    {
        FrostAdapter::FrostAdapter(_In_ ICore * pCore) :
            m_Core(pCore), m_DC(nullptr), m_GLRC(nullptr)
        {
            gpVoodooCore = m_Core;

            ILoggerRef logger = m_Core->GetLogger();

            // Hook OpenGL functions
            logger->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, L"Beginning OpenGL hook procedure.");

            IHookManagerRef hooker = m_Core->GetHookManager();
            bool success = true;

            // System-related
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glGetString)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glViewport)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglCreateContext)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglDeleteContext)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglGetProcAddress)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglMakeCurrent)));

            // Shader-related
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glClear)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglSwapLayerBuffers)));

            // Material-related
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glBindTexture)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glDeleteTextures)));

            // Shader/material shared
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glBegin)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glDrawElements)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glEnd)));

            // Fog-related
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glEnable)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glFogf)));
            success &= SUCCEEDED(hooker->Add(VOODOO_OGL_HOOK_PARAMS(glFogfv)));

            // Check the results and handle
            if (success)
            {
                logger->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, L"OpenGL hooked successfully.");
            }
            else
            {
                logger->LogMessage(VSLog_ModError, VOODOO_FROST_NAME, L"OpenGL hook procedure failed.");
                return;
            }

            logger->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, L"Frost adapter initialized.");
        }

        FrostAdapter::~FrostAdapter(void)
        {
            // mLogger->Log(VSLog_ModInfo, VOODOO_FROST_NAME, "Frost adapter shutdown.");
        }

        uint32_t VOODOO_METHODTYPE FrostAdapter::AddRef() const
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE FrostAdapter::Release() const
        {
            uint32_t value = SAFE_DECREMENT(m_Refs);

            if (value == 0)
            {
                delete this;
            }

            return value;
        }

        VoodooResult VOODOO_METHODTYPE FrostAdapter::QueryInterface(_In_ const Uuid clsid, _Deref_out_opt_ const IObject ** ppOut) const
        {
            if (!ppOut)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else
            {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                }
                else if (clsid == IID_IAdapter)
                {
                    *ppOut = static_cast<const IAdapter*>(this);
                }
                else if (clsid == CLSID_FrostAdapter)
                {
                    *ppOut = static_cast<const FrostAdapter*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return VSFERR_INVALIDUUID;
                }

                (*ppOut)->AddRef();
                return VSF_OK;
            }
        }

        String VOODOO_METHODTYPE FrostAdapter::ToString() const
        {
            return VSTR("FrostAdapter()");
        }

        ICore * VOODOO_METHODTYPE FrostAdapter::GetCore() const
        {
            return m_Core;
        }

        String VOODOO_METHODTYPE FrostAdapter::GetName() CONST
        {
            return VSTR("FrostAdapter");
        }

        VOODOO_METHODDEF(FrostAdapter::GetProperty)(const Uuid propid, _In_ Variant * pValue) CONST
        {
            UNREFERENCED_PARAMETER(propid);
            UNREFERENCED_PARAMETER(pValue);

            return VSFERR_NOTIMPLEMENTED;
        }

        VOODOO_METHODDEF(FrostAdapter::SetProperty)(const Uuid propid, _In_ Variant * pValue)
        {
            UNREFERENCED_PARAMETER(propid);
            UNREFERENCED_PARAMETER(pValue);

            return VSFERR_NOTIMPLEMENTED;
        }

        VoodooResult VOODOO_METHODTYPE FrostAdapter::SetEffect(IEffect * pEffect)
        {
            if (!pEffect)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_FROST_NAME, VSTR("Unable to bind null effect."));
                return VSFERR_INVALIDPARAMS;
            }

            return VSFERR_NOTIMPLEMENTED;
        }

        IEffect * VOODOO_METHODTYPE FrostAdapter::GetEffect() CONST
        {
            return nullptr;
        }

        VoodooResult VOODOO_METHODTYPE FrostAdapter::ResetEffect()
        {
            return VSFERR_NOTIMPLEMENTED;
        }

        VoodooResult VOODOO_METHODTYPE FrostAdapter::SetPass(_In_ IPass * pPass)
        {
            if (!pPass)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_FROST_NAME, VSTR("Unable to bind null pass."));
                return VSFERR_INVALIDPARAMS;
            }

            return VSFERR_NOTIMPLEMENTED;
        }

        IPass * VOODOO_METHODTYPE FrostAdapter::GetPass() CONST
        {
            return m_BoundPass.get();
        }

        VoodooResult VOODOO_METHODTYPE FrostAdapter::ResetPass()
        {
            return VSFERR_NOTIMPLEMENTED;
        }

        ITexture * VOODOO_METHODTYPE FrostAdapter::CreateTexture(_In_ const String & name, _In_ const TextureDesc pDesc)
        {
            GLuint texture;
            GLint texFmt, texIFmt, texType;

            switch (pDesc.Format)
            {
            case VSFmtRGBA8:
                texFmt = GL_RGBA;
                texIFmt = GL_RGBA8;
                texType = GL_UNSIGNED_BYTE;
                break;
            case VSFmtRGB8:
                texFmt = GL_RGB;
                texIFmt = GL_RGB8;
                texType = GL_UNSIGNED_BYTE;
                break;
            case VSFmtRGBA16F:
                texFmt = GL_RGBA;
                texIFmt = GL_RGBA16F;
                texType = GL_FLOAT;
                break;
            case VSFmtRGBA32F:
                texFmt = GL_RGBA;
                texIFmt = GL_RGBA32F;
                texType = GL_DOUBLE;
                break;
            case VSFmtD16:
                texFmt = GL_DEPTH_COMPONENT;
                texIFmt = GL_DEPTH_COMPONENT16;
                texType = GL_FLOAT;
                break;
            case VSFmtD32:
                texFmt = GL_DEPTH_COMPONENT;
                texIFmt = GL_DEPTH_COMPONENT32;
                texType = GL_DOUBLE;
                break;
            case VSFmtUnknown:
            default:
                m_Core->GetLogger()->LogMessage(VSLog_ModWarning, VOODOO_FROST_NAME, VSTR("Unable to resolve texture format."));
                return false;
            }

            glGenTextures(1, &texture);

            GLenum error = glGetError();

            while (error != GL_NO_ERROR)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModWarning, VOODOO_FROST_NAME, Format("OpenGL returned error %u: %S") << error << glGetString(error));
                error = glGetError();
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, texIFmt, pDesc.Size.X, pDesc.Size.Y, 0, texFmt, texType, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);

            error = glGetError();
            while (error != GL_NO_ERROR)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModWarning, VOODOO_FROST_NAME, Format("OpenGL returned error %u: %S") << error << glGetString(error));
                error = glGetError();
            }

            if (glIsTexture(texture))
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("OpenGL texture %u created successfully.") << texture);
            }
            else
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModDebug, VOODOO_FROST_NAME, Format("OpenGL create failed, returned texture %u.") << texture);
            }

            return new FrostTexture(m_Core, name, texture);
        }

        VoodooResult VOODOO_METHODTYPE FrostAdapter::LoadTexture(_In_ IImage * const pFile, _In_ const TextureRegion pRegion, _Inout_ ITexture * const pTexture)
        {
            UNREFERENCED_PARAMETER(pFile);
            UNREFERENCED_PARAMETER(pRegion);
            UNREFERENCED_PARAMETER(pTexture);

            return false;
        }

#pragma warning(push)
#pragma warning(disable : 6385)
        VoodooResult VOODOO_METHODTYPE FrostAdapter::DrawGeometry(_In_ const uint32_t offset, _In_ const uint32_t count, _In_ void * const pData, _In_ const VertexFlags flags)
        {
            UNREFERENCED_PARAMETER(flags);

            if (pData)
            {
                VertexDesc * pVerts = reinterpret_cast<VertexDesc*>(pData);

                glBegin(GL_QUADS);
                for (uint32_t v = offset; v < offset + count; ++v)
                {
                    glTexCoord3f(pVerts[v].TexCoord[0].X, pVerts[v].TexCoord[0].Y, pVerts[v].TexCoord[0].Z);
                    glVertex3f(pVerts[v].Position.X, pVerts[v].Position.Y, pVerts[v].Position.Z);
                }
                glEnd();
            }
            else
            {
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2d(0.0f, 0.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2d(0.0f, 250.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2d(250.0f, 250.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2d(250.0f, 0.0f);
                glEnd();
            }

            return VSF_OK;
        }
#pragma warning(pop)

        VoodooResult VOODOO_METHODTYPE FrostAdapter::BindTexture(_In_ IParameter* const pParam, _In_opt_ ITexture* const pTexture)
        {
            if (!pParam || !pTexture) return false;

            return VSFERR_NOTIMPLEMENTED;
        }

        void FrostAdapter::DrawEffect(_In_ IEffect * effect)
        {
            if (!effect)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_FROST_NAME, VSTR("Unable to draw null shader."));
                return;
            }

            ITechniqueRef tech = effect->GetDefaultTechnique();

            if (!tech)
            {
                m_Core->GetLogger()->LogMessage
                (
                    VSLog_ModError, VOODOO_FROST_NAME,
                    Format("No default technique given for shader %1%.") << effect 
                );
                return;
            }

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, 250.0, 250.0, 0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            this->SetEffect(effect);
            uint32_t passes = tech->GetPassCount();

            for (size_t curpass = 0; curpass < passes; ++curpass)
            {
                IPassRef pass = tech->GetPass(curpass);

                this->SetPass(pass.get());
                //this->DrawGeometry(0, 0, nullptr, VF_None);
                this->ResetPass();

                ITextureRef target = pass->GetTarget(0);

                if (target)
                {
                    Variant targetVar = CreateVariant();
                    if (SUCCEEDED(target->GetProperty(PropIds::OpenGLTexture, &targetVar)) && targetVar.Type == UT_UInt32)
                    {
                        GLuint passtarget = (GLuint) targetVar.VUInt32.X;

                        glBindTexture(GL_TEXTURE_2D, passtarget);
                        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
                    }
                }
            }
            this->ResetEffect();

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }

        /**
         * Frost
         */
        void FrostAdapter::SetDC(_In_opt_ HDC hdc)
        {
            m_DC = hdc;
        }

        /**
         *
         */
        void FrostAdapter::SetGLRC(_In_opt_ HGLRC hglrc)
        {
            m_GLRC = hglrc;

            if (hglrc != nullptr)
            {
                // Setup resources
                TextureDesc desc = { {256, 256, 0}, false, true, VSFmtRGBA8};

                m_TexDepthFrame = this->CreateTexture(L":depthframe", desc);
                m_TexThisFrame = this->CreateTexture(L":thisframe", desc);
                m_TexLastPass = this->CreateTexture(L":lastpass", desc);
                m_TexLastShader = this->CreateTexture(L":lastshader", desc);

                Variant textureVar = CreateVariant();
                if (SUCCEEDED(m_TexDepthFrame->GetProperty(PropIds::OpenGLTexture, &textureVar)))
                {
                    gDepthTexture = (GLint)textureVar.VInt32.X;
                }
                if (SUCCEEDED(m_TexDepthFrame->GetProperty(PropIds::OpenGLTexture, &textureVar)))
                {
                    gThisFrame = (GLint)textureVar.VInt32.X;
                }
                if (SUCCEEDED(m_TexDepthFrame->GetProperty(PropIds::OpenGLTexture, &textureVar)))
                {
                    gLastPass = (GLint)textureVar.VInt32.X;
                }
                if (SUCCEEDED(m_TexDepthFrame->GetProperty(PropIds::OpenGLTexture, &textureVar)))
                {
                    gLastShader = (GLint)textureVar.VInt32.X;
                }

                // Load shader
                IFile * shaderFile = m_Core->GetFileSystem()->GetFile(L"test.fx");
                gpTestEffect = m_Core->CreateEffect(shaderFile);
            }
            else
            {
                m_TexLastPass = nullptr;
                m_TexLastShader = nullptr;

                gpTestEffect = nullptr;
            }
        }
    }
}
