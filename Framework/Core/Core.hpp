/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
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
#pragma once
#include "Meta.hpp"

namespace VoodooShader
{
    /* @addtogroup VoodooCore @{ */

#ifndef VOODOO_NO_CG
    /**
     * Non-member function provided to Cg as an error handling callback. 
     * 
     * @param context The Cg context throwing the error. 
     * @param error   The error code. 
     * @param core    If non-null, the core associated with the error
     */
    void Voodoo_CgErrorHandler_Func(_In_ CGcontext pContext, _In_ CGerror Error, _In_opt_ void * pCore);
#endif

    /**
     * Creates a new core. This function is exported and meant for use by the loader.
     * 
     * @param pInitParams Setup parameters for this core. 
     * @return            A new Core object.
     * @throws std::exception in case of errors, if CatchErrors is false.
     */
    _Check_return_ 
    Core * CreateCore(_In_ InitParams *pInitParams, _In_ bool CatchErrors = true);

    /**
     * Core engine class for the Voodoo Shader Framework. Provides centralized management and handling for
     * shaders, textures, plugins and variable/configuration mechanics.
     */
    class Core
    {
    public:

        /**
         * Releases all references to modules and objects held by the core, including shaders, textures, 
         * parameters, etc. This may cause the destruction of objects and unloading of modules. 
         * 
         * @warning Resources associated with this core may be undefined or the underlying Cg objects 
         *     destroyed after the core is destroyed. They must not be used.
         */
        virtual ~Core(void);

        /**
         * Retrieves this core's variable parser. 
         * 
         * @return A reference to the core's parser (always valid).
         */
        virtual ParserRef GetParser(void);

        /**
         * Retrieves this core's IHookManager implementation. 
         * 
         * @return A reference to the hook manager instance (always valid).
         */
        virtual IHookManagerRef GetHookManager(void);

        /**
         * Retrieves this core's IFileSystem implementation. 
         * 
         * @return A shared pointer to the filesystem instance (always valid).
         */
        virtual IFileSystemRef GetFileSystem(void);

        /**
        * Retrieve the IAdapter attached to this Core. 
        * 
        * @return A reference to the adapter instance (always valid).
        */
        virtual IAdapterRef GetAdapter(void);

        /**
         * Retrieve the ILogger attached to this Core. 
         * 
         * @return A reference to the logger instance (always valid).
         */
        virtual ILoggerRef GetLogger(void);

        /**
         * Retrieve the Xml config document for this Core. 
         * 
         * @note This is actually a <code>pugi::xml_document *</code>, but stored and provided as a 
         *     <code>void *</code> so linking against the Core doesn't require the pugixml headers. To use 
         *     this, simply cast it.
         */
        virtual void * GetConfig(void);

        /**
         * Retrieve the Cg context associated with this Core. 
         * 
         * @return The Cg context currently associated with the core.
         * 
         * @note Each Voodoo Core is associated with a single Cg context. This context is used to create all
         *     @ref Shader shaders and most other graphics resources. 
         */
        _Check_return_ 
        virtual CGcontext GetCgContext(void);

        /**
         * Sets the Cg context attached to the core. 
         * 
         * @param pContext The Cg context to bind. 
         * @return False if a context is already bound, true if no context is bound or a context is being 
         *     unbound.
        */
        virtual bool SetCgContext(_In_opt_ CGcontext pContext);

        /**
         * Loads and compiles an effect from file, using the current filesystem and search paths. 
         * 
         * @param Filename The file to load and compile. May be an absolute or relative filename.
         * @param ppArgs Optional arguments providing compiler directives, usually shader model specific 
         *     definitions or preprocessor defines.
         */
        virtual ShaderRef CreateShader(_In_ String Filename, _In_opt_ const char ** ppArgs);

        /**
         * Creates a global virtual parameter. This parameter exists in the Cg runtime, but is not a part of
         * any shader or program. 
         * 
         * This is most useful for creating parameters that must be shared between programs. Only parameters
         * created with this function may be used in Parameter::Link(). 
         * 
         * @param Name The name for this parameter. 
         * @param Type The type of the parameter to create.
         * 
         * @return A new parameter. 
         * 
         * @note This function is the only way to create global parameters. You can then attach effect 
         *     parameters to the global and any value changes will propagate down.
         */
        virtual ParameterRef CreateParameter(_In_ String Name, _In_ ParameterType Type);

