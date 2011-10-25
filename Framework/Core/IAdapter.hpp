/**
 * This file is part of the Voodoo IShader Framework, a comprehensive shader support library. 
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

#include "IObject.hpp"

namespace VoodooShader
{
    /**
    * Graphics adapter class, responsible for interfacing the Voodoo core with a given graphics API. 
    * 
    * @note This class should be implemented for a specific graphics application; some generic adapters are provided for 
    *     major APIs, but in most cases application-specific behavior will make these only partially helpful. 
    * 
    * @sa See the @ref adapterspec "adapter specifications" for details on what is required of adapter modules and classes.
    */
    class IAdapter :
        public IObject
    {
    public:
        virtual ~IAdapter(void) throw() { };
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual int32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual int32_t Release(void) const throw() = 0;

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
         * Loads a pass in a manner that is compatible with the adapter's underlying API. For Cg-supported APIs, this may 
         * use the appropriate module loading function (cgD3D9LoadProgram, cgGLLoadProgram, etc). This may involve additional
         * compilation or linking. In some cases, the pass may not be specifically loaded and this may simply return true. 
         * 
         * @param Pass The pass to be loaded.
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
         * @param Pass The pass to unload. 
         * 
         * @warning There is no guarantee what will occur within this function, but the pass given should @e never be bound 
         *     after being unloaded.
         */
        virtual bool UnloadPass(_In_ IPass * const pPass) throw() = 0;

        /**
         * Causes the adapter to load a pass for drawing. The adapter may choose which programs to bind and how to handle 
         * the binding, as well as managing any errors that may arise. 
         * 
         * @param pass A shared pointer to the pass to be bound. If this is null, the adapter must unbind the last bound
         *     pass and reset all states.
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
        virtual void SetPass(_In_opt_ IPass * const pPass) throw() = 0;

        /**
         * Get the currently bound pass.
         * 
         * @return The bound pass, or nullptr if none.
         */
        virtual IPass * GetPass(void) const throw() = 0;

        /**
         * Sets a render target for the adapter.
         * 
         * @param Index Target index to set. 
         * @param Target Texture to use as the render target. 
         * @return True if the target was successfully bound. 
         * 
         * @note Some texture formats, combinations of formats, textures and indices may be invalid for use as a render 
         *     target. Most common formats should be supported, so long as all targets are the same format. Many cards allow 
         *     up to 4 targets. Anything beyond this should not be expected to be widely supported.
         */
        virtual bool SetTarget(_In_ uint32_t index, _In_opt_ ITexture * const pTarget) throw() = 0;

        /**
         * Gets a render target from the adapter. 
         * 
         * @param Index The target index to get.
         * @returns The texture bound to the target, or nullptr if no bound texture.
         */
        virtual ITexture * GetTarget(_In_ uint32_t index) const throw() = 0;

        /**
         * Creates a named texture within the API and returns it for registration and handling. 
         *    
         * @param name The name of the texture, usually a fully-qualified name. 
         * @param desc Description of the texture, size and format. 
         * @return A shared pointer to the created texture. 
         * 
         * @note Only Voodoo texture formats are supported, API-specific formats are not and @e must return errors. 
         * 
         * @note This must create a texture that can either be rendered directly into or that can have the backbuffer copied 
         *     into it efficiently, depending on how the adapter chooses to implement RTT.
         *     
         * @warning You must not call this method directly; it should only be used via Core::CreateTexture(). This method 
         *     does not set up the texture for use with the core. 
         */
        virtual ITexture * CreateTexture(_In_ const String & name, _In_ TextureDesc desc) throw() = 0;

        /**
         * Loads a named texture into the API and registers it with the Core. The texture source is provided here and all 
         * dimensions should be drawn from that, with as little conversion as possible. 
         * 
         * @param Name The filename to load.
         */
        virtual ITexture * LoadTexture(_In_ const String & name, _In_ TextureRegion region) throw() = 0;

        /**
         * Draws geometry from system memory.
         * 
         * @param Vertexes The number of vertexes to draw. 
         * @param pVertexData This must contain vertex data for the given number of verts.
         */
        virtual void DrawGeometry(_In_ int32_t vertexCount, _In_count_(vertexCount) const VertexStruct * const pVertexData) throw() = 0;

        /**
         * Downloads a parameter's value from system RAM to VRAM, verifying that the value on the GPU (and in use) is the 
         * latest value set in the CPU memory buffer (all parameter set commands operate on the system RAM buffer for speed 
         * and compatibility). 
         * 
         * @param Parameter The parameter to apply.
         */
        virtual void ApplyParameter(_In_ IParameter * const pParameter) throw() = 0;

        /**
         * Set a property on the adapter. 
         * 
         * @param Name The property name to set. 
         * @param Value The value to set the property to. 
         * 
         * @note Adapters may define the meaning of any properties not given in the core adapter specification. Adapters may
         *     require a specific format or form for properties.
         */
        virtual void SetProperty(const String & name, const String & value) throw() = 0;

        virtual String GetProperty(const String & name) const throw() = 0;

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
         * @param Error The error raised. 
         * 
         * @note Not all Cg errors will call this; the core performs error validation and handles all context-related errors
         *     internally, as well as logging what data it can find. Malformed errors will not be passed to the adapter.
         *     
         * @note In the case of compiler errors, the exact messages can be gotten by calling cgGetLastListing until it 
         *     returns nullptr. The Core error handler does just this, @e after the adapter's error handler returns. Thus, if 
         *     the adapter does not dump compiler listings, the core will.
         */
        virtual void HandleError(_In_ CGcontext const pContext, _In_ int32_t error) throw() = 0;
    };
}
