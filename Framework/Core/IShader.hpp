/**
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
    class IShader :
        public IObject
    {
    public:
        /**
         * Destroys the shader, unlinking and cleaning up the effect and all techniques.
         */
        virtual ~IShader(void) throw() {};

        virtual uint32_t AddRef(void) const throw() = 0;
        virtual uint32_t Release(void) const throw() = 0;
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw() = 0;
        virtual String ToString(void) const throw() = 0;
        virtual ICore * GetCore(void) const throw() = 0;

        virtual String GetName() const throw() = 0;

        /**
         * Get the number of validated techniques this shader contains.
         *
         * @return The number of techniques.
         */
        virtual const uint32_t GetTechniqueCount(void) const throw() = 0;

        /**
         * Retrieve a technique from the shader by name. Most cases should use the default technique, but some specific
         * applications may want a particular technique.
         *
         * @param index The index of the technique to get.
         * @return The technique, if it is found.
         */
        virtual ITechnique * GetTechnique(_In_ const uint32_t index) const throw() = 0;

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
        virtual ITechnique * GetDefaultTechnique(void) const throw() = 0;

        /**
         * Set a technique from this shader to be used as the default technique. Some adapter functions simply retrieve the
         * default technique to draw with.
         *
         * @param pTechnique The technique within this shader to use.
         * @return Success of the setting.
         *
         * @note This will validate that the technique belongs to this shader.
         */
        virtual bool SetDefaultTechnique(_In_ ITechnique * const pTechnique) throw() = 0;

        /**
         * Retrieve the number of effect-level parameters in this shader. These hold a single value for all passes.
         *
         * @return The parameter count.
         */
        virtual const uint32_t GetParameterCount(void) const throw() = 0;

        /**
         * Retrieve a specific parameter from the shader. These may be modified at runtime and will automatically update Cg
         * and the GPU with their value (in most cases).
         *
         * @param index The index of the parameter to retrieve.
         * @return The parameter, if valid.
         */
        virtual IParameter * GetParameter(_In_ const uint32_t index) const throw() = 0;

        /**
         * Retrieve the underlying Cg technique.
         *
         * @return A pointer to the Cg technique.
         */
        virtual CGeffect GetCgEffect(void) const throw() = 0;
    };
    /**
     * @}
     */
}