        /**
         * Registers a texture with this Core. 
         * 
         * Textures will not be used by the shader linker unless they have been registered with the core. 
         * 
         * @param Name The texture name (must be unique). 
         * @param Desc Information for the texture to be created.
         * @return A shared pointer to the newly created Texture object, if successful.
         * 
         * @note This method calls IAdapter::CreateTexture() to handle the actual creation, then registers 
         *     the returned texture with the core and sets things up properly.
         */
        virtual TextureRef CreateTexture(_In_ String Name, _In_ TextureDesc Desc);

        /**
         * Retrieve a parameter by name. 
         * 
         * @param Name The name to search for. 
         * @param Type The type to verify. 
         * @return A reference to the parameter if found, null reference otherwise.
         * 
         * @note If a parameter with a matching name is found, the type will be checked. If @arg Type is
         *     PT_Unknown, any type parameter will be returned (only the name will be tested). 
         */
        virtual ParameterRef GetParameter(_In_ String Name, _In_ ParameterType Type);

        /**
         * Retrieves a texture from the Core's texture map by name. 
         * 
         * @param Name The texture name. 
         * @return A reference to the Texture if it exists, empty otherwise.
         */
        virtual TextureRef GetTexture(_In_ String Name);

        /**
         * Removes a virtual parameter from Core. If all references are released, the parameter is
         * destroyed.
         * 
         * @param Name The name of the parameter.
         * @return True if the parameter was found and removed, false if not found.
         */
        virtual bool RemoveParameter(_In_ String Name);

        /**
         * Removes a texture from the Core's texture map and unbinds it from any
         * specialized functions it may be attached to. 
         * 
         * @param Name The name of texture to be removed.
         * @return True if the texture was found and removed, false if not.
         */
        virtual bool RemoveTexture(_In_ String Name);

        /**
         * Retrieves a texture from the Core's texture map by stage. Each specialized texture stage 
         * may have a single texture bound to it for use by the shader linker. 
         * 
         * @param Stage The stage whose bound texture should be returned.
         * @return A reference to the Texture if one is bound, empty otherwise. 
         * 
         * @sa To bind a texture to one of the special functions, use Core::SetStageTexture().
         */
        virtual TextureRef GetStageTexture(_In_ TextureStage Stage);

        /**
         * Binds a texture to a specialized stage for the shader linker. 
         * 
         * @param Stage The texture stage to set.
         * @param pTexture The texture to bind. 
         */
        virtual void SetStageTexture(_In_ TextureStage Stage, _In_ TextureRef pTexture);

    private:
        friend Core *CreateCore(_In_ InitParams *pInitParams);

#ifndef VOODOO_NO_CG
        friend void Voodoo_CgErrorHandler_Func(CGcontext context, CGerror error, void *core);
#endif

        /**
         * Create a new Voodoo Core and associated Cg context. 
         * 
         * @param pInitParams Setup parameters for this core. 
         * @return A new core. 
         * @throws std::exception in case of errors. 
         * 
         * @note To create a core, you must call CreateCore().
         */
        Core(_In_ InitParams * pInitParams);

        /**
         * Error handling callback for the Cg context. If an internal Cg error occurs,
         * this function will be called with as much information as possible. While error
         * recovery may not be possible, this does log the error in detail (if a pCore is
         * provided). @param context The Cg context the error occurred in. @param error
         * The error code. @param pCore The pCore to use for error logging, if one was
         * provided the Cg context during creation. This may be NULL.
         */
        void CgErrorHandler(_In_ CGcontext context, _In_ int error);

    private:
        /* Paths. */
        String m_GlobalRoot;
        String m_LocalRoot;
        String m_RunRoot;
        String m_Target;
        String m_Loader;
        String m_Config;

        /* Config file (actually a <code>pugi::xml_document *</code>, stored as void). */
        void * m_ConfigFile;

        /* Cg context used by this pCore. */
        CGcontext m_CgContext;

        /* The currently bound (active) IAdapter implementation. */
        IAdapterRef m_Adapter;

        /* The current ILogger implementation. */
        ILoggerRef m_Logger;

        /* The current IHookManager implementation. */
        IHookManagerRef m_HookManager;

        /* The current IFileSystem implementation. */
        IFileSystemRef m_FileSystem;

        /* The current module manager. */
        ModuleManagerRef m_ModuleManager;

        /* The current variable parser. */
        ParserRef m_Parser;

        /* Collection of all shaders created by this pCore. */
        ShaderVector m_Shaders;
        
        /* Collection of all usable textures. */
        TextureMap m_Textures;
        
        /**
         * Collection of all virtual parameters created by this pCore and used by the Cg
         * context.
         */
        ParameterMap m_Parameters;

        /* Default pass target texture for shader linker. */
        TextureRef m_LastPass;

        /* Default technique target for shader linker. */
        TextureRef m_LastShader;
    };
    /* @} */
}
