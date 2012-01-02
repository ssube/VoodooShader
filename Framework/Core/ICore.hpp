/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
#pragma once

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_framework_interfaces
     * @{
     */

    /**
     * @class ICore
     *
     * ICore engine class for the Voodoo Shader Framework. Provides centralized management and handling for
     * shaders, textures, plugins and variable/configuration mechanics.
     *
     * @iid e6f31289-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(ICore, IObject, {0x89, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name Startup/Shutdown Methods
         * @{
         */
        /**
         * Initializes the core, loading the config file and all required modules and creating objects.
         *
         * @param config Name of the configuration file used to initialize this core.
         * @return Success of the initialization.
         */
        _Check_return_ VOODOO_METHOD(Initialize)(_In_ const wchar_t * const config) PURE;
        /**
         * Cleans up all modules and objects. Invalidates all objects created by this core. This method
         * provides an early cleanup (in cases where process-termination cleanup may cause crashes). If successful, this
         * core may be reinitialized.
         *
         * @return Success of the destroy operation.
         *
         * @warning If this method fails, this core is in an undefined state and must be destroyed; calling any method but
         *      AddRef or Release is undefined. All resources created by this core are immediately considered invalid.
         */
        _Check_return_ VOODOO_METHOD(Reset)() PURE;
        /**
         * @}
         * @name Core Field Methods
         * Getters and setters for core fields.
         * @{
         */
        /**
         * Retrieves this core's variable parser.
         *
         * @return A reference to the core's parser (always valid).
         */
        VOODOO_METHOD_(IParser *, GetParser)() CONST PURE;
        /**
         * Retrieves this core's IHookManager implementation.
         *
         * @return A reference to the hook manager instance (always valid).
         */
        VOODOO_METHOD_(IHookManager *, GetHookManager)() CONST PURE;
        /**
         * Retrieves this core's IFileSystem implementation.
         *
         * @return A shared pointer to the file system instance (always valid).
         */
        VOODOO_METHOD_(IFileSystem *, GetFileSystem)() CONST PURE;
        /**
         * Retrieve the IAdapter attached to this ICore.
         *
         * @return A reference to the adapter instance (always valid).
         */
        VOODOO_METHOD_(IAdapter *, GetAdapter)() CONST PURE;
        /**
         * Retrieve the ILogger attached to this ICore.
         *
         * @return A reference to the logger instance (always valid).
         */
        VOODOO_METHOD_(ILogger *, GetLogger)() CONST PURE;
        /**
         * Retrieve the Xml config document for this ICore.
         *
         * @return A reference to the config.
         */
        VOODOO_METHOD_(XmlDocument, GetConfig)() CONST PURE;
        /**
         * Retrieve the Cg context associated with this ICore.
         *
         * @return The Cg context currently associated with the core.
         *
         * @note Each Voodoo ICore is associated with a single Cg context. This context is used to create all
         *     @ref IShader shaders and most other graphics resources.
         */
        VOODOO_METHOD_(CGcontext, GetCgContext)() CONST PURE;
        /**
         * Sets the Cg context attached to the core.
         *
         * @param pContext The Cg context to bind.
         * @return False if a context is already bound, true if no context is bound or a context is being
         *     unbound.
         */
        VOODOO_METHOD(SetCgContext)(_In_opt_ CGcontext const pContext) PURE;
        /**
         * Retrieves a texture from the ICore's texture map by stage. Each specialized texture stage
         * may have a single texture bound to it for use by the shader linker.
         *
         * @param stage The stage whose bound texture should be returned.
         * @return A reference to the Texture if one is bound, empty otherwise.
         *
         * @sa To bind a texture to one of the special functions, use ICore::SetStageTexture().
         */
        VOODOO_METHOD_(ITexture *, GetStageTexture)(_In_ const TextureStage stage) CONST PURE;
        /**
         * Binds a texture to a specialized stage for the shader linker.
         *
         * @param stage The texture stage to set.
         * @param pTexture The texture to bind.
         */
        VOODOO_METHOD_(void, SetStageTexture)(_In_ const TextureStage stage, _In_opt_ ITexture * const pTexture) PURE;
        /**
         * @}
         * @name Resource Methods
         * Creator methods for the core's resource factory aspect, and retrieval and removal methods for the internal
         * resource indices.
         * @{
         */
        /**
         * Loads and compiles an effect from file, using the current file system and search paths.
         *
         * @param pFile The file to load and compile. May be an absolute or relative filename.
         * @param ppArgs Optional arguments providing compiler directives, usually shader model specific
         *     definitions or preprocessor defines.
         */
        VOODOO_METHOD_(IShader *, CreateShader)(_In_ const IFile * const pFile, _In_opt_ const char ** ppArgs) PURE;
        /**
         * Creates a global virtual parameter. This parameter exists in the Cg runtime, but is not a part of
         * any shader or program.
         *
         * This is most useful for creating parameters that must be shared between programs. Only parameters
         * created with this function may be used in Parameter::Link().
         *
         * @param name The name for this parameter.
         * @param type The type of the parameter to create.
         *
         * @return A new parameter.
         *
         * @note This function is the only way to create global parameters. You can then attach effect
         *     parameters to the global and any value changes will propagate down.
         */
        VOODOO_METHOD_(IParameter *, CreateParameter)(_In_ const String & name, _In_ const ParameterType type) PURE;
        /**
         * Registers a texture with this ICore.
         *
         * Textures will not be used by the shader linker unless they have been registered with the core.
         *
         * @param name The texture name (must be unique).
         * @param desc Information for the texture to be created.
         * @return A shared pointer to the newly created Texture object, if successful.
         *
         * @note This method calls IAdapter::CreateTexture() to handle the actual creation, then registers
         *     the returned texture with the core and sets things up properly.
         */
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ const String & name, _In_ const TextureDesc desc) PURE;
        /**
         * Retrieve a parameter by name.
         *
         * @param name The name to search for.
         * @param type The type to verify.
         * @return A reference to the parameter if found, nullptr reference otherwise.
         *
         * @note If type is PT_Unknown, no type checking is performed, only the name is used.
         */
        VOODOO_METHOD_(IParameter *, GetParameter)(_In_ const String & name, _In_ const ParameterType type) CONST PURE;
        /**
         * Retrieves a texture from the ICore's texture map by name.
         *
         * @param name The texture name.
         * @return A reference to the Texture if it exists, empty otherwise.
         */
        VOODOO_METHOD_(ITexture *, GetTexture)(_In_ const String & name) CONST PURE;
        /**
         * Removes a virtual parameter from ICore. If all references are released, the parameter is
         * destroyed.
         *
         * @param name The name of the parameter.
         * @return True if the parameter was found and removed, false if not found.
         */
        VOODOO_METHOD(RemoveParameter)(_In_ const String & name) PURE;
        /**
         * Removes a texture from the ICore's texture map and unbinds it from any
         * specialized functions it may be attached to.
         *
         * @param name The name of texture to be removed.
         * @return True if the texture was found and removed, false if not.
         */
        VOODOO_METHOD(RemoveTexture)(_In_ const String & name) PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
