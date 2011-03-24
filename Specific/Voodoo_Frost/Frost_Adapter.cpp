#include "Frost_Adapter.hpp"
#include "VoodooGL.hpp"

#define VOODOO_OGL_HOOK_PARAMS(x) #x, &x, &v##x

namespace VoodooShader
{
    namespace Frost
    {
        Adapter::Adapter(_In_ Core * core)
            : mCore(core), mDC(NULL), mGLRC(NULL)
        {
            mCore->Log(LL_Debug, VOODOO_FROST_NAME, "Beginning OpenGL hook procedure.");

            IHookManager * VoodooHooker = mCore->GetHookManager();
            bool success = true;

            // System-related
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glGetString));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glViewport));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglCreateContext));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglDeleteContext));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglGetProcAddress));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglMakeCurrent));

            // Shader-related
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glClear));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglSwapLayerBuffers));

            // Material-related
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glBindTexture));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glDeleteTextures));

            // Shader/material shared
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glBegin));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glDrawElements));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glEnd));

            // Fog-related
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glEnable));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glFogf));
            success &= VoodooHooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glFogfv));

            // Check the results and handle
            if ( success )
            {
                VoodooCore->Log(LL_Info, VOODOO_FROST_NAME, "OpenGL hooked successfully.");
            } else {
                VoodooCore->Log(LL_Error, VOODOO_FROST_NAME, "OpenGL hook procedure failed.");
            }
        }

        Adapter::~Adapter()
        {

        }

        // IObject
        void Adapter::DestroyObject()
        {
            delete this;
        }

        const char * Adapter::GetObjectClass()
        {
            return "Frost_Adapter";
        }

        // IAdapter
        bool Adapter::LoadPass( _In_ Pass * pass )
        {
            if ( pass == NULL )
            {
                mCore->Log(LL_Error, VOODOO_FROST_NAME, "Unable to load null pass.");
            }

            CGprogram vert = pass->GetProgram(PS_Vertex);
            CGprogram frag = pass->GetProgram(PS_Fragment);

            cgGLLoadProgram(vert);
            cgGLLoadProgram(frag);

            return true;
        }

        void Adapter::BindPass( _In_ PassRef pass )
        {
            mBoundVert = pass->GetProgram(PS_Vertex);
            mBoundFrag = pass->GetProgram(PS_Fragment);

            cgGLBindProgram(mBoundVert);
            cgGLBindProgram(mBoundFrag);
        }

        void Adapter::UnbindPass()
        {
            //cgGLUnbindProgram(CGprofile::mBoundVert);
            //cgGLUnbindProgram(mBoundFrag);
        }

        void Adapter::DrawQuad(Vertex * vertexData)
        {
            glBegin(GL_QUADS);
            glVertex3f(vertexData[0].x, vertexData[0].y, vertexData[0].z);
            glTexCoord2f(vertexData[0].tu, vertexData[0].tv);
            glVertex3f(vertexData[1].x, vertexData[1].y, vertexData[1].z);
            glTexCoord2f(vertexData[1].tu, vertexData[1].tv);
            glVertex3f(vertexData[2].x, vertexData[2].y, vertexData[2].z);
            glTexCoord2f(vertexData[2].tu, vertexData[0].tv);
            glVertex3f(vertexData[3].x, vertexData[3].y, vertexData[3].z);
            glTexCoord2f(vertexData[3].tu, vertexData[3].tv);
            glEnd();
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
                GLuint textureID = (GLuint)param->GetTexture()->GetTexture();
                cgGLSetupSampler(cgparam, textureID);
            } else {
                mCore->Log(LL_Warning, VOODOO_FROST_NAME, "Unable to apply parameter %s.", param->GetName().c_str());
            }
        }

        void Adapter::DrawShader( _In_ ShaderRef shader )
        {
            TechniqueRef tech = shader->GetDefaultTechnique();

            size_t passes = tech->GetPassCount();
            for ( size_t curpass = 0; curpass < passes; ++curpass )
            {
                PassRef pass = tech->GetPass(curpass);

                this->BindPass(pass);
                this->DrawQuad(NULL);
                this->UnbindPass();
            }
        }

        TextureRef Adapter::CreateTexture( _In_ String name, _In_ TextureDesc desc )
        {
            GLuint texture;
            GLint internalFormat = GL_RGBA8; //Frost::Converter::ToGLFormat(desc.Format);
            void * data = malloc(desc.Width * desc.Height * 4);
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, desc.Width, desc.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glBindTexture(GL_TEXTURE_2D, 0);
            free(data);

            return mCore->AddTexture(name, (void*)texture);
        }

        bool Adapter::ConnectTexture( _In_ ParameterRef param, _In_ TextureRef texture )
        {
            cgGLSetupSampler(param->GetCgParameter(), (GLuint)texture->GetTexture());
            return true;
        }

        void Adapter::HandleError( _In_ CGcontext context, _In_ CGerror error, _In_ void * core )
        {
            if ( core )
            {
                Core::CgErrorHandler(context, error, core);
            }
        }

        // Frost

        void Adapter::SetDC(_In_ HDC hdc)
        {
            mDC = hdc;
        }

        void Adapter::SetGLRC(_In_ HGLRC hglrc)
        {
            mGLRC = hglrc;

            cgGLRegisterStates(mCore->GetCgContext());
        }
    }
}