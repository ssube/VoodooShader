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
     * Each IPass contains a single set of programs, each operating on a different stage of the render pipeline.
     *
     * @note A pass may contain programs for each stage. Valid stages vary by underlying API and version. In OpenGL and
     *     DirectX 9 and earlier, stages may be left to the fixed-function pipeline by not specifying any program to be
     *     used. Later APIs require the vertex and pixel shaders to be specified in each pass.
     *
     * @warning Some adapters may require programs to be provided for certain stages.
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
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         */

        VOODOO_METHOD_(String, GetName)() CONST PURE;
        /**
         * Retrieve a the target texture buffer this pass should render to. This must be a texture created with the render
         * target flag set.
         *
         * @param index The target index.
         * @return The target texture.
         */
        VOODOO_METHOD_(ITexture *, GetTarget)(uint32_t index) CONST PURE;
        /**
         * Set the target texture buffer this pass should render to. This must be a texture created with the render
         * target flag set.
         *
         * @param index The target index.
         * @param pTarget The target texture.
         * @return The target texture.
         */
        VOODOO_METHOD(SetTarget)(uint32_t index, ITexture * pTarget) PURE;
        /**
         * Retrieve the parent technique of this pass.
         *
         * @return The parent technique.
         */
        VOODOO_METHOD_(ITechnique *, GetTechnique)() CONST PURE;
        /**
         * Retrieve a specific program stage from this pass.
         *
         * @param stage The stage to retrieve.
         * @return The program corresponding to the desired stage.
         */
        VOODOO_METHOD_(CGprogram, GetProgram)(_In_ const ProgramStage stage) CONST PURE;
        /**
         * Retrieve the underlying Cg technique.
         *
         * @return A pointer to the Cg technique.
         */
        VOODOO_METHOD_(CGpass, GetCgPass)() CONST PURE;
    };
    /**
     * @}
     */
}
