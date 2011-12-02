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
     * @class IParameter
     *
     * Provides access to shader parameters, with attach/detach and value set (for most data types).
     *
     * @par IID
     *      e6f31291-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IParameter, IObject, {0x91, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;

        VOODOO_METHOD_(String, GetName)() CONST PURE;
        /**
         * Retrieves the type of this parameter. This specifies what type and how many data components are used (one texture
         * or 1-16 floats).
         */
        VOODOO_METHOD_(ParameterType, GetType)() CONST PURE;
        /**
         * Checks if the parameter is virtual (belongs to the core only) or physical (from a shader).
         *
         * @return Virtual status.
         */
        VOODOO_METHOD(IsVirtual)() CONST PURE;
        /**
         * Attaches a second parameter to this one, forcing the other to update whenever this value is changed.
         *
         * @param pParam The parameter to bind to this one.
         * @return Success of the bind.
         *
         * @warning This @e cannot be used to bind one effect's parameter to another. It can only be used to bind
         *     actual parameters to virtual parameters.
         *
         */
        VOODOO_METHOD(AttachParameter)(_In_opt_ IParameter * const pParam) PURE;
        /**
         * Detaches this parameter from its source (set with a call to IParameter::AttachParameter(IParameter * const)).
         *
         * @warning If called on a parameter that has other parameters bound to it (a source), it will disconnect all of
         *      those. This breaks connections from either end, source or bound.
         */
        VOODOO_METHOD(DetachParameter)() PURE;
        /**
         * Get the component count for this parameter.
         *
         * @return The number of components.
         *
         * @note This will have different meanings depending on parameter type:
         *     @li For a texture, this represents the number of dimensions (2 for texture2D).
         *     @li For a vector or matrix, this represents the raw number of components (2 for Float2, 4 for Float2x2, etc).
         *     @li For an array, this represents the number of elements in the array (2 for Light[2]).
         *     @li Structs and other types that do not also match one of the above will always return -1.
         */
        VOODOO_METHOD_(const uint32_t, GetComponents)() CONST PURE;
        /**
         * Retrieves the texture source for this parameter.
         *
         * @return The texture source, if this parameter has a texture.
         */
        VOODOO_METHOD_(ITexture *, GetTexture)() CONST PURE;
        VOODOO_METHOD_(void, SetTexture)(_In_opt_ ITexture * const pTexture) PURE;
        /**
         * Retrieves the float buffer for this parameter. This contains all 16 float
         * components, for all sizes (float1 to Float4x4). Any component may be written
         * to, but only the appropriate number will be sent to the Cg parameter.
         */
        _Ret_count_c_(16) VOODOO_METHOD_(float * const, GetScalar)() PURE;
        VOODOO_METHOD_(void, SetScalar)(_In_ const uint32_t count, _In_count_(count) float * const pValues) PURE;
        VOODOO_METHOD_(IShader * const, GetShader)() CONST PURE;
        /**
         * Retrieves the underlying Cg parameter object.
         *
         * @returns The Cg parameter this object is bound to.
         */
        VOODOO_METHOD_(CGparameter, GetCgParameter)() CONST PURE;
    };
    /**
     * @}
     */
}
