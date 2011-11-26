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
     * @addtogroup voodoo_uuids 
     */
    DEFINE_IID(ITechnique) = {0x95, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

    /**
     * Contains a set of passes, designed to be used sequentially to create a shader effect. Typically, a single technique
     * will be used, although shaders may contain multiple techniques. Each technique may contain a number of passes and
     * some technique-level metadata.
     *
     * @note All passes in a valid technique are guaranteed to be valid.
     * @warning While a shader may contain a number of techniques, not all are guaranteed to be valid. Techniques are
     *     typically validated when the Shader is created and loaded.
     *      
     * @par IID
     *      e6f31295-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(ITechnique, IObject)
    {
    public:
        VOODOO_METHODCALL_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHODCALL_(uint32_t, Release)() CONST PURE;
        VOODOO_METHODCALL(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHODCALL_(String, ToString)() CONST PURE;
        VOODOO_METHODCALL_(ICore *, GetCore)() CONST PURE;

        VOODOO_METHODCALL_(String, GetName)() CONST PURE;

        /**
         * Retrieve the technique's final target. This is the surface the technique expects the results of the final pass to
         * end up in. The technique may render to scratch textures, but the final result should go into this texture (not,
         * for example, the lastshader texture).
         *
         * @return A reference to the target texture
         */
        VOODOO_METHODCALL_(ITexture *, GetTarget)() CONST PURE;

        /* Retrieve the number of passes in this technique.
         *
         * @return The number of passes
         */
        VOODOO_METHODCALL_(const uint32_t, GetPassCount)() CONST PURE;

        /**
         * Retrieve a specific pass from this technique.
         *
         * @param index The pass number to retrieve.
         * @return A reference to the given pass.
         */
        VOODOO_METHODCALL_(IPass *, GetPass)(_In_ const uint32_t index) CONST PURE;

        /**
         * Retrieve the parent shader of this technique.
         *
         * @return The parent shader.
         */
        VOODOO_METHODCALL_(IShader *, GetShader)() CONST PURE;

        /**
         * Retrieve the underlying Cg technique.
         *
         * @return A pointer to the Cg technique.
         */
        VOODOO_METHODCALL_(CGtechnique, GetCgTechnique)() CONST PURE;
    };
    /**
     * @}
     */
}
