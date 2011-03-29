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

#include "Parameter.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Creates a new core. This function is exported and meant for use by the loader.
     * 
     * @param path The base path to use for this core. Acts as the root for all modules and
     *        resources.
     * @return A new Core object.
     * @throws std::exception in case of errors.
     */
    _Check_return_
    VOODOO_API Core * CreateCore
    (
        _In_ const char * path,
        _In_ const char * startdir
    );
        
    /**
     * Destroys an existing Core, destroying all resources and the Cg context associated 
     * with the Core. This will invalidate all shaders and other elements using the given
     * Core. This function is exported and meant for use by the loader.
     * 
     * @param core The Core to be destroyed.
     * @warning Cached Voodoo functions or resources must not be used after the Core that
     *          created them has been destroyed.
     */
    VOODOO_API void DestroyCore
    (
        _In_ Core * core
    );

    /**
     * Core engine class for the Voodoo Shader Framework. Manages a variety of major functions
     * and contains a Cg context used by shaders.
     */
    class VOODOO_API Core
    {
    public:
        /**
        * Create a new Voodoo Core and associated Cg context.
        *          
        * @param path The base path to use for this core. Acts as the root for all modules and
        *        resources.
        * @return A reference to the new core.
        *
        * @note You can not call this function externally, you must call CreateCore() instead.
        * 
        * @todo Test multi-core/multi-adapter systems. If anyone has info or knows of a 
        *       game/app that uses multiple D3D/OGL render contexts, please let me know.
        */
        Core
        (
            _In_ const char * path,
            _In_ const char * startdir
        );

        /**
         * Destroys the associated Cg context, unloads addon modules and cleans up
         * resources created by this core. 
         * 
         * @warning Destroying a Core will invalidate any dynamically created classes, 
         *          shaders or other Cg-dependent objects, remove hooks and generally
         *          break things. This is usually called when Voodoo is unloading or
         *          the runtime is done using this Core and/or adapter.
         *          
         * @bug The call to cgDestroyContext() within this can cause a crash. Not
         *      sure why, but it only happens when there are outstanding resources.
         *      Additional resource cleanup has been implemented throughout Voodoo,
         *      test fully. This is a segfault on exit, causing a fatal crash.
         */
        ~Core();

        /**
         * Retrieve the build version for this Core. This is equivalent to the
         * <code>ModuleVersion</code> function exported by addon modules. 
         * 
         * @note This may be used by modules to test for a minimum supported core version,
         *       but version testing with this isn't foolproof. Checking the core revision
         *       should work for most things.
         * 
         * @return The core module's version info.
         */
        Version GetVersion();

        /**
         * Retrieves the base path this core was created with. 
         * 
         * @note This is not the Voodoo binary folder, but the main Voodoo framework folder 
         *       as indicated by the registry. Working from this path, you can locate the 
         *       Voodoo binary folder (append "bin\") or resource folders. 
         *       
         * @note The path given always ends in a backslash, so appending a file or folder name
         *       will create a valid path.
         *       
         * @return The core's base path.
         */
        String GetBasePath();

         /**
          * Retrieves this core's module manager.
          * 
          * @note Whenever you need to load additional modules or use dynamically
          *       loaded classes, load or create them through the module manager.
          *       
          * @return A shared pointer to the module manager (always valid).
          */
         ModuleManagerRef GetModuleManager();

        /**
         * Retrieves this core's IHookManager implementation.
         *        
         * @return A shared pointer to the hook manager or empty if none exists.
         */
         IHookManagerRef GetHookManager();

        /**
         * Retrieve the IAdapter attached to this Core.
         *
         * @return A shared pointer to the adapter or empty if no adapter is attached.
         */
        IAdapterRef GetAdapter();
        
        /**
         * Retrieve the ILogger attached to this Core.
         *
         * @return A shared pointer to the logger or empty if no logger is attached.
         */
        ILoggerRef GetLogger();

        void SetCgContext(_In_opt_ CGcontext context);

        /**
         * Retrieve the Cg context associated with this Core.
         * 
         * @note Each Voodoo Core is associated with a single Cg context. This context is used
         *       to create all @ref Shader shaders and most other graphics resources. 
         *       
         * @return The Cg context.
         */
        _Check_return_
        CGcontext GetCgContext();

        /**
         * Create a new shader effect from a file, or copies the effect if the
         * file has already been loaded.
         * 
         * @note Effects, once compiled successfully, are cached by filename. 
         *       Repeated calls to Core::CreateShader() will not recompile the 
         *       file once it has succeeded, they just copy the effect for speed.
         *       If the effect <em>fails</em> compilation, repeated calls will
         *       retry.
         *
         * @param filename The file to load and compile.
         * @param args Optional arguments providing compiler directives, usually
         *        shader model-specific definitions or preprocessor defines.
         */
        ShaderRef CreateShader
        (
            _In_ String filename, 
            _In_opt_ const char ** args
        );

        /**
         * Creates a global-level virtual parameter. This parameter exists
         * in the Cg runtime, but is not a part of any shader or program. This
         * is useful for creating parameters that must be shared between 
         * programs. Only parameters created with this function may be used in 
         * Parameter::Link().
         *
         * @param name The name for this parameter.
         * @param type The type of the parameter to create.
         * @return A new parameter.
         * @throws Exception if a parameter with the given name already exists.
         *
         * @note This function is the only way to create global parameters. You
         *        can then attach effect parameters and adjust only the global
         *        parameter (an easy way to manage system time or such things).
         */
        ParameterRef CreateParameter
        (
            _In_ String name, 
            _In_ ParameterType type
        );

        /**
         * Registers a texture with this Core. Texture will not be used by the 
         * shader linker unless they have been registered with the core. 
         *
         * @param name The texture name (must be unique).
         * @param data A pointer to any texture data to be stored. This should 
         *        be interpreted and manipulated by the Adapter, the Core merely 
         *        stores the pointer.
         * @return A shared pointer to the newly created Texture object, if 
         *        successful.
         * 
         * @throws Exception if a texture with the same name already exists.
         *
         * @sa To create a texture, see Adapter::CreateTexture() (the texture
         *      is created and registered with a call to this function). This
         *      method does not create a texture, it merely notifies the Core
         *      one exists and returns a shared pointer to the metadata.
         */
        TextureRef AddTexture
        (
            _In_ String name, 
            _In_ void * data
        );

        /**
         * Retrieves a texture from the Core's texture map by name. 
         * 
         * @param name The texture name.
         * @return A shared pointer to the Texture object if it exists, otherwise 
         *         an empty shared pointer).
         *
         * @sa To create a texture, use IAdapter::CreateTexture()
         * @sa To register an existing texture with the core, use Core::AddTexture()
         */
        TextureRef GetTexture
        (
            _In_ String name
        );

        /**
         * Retrieves a texture from the Core's texture map by function. Each
         * specialized texture function may have a single texture bound to it for
         * use by the shader linker.
         *
         * @param function The function whose bound texture should be returned.
         * @return A shared pointer to the Texture object if it exists, otherwise 
         *         an empty shared pointer.
         *         
         * @sa To bind a texture to one of the special functions, use Core::SetTexture()
         */
        TextureRef GetTexture
        (
            _In_ TextureType function
        );

        /**
         * Retrieve a parameter by name.
         *
         * @param name The name to search for.
         * @param type The type to verify. If a parameter with a matching name
         *        is found, the type will be checked. If this is PT_Unknown, any
         *        type parameter will be returned (only the name will be tested).
         * @return A reference to the parameter, if one is found. An empty shared
         *        pointer otherwise.
         */
        ParameterRef GetParameter
        (
            _In_ String name, 
            _In_ ParameterType type
        );

        //! @todo Add a Core::GetShader() function

        /**
         * Binds a texture to a specialized function for the shader linker. 
         *
         * @param function The function to set.
         * @param texture The texture to bind.
         *
         * @throws Exception if function is invalid or TT_Unknown.
         * @throws Exception if function is TT_Generic (all textures are generic by
         *          default, you cannot set a texture to generic).
         */
        void SetTexture
        (
            _In_ TextureType function, 
            _In_ TextureRef texture
        );

        /**
         * Removes a texture from the Core's texture map and unbinds it from any
         * specialized functions it may be attached to.
         * 
         * @param texture The texture to remove.
         */
        void RemoveTexture
        (
            _In_ String texture
        );

    //protected:
        /**
         * Error handling callback for the Cg context. If an internal Cg error occurs,
         * this function will be called with as much information as possible. While error
         * recovery may not be possible, this does log the error in detail (if a core is
         * provided).
         * 
         * @param context The Cg context the error occurred in.
         * @param error The error code.
         * @param core The core to use for error logging, if one was provided the Cg context
         *             during creation. This may be NULL.
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
         * The current module manager.
         */
        ModuleManagerRef mModManager;

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
