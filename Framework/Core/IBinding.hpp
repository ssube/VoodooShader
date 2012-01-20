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
     * @class IBinding
     *
     * Hardware API binding interface. Provides a consistent but abstracted view of the API, hidden from the user and 
     * handled by the core.
     * 
     * @iid e6f31288-05af-11e1-9e05-005056c00008
     * 
     * @warning The IBinding interface is
     */
    VOODOO_INTERFACE(IBinding, IObject, ({0x88, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
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
         * @name IBinding Methods
         * @{
         */
        VOODOO_METHOD(Init)(uint32_t count, _In_count_(count) Variant * pParams) PURE;
        /**
         * @}
         * @name Effect Methods
         * @{
         */
        VOODOO_METHOD_(IEffect *, CreateEffect)(CONST String & source) PURE;
        VOODOO_METHOD_(IEffect *, CreateEffectFromFile)(CONST IFile * pFile) PURE;
        /**
         * @}
         * @name Parameter Methods
         * @{
         */
        VOODOO_METHOD_(IParameter *, CreateParameter)(CONST String & name, ParameterDesc desc) PURE;
        /**
         * @}
         * @name Texture Methods
         * @{
         */
        VOODOO_METHOD_(ITexture *, CreateTexture)(CONST String & name, TextureDesc desc) PURE;
        VOODOO_METHOD_(ITexture *, CreateTextureFromFile)(CONST String & name, _In_ IFile * pFile) PURE;
        /**
         * @}
         */
    };
}
