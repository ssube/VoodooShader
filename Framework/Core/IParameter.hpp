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
     * @class IParameter
     *
     * Provides access to shader parameters, with attach/detach and value set (for most data types).
     *
     * @iid     e6f31291-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IParameter, IResource, ({0x91, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
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
         * @name IResource Methods
         * @{
         */
        VOODOO_METHOD_(String, GetName)() CONST;
        VOODOO_METHOD(GetProperty)(_In_ CONST Uuid propid, _Out_ Variant * pValue) CONST;
        VOODOO_METHOD(SetProperty)(_In_ CONST Uuid propid, _In_ Variant * pValue);
        /**
         * @}
         * @name Value Type Methods
         * @{
         */
        /**
         * Retrieves the type and dimensions of this parameter. 
         */
        VOODOO_METHOD_(ParameterDesc, GetDesc)() CONST PURE;
        /**
         * @}
         * @name Value Access Methods
         * @{
         */
        VOODOO_METHOD(GetBool)(_Out_ bool * pVal) CONST PURE;
        VOODOO_METHOD(GetFloat)(_Out_ float * pVal) CONST PURE;
        VOODOO_METHOD(GetInt)(_Out_ int32_t * pVal) CONST PURE;
        VOODOO_METHOD(GetString)(_Out_ String * pVal) CONST PURE;
        VOODOO_METHOD(GetTexture)(_Out_ ITexture ** pVal) CONST PURE;
        VOODOO_METHOD(GetVector)(_Out_ Float4 * pVal) CONST PURE;
        VOODOO_METHOD(SetBool)(_In_ CONST bool val) PURE;
        VOODOO_METHOD(SetFloat)(_In_ CONST float val) PURE;
        VOODOO_METHOD(SetInt)(_In_ CONST int32_t val) PURE;
        VOODOO_METHOD(SetString)(_In_ CONST String & val) PURE;
        VOODOO_METHOD(SetTexture)(_In_ ITexture * pVal) PURE;
        VOODOO_METHOD(SetVector)(_In_ CONST Float4 val) PURE;
        /**
         * @}
         * @name Link Methods
         * @{
         */
        /**
         * Checks if the parameter is virtual (belongs to the core only) or physical (from a program).
         */
        VOODOO_METHOD_(bool, IsVirtual)() CONST PURE;
        /**
         * Attaches another parameter to this one, forcing the other to be updated whenever this parameter changes value.
         *
         * @param pParam The parameter to bind to this one.
         *
         * @warning This @e cannot be used to bind one effect's parameter to another. It can only be used to bind
         *     parameters to virtual parameters.
         *
         */
        VOODOO_METHOD(AttachParameter)(_In_ IParameter * pParam) PURE;
        /**
         * Detaches a parameter from this one.
         *
         * @warning If called on a parameter that has other parameters bound to it (a source), it will disconnect all of
         *      those. This breaks connections from either end, source or bound.
         */
        VOODOO_METHOD(DetachParameter)(_In_ IParameter * pParam) PURE;
        /**
         * @}
         */
        VOODOO_METHOD_(IEffect *, GetEffect)() CONST PURE;
    };
    /**
     * @}
     */
}
