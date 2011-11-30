#include "Frost_Adapter.hpp"
#include "VoodooGL.hpp"

#define VOODOO_OGL_HOOK_PARAMS(x) #x, &x, &v##x

namespace VoodooShader
{
    namespace Frost
    {
        FrostAdapter::FrostAdapter(_In_ ICore * pCore) :
            m_Core(pCore), m_DC(nullptr), m_GLRC(nullptr)
        {
            gpVoodooCore = m_Core;

            ILoggerRef logger = m_Core->GetLogger();

            // Get the handles to the needed hook modules
            HMODULE procmodule = GetModuleHandle(nullptr);

            if (procmodule)
            {
                char procpath[MAX_PATH];

                GetModuleFileName(procmodule, procpath, MAX_PATH);

                logger->Log(LL_ModInfo, VOODOO_FROST_NAME, "Frost loaded into process \"%s\".", procpath);

                char *pos = strrchr(procpath, '\\');

                if (pos != nullptr)
                {
                    if (strcmp(pos + 1, "nwmain.exe") != 0)
                    {
                        logger->Log
                            (
                            LL_Mod,
                            VOODOO_FROST_NAME,
                            "The process does not appear to be the engine. Frost will not run."
                            );
                        return;
                    }
                }
            }
            else
            {
                logger->Log(LL_ModError, VOODOO_FROST_NAME, "Unable to find target module. Frost will not run.");

                return;
            }

            logger->Log(LL_ModDebug, VOODOO_FROST_NAME, "Beginning OpenGL hook procedure.");

            IHookManager* hooker = m_Core->GetHookManager();
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
                logger->Log(LL_ModInfo, VOODOO_FROST_NAME, "OpenGL hooked successfully.");
            }
            else
            {
                logger->Log(LL_ModError, VOODOO_FROST_NAME, "OpenGL hook procedure failed.");
                return;
            }

            logger->Log(LL_ModInfo, VOODOO_FROST_NAME, "Frost adapter initialized.");
        }

        /**
        *
        */
        FrostAdapter::~FrostAdapter(void)
        {

            // mLogger->Log(LL_ModInfo, VOODOO_FROST_NAME, "Frost adapter shutdown.");
        }

        /**
        * IObject
        */
        const char *FrostAdapter::GetObjectClass(void)
        {
            return "Frost_Adapter";
        }

        /**
        * IAdapter
        */
        bool FrostAdapter::LoadPass(_In_ IPass *pass)
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        /**
        *
        */
        bool FrostAdapter::UnloadPass(_In_ IPass *pass)
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        /**
        *
        */
        void FrostAdapter::BindPass(_In_ IPass* pass)
        {

            CGpass cgpass = pass->GetCgPass();

            cgSetPassState(cgpass);

            mLastPass = cgpass;
        }

        /**
        *
        */
        void FrostAdapter::UnbindPass(void)
        {
            cgResetPassState(mLastPass);
        }

