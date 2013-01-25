/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

#include "VoodooFramework.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_interfaces
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
    VOODOO_INTERFACE(ICore, IObject, ({0x89, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut) PURE;
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
         * @param   config  Name of the configuration file used to initialize this core.
         * @return          Success of the initialization.
         */
        _Check_return_ VOODOO_METHOD(Init)(_In_z_ CONST wchar_t * config) PURE;
        /**
         * Binds the core to a particular compiler profile. This loads and initializes the compiler, attaching it to this
         * core until the core is reset or destroyed. Compiler profiles interface the Voodoo Shader core with a given
         * hardware API, version, or profile.
         * 
         * @param   profile The compiler to use.
         * @param   count   The number of parameters passed.
         * @param   pParams Parameters for the binding.
         * 
         * @note The number and contents of @a pParams are compiler-defined. Most standard compilers take a single 
         *      parameter, with the device or context to be used.
         *
         * @pre ICore::Init()
         * @post If this call succeeds, resources may be created from this core.
         */
        _Check_return_ VOODOO_METHOD(Bind)(_In_ CONST CompilerProfile profile, _In_ CONST uint32_t count, _In_reads_(count) Variant * pParams) PURE;
        /**
         * Cleans up all modules and objects. Invalidates all objects created by this core. This method
         * provides an early cleanup (in cases where process-termination cleanup may cause crashes). If successful, this
         * core may be reinitialized.
         *
         * @return          Success of the destroy operation.
         *
         * @warning If this method fails, this core is in an undefined state and must be destroyed; calling any method but
         *      AddRef or Release is undefined. All resources created by this core are immediately considered invalid.
         *
         * @pre ICore::Init()
         * @post If this call fails, the core is in an undefined state and must be destroyed.
         */
        _Check_return_ VOODOO_METHOD(Reset)() PURE;
        /**
         * @}
         * @name Callback Methods
         * @{
         */
        /**
         * Registers a callback with this core for a particular event. This allows plugins to subscribe to various core
         * events, such as init finalization, reset, and so on. Not all of the events are directly triggered by the core,
         * this acts as a centralized event processor for the entire framework.
         *
         * @param   event   The Uuid of the event to subscribe to. 
         * @param   func    The callback function for the event.
         *
         * @warning It is not possible to type-check function pointers across module boundaries. <b>All callbacks must meet
         *      the given function prototype exactly, or the library can crash.</b> To help simplify this, the callback
         *      prototype takes a list of variants, allowing a variety of parameters while retaining safety (this will be
         *      changed to a map of variants in the future, once a map class is added).
         */
        VOODOO_METHOD(OnEvent)(_In_ CONST Uuid event, _In_ Functions::CallbackFunc func) PURE;
        /**
         * Removes a callback from an event on this core. The callback will no longer be notified when the event is called.
         *
         * @param   event   The Uuid of the event to remove from.
         * @param   func    The callback function to remove.
         *
         * @pre ICore::OnEvent() must be called with the same parameters.
         */
        VOODOO_METHOD(DropEvent)(_In_ CONST Uuid event, _In_ Functions::CallbackFunc func) PURE;
        /**
         * Broadcasts an event to all registered handlers, passing the provided argument list.
         *
         * @param   event   The Uuid of the event to call.
         * @param   count   The number of variants passed, may be 0.
         * @param   pArgs   The list of variants passed as arguments, may be nullptr if @a count is 0.
         *
         * @warning This calls all registered callbacks for the given event. The order registered callbacks will be called 
         *      in is undefined.
         * @warning Callbacks can modify the arguments, but not change the count.
         * @warning If a callback returns a failure value, the loop will be aborted and remaining callbacks will not be
         *      called. The failure will be returned by this method. If no callbacks fail, this will return generic success.
         */
        VOODOO_METHOD(CallEvent)(_In_ CONST Uuid event, _In_ CONST uint32_t count, _In_reads_opt_(count) Variant * pArgs) PURE;
        /**
         * @}
         * @name Core Field Methods
         * Getters and setters for core fields.
         * @{
         */
        /**
         * Retrieve the Xml config document for this ICore.
         *
         * @return A reference to the config.
         */
        VOODOO_METHOD_(XmlDocument, GetConfig)() CONST PURE;
        /**
         * Retrieves this core's IFileSystem implementation.
         *
         * @return A shared pointer to the file system instance (always valid).
         */
        VOODOO_METHOD_(IFileSystem *, GetFileSystem)() CONST PURE;
        /**
         * Retrieves this core's IHookManager implementation.
         *
         * @return A reference to the hook manager instance (always valid).
         */
        VOODOO_METHOD_(IHookManager *, GetHookManager)() CONST PURE;
        /**
         * Retrieve the ILogger attached to this ICore.
         *
         * @return A reference to the logger instance (always valid).
         */
        VOODOO_METHOD_(ILogger *, GetLogger)() CONST PURE;
        /**
         * Retrieves this core's variable parser.
         *
         * @return A reference to the core's parser (always valid).
         */
        VOODOO_METHOD_(IParser *, GetParser)() CONST PURE;
        /**
         * @}
         * @name Resource Methods
         * Creator methods for the core's resource factory aspect, and retrieval and removal methods for the internal
         * resource indices. Generally, these require an active binding to be used.
         * @{
         */
        /**
         * Loads and compiles an effect from file, using the current file system and search paths.
         *
         * @param   pFile   The file to load and compile.
         *
         * @pre ICore::Bind must be called successfully before use.
         */
        VOODOO_METHOD_(IEffect *, CreateEffect)(_In_ IFile * pFile) PURE;
        /**
         * Creates a global virtual parameter. This parameter exists in the core, but is not a part of any effect.
         *
         * This is most useful for creating parameters that must be shared between programs. Only parameters
         * created with this function may be used in Parameter::Link().
         *
         * @param   name    The name for this parameter.
         * @param   desc    The desc of the parameter to create.
         *
         * @pre ICore::Bind must be called successfully before use.
         *
         * @note This function creates global parameters, which are not associated with any effect. They are primarily used
         *      to propagate values down to other parameters, both global and effect.
         */
        VOODOO_METHOD_(IParameter *, CreateParameter)(_In_ CONST String & name, _In_ CONST ParameterDesc desc) PURE;
        /**
         * Registers a texture with this core.
         *
         * @param   name    The texture name (must be unique).
         * @param   desc    Information for the texture to be created.
         *
         * @pre ICore::Bind must be called successfully before use.
         *
         * @note This method calls IAdapter::CreateTexture() to handle the actual creation, then registers
         *     the returned texture with the core and sets things up properly.
         */
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ CONST String & name, _In_ CONST TextureDesc desc) PURE;
        /**
         * Loads a texture from file and registers it with this core. The texture file dictates size and format of the
         * resulting object, and only supported formats will be loaded. This relies in part on the active binding, but
         * Voodoo Shader formats have been chosen to provide the widest compatibility possible.
         * 
         * @param   name    The texture's name, used to register it with the core.
         * @param   pFile   The file to load.
         *
         * @pre ICore::Bind must be called successfully before use.
         */
        VOODOO_METHOD_(ITexture *, LoadTexture)(_In_ CONST String & name, _In_ IFile * pFile) PURE;
        /**
         * Retrieve a parameter by name.
         *
         * @param   name    The name to search for.
         * @param   desc    The desc to verify.
         *
         * @note If type is VSPT_Unknown, no type checking is performed, only the name is used.
         */
        VOODOO_METHOD_(IParameter *, GetParameter)(_In_ CONST String & name, _In_ CONST ParameterDesc desc) CONST PURE;
        /**
         * Retrieves a texture from the ICore's texture map by name.
         *
         * @param   name    The texture name.
         */
        VOODOO_METHOD_(ITexture *, GetTexture)(_In_ CONST String & name) CONST PURE;
        /**
         * Removes a virtual parameter from the core's parameter index.
         *
         * @param   name    The name of the parameter.
         * @return          Succeeds if the parameter was found and removed.
         */
        VOODOO_METHOD(RemoveParameter)(_In_ CONST String & name) PURE;
        /**
         * Removes a texture from the core's texture index.
         *
         * @param   name    The name of texture to be removed.
         * @return          Succeeds if the texture was found and removed.
         */
        VOODOO_METHOD(RemoveTexture)(_In_ CONST String & name) PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
