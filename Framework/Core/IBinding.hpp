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
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut) PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name IBinding Methods
         * @{
         */
        /**
         * Initializes the binding, attaching it to a hardware context. This takes a binding-defined set of variants with
         * input parameters. Refer to the binding's documentation for required parameters. Any divergence from the
         * documented parameter list @a must cause this call to fail, including extra variants.
         * 
         * @note If this fails, the binding is considered in an undefined state and resources must not be created from it
         *      until it is reset.
         */
        VOODOO_METHOD(Init)(_In_ CONST uint32_t count, _In_reads_(count) Variant * pParams) PURE;
        /**
         * Resets the binding, releasing the hardware context and invalidating any resources created from it. This must not
         * destroy the hardware context @a explicitly, as it may still be in use by the host, but it must release the
         * binding's held reference, if such a thing is applicable.
         * 
         * @note This should not fail, if at all possible. If it does, the binding and all resources created from it are
         *      in an undefined state and must be destroyed without further use.
         */
        VOODOO_METHOD(Reset)() PURE;
        /**
         * @}
         * @name Effect Methods
         * @{
         */
        /**
         * Creates and links an effect from a string of source code. The source may be formatted or use any preprocessor
         * directives a file may.
         * 
         * @param   source  The code to compile.
         * 
         * @warning In many bindings, the source code must be converted to single-byte characters before compilation. This
         *      string should not contain characters requiring multiple bytes within the code, or behavior is undefined and
         *      will likely result in compiler or linker errors.
         */
        VOODOO_METHOD_(IEffect *, CreateEffect)(_In_ CONST String & source) PURE;
        /**
         * Creates and links an effect from a file.
         * 
         * @param   pFile   The file to compile.
         * 
         * @warning In many bindings, the source code must be in a single-byte character set. This may cause the file to be
         *      interpreted as such by the compiler. Using characters that require multiple bytes may cause undefined
         *      behavior and will likely result in compiler or linker errors.
         */
        VOODOO_METHOD_(IEffect *, CreateEffectFromFile)(_In_ CONST IFile * pFile) PURE;
        /**
         * @}
         * @name Parameter Methods
         * @{
         */
        /**
         * Creates a virtual parameter, as necessary by the binding. The parameter must maintain its name and description,
         * including type, value get and set methods must work as expected, and it must be able to be linked to other
         * parameters. However, it must not have a parent effect.
         * 
         * @param   name    The parameter's name.
         * @param   desc    The parameter's description.
         * 
         * @pre IBinding::Init() must have been previously called, successfully.
         */
        VOODOO_METHOD_(IParameter *, CreateParameter)(_In_ CONST String & name, _In_ CONST ParameterDesc desc) PURE;
        /**
         * @}
         * @name Texture Methods
         * @{
         */
        /**
         * Creates a new texture with the given name and description. The contents of this texture are initially undefined. 
         * 
         * @param   name    The texture's name (registered with the core).
         * @param   desc    The texture's description.
         */
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ CONST String & name, _In_ CONST TextureDesc desc) PURE;
        /**
         * Creates a new texture with the given name and loads data into it from a file. The texture will take the size and
         * format of the file.
         * 
         * @param   name    The texture's name (registered with the core).
         * @param   pFile   The texture file to load.
         */
        VOODOO_METHOD_(ITexture *, CreateTextureFromFile)(_In_ CONST String & name, _In_ IFile * pFile) PURE;
        /**
         * Creates a null texture from the device.
         */
        VOODOO_METHOD_(ITexture *, CreateNullTexture)() PURE;
        /**
         * @}
         */
    };
}
