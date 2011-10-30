/**
 * This file is part of the Voodoo IShader Framework, a comprehensive shader support library. 
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
  * @addtogroup VoodooCore @{
  */

 /**
  * Contains a set of passes, designed to be used sequentially to create a shader effect. Typically, a single technique
  * will be used, although shaders may contain multiple techniques. Each technique may contain a number of passes and
  * some technique-level metadata. 
  * 
  * @note All passes in a valid technique are guaranteed to be valid. 
  * @warning While a shader may contain a number of techniques, not all are guaranteed to be valid. Techniques are 
  *     typically validated when the Shader is created and loaded.
  */
    class ITechnique :
        public IObject
    {
    public:
        virtual ~ITechnique(void) {};

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
         * Retrieve a fully qualified technique name (including shader name) from the technique. 
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Retrieve the core this technique was compiled under.
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        /**
         * Retrieve the technique's final target. This is the surface the technique expects the results of the final pass to 
         * end up in. The technique may render to scratch textures, but the final result should go into this texture (not, 
         * for example, the lastshader texture). 
         * 
         * @return A reference to the target texture
         */
        virtual ITexture * GetTarget(void) const throw() = 0;

        /* Retrieve the number of passes in this technique. 
         * 
         * @return The number of passes 
         */
        virtual const uint32_t GetPassCount(void) const throw() = 0;

        /**
         * Retrieve a specific pass from this technique. 
         * 
         * @param index The pass number to retrieve. 
         * @return A reference to the given pass. 
         */
        virtual IPass * GetPass(_In_ const uint32_t index) const throw() = 0;

        /**
         * Retrieve the parent shader of this technique.
         * 
         * @return The parent shader.
         */
        virtual IShader * GetShader(void) const throw() = 0;

        /** 
         * Retrieve the underlying Cg technique. 
         * 
         * @return A pointer to the Cg technique. 
         */
        virtual CGtechnique GetCgTechnique(void) const throw() = 0;
    };
}
