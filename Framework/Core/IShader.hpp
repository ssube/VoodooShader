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
     * Complete shader class, managing techniques, passes and metadata.
     *
     * Shaders contain significant linking and processing data, as well as the Voodoo-specific linking stage. IShader file
     * loading and compilation are handled internally, the shader can be constructed from a file location or existing Cg
     * effect object.
     *
     * @note Shaders correspond to DirectX effects or a group of OpenGL programs.
     * @note A shader may contain multiple @ref ITechnique techniques. IShader techniques must be separately validated,
     *     only techniques valid on the current hardware will be usable in the shader.
     *
     * @warning For any technique in a shader to be valid, the entire shader must compile without errors. This
     *     <em>does not</em> mean that all techniques will be valid.
     */
    VOODOO_INTERFACE(IShader, IObject, {0x94, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;

        VOODOO_METHOD_(String, GetName)() CONST PURE;
        /**
         * Get the number of validated techniques this shader contains.
         *
         * @return The number of techniques.
         */
        VOODOO_METHOD_(const uint32_t, GetTechniqueCount)() CONST PURE;
        /**
         * Retrieve a technique from the shader by name. Most cases should use the default technique, but some specific
         * applications may want a particular technique.
         *
         * @param index The index of the technique to get.
         * @return The technique, if it is found.
         */
        VOODOO_METHOD_(ITechnique *, GetTechnique)(_In_ const uint32_t index) CONST PURE;
        /**
         * Retrieves the default technique from this shader. All drawing should be done with the default technique: it is
         * guaranteed to be valid for the current shader profiles and API. The default technique is the first technique
         * to validate.

         * @return A reference to the default technique.
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
         * @return Success of the setting.
         *
         * @note This will validate that the technique belongs to this shader.
         */
        VOODOO_METHOD(SetDefaultTechnique)(_In_ ITechnique * const pTechnique) PURE;
        /**
         * Retrieve the number of effect-level parameters in this shader. These hold a single value for all passes.
         *
         * @return The parameter count.
         */
        VOODOO_METHOD_(const uint32_t, GetParameterCount)() CONST PURE;
        /**
         * Retrieve a specific parameter from the shader. These may be modified at runtime and will automatically update Cg
         * and the GPU with their value (in most cases).
         *
         * @param index The index of the parameter to retrieve.
         * @return The parameter, if valid.
         */
        VOODOO_METHOD_(IParameter *, GetParameter)(_In_ const uint32_t index) CONST PURE;
        /**
         * Retrieve the underlying Cg technique.
         *
         * @return A pointer to the Cg technique.
         */
        VOODOO_METHOD_(CGeffect, GetCgEffect)() CONST PURE;
    };
    /**
     * @}
     */
}
