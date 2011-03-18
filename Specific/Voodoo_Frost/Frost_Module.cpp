
#include "Frost_Module.hpp"

VoodooShader::Core * VoodooCore = NULL;
HookManager * VoodooHooker = NULL;

namespace VoodooShader
{
    
};

        /**
         * Install all significant OpenGL hooks. 
         *
         * @todo This function needs moved out into the DLL init process.
         */
        void HookOpenGL(void)
        {
            VoodooCore->Log("Voodoo Frost: Beginning OpenGL hook procedure.\n");

            bool success = true;

            // System-related
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glGetString));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glViewport));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglCreateContext));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglDeleteContext));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglGetProcAddress));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglMakeCurrent));

            // Shader-related
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glClear));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(wglSwapLayerBuffers));

            // Material-related
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glBindTexture));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glDeleteTextures));

            // Shader/material shared
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glBegin));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glDrawElements));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glEnd));

            // Fog-related
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glEnable));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glFogf));
            success &= VoodooHooker->InstallHook(HOOK_PARAMS(glFogfv));

            // Check the results and handle
            if ( success )
            {
                VoodooCore->Log("Voodoo Frost: OpenGL hooked successfully.\n");
            } else {
                VoodooCore->Log("Voodoo Frost: OpenGL hook procedure failed.\n");
            }
        }