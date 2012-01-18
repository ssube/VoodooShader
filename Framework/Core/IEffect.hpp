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
     * @class IEffect
     *
     * Complete effect class, managing techniques, passes and metadata.
     *
     * Effects contain significant linking and processing data, as well as the Voodoo-specific linking stage. IShader file
     * loading and compilation are handled internally, with the shader being compiled from a VSFX file.
     *
     * @note A shader may contain multiple @ref ITechnique techniques. IShader techniques must be separately validated,
     *     only techniques valid on the current hardware will be usable in the shader.
     *
     * @warning For any technique in a shader to be valid, the entire shader must compile without errors. This
     *     <em>does not</em> mean that all techniques will be valid. Care must be taken when using the latest profile
     *     keyword, as this may cause unsupported techniques to generate compile errors, preventing all techniques
     *     from being used.
     *
     * @restag  None.
     * @iid     e6f31294-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IEffect, IResource, ({0x94, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
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
        VOODOO_METHOD(GetProperty)(const Uuid propid, _In_ Variant * pValue) CONST;
        VOODOO_METHOD(SetProperty)(const Uuid propid, _In_ Variant * pValue);
        /**
         * @}
         * @name Effect Methods
         * @{
         */
        /**
         * Binds this effect to the hardware, capturing state and downloading parameters as needed. The currently active
         * technique will be used.
         * 
         * @param   pPassCount  A uint32_t to be filled with the number of passes needed to render the full effect.
         * @param   restore     If true, the effect will save hardware states, reset the device to defaults, and later 
         *                      restore the original states when the effect is unbound.
         * 
         * @note Passes from an effect must not be used in IAdapter::SetPass until the effect has been set. When desired
         *      passes have been used and reset, the effect must be reset. Failing to reset the effect may cause unwanted
         *      states for future draw calls.
         */
        VOODOO_METHOD(SetEffect)(_Out_ uint32_t * pPassCount, bool restore = true) PURE;
        /**
         * Reset this effect, unbinding it from hardware and resetting states if needed.
         */
        VOODOO_METHOD(ResetEffect)() PURE;
        /**
         * Binds a pass from the active technique to the hardware, settings states as needed. The shaders to be bound
         * depend on the API and flags given.
         *
         * @param   passIndex   The index of the pass to set.
         * @param   stages      A mask limiting which shader stages to use. This can be used to disable vertex shaders for
         *                      post-processing or other effects.
         *
         * @note Each call to SetPass must have a corresponding call to ResetPass.
         */
        VOODOO_METHOD(SetPass)(_In_ const uint32_t passIndex, ShaderStage stages = VSStage_All) PURE;
        /**
         * Resets the state of the adapter and unbinds the pass. This may trigger state reset or texture copying, so it is
         * required for proper shader handling.
         */
        VOODOO_METHOD(ResetPass)() PURE;
        /**
         * @}
         * @name Parameter Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, GetParameterCount)() CONST PURE;
        VOODOO_METHOD_(IParameter *, GetParameter)(const uint32_t index) CONST PURE;
        VOODOO_METHOD_(IParameter *, GetParameterByName)(const String & name) CONST PURE;
        /**
         * @}
         * @name Technique Methods
         * @{
         */
        /**
         * Get the number of validated techniques this shader contains.
         */
        VOODOO_METHOD_(uint32_t, GetTechniqueCount)() CONST PURE;
        /**
         * Retrieve a technique from the shader by name. Most cases should use the default technique, but some specific
         * applications may want a particular technique.
         *
         * @param   index   The index of the technique to get.
         */
        VOODOO_METHOD_(ITechnique *, GetTechnique)(const uint32_t index) CONST PURE;
        VOODOO_METHOD_(ITechnique *, GetTechniqueByName)(const String & name) CONST PURE;
        /**
         * Retrieves the default technique from this shader. All drawing should be done with the default technique: it is
         * guaranteed to be valid for the current shader profiles and API. The default technique is the first technique
         * to validate.
         *
         * @note To influence the priority of techniques, simply place the most specific or least compatible first (often
         *     high-quality ones) and count down with the most compatible and simplest technique last. The first valid
         *     technique found becomes the default.
         */
        VOODOO_METHOD_(ITechnique *, GetDefaultTechnique)() CONST PURE;
        /**
         * Set a technique from this shader to be used as the default technique. Some adapter functions simply retrieve the
         * default technique to draw with.
         *
         * @param pTechnique The technique within this shader to use.
         *
         * @note This will validate that the technique belongs to this shader.
         */
        VOODOO_METHOD(SetDefaultTechnique)(_In_ ITechnique * const pTechnique) PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