        /**
        *
        */
        void FrostAdapter::DrawQuad(Vertex *vertexData)
        {
            if (vertexData)
            {
                glBegin(GL_QUADS);
                glTexCoord2f(vertexData[0].tu, vertexData[0].tv);
                glVertex3f(vertexData[0].x, vertexData[0].y, vertexData[0].z);
                glTexCoord2f(vertexData[1].tu, vertexData[1].tv);
                glVertex3f(vertexData[1].x, vertexData[1].y, vertexData[1].z);
                glTexCoord2f(vertexData[2].tu, vertexData[0].tv);
                glVertex3f(vertexData[2].x, vertexData[2].y, vertexData[2].z);
                glTexCoord2f(vertexData[3].tu, vertexData[3].tv);
                glVertex3f(vertexData[3].x, vertexData[3].y, vertexData[3].z);
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
        }

        /**
        *
        */
        void FrostAdapter::ApplyParameter(_In_ IParameter* param)
        {

            ParameterType pt = param->GetType();
            ParameterCategory pc = Converter::ToParameterCategory(pt);
            CGparameter cgparam = param->GetCgParameter();

            if (pc == PC_Float)
            {

                float *values = param->Getfloat();

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

                GLuint textureID = (GLuint) param->GetTexture()->GetData();

                cgGLSetupSampler(cgparam, textureID);
            }
            else
            {
                mLogger->Log(LL_Mod, VOODOO_FROST_NAME, "Unable to apply parameter %s.", param->ToString().GetData());
            }
        }

        /**
        *
        */
        void FrostAdapter::DrawShader(_In_ IShader* shader)
        {
            if (shader.get() == nullptr)
            {
                mLogger->Log(LL_ModError, VOODOO_FROST_NAME, "Unable to draw nullptr shader.");
                return;
            }

            ITechnique* tech = shader->GetDefaultTechnique();

            if (tech.get() == nullptr)
            {
                mLogger->Log
                    (
                    LL_ModError,
                    VOODOO_FROST_NAME,
                    "No default technique given for shader %s.",
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

                IPass* pass = tech->GetPass(curpass);

                this->BindPass(pass);
                this->DrawQuad(nullptr);
                this->UnbindPass();

                ITexture* target = pass->GetTarget();

                if (target.get())
                {

                    GLuint passtarget = (GLuint) target->GetData();

                    glBindTexture(GL_TEXTURE_2D, passtarget);
                    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
                }
            }

            ITexture* target = tech->GetTarget();

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
        *
        */
        ITexture* FrostAdapter::CreateTexture(_In_ String name, _In_ TextureDesc desc)
        {

            GLuint texture;
            GLint texFmt, texIFmt, texType;

            switch (desc.Format)
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
                Throw(VOODOO_FROST_NAME, "Unable to resolve texture format.", mCore);
            }

            glGenTextures(1, &texture);

            GLenum error = glGetError();

            while (error != GL_NO_ERROR)
            {
                mLogger->Log(LL_ModWarn, VOODOO_FROST_NAME, "OpenGL returned error %u: %s", error, glGetString(error));
                error = glGetError();
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, texIFmt, desc.Width, desc.Height, 0, texFmt, texType, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);

            error = glGetError();
            while (error != GL_NO_ERROR)
            {
                mLogger->Log(LL_ModWarn, VOODOO_FROST_NAME, "OpenGL returned error %u: %s", error, glGetString(error));
                error = glGetError();
            }

            if (glIsTexture(texture))
            {
                mLogger->Log(LL_ModDebug, VOODOO_FROST_NAME, "OpenGL texture %u created successfully.", texture);
            }
            else
            {
                mLogger->Log(LL_ModDebug, VOODOO_FROST_NAME, "OpenGL create failed, returned texture %u.", texture);
            }

            return mCore->AddTexture(name, (void *) texture);
        }

        /**
        *
        */
        bool FrostAdapter::ConnectTexture(_In_ IParameter* param, _In_ ITexture* texture)
        {
            param->Set(texture);
            cgGLSetupSampler(param->GetCgParameter(), (GLuint) texture->GetData());
            return true;
        }

        /**
        *
        */
        void FrostAdapter::HandleError(_In_ CGcontext context, _In_ CGerror error, _In_ void *core)
        {
            UNREFERENCED_PARAMETER(context);
            UNREFERENCED_PARAMETER(core);

            mLogger->Log(LL_ModError, VOODOO_FROST_NAME, "Received Cg error %u: %s", error, cgGetErrorString(error));
        }

        /**
        * Frost
        */
        void FrostAdapter::SetDC(_In_opt_ HDC hdc)
        {
            mDC = hdc;
        }

        /**
        *
        */
        void FrostAdapter::SetGLRC(_In_opt_ HGLRC hglrc)
        {
            if (hglrc != nullptr)
            {
                // Init Cg
                this->m_CgContext = cgCreateContext();

                if (!cgIsContext(this->m_CgContext))
                {
                    throw std::exception("Unable to create Cg context.");
                }

                cgSetContextBehavior(m_CgContext, CG_BEHAVIOR_LATEST);
                cgSetLockingPolicy(CG_NO_LOCKS_POLICY);

                cgSetAutoCompile(m_CgContext, CG_COMPILE_IMMEDIATE);
                cgSetParameterSettingMode(m_CgContext, CG_IMMEDIATE_PARAMETER_SETTING);

                cgGLRegisterStates(m_CgContext);
                cgGLSetManageTextureParameters(m_CgContext, CG_TRUE);

                m_Core->SetCgContext(m_CgContext);

                // Setup resources
                TextureDesc desc = { {256, 256, 0}, false, true, TF_RGBA8};

                m_TexDepthFrame = this->CreateTexture(":depthframe", desc);
                m_TexThisFrame = this->CreateTexture(":thisframe", desc);
                m_TexLastPass = this->CreateTexture(":lastpass", desc);
                m_TexLastShader = this->CreateTexture(":lastshader", desc);

                m_Core->SetStageTexture(TT_ShaderTarget, m_TexLastShader);
                m_Core->SetStageTexture(TT_PassTarget, m_TexLastPass);

                gDepthTexture = (GLint) m_TexDepthFrame->GetData();
                gThisFrame = (GLint) m_TexThisFrame->GetData();
                gLastPass = (GLint) m_TexLastPass->GetData();
                gLastShader = (GLint) m_TexLastShader->GetData();

                // Load shader
                gpTestShader = m_Core->CreateShader("test.cgfx", nullptr);
            }
            else
            {
                mCore->SetCgContext(nullptr);

                mTexLastPass = nullptr;
                mTexLastShader = nullptr;

                TestShader = nullptr;
                cgDestroyContext(mCgContext);
            }

            mGLRC = hglrc;
        }
    }
}
