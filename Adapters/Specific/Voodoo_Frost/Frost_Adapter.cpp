#include "Frost_Adapter.hpp"
#include "VoodooGL.hpp"

#define VOODOO_OGL_HOOK_PARAMS(x) #x, &x, &v##x

namespace VoodooShader
{
    namespace Frost
    {
        Adapter::Adapter(_In_ Core * core)
            : mCore(core), mDC(NULL), mGLRC(NULL), mCgContext(NULL)
        {
            VoodooCore = mCore;
            VoodooLogger = mLogger = mCore->GetLogger();
            VoodooFrost = this;

            // Get the handles to the needed hook modules
            HMODULE procmodule = GetModuleHandle(NULL);
            if ( procmodule )
            {
                char procpath[MAX_PATH];
                GetModuleFileName(procmodule, procpath, MAX_PATH);
                
                mLogger->Log(LL_Info, VOODOO_FROST_NAME, "Frost loaded into process \"%s\".", procpath);

                char * pos = strrchr(procpath, '\\');
                if ( pos != NULL )
                {
                    if ( strcmp(pos+1, "nwmain.exe") != 0 )
                    {
                        mLogger->Log(LL_Warning, VOODOO_FROST_NAME, "The process does not appear to be the engine. Frost will not run.");
                        return;
                    }
                }
            } else {
                mLogger->Log(LL_Error, VOODOO_FROST_NAME, "Unable to find target module. Frost will not run.");

                return;
            }

            mLogger->Log(LL_Debug, VOODOO_FROST_NAME, "Beginning OpenGL hook procedure.");

            IHookManagerRef hooker = mCore->GetHookManager();
            bool success = true;

            // System-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glGetString));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glViewport));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglCreateContext));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglDeleteContext));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglGetProcAddress));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglMakeCurrent));

            // Shader-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glClear));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglSwapLayerBuffers));

            // Material-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glBindTexture));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glDeleteTextures));

            // Shader/material shared
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glBegin));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glDrawElements));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glEnd));

            // Fog-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glEnable));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glFogf));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glFogfv));

            // Check the results and handle
            if ( success )
            {
                mLogger->Log(LL_Info, VOODOO_FROST_NAME, "OpenGL hooked successfully.");
            } else {
                mLogger->Log(LL_Error, VOODOO_FROST_NAME, "OpenGL hook procedure failed.");
                return;
            }

            mLogger->Log(LL_Info, VOODOO_FROST_NAME, "Frost adapter initialized.");
        }

        Adapter::~Adapter()
        {
            //mLogger->Log(LL_Info, VOODOO_FROST_NAME, "Frost adapter shutdown.");
        }

        // IObject
        const char * Adapter::GetObjectClass()
        {
            return "Frost_Adapter";
        }

        // IAdapter
        bool Adapter::LoadPass( _In_ Pass * pass )
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        bool Adapter::UnloadPass( _In_ Pass * pass )
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        void Adapter::BindPass( _In_ PassRef pass )
        {
            CGpass cgpass = pass->GetCgPass();
            cgSetPassState(cgpass);

            mLastPass = cgpass;
        }

        void Adapter::UnbindPass()
        {
            cgResetPassState(mLastPass);
        }

        void Adapter::DrawQuad(Vertex * vertexData)
        {
            if ( vertexData )
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
            } else {
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

        void Adapter::ApplyParameter( _In_ ParameterRef param )
        {
            ParameterType pt = param->GetType();
            ParameterCategory pc = Converter::ToParameterCategory(pt);
            CGparameter cgparam = param->GetCgParameter();

            if ( pc == PC_Float )
            {
                float * values = param->GetFloat();
                switch ( pt )
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
            } else if ( pc == PC_Sampler ) {
                GLuint textureID = (GLuint)param->GetTexture()->GetData();
                cgGLSetupSampler(cgparam, textureID);
            } else {
                mLogger->Log(LL_Warning, VOODOO_FROST_NAME, "Unable to apply parameter %s.", param->GetName().c_str());
            }
        }

        void Adapter::DrawShader( _In_ ShaderRef shader )
        {
            if ( shader.get() == NULL )
            {
                mLogger->Log(LL_Error, VOODOO_FROST_NAME, "Unable to draw null shader.");
                return;
            }

            TechniqueRef tech = shader->GetDefaultTechnique();

            if ( tech.get() == NULL )
            {
                mLogger->Log(LL_Error, VOODOO_FROST_NAME, "No default technique given for shader %s.", shader->GetName().c_str());
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
            for ( size_t curpass = 0; curpass < passes; ++curpass )
            {
                PassRef pass = tech->GetPass(curpass);

                this->BindPass(pass);
                this->DrawQuad(NULL);
                this->UnbindPass();

                TextureRef target = pass->GetTarget();
                if ( target.get() )
                {
                    GLuint passtarget = (GLuint)target->GetData();
                    glBindTexture(GL_TEXTURE_2D, passtarget);
                    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
                }
            }

            TextureRef target = tech->GetTarget();
            if ( target.get() )
            {
                GLuint techtarget = (GLuint)target->GetData();
                glBindTexture(GL_TEXTURE_2D, techtarget);
                glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
            }

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }

        TextureRef Adapter::CreateTexture( _In_ String name, _In_ TextureDesc desc )
        {
            GLuint texture;
            GLint texFmt, texIFmt, texType;
            switch ( desc.Format )
            {
            case TF_RGBA8:
                texFmt = GL_RGBA; //Frost::Converter::ToGLFormat(desc.Format);
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
            GLenum error =  glGetError();
            while ( error != GL_NO_ERROR )
            {
                mLogger->Log(LL_Warning, VOODOO_FROST_NAME, "OpenGL returned error %u: %s", error, glGetString(error));
                error =  glGetError();
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, texIFmt, desc.Width, desc.Height, 0, texFmt, texType, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            error =  glGetError();
            while ( error != GL_NO_ERROR )
            {
                mLogger->Log(LL_Warning, VOODOO_FROST_NAME, "OpenGL returned error %u: %s", error, glGetString(error));
                error =  glGetError();
            }

            if ( glIsTexture(texture) )
            {
                mLogger->Log(LL_Debug, VOODOO_FROST_NAME, "OpenGL texture %u created successfully.", texture);
            } else {
                mLogger->Log(LL_Debug, VOODOO_FROST_NAME, "OpenGL create failed, returned texture %u.", texture);
            }

            return mCore->AddTexture(name, (void*)texture);
        }

        bool Adapter::ConnectTexture( _In_ ParameterRef param, _In_ TextureRef texture )
        {
            param->Set(texture);
            cgGLSetupSampler(param->GetCgParameter(), (GLuint)texture->GetData());
            return true;
        }

        void Adapter::HandleError( _In_ CGcontext context, _In_ CGerror error, _In_ void * core )
        {
            UNREFERENCED_PARAMETER(context);
            UNREFERENCED_PARAMETER(core);

            mLogger->Log(LL_Error, VOODOO_FROST_NAME, "Received Cg error %u: %s", error, cgGetErrorString(error));
        }

        // Frost
        void Adapter::SetDC(_In_opt_ HDC hdc)
        {
            mDC = hdc;
        }

        void Adapter::SetGLRC(_In_opt_ HGLRC hglrc)
        {
            if ( hglrc != NULL )
            {
                // Init Cg
                this->mCgContext = cgCreateContext();

                if ( !cgIsContext(this->mCgContext) )
                {
                    throw std::exception("Unable to create Cg context.");
                }

                cgSetContextBehavior(mCgContext, CG_BEHAVIOR_LATEST);
                cgSetLockingPolicy(CG_NO_LOCKS_POLICY);

                cgSetAutoCompile(mCgContext, CG_COMPILE_IMMEDIATE);
                cgSetParameterSettingMode(mCgContext, CG_IMMEDIATE_PARAMETER_SETTING);

                cgGLRegisterStates(mCgContext);
                cgGLSetManageTextureParameters(mCgContext, CG_TRUE);

                mCore->SetCgContext(mCgContext);

                // Setup resources

                TextureDesc desc;
                desc.Width = desc.Height = 256;
                desc.Depth = 1;
                desc.Mipmaps = false;
                desc.Format = TF_RGBA8;

                mTexDepthFrame = this->CreateTexture(":depthframe", desc);
                mTexThisFrame = this->CreateTexture(":thisframe", desc);
                mTexLastPass = this->CreateTexture(":lastpass", desc);
                mTexLastShader = this->CreateTexture(":lastshader", desc);

                mCore->SetTexture(TT_ShaderTarget, mTexLastShader);
                mCore->SetTexture(TT_PassTarget, mTexLastPass);

                gDepthTexture = (GLint)mTexDepthFrame->GetData();
                gThisFrame = (GLint)mTexThisFrame->GetData();
                gLastPass = (GLint)mTexLastPass->GetData();
                gLastShader = (GLint)mTexLastShader->GetData();

                // Load shader
                TestShader = mCore->CreateShader("test.cgfx", NULL);
                TestShader->Link();
            } else {
                mCore->SetCgContext(NULL);

                mTexLastPass = NULL;
                mTexLastShader = NULL;

                TestShader = NULL;

                //
                cgDestroyContext(mCgContext);
            }

            mGLRC = hglrc;
        }
    }
}