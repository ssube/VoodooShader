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

#include "Frost_Adapter.hpp"

#include "Frost_Texture.hpp"

#include "VoodooGL.hpp"

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
            logger->Log(LL_ModDebug, VOODOO_FROST_NAME, L"Beginning OpenGL hook procedure.");

            IHookManagerRef hooker = m_Core->GetHookManager();
            bool success = true;

            // System-related
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glGetString));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glViewport));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglCreateContext));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglDeleteContext));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglGetProcAddress));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglMakeCurrent));

            // Shader-related
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glClear));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(wglSwapLayerBuffers));

            // Material-related
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glBindTexture));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glDeleteTextures));

            // Shader/material shared
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glBegin));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glDrawElements));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glEnd));

            // Fog-related
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glEnable));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glFogf));
            success &= hooker->Add(VOODOO_OGL_HOOK_PARAMS(glFogfv));

            // Check the results and handle
            if (success)
            {
                logger->Log(LL_ModInfo, VOODOO_FROST_NAME, L"OpenGL hooked successfully.");
            }
            else
            {
                logger->Log(LL_ModError, VOODOO_FROST_NAME, L"OpenGL hook procedure failed.");
                return;
            }

            logger->Log(LL_ModInfo, VOODOO_FROST_NAME, L"Frost adapter initialized.");
        }

        FrostAdapter::~FrostAdapter(void)
        {

            // mLogger->Log(LL_ModInfo, VOODOO_FROST_NAME, "Frost adapter shutdown.");
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

        bool VOODOO_METHODTYPE FrostAdapter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_FrostAdapter;
                    return true;
                } else {
                    return false;
                }
            } else {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                } else if (clsid == IID_IAdapter) {
                    *ppOut = static_cast<const IAdapter*>(this);
                } else if (clsid == CLSID_FrostAdapter) {
                    *ppOut = static_cast<const FrostAdapter*>(this);
                } else {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String VOODOO_METHODTYPE FrostAdapter::ToString() const
        {
            return L"FrostAdapter()";
        }

        ICore * VOODOO_METHODTYPE FrostAdapter::GetCore() const
        {
            return m_Core;
        }

        bool VOODOO_METHODTYPE FrostAdapter::LoadPass(_In_ IPass *pass)
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        bool VOODOO_METHODTYPE FrostAdapter::UnloadPass(_In_ IPass *pass)
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        bool VOODOO_METHODTYPE FrostAdapter::SetPass(_In_ IPass * pPass)
        {
            if (!pPass)
            {
                m_Core->GetLogger()->Log(LL_ModError, VOODOO_FROST_NAME, L"Unable to bind null pass.");
                return false;
            }

            CGpass cgpass = pPass->GetCgPass();

            cgSetPassState(cgpass);

            m_BoundPass = pPass;
        }

        IPass * VOODOO_METHODTYPE FrostAdapter::GetPass() CONST
        {
            return m_BoundPass.get();
        }

        bool VOODOO_METHODTYPE FrostAdapter::ResetPass(_In_ IPass * pPass)
        {
            cgResetPassState(pPass->GetCgPass());
        }

        bool VOODOO_METHODTYPE FrostAdapter::SetTarget(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget)
        {
            UNREFERENCED_PARAMETER(index);
            UNREFERENCED_PARAMETER(pTarget);

            // Set render target (somewhat complex for OGL)
            return true;
        }

        ITexture * VOODOO_METHODTYPE FrostAdapter::GetTarget(uint32_t index) CONST
        {
            UNREFERENCED_PARAMETER(index);

            return nullptr;
        }

        ITexture * VOODOO_METHODTYPE FrostAdapter::CreateTexture(_In_ const String & name, _In_ const TextureDesc pDesc)
        {
            GLuint texture;
            GLint texFmt, texIFmt, texType;

            switch (pDesc.Format)
            {
            case TF_RGBA8:
                texFmt = GL_RGBA; // Frost::Converter::ToGLFormat(desc.Format);
                ///
                texIFmt = GL_RGBA8;
                texType = GL_UNSIGNED_BYTE;
                break;

            case TF_RGB8:
                texFmt = GL_RGB;
                texIFmt = GL_RGB8;
                texType = GL_UNSIGNED_BYTE;
                break;

            case TF_RGBA16F:
                texFmt = GL_RGBA;
                texIFmt = GL_RGBA16F;
                texType = GL_FLOAT;
                break;

            case TF_RGBA32F:
                texFmt = GL_RGBA;
                texIFmt = GL_RGBA32F;
                texType = GL_DOUBLE;
                break;

            case TF_D16:
                texFmt = GL_DEPTH_COMPONENT;
                texIFmt = GL_DEPTH_COMPONENT16;
                texType = GL_FLOAT;
                break;
            case TF_D32:
                texFmt = GL_DEPTH_COMPONENT;
                texIFmt = GL_DEPTH_COMPONENT32;
                texType = GL_DOUBLE;
                break;
            case TF_Unknown:
            default:
                m_Core->GetLogger()->Log(LL_ModWarn, VOODOO_FROST_NAME, L"Unable to resolve texture format.");
                return false;
            }

            glGenTextures(1, &texture);

            GLenum error = glGetError();

            while (error != GL_NO_ERROR)
            {
                m_Core->GetLogger()->Log(LL_ModWarn, VOODOO_FROST_NAME, L"OpenGL returned error %u: %S", error, glGetString(error));
                error = glGetError();
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, texIFmt, pDesc.Size.X, pDesc.Size.Y, 0, texFmt, texType, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);

            error = glGetError();
            while (error != GL_NO_ERROR)
            {
                m_Core->GetLogger()->Log(LL_ModWarn, VOODOO_FROST_NAME, L"OpenGL returned error %u: %S", error, glGetString(error));
                error = glGetError();
            }

            if (glIsTexture(texture))
            {
                m_Core->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"OpenGL texture %u created successfully.", texture);
            }
            else
            {
                m_Core->GetLogger()->Log(LL_ModDebug, VOODOO_FROST_NAME, L"OpenGL create failed, returned texture %u.", texture);
            }

            return new FrostTexture(m_Core, name, texture);
        }

        bool VOODOO_METHODTYPE FrostAdapter::LoadTexture(_In_ IImage * const pFile, _In_ const TextureRegion pRegion, _Inout_ ITexture * const pTexture)
        {
            UNREFERENCED_PARAMETER(pFile);
            UNREFERENCED_PARAMETER(pRegion);
            UNREFERENCED_PARAMETER(pTexture);

            return false;
        }

        bool VOODOO_METHODTYPE FrostAdapter::DrawGeometry(_In_ const uint32_t offset, _In_ const uint32_t count, _In_ void * const pData, _In_ const VertexFlags flags)
        {
            UNREFERENCED_PARAMETER(flags);

            if (pData)
            {
                VertexStruct * pVerts = reinterpret_cast<VertexStruct*>(pData);

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

            return true;
        }

        bool VOODOO_METHODTYPE FrostAdapter::ApplyParameter(_In_ IParameter * pParam)
        {
            ParameterType pt = pParam->GetType();
            ParameterCategory pc = Converter::ToParameterCategory(pt);
            CGparameter cgparam = pParam->GetCgParameter();

            if (pc == PC_Float)
            {
                float * values = pParam->GetScalar();

                switch (pt)
                {
                case PT_Float1:
                    cgGLSetParameter1fv(cgparam, values);
                    break;

                case PT_Float2:
                    cgGLSetParameter2fv(cgparam, values);
                    break;

                case PT_Float3:
                    cgGLSetParameter3fv(cgparam, values);
                    break;

                case PT_Float4:
                    cgGLSetParameter4fv(cgparam, values);
                    break;
                }
            }
            else if (pc == PC_Sampler)
            {

                GLuint textureID = (GLuint) pParam->GetTexture()->GetData();

                cgGLSetupSampler(cgparam, textureID);
            }
            else
            {
                m_Core->GetLogger()->Log(LL_ModError, VOODOO_FROST_NAME, L"Unable to apply parameter %s.", pParam->ToString().GetData());
            }
        }

        bool VOODOO_METHODTYPE FrostAdapter::SetProperty(const wchar_t * name, Variant * pValue)
        {
            UNREFERENCED_PARAMETER(name);
            UNREFERENCED_PARAMETER(pValue);

            return false;
        }

        bool VOODOO_METHODTYPE FrostAdapter::GetProperty(const wchar_t * name, Variant * pValue) const
        {
            UNREFERENCED_PARAMETER(name);
            UNREFERENCED_PARAMETER(pValue);

            return false;
        }

        bool VOODOO_METHODTYPE FrostAdapter::ConnectTexture(_In_ IParameter* const pParam, _In_opt_ ITexture* const pTexture)
        {
            if (!pParam) return false;

            pParam->SetTexture(pTexture);
            if (pTexture)
            {
                cgGLSetupSampler(pParam->GetCgParameter(), (GLuint) pTexture->GetData());
            } else {
                cgGLSetupSampler(pParam->GetCgParameter(), 0);
            }
            return true;
        }

        bool VOODOO_METHODTYPE FrostAdapter::HandleError(CGcontext pContext, uint32_t error)
        {
            UNREFERENCED_PARAMETER(pContext);
            UNREFERENCED_PARAMETER(error);

            return false;
        }

        void FrostAdapter::DrawShader(_In_ IShader* shader)
        {
            if (!shader)
            {
                m_Core->GetLogger()->Log(LL_ModError, VOODOO_FROST_NAME, L"Unable to draw nullptr shader.");
                return;
            }

            ITechniqueRef tech = shader->GetDefaultTechnique();

            if (tech.get() == nullptr)
            {
                m_Core->GetLogger()->Log
                    (
                    LL_ModError,
                    VOODOO_FROST_NAME,
                    L"No default technique given for shader %s.",
                    shader->ToString().GetData()
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

            size_t passes = tech->GetPassCount();

            for (size_t curpass = 0; curpass < passes; ++curpass)
            {
                IPassRef pass = tech->GetPass(curpass);

                this->SetPass(pass.get());
                //this->DrawGeometry(0, 0, nullptr, VF_None);
                this->ResetPass(pass.get());

                ITextureRef target = pass->GetTarget(0);

                if (target.get())
                {
                    GLuint passtarget = (GLuint) target->GetData();

                    glBindTexture(GL_TEXTURE_2D, passtarget);
                    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
                }
            }

            ITextureRef target = tech->GetTarget();

            if (target.get())
            {

                GLuint techtarget = (GLuint) target->GetData();

                glBindTexture(GL_TEXTURE_2D, techtarget);
                glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
            }

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
                // Init Cg
                CGcontext context = cgCreateContext();

                if (!cgIsContext(context))
                {
                    m_Core->GetLogger()->Log(LL_ModError, VOODOO_FROST_NAME, L"Unable to create Cg context.");
                    return;
                }

                m_Core->SetCgContext(context);

                cgSetContextBehavior(context, CG_BEHAVIOR_LATEST);
                cgSetLockingPolicy(CG_NO_LOCKS_POLICY);

                cgSetAutoCompile(context, CG_COMPILE_IMMEDIATE);
                cgSetParameterSettingMode(context, CG_IMMEDIATE_PARAMETER_SETTING);

                cgGLRegisterStates(context);
                cgGLSetManageTextureParameters(context, CG_TRUE);

                m_Core->SetCgContext(context);

                // Setup resources
                TextureDesc desc = { {256, 256, 0}, false, true, TF_RGBA8};

                m_TexDepthFrame = this->CreateTexture(L":depthframe", desc);
                m_TexThisFrame = this->CreateTexture(L":thisframe", desc);
                m_TexLastPass = this->CreateTexture(L":lastpass", desc);
                m_TexLastShader = this->CreateTexture(L":lastshader", desc);

                m_Core->SetStageTexture(TS_Shader, m_TexLastShader.get());
                m_Core->SetStageTexture(TS_Pass, m_TexLastPass.get());

                gDepthTexture = (GLint) m_TexDepthFrame->GetData();
                gThisFrame = (GLint) m_TexThisFrame->GetData();
                gLastPass = (GLint) m_TexLastPass->GetData();
                gLastShader = (GLint) m_TexLastShader->GetData();

                // Load shader
                IFile * shaderFile = m_Core->GetFileSystem()->GetFile(L"test.cgfx");
                gpTestShader = m_Core->CreateShader(shaderFile, nullptr);
            }
            else
            {
                m_Core->SetCgContext(nullptr);

                m_TexLastPass = nullptr;
                m_TexLastShader = nullptr;

                gpTestShader = nullptr;
            }
        }
    }
}
