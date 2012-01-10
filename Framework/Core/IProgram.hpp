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
     * @class IProgram
     *
     * A compiled program, created from shader source code and a single entry-point function. Code is compiled to fit a
     * particular profile, following the features given.
     * 
     * @iid     e6f31293-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IProgram, IResource, ({0x98, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid refid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name IResource Methods
         * @{
         */
        VOODOO_METHOD_(String, GetName)() CONST PURE;
        VOODOO_METHOD(GetProperty)(const String & name, _In_ Variant * pValue) CONST;
        VOODOO_METHOD(SetProperty)(const String & name, _In_ const Variant & value);
        /**
         * @}
         * @name IProgram Methods
         * @{
         */
        /**
         * Retrieve a uniform variable from this pass.
         * 
         * @param name      The name of the uniform to get.
         */
        VOODOO_METHOD_(IParameter *, GetUniform)(const String & name) CONST PURE;
        /**
         * Retrieve a 
        /**
         * Gets the profile this program is compiled with.
         */
         VOODOO_METHOD_(ProgramProfile, GetProfile)() CONST PURE;
         /**
          * Sets the profile this program is compiled with.
          * 
          * @warning        This may trigger recompilation.
          */
         VOODOO_METHOD(SetProfile)(const ProgramProfile profile) PURE;
         /**
          * Gets the function this program is created from.
          */
         VOODOO_METHOD_(String, GetFunction)() CONST PURE;
         /**
          * Sets the function this program is created from. This must be a valid function with profile-appropriate semantics
          * in the source code (or included files).
          * 
          * @warning        This may trigger recompilation.
          */
         VOODOO_METHOD(SetFunction)(const String & function) PURE;
         /**
          * @}
          */
        /**
         * Forces the shader to be recompiled with the current set of definitions.
         * 
         * @param flags     The flags to be used during compilation. If this is CF_Default, the current set of core defaults
         *                  are used.
         *                  
         * @warning         This is slow.
         */
        VOODOO_METHOD(Compile)(const CompileFlags flags = CF_Default) PURE;
    };
    /**
     * @}
     */
}