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
     * @class IPass
     *
     * Each pass contains a single set of shaders, each operating on a different stage of the render pipeline. Passes are
     * provided by the binding.
     *
     * @note A pass may contain programs for each stage, but stages used vary by binding. Some bindings may require programs
     *      be provided for some stages (typically vertex and pixel), and may ignore other stages. See the binding's 
     *      documentation for details.
     *
     * @iid e6f31293-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IPass, IResource, ({0x93, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut) PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name IResource Methods
         * @{
         */
        VOODOO_METHOD_(String, GetName)() CONST;
        VOODOO_METHOD(GetProperty)(_In_ CONST Uuid propid, _Deref_out_ Variant * pValue) CONST;
        VOODOO_METHOD(SetProperty)(_In_ CONST Uuid propid, _In_ Variant * pValue);
        /**
         * @}
         * @name IPass Methods
         * @{
         */
        /**
         * Binds a pass from the active technique to the hardware, settings states as needed. The shaders to be bound
         * depend on the binding and flags given.
         *
         * @note Each call to SetPass must have a corresponding call to ResetPass. Passes must not be bound unless their
         *      parent effect is bound, doing so may cause errors or undefined behavior.
         *      
         * @note This call will bind texture sources needed by the pass and targets given in the pass. However, the adapter 
         *      may override those by binding @a after binding the pass. Using IPass::SetTarget() is prefered, but manual
         *      binding is allowed. It may cause unexpected results, depending on the effect in use.
         */
        VOODOO_METHOD(Bind)() PURE;
        /**
         * Resets the state of the adapter and unbinds the pass. This may trigger state reset or texture copying, so it is
         * required for proper shader handling.
         */
        VOODOO_METHOD(Reset)() PURE;
        /**
         * Retrieve a the target texture buffer this pass should render to. This must be a texture created with the render
         * target flag set.
         *
         * @param   index       The target index.
         * @return              The target texture.
         */
        VOODOO_METHOD_(ITexture *, GetTarget)(_In_ CONST uint32_t index) CONST PURE;
        /**
         * Set a target texture buffer this pass should render to. This must be a texture created with the render
         * target flag set.
         *
         * @param   index       The target index.
         * @param   pTarget     The target texture.
         */
        VOODOO_METHOD(SetTarget)(_In_ CONST uint32_t index,_In_  ITexture * pTarget) PURE;
        /**
         * Retrieve a specific shader stage from this pass.
         *
         * @param   stage       The stage to retrieve.
         * @param   pValue      A Variant to be filled with the shader.
         */
        VOODOO_METHOD(GetShader)(_In_ CONST ShaderStage stage, _Out_ Variant * pValue) CONST PURE;
        /**
         * Retrieve the parent technique of this pass.
         */
        VOODOO_METHOD_(ITechnique *, GetTechnique)() CONST PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
