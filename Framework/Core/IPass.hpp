/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
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
     * Each IPass contains a single set of programs, each operating on a different stage of the render pipeline. 
     * 
     * @note A pass may contain programs for each stage. Valid stages vary by underlying API and version. In OpenGL and
     *     DirectX 9 and earlier, stages may be left to the fixed-function pipeline by not specifying any program to be 
     *     used. Later APIs require the vertex and pixel shaders to be specified in each pass. 
     * 
     * @warning Some adapters may require programs to be provided for certain stages.
     */
    class IPass : 
        public IObject
    {
    public:
        virtual ~IPass(void) throw() {};

        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Retrieve the fully qualified pass name, including technique and shader name.
         * 
         * @return The pass name
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        virtual String GetName() const throw() = 0;

        /**
         * Retrieve the target texture buffer this pass should render to. This must be a texture created with the render
         * target flag set. 
         * 
         * @return The target texture.
         */
        virtual ITexture * GetTarget(void) const throw() = 0;

        /**
         * Retrieve the parent technique of this pass.
         * 
         * @return The parent technique.
         */
        virtual ITechnique * GetTechnique(void) const throw() = 0;

        /**
         * Retrieve a specific program stage from this pass.
         * 
         * @param stage The stage to retrieve. 
         * @return The program corresponding to the desired stage.
         */
        virtual CGprogram GetProgram(_In_ const ProgramStage stage) const throw() = 0;

        /**
         * Retrieve the underlying Cg technique. 
         * 
         * @return A pointer to the Cg technique. 
         */
        virtual CGpass GetCgPass(void) const throw() = 0;
    };
    /**
     * @}
     */
}
