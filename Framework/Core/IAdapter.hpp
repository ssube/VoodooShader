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

#include "VoodooFramework.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_interfaces
     * @{
     */

    /**
     * @class IAdapter
     *
     * Graphics adapter class, responsible for interfacing the Voodoo core with a given graphics API.
     *
     * @note This class should be implemented for each specific graphics application; some generic adapters are provided for
     *     major APIs, but in most cases application-specific behavior will make these only partially helpful.
     *
     * @sa See the @ref voodoo_spec_adapter "adapter specifications" for details on what is required of adapter modules and
     *      classes.
     *
     * @iid e6f31288-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IAdapter, IObject, ({0x88, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid refid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name IAdapter Methods
         * @{
         */
        /**
         * Set a property on the adapter.
         *
         * @param name The property name to set.
         * @param pValue The value to set the property to.
         * @return True if any value was changed.
         *
         * @note Adapters may define the meaning of any properties not given in the core adapter specification. Adapters may
         *     require a specific format or form for properties.
         *
         * @note To allow for significant variations in property setups and allow for use outside of the framework
         *      (particularly from the loader), this function takes the name as a basic string and a variant for the value.
         *
         * @warning Adapters may change values stored in the variant given, but must not change the type.
         */
        VOODOO_METHOD(SetProperty)(_In_ const wchar_t * name, _In_ Variant * const pValue) PURE;
        /**
         * Get a property from the adapter.
         *
         * @param name The property to get.
         * @param pValue Value of the property, if it exists. Variant is unchanged otherwise.
         *
         * @return Success of the property get.
         */
        VOODOO_METHOD(GetProperty)(_In_ const wchar_t * name, _Out_ Variant * const pValue) CONST PURE;
        /**
         * @}
         * @name Pass Methods
         * @{
         */
        /**
         * Loads a pass in a manner that is compatible with the adapter's underlying API. For Cg-supported APIs, this may
         * use the appropriate module loading function (cgD3D9LoadProgram, cgGLLoadProgram, etc). This may involve
         * additional compilation or linking. In some cases, the pass may not be specifically loaded and this may simply
         * return true.
         *
         * @param   pPass   The pass to be loaded.
         * @return          Whether or not the pass was successfully loaded.
         *
         * @warning         The adapter may load any combination of programs from the given pass and may ignore the 
         *                  remainder. If this call returns true, the pass may be bound to this adapter in the future. If 
         *                  this returns false, the pass may be unable to be bound.
         */
        VOODOO_METHOD(LoadPass)(_In_ IPass * const pPass) PURE;
        /**
         * Check if a pass has been loaded successfully.
         * 
         * @param   pPass   The pass to check.
         */
        VOODOO_METHOD(IsPassLoaded)(_In_ IPass * const pPass) CONST PURE;
        /**
         * Unloads a pass from the adapter.
         *
         * @param   pPass   The pass to unload.
         *
         * @warning         A pass given should not be bound after being unloaded.
         */
        VOODOO_METHOD(UnloadPass)(_In_ IPass * const pPass) PURE;
        /**
         * Causes the adapter to load a pass for drawing. The adapter may choose which programs to bind and how to handle
         * the binding, as well as managing any errors that may arise.
         *
         * @param   pPass   A shared pointer to the pass to be bound.
         * @return          Success of the binding operation.
         *
         * @note Adapters may, at their discretion, bind some or even no programs to the true graphics API. An example of 
         *      this is the Direct3D 9 adapter, which binds only vertex and fragment programs, ignoring any geometry, domain 
         *      or hull programs.
         *
         * @note The Adapter may implement passes using deferred parameters; if so it must update all relevant parameters 
         *      before binding the pass. Virtual parameters must be updated before actual, to guarantee propagation. All 
         *      parameters updates must be performed before this function returns and take effect for any draw calls coming 
         *      after this call.
         *
         * @note Each call to SetPass should have a corresponding call to ResetPass.
         */
        VOODOO_METHOD(SetPass)(_In_ IPass * const pPass) PURE;
        /**
         * Get the currently bound pass.
         */
        VOODOO_METHOD_(IPass *, GetPass)() CONST PURE;
        /**
         * Resets the state of the adapter and unbinds the pass. This may trigger state reset or texture copying, so it is
         * required for proper shader handling.
         * 
         * @param   pPass   The pass to reset. This must be the currently bound pass.
         */
        VOODOO_METHOD(ResetPass)(_In_ IPass * const pPass) PURE;
        /**
         * @}
         * @name Texture Methods
         * @{
         */
        /**
         * Creates a texture within the adapter.
         *
         * @param   name    The name of the texture, usually a fully-qualified name.
         * @param   pDesc   Description of the texture, size and format.
         * @return          The texture created, if successful; a nullptr otherwise.
         *
         * @note Only Voodoo texture formats are supported, API-specific formats are not and @b must return errors.
         *
         * @note If @a pDesc's RenderTarget flag is true, this must create a texture that can be rendered directly into or 
         *      that have the backbuffer copied efficiently into it, depending on how the adapter chooses to implement RTT.
         *
         * @warning You should not call this method directly; it should only be used via Core::CreateTexture(). This method
         *     does not set up the texture for use with the core.
         */
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ const String & name, _In_ const TextureDesc pDesc) PURE;
        /**
         * Loads a texture data from an image into an existing texture. The texture format the texture was created with and
         * format of the image given must match. Texture size is bounded by the texture, as are mipmap and render target
         * settings. Texture origin is determined by the region provided here.
         *
         * @param   pImage  The image to load data from.
         * @param   region  The region of the image to be used.
         * @param   pTexture The texture to load the data into.
         * @return          Success of the texture load.
         *
         * @note As TextureRegion derives from TextureDesc, it may be simpler to use the same struct for both this and the
         *      call to IAdapter::CreateTexture().
         */
        VOODOO_METHOD(LoadTexture)(_In_ IImage * const pImage, _In_ const TextureRegion region, _Inout_ ITexture * const pTexture) PURE;
        /**
         * Connects a texture to a sampler-type parameter. This is performed differently in each API, but often uses
         * Cg-provided functions (in OpenGL, cgGLSetTextureParameter). The parameter and texture should be connected for the
         * duration of the shader's life or until a different texture is bound to the parameter.
         *
         * @param   pParam  The parameter to bind to (must be a sampler type).
         * @param   pTexture The texture to be bound.
         * @return          Whether or not the binding was successful
         */
        VOODOO_METHOD(ConnectTexture)(_In_ IParameter * const pParam, _In_ ITexture * const pTexture) PURE;
        /**
         * @}
         * @name Geometry Methods
         * @{
         */
        /**
         * Draws geometry from system memory.
         *
         * @param   offset  A parameter with adapter-defined meaning, may be first vertex offset.
         * @param   count   A parameter with adapter-defined meaning, may be number of vertexes.
         * @param   pData   A pointer to the data to be used for vertexes, may be an adapter or buffer of VertexStructs.
         * @param   flags   Vertex flags for this data set, particularly whether the verts are pretransformed.
         * @return          Success of the draw operation.
         *
         * @note If VF_Buffer is set in flags, pData must be a buffer of some type. Otherwise, pData must be an array of
         *      VertexStructs. If VF_Transformed is set, the vertex data should be interpreted as being pretransformed;
         *      otherwise it should be taken as raw vertexes.
         */
        VOODOO_METHOD(DrawGeometry)(_In_ const uint32_t offset, _In_ const uint32_t count, _In_ void * const pData, _In_ const VertexFlags flags) PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
