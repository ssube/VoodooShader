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
     * Creates a new core.
     * 
     * @param logfile The log file to use for the core.
     */
    _Check_return_
    VOODOO_API Core * CreateCore
    (
        _In_ const char * path
    );
        
    /**
     * Destroys an existing Core, destroying all resources and the Cg
     * context associated with the Core. This may invalidate all shaders
     * and other elements associated with Voodoo. 
     * 
     * @warning No cached Voodoo functions or resources should be used after
     *      the Core they were created with has been destroyed.
     */
    VOODOO_API void DestroyCore
    (
        _In_ Core * core
    );

    /**
     * Core engine class for the Voodoo Shader Framework. Provides a flexible, 
     * uniform API for a complete shader framework. Using various @ref Adapter 
     * "adapters", it is possible to tie the Voodoo Core into almost any game or
     * program, during or after development.
     */
    class VOODOO_API Core
    {
    public:
        /**
        * Create a new Voodoo Core and associated Cg context.
        *          
        * @param logfile The filename for this Core's log (will be used by most 
        *        managers and connected adapters).
        * @return A reference to the new core.
        *
        * @note You can not call this function externally, you must call 
        *        Core::Create(String) instead.
        * @note Avoid using more than one core at any point in time. I'm not 
        *        entirely sure how well the Cg runtime handles this. For most 
        *        games, a single render context is used, so no more than one 
        *        adapter and core should be necessary.
        * @todo Test multi-core/multi-adapter systems. If anyone has info or 
        *        knows of a game/app that uses multiple D3D/OGL render contexts, 
        *        please let me know.
        */
        Core
        (
            _In_ const char * path
        );

        /**
         * Default destructor for Voodoo @ref Core "Cores".
         */
        ~Core();

        /**
         * Retrieve the Cg context associated with this Core.
         * 
         * @note Each Voodoo Core is associated with a Cg context.
         */
        CGcontext GetCGContext();

        /**
         * Retrieve this module's version. This can be used to test
         * compatibility.
         *
         * @return The module's version info.
         */
        Version GetVersion();

        /**
         * Writes a string to the log file using standard printf syntax.
         * 
         * @param level The level for this message.
         * @param module The logging module's name.
         * @param msg The format string to use
         * @param ... Parameters to insert
         */
        void Log
        (
            _In_ LogLevel level,
            _In_ const char * module, 
            _In_ _Printf_format_string_ const char * msg, 
            ...
        );

        /**
         * Writes module version info to the log file.
         */
        void LogModule
        (
            _In_ Version version
        );

        /**
         * Gets a pointer to the this core's HookManager.
         */
         HookManager * GetHookManager();

        /**
         * Retrieve the Adapter attached to this Core.
         *
         * @return A pointer to the Adapter or NULL if no Adapter is attached.
         */
        _Check_return_
        Adapter * GetAdapter();

        /**
         * Create a new shader effect from a file. 
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
         *      is created and registered with a call to this function).
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
         * @return A shared pointer to the Texture object, if it exists 
         *         (otherwise an empty shared pointer).
         *
         * @note This will not create a texture, only retrieve an existing one.
         */
        TextureRef GetTexture
        (
            _In_ String name
        );

        /**
         * Retrieves a texture from the Core's texture map by function.
         *
         * @param function The texture type to return.
         * @return A shared reference to the Texture object if it exists,
         *        otherwise an empty reference.
         *
         * @note This will not create textures and all special textures are
         *        empty until set.
         */
        TextureRef GetTexture
        (
            _In_ TextureType function
        );

        /**
         * Assign a texture to a specialized function. This is used to bind
         * textures as the last-pass and last-shader render targets.
         *
         * @param function The function to set
         * @param texture The texture to bind
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
         * Removes a texture from the Core's texture map. 
         */
        void RemoveTexture
        (
            _In_ String texture
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
         * Retrieve a parameter by name.
         *
         * @param name The name to search for.
         * @param type The type to verify. If a parameter with a matching name
         *        is found, the type will be checked. If this is PT_Unknown, any
         *        type parameter will be returned (only the name will be tested).
         * @return A reference to the parameter, if one is found. A null shared
         *        pointer otherwise.
         */
        ParameterRef GetParameter
        (
            _In_ String name, 
            _In_ ParameterType type
        );

    //protected:
        static void CGErrorHandler
        (
            _In_ CGcontext context, 
            _In_ CGerror error, 
            _In_opt_ void * core
        );

    private:
        /**
         * Load and retrieve functions from the support libraries.
         */
        void LoadSupportLibs();
        void LoadAdapter();

        String mBasePath;
        String mAdapterName;
        String mLogName;

        HMODULE mModuleAdapter;
        HMODULE mModuleLogger;
        HMODULE mModuleHooker;
        Functions::AdapterCreate  mFuncAdapterCreate;
        Functions::AdapterDestroy mFuncAdapterDestroy;
        Functions::HookerCreate   mFuncHookerCreate;
        Functions::HookerDestroy  mFuncHookerDestroy;
        Functions::LoggerCreate   mFuncLoggerCreate;
        Functions::LoggerDestroy  mFuncLoggerDestroy;

        /**
         * Reference to the currently bound (active) adapter.
         */
        Adapter * mAdapter;

        /**
         * Reference to the current Logger.
         */
        Logger * mLogger;

        /**
         * Reference to the current HookManager.
         */
        HookManager * mHooker;

        // Manager objects:
        FullscreenManager * mManagerFS;
        MaterialManager   * mManagerMat;

        // Cg objects:
        CGcontext mCGContext;

        // Content caches:
        ShaderMap mShaders;
        TextureMap mTextures;
        ParameterMap mParameters;

        // Special textures
        TextureRef mLastPass;
        TextureRef mLastShader;
    };
}

#endif
