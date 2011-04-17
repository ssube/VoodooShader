/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#ifndef VOODOO_CORE_HPP
#define VOODOO_CORE_HPP

#include "Meta.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Core engine class for the Voodoo Shader Framework. Manages a variety of major functions and
     * contains a Cg context used by shaders.
     */
    DECLARE_INTERFACE_(ICore, IUnknown)
    {
        /**
         * Create a new Voodoo Core and associated Cg context.
         *          
         * @param globalroot The base path to use for this core. Acts as the root for all modules and
         *    resources.
         * @param runroot The local base path, used for some resources.
         * @return A new core.
         * @throws std::exception in case of errors.
         *
         * @note You can not call this function externally, you must call CreateCore() instead.
         */
        Core
        (
            _In_ const char * globalroot,
            _In_ const char * runroot
        );

        /**
         * Releases all references to modules and objects held by the core, including shaders,
         * textures, parameters, etc. This may cause the destruction of objects and unloading
         * of modules. It should not invalidate loaded resources that are held in other locations.
         */
        ~Core();

        /**
         * Retrieves this core's variable parser.
         * 
         * @return A shared pointer to the parser (always valid).
         */
        STDMETHOD(GetParser)(IParser ** pParser) PURE;

        /**
         * Retrieves this core's IHookManager implementation.
         *        
         * @return A shared pointer to the hook manager or empty if none exists.
         */
        STDMETHOD(GetHookManager)(_Out_ IHookManager ** ppHookManager) PURE;

         /**
          * Retrieves this core's IFileSystem implementation.
          * 
          * @return A shared pointer to the filesystem or empty if none exists.
          */
        STDMETHOD(GetFileSystem)(_Out_ IFileSystem ** ppFileSystem) PURE;

        /**
         * Retrieve the IAdapter attached to this Core.
         *
         * @return A shared pointer to the adapter or empty if no adapter is attached.
         */
        STDMETHOD(GetAdapter)(_Out_ IAdapter ** ppAdapter) PURE;
        
        /**
         * Retrieve the ILogger attached to this Core.
         *
         * @return A shared pointer to the logger or empty if no logger is attached.
         */
        STDMETHOD(GetLogger)(_Out_ ILogger ** ppLogger) PURE;

        /**
         * Retrieve the Xml config document for this Core.
         * 
         * @warning This is actually a <code>pugi::xml_document *</code>, but stored and provided
         *    as a <code>void *</code> so linking against the Core doesn't require the pugixml
         *    headers. To use this, simply cast it into the actual type.
         */
        STDMETHOD(GetConfig)(_Out_ void ** ppConfig) PURE;

        STDMETHOD(SetCgContext)(_In_ void * pContext) PURE;

        /**
         * Retrieve the Cg context associated with this Core.
         * 
         * @note Each Voodoo Core is associated with a single Cg context. This context is used
         *    to create all @ref Shader shaders and most other graphics resources. 
         *       
         * @return The Cg context.
         */
        STDMETHOD(GetCgContext)(_In_ void ** ppContext) PURE;

        /**
         * Tries to compile a new shader effect from a file, or copies the effect if the
         * file has already been loaded.
         * 
         * @param pFilename The file to load and compile.
         * @param ppArgs Optional arguments providing compiler directives, usually shader 
         *    model-specific definitions or preprocessor defines.
         */
        STDMETHOD(CreateShader)
        (
            _In_ LPCSTR pFilename, 
            _In_opt_ LPCSTR * ppArgs
            _Out_ IShader ** ppShader
        ) PURE;

        /**
         * Creates a global-level virtual parameter. This parameter exists in the Cg runtime, but 
         * is not a part of any shader or program. This is useful for creating parameters that must 
         * be shared between programs. Only parameters created with this function may be used in 
         * Parameter::Link().
         *
         * @param name The name for this parameter.
         * @param type The type of the parameter to create.
         * @return A new parameter.
         * @throws Exception if a parameter with the given name already exists.
         *
         * @note This function is the only way to create global parameters. You can then attach 
         *    effect parameters and adjust only the global parameter (an easy way to manage system 
         *    time or such things).
         */
        STDMETHOD(CreateParameter)
        (
            _In_ LPCSTR pName, 
            _In_ ParameterType Type,
            _Out_ IParameter ** ppParameter;
        ) PURE;

        /**
         * Registers a texture with this Core. Texture will not be used by the shader linker unless 
         * they have been registered with the core. 
         *
         * @param name The texture name (must be unique).
         * @param data A pointer to any texture data to be stored. This should be interpreted and 
         *    manipulated by the Adapter, the Core merely stores the pointer.
         * @return A shared pointer to the newly created Texture object, if successful.
         * 
         * @throws Exception if a texture with the same name already exists.
         *
         * @sa To create a texture, see Adapter::CreateTexture() (the texture is created and 
         *    registered with a call to this function). This  method does not create a texture, it 
         *    merely notifies the Core one exists and returns a shared pointer to the metadata.
         */
        STDMETHOD(AddTexture)
        (
            _In_ LPCSTR pName, 
            _In_ void * pData
        ) PURE;

        /**
         * Retrieves a texture from the Core's texture map by name. 
         * 
         * @param name The texture name.
         * @return A shared pointer to the Texture object if it exists, otherwise an empty shared 
         *    pointer).
         *
         * @sa To create a texture, use IAdapter::CreateTexture()
         * @sa To register an existing texture with the core, use Core::AddTexture()
         */
        STDMETHOD(GetTexture)
        (
            _In_ LPCSTR pName,
            _Out_ ITexture ** ppTexture
        ) PURE;

        /**
         * Retrieves a texture from the Core's texture map by function. Each specialized texture 
         * function may have a single texture bound to it for use by the shader linker.
         *
         * @param function The function whose bound texture should be returned.
         * @return A shared pointer to the Texture object if it exists, otherwise an empty shared 
         *    pointer.
         *         
         * @sa To bind a texture to one of the special functions, use Core::SetTexture()
         */
        STDMETHOD(GetTexture)
        (
            _In_ TextureType Function,
            _Out_ ITexutre ** ppTexture
        ) PURE;

        /**
         * Retrieve a parameter by name.
         *
         * @param name The name to search for.
         * @param type The type to verify. If a parameter with a matching name is found, the type 
         *    will be checked. If this is PT_Unknown, any type parameter will be returned (only the 
         *    name will be tested).
         * @return A reference to the parameter, if one is found. An empty shared pointer otherwise.
         */
        STDMETHOD(GetParameter)
        (
            _In_ LPCSTR pName, 
            _In_ ParameterType Type,
            _Out_ IParameter ** ppParameter
        ) PURE;

        /**
         * Binds a texture to a specialized function for the shader linker. 
         *
         * @param function The function to set.
         * @param texture The texture to bind.
         *
         * @throws Exception if function is invalid or TT_Unknown.
         * @throws Exception if function is TT_Generic (all textures are generic by default, you 
         *    cannot set a texture to generic).
         */
        STDMETHOD(SetTexture)
        (
            _In_ TextureType Function, 
            _In_opt_ ITexture * pTexture
        ) PURE;

        /**
         * Removes a texture from the Core's texture map and unbinds it from any specialized 
         * functions it may be attached to.
         * 
         * @param texture The texture to remove.
         */
        STDMETHOD(RemoveTexture)
        (
            _In_ LPCSTR pTexture
        ) PURE;

    private:
        /**
         * Error handling callback for the Cg context. If an internal Cg error occurs, this function 
         * will be called with as much information as possible. While error recovery may not be 
         * possible, this does log the error in detail (if a core is provided).
         * 
         * @param context The Cg context the error occurred in.
         * @param error The error code.
         * @param core The core to use for error logging, if one was provided the Cg context
         *    during creation. This may be NULL.
         */
        static void CgErrorHandler
        (
            _In_ CGcontext context, 
            _In_ CGerror error, 
            _In_opt_ void * core
        );

    private:
        /**
         * Base path this core was created with.
         */
        String mGlobalRoot;
        String mLocalRoot;
        String mRunRoot;
        String mTarget;

        /**
         * Config file (actually a <code>pugi::xml_document *</code>, stored as void).
         */
        void * mConfig;

        /**
         * Cg context used by this core.
         */
        CGcontext mCgContext;

        /**
         * The currently bound (active) IAdapter implementation.
         */
        IAdapterRef mAdapter;

        /**
         * The current ILogger implementation.
         */
        ILoggerRef mLogger;

        /**
         * The current IHookManager implementation.
         */
        IHookManagerRef mHooker;

        /**
         * The current IFileSystem implementation.
         */
        IFileSystemRef mFileSystem;

        /**
         * The current module manager.
         */
        ModuleManagerRef mModManager;

        /**
         * The current variable parser.
         */
        ParserRef mParser;

        /**
         * Collection of all shaders created by this core, by name with
         * shared pointers cached.
         */
        ShaderMap mShaders;

        /**
         * Collection of all usable textures.
         */
        TextureMap mTextures;

        /**
         * Collection of all virtual parameters created by this core and
         * used by the Cg context.
         */
        ParameterMap mParameters;

        /**
         * Default pass target texture for shader linker.
         */
        TextureRef mLastPass;

        /**
         * Default technique target for shader linker.
         */
        TextureRef mLastShader;
    };
    /**
     * @}
     */
}

#endif
