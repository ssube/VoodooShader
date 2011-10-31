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
 **/
#pragma once

#include "IObject.hpp"

namespace VoodooShader
{
    /** 
     * @addtogroup VoodooCore 
     * @{ 
     */

    /**
     * ICore engine class for the Voodoo Shader Framework. Provides centralized management and handling for
     * shaders, textures, plugins and variable/configuration mechanics.
     */
    class ICore :
        public IObject
    {
    public:
        /**
         * Releases all references to modules and objects held by the core, including shaders, textures, 
         * parameters, etc. This may cause the destruction of objects and unloading of modules. 
         * 
         * @warning Resources associated with this core may be undefined or the underlying Cg objects 
         *     destroyed after the core is destroyed. They must not be used.
         */
        virtual ~ICore(void) throw() {};
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Retrieves this core's variable parser. 
         * 
         * @return A reference to the core's parser (always valid).
         */
        virtual IParser * GetParser(void) const throw() = 0;

        /**
         * Retrieves this core's IHookManager implementation. 
         * 
         * @return A reference to the hook manager instance (always valid).
         */
        virtual IHookManager * GetHookManager(void) const throw() = 0;

        /**
         * Retrieves this core's IFileSystem implementation. 
         * 
         * @return A shared pointer to the filesystem instance (always valid).
         */
        virtual IFileSystem * GetFileSystem(void) const throw() = 0;

        /**
         * Retrieve the IAdapter attached to this ICore. 
         * 
         * @return A reference to the adapter instance (always valid).
         */
        virtual IAdapter * GetAdapter(void) const throw() = 0;

        /**
         * Retrieve the ILogger attached to this ICore. 
         * 
         * @return A reference to the logger instance (always valid).
         */
        virtual ILogger * GetLogger(void) const throw() = 0;

        /**
         * Retrieve the Xml config document for this ICore. 
         * 
         * @return A reference to the config.
         */
        virtual XmlDocument GetConfig(void) const throw() = 0;

        /**
         * Retrieve the Cg context associated with this ICore. 
         * 
         * @return The Cg context currently associated with the core.
         * 
         * @note Each Voodoo ICore is associated with a single Cg context. This context is used to create all
         *     @ref Shader shaders and most other graphics resources. 
         */
        virtual CGcontext GetCgContext(void) const throw() = 0;

        /**
         * Sets the Cg context attached to the core. 
         * 
         * @param pContext The Cg context to bind. 
         * @return False if a context is already bound, true if no context is bound or a context is being 
         *     unbound.
         */
        virtual bool SetCgContext(_In_opt_ CGcontext const pContext) throw() = 0;

        /**
         * Loads and compiles an effect from file, using the current filesystem and search paths. 
         * 
         * @param Filename The file to load and compile. May be an absolute or relative filename.
         * @param ppArgs Optional arguments providing compiler directives, usually shader model specific 
         *     definitions or preprocessor defines.
         */
        virtual IShader * CreateShader(_In_ const IFile * const pFile, _In_opt_ const char ** ppArgs) throw() = 0;

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
        virtual IParameter * CreateParameter(_In_ const String & name, _In_ const ParameterType type) throw() = 0;

        /**
         * Registers a texture with this ICore. 
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
        virtual ITexture * CreateTexture(_In_ const String & name, _In_ const TextureDesc * const pDesc) throw() = 0;

        /**
         * Retrieve a parameter by name. 
         * 
         * @param Name The name to search for. 
         * @param Type The type to verify. 
         * @return A reference to the parameter if found, nullptr reference otherwise.
         * 
         * @note If a parameter with a matching name is found, the type will be checked. If @arg Type is
         *     PT_Unknown, any type parameter will be returned (only the name will be tested). 
         */
        virtual IParameter * GetParameter(_In_ const String & name, _In_ const ParameterType type) const throw() = 0;

        /**
         * Retrieves a texture from the ICore's texture map by name. 
         * 
         * @param Name The texture name. 
         * @return A reference to the Texture if it exists, empty otherwise.
         */
        virtual ITexture * GetTexture(_In_ const String & name) const throw() = 0;

        /**
         * Removes a virtual parameter from ICore. If all references are released, the parameter is
         * destroyed.
         * 
         * @param Name The name of the parameter.
         * @return True if the parameter was found and removed, false if not found.
         */
        virtual bool RemoveParameter(_In_ const String & name) throw() = 0;

        /**
         * Removes a texture from the ICore's texture map and unbinds it from any
         * specialized functions it may be attached to. 
         * 
         * @param Name The name of texture to be removed.
         * @return True if the texture was found and removed, false if not.
         */
        virtual bool RemoveTexture(_In_ const String & name) throw() = 0;

        /**
         * Retrieves a texture from the ICore's texture map by stage. Each specialized texture stage 
         * may have a single texture bound to it for use by the shader linker. 
         * 
         * @param stage The stage whose bound texture should be returned.
         * @return A reference to the Texture if one is bound, empty otherwise. 
         * 
         * @sa To bind a texture to one of the special functions, use ICore::SetStageTexture().
         */
        virtual ITexture * GetStageTexture(_In_ const TextureStage stage) const throw() = 0;

        /**
         * Binds a texture to a specialized stage for the shader linker. 
         * 
         * @param Stage The texture stage to set.
         * @param pTexture The texture to bind. 
         */
        virtual void SetStageTexture(_In_ const TextureStage stage, _In_ ITexture * const pTexture) throw() = 0;
    };
    /**
     * @}
     */
}
