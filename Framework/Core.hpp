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

#include "Exception.hpp"
#include "Logger.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Parameter.hpp"

namespace VoodooShader
{
    /**
     * Core engine class for the Voodoo Shader Framework. Provides a flexible, 
     * uniform API for a complete shader framework. Using various @ref Adapter 
     * "adapters", it is possible to tie the Voodoo Core into almost any game or
     * program, during or after development.
     */
    class VOODOO_API Core
    {
    public:
        __checkReturn
        static Core * Create
        (
            __in_opt String logfile = "Voodoo.log"
        );

        static void Destroy
        (
            __in __notnull Core * core
        );

        /**
         * Retrieve the Cg context associated with this Core.
         * 
         * @note Each Voodoo Core is associated with exactly one Cg context 
         *        (Voodoo acts as an OO-wrapper around Cg with enhanced features).
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
         * Enable or disable debug mode at runtime.
         *
         * @param mode The desired mode
         *
         * @note This function only has an effect in release builds, in debug
         *        builds debug mode is always on.
         */
        void SetDebug
        (
            __in bool mode
        );

        /**
         * Writes a string to the log file using standard printf syntax.
         * 
         * @param msg The format string to use
         * @param ... Parameters to insert
         */
        void Log
        (
            __in __notnull const char * msg, 
            ...
        );

        /**
         * Writes a string to the log file, if the core is in debug mode.
         *
         * @param msg The format string to use
         * @param ... Parameters
         *
         * @note In non-debug builds, this will not log anything.
         */
        void Debug
        (
            __in __notnull const char * msg, 
            ...
        );

        /**
         * Attach an Adapter to this Core.
         *
         * @param adapter The Adapter to attach (will be used for all graphics 
         *        calls).
         * @throws Exception if an Adapter is already connected to this Core.
         */
        void SetAdapter
        (
            __in __readonly __maybenull Adapter * adapter
        );

        /**
         * Retrieve the Adapter attached to this Core.
         *
         * @return A pointer to the Adapter or NULL if no Adapter is attached.
         */
        __checkReturn
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
            __in String filename, 
            __in __readonly __maybenull const char ** args
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
         * @warning This should be called only from Adapter::CreateTexture().
         */
        TextureRef CreateTexture
        (
            __in String name, 
            __in __readonly __notnull void * data
        );

        /**
         * Retrieves a texture from the Core's texture map by name. 
         * 
         * @param name The texture name.
         * @return A shared pointer to the Texture object, if it exists 
         *        (otherwise an empty    shared pointer).
         *
         * @note This will not create a texture, only retrieve an existing one.
         */
        TextureRef GetTexture
        (
            __in String name
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
            __in TextureType function
        );

        /**
         * Assign a texture to a specialized function. This is used to bind
         * textures as the last-pass and last-shader render targets.
         *
         * @param function The function to set
         * @param texture The texture to bind
         */
        void SetTexture
        (
            __in TextureType function, 
            __in TextureRef texture
        );

        /**
         * Removes a texture from the Core's texture map. 
         */
        void RemoveTexture
        (
            __in TextureRef texture
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
            __in String name, 
            __in ParameterType type
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
            __in String name, 
            __in ParameterType type
        );

    //protected:
        static void CGErrorHandler
        (
            __in CGcontext context, 
            __in CGerror error, 
            __in __maybenull void * core
        );

    private:
        /**
        * Create a new Voodoo Core and associated Cg context.
        *          
        * @param logfile The filename for this Core's log (will be used by most 
        *        managers and connected adapters).
        * @return A reference to the new core.
        *
        * @note Avoid using more than one core at any point in time. I'm not 
        *        entirely sure how well the Cg runtime handles this. For most 
        *        games, a single render context is used, so no more than one 
        *        adapter and core should be necessary.
        * @note You can not call this function externally, you must call 
        *        Core::Create(String) instead.
        * @todo Test multi-core/multi-adapter systems. If anyone has info or 
        *        knows of a game/app that uses multiple D3D/OGL render contexts, 
        *        please let me know.
        */
        Core
        (
            __in String logfile
        );

        /**
         * Default destructor for Voodoo @ref Core "Cores".
         */
        ~Core();

        /**
         * Reference to the currently bound (active) adapter.
         */
        Adapter * mAdapter;

        /**
         * Reference to the current log file. This uses Voodoo's internal
         * Logger class at this time, but can be changed to use any similar
         * class (XML logging will be made available in the near future).
         *
         * @todo Implement an XML logger.
         */
        Logger * mLogger;

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

        // Internal flags
        bool mDebugMode;
    };
}

#endif
