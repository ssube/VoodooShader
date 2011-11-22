/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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
     * @addtogroup voodoo_uuids 
     */
    DEFINE_IID(IAdapter) = {0x88, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

    /**
     * Graphics adapter class, responsible for interfacing the Voodoo core with a given graphics API.
     *
     * @note This class should be implemented for a specific graphics application; some generic adapters are provided for
     *     major APIs, but in most cases application-specific behavior will make these only partially helpful.
     *
     * @sa See the @ref voodoo_spec_adapter "adapter specifications" for details on what is required of adapter modules and
     *      classes.
     *      
     * @par IID
     *      e6f31288-05af-11e1-9e05-005056c00008
     */
    class IAdapter :
        public IObject
    {
    public:
        virtual ~IAdapter(void) throw() {};

        virtual uint32_t AddRef(void) const throw() = 0;
        virtual uint32_t Release(void) const throw() = 0;
        virtual bool QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw() = 0;
        virtual String ToString(void) const throw() = 0;
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Loads a pass in a manner that is compatible with the adapter's underlying API. For Cg-supported APIs, this may
         * use the appropriate module loading function (cgD3D9LoadProgram, cgGLLoadProgram, etc). This may involve
         * additional compilation or linking. In some cases, the pass may not be specifically loaded and this may simply
         * return true.
         *
         * @param pPass The pass to be loaded.
         * @return Whether or not the pass was successfully loaded.
         *
         * @warning There is no guarantee what programs, if any, will be loaded or how this will be performed. The only
         *     guarantee is that if this call returns true, the pass may be bound to this adapter in the future. If this
         *     returns false, the pass must not be bound.
         */
        virtual bool LoadPass(_In_ IPass * const pPass) throw() = 0;

        /**
         * Unloads a pass from the adapter.
         *
         * @param pPass The pass to unload.
         *
         * @warning There is no guarantee what will occur within this function, but the pass given should @e never be bound
         *     after being unloaded.
         */
        virtual bool UnloadPass(_In_ IPass * const pPass) throw() = 0;

        /**
         * Causes the adapter to load a pass for drawing. The adapter may choose which programs to bind and how to handle
         * the binding, as well as managing any errors that may arise.
         *
         * @param pPass A shared pointer to the pass to be bound. If this is null, the adapter must unbind the last bound
         *     pass and reset all states.
         * @return Success of the binding operation.
         *
         * @note Adapters may, at their discretion, bind some or even no programs to the true graphics API. An example of
         *     this is the Direct3D 9 adapter, which binds only vertex and fragment programs, ignoring any geometry, domain
         *     or hull programs.
         *
         * @note The Adapter may implement passes using deferred parameters; if so it @e must update all pass parameters,
         *     virtual first and actual parameters second (to make sure virtual param values get propagated). All parameters
         *     updates must be performed before this function returns and take effect for any draw calls coming after this
         *     call.
         *
         * @note Adapters must restore all states when a pass is unbound to the exact previous state. The exact
         *     implementation is left to the adapter, but DX provides some APIs to handle this rather well.
         */
        virtual bool SetPass(_In_opt_ IPass * const pPass = nullptr) throw() = 0;

        /**
         * Get the currently bound pass.
         *
         * @return The bound pass, or nullptr if none.
         */
        virtual IPass * GetPass(void) const throw() = 0;

        /**
         * Sets a render target for the adapter.
         *
         * @param index Target index to set.
         * @param pTarget Texture to use as the render target.
         * @return True if the target was successfully bound.
         *
         * @note Some texture formats, combinations of formats, textures and indices may be invalid for use as a render
         *     target. Most common formats should be supported, so long as all targets are the same format. Many cards allow
         *     up to 4 targets. Anything beyond this should not be expected to be widely supported.
         */
        virtual bool SetTarget(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget = nullptr) throw() = 0;

        /**
         * Gets a render target from the adapter.
         *
         * @param index The target index to get.
         * @returns The texture bound to the target, or nullptr if no bound texture.
         */
        virtual ITexture * GetTarget(_In_ const uint32_t index) const throw() = 0;

        /**
         * Creates a texture within the adapter and binds it to a texture.
         *
         * @param name The name of the texture, usually a fully-qualified name.
         * @param pDesc Description of the texture, size and format.
         * @param pTexture The texture the new adapter texture should be bound to.
         * @return Success of the create and bind.
         *
         * @note Only Voodoo texture formats are supported, API-specific formats are not and @b must return errors.
         *
         * @note This must create a texture that can either be rendered directly into or that can have the backbuffer copied
         *     into it efficiently, depending on how the adapter chooses to implement RTT.
         *
         * @warning You should not call this method directly; it should only be used via Core::CreateTexture(). This method
         *     does not set up the texture for use with the core.
         */
        virtual bool CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc, _Inout_ ITexture * const pTexture) throw() = 0;

        /**
         * Loads a named texture into the API and registers it with the Core. The texture source is provided here and all
         * dimensions should be drawn from that, with as little conversion as possible.
         *
         * @param pImage The image to load.
         * @param pRegion The region of the file that should be used.
         * @return The texture, if successfully created. A nullptr otherwise.
         */
        virtual bool LoadTexture(_In_ IImage * const pImage, _In_ const TextureRegion * pRegion, _Inout_ ITexture * const pTexture) throw() = 0;

        /**
         * Draws geometry from system memory.
         *
         * @param count The number of vertexes to draw.
         * @param pVertexData This must contain vertex data for the given number of verts.
         * @param flags Vertex flags for this data set, particularly whether the verts are pretransformed.
         * @return Success of the draw operation.
         */
        virtual bool DrawGeometry
        (
            _In_ const uint32_t count,
            _In_count_(count) VertexStruct * const pVertexData,
            _In_ const VertexFlags flags = VF_Transformed
        ) throw() = 0;

        /**
         * Downloads a parameter's value from system RAM to VRAM, verifying that the value on the GPU (and in use) is the
         * latest value set in the CPU memory buffer (all parameter set commands operate on the system RAM buffer for speed
         * and compatibility).
         *
         * @param pParameter The parameter to apply.
         */
        virtual bool ApplyParameter(_In_ IParameter * const pParameter) throw() = 0;

        /**
         * Set a property on the adapter.
         *
         * @param name The property name to set.
         * @param value The value to set the property to.
         * @return True if any value was changed.
         *
         * @note Adapters may define the meaning of any properties not given in the core adapter specification. Adapters may
         *     require a specific format or form for properties.
         *
         * @note To allow for significant variations in property setups and allow for use outside of the framework
         *      (particularly from the loader), this function takes the name as a basic string and a variant for the value.
         *
         * @warning Adapters <b>may</b> change values stored in the variant given. However, they must return true if so, and
         *      must verify the type is correct.
         */
        virtual bool SetProperty(_In_ const wchar_t * name, _In_ Variant & value) throw() = 0;

        /**
         * Get a property from the adapter.
         *
         * @param name The property to get.
         *
         * @return The value of the property (lexically cast to a string if necessary).
         */
        virtual Variant GetProperty(_In_ const wchar_t * name) const throw() = 0;

        /**
         * Connects a texture to a sampler-type parameter. This is performed differently in each API, but often uses
         * Cg-provided functions (in OpenGL, cgGLSetTextureParameter). The parameter and texture should be connected for the
         * duration of the shader's life or until the parameter is bound to a different texture.
         *
         * @param pParam The parameter to bind to (must be a sampler type).
         * @param pTexture The texture to be bound.
         * @return Whether or not the binding was successful
         */
        virtual bool ConnectTexture(_In_ IParameter * const pParam, _In_ ITexture * const pTexture) throw() = 0;

        /**
         * A generic error-handling callback provided to the Cg runtime. This will be called by Core::CgErrorHandler() in
         * the event of a error. This allows the adapter to internally handle any cleanup or other state changes that become
         * necessary.
         *
         * @param pContext The associated Cg context.
         * @param error The error raised.
         * @return True if the adapter has successfully handled the error internally.
         *
         * @note Not all Cg errors will call this; the core performs error validation and handles all context-related errors
         *     internally, as well as logging what data it can find. Malformed errors will not be passed to the adapter.
         *
         * @note In the case of compiler errors, the exact messages can be gotten by calling cgGetLastListing until it
         *     returns nullptr. The Core error handler does just this, @e after the adapter's error handler returns. Thus,
         *     if the adapter does not dump compiler listings, the core will. This is intended to ensure compilation errors
         *     are noted, but provide the adapter a chance to handle and potentially correct them.
         */
        virtual bool HandleError(_In_ CGcontext const pContext, _In_ uint32_t error) throw() = 0;
    };
    /**
     * @}
     */
}
