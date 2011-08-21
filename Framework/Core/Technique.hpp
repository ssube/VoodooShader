/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

#include "Meta.hpp"

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Contains a set of passes, designed to be used sequentially to create a 
     * shader effect. Typically, a single technique will be used, although
     * shaders may contain multiple techniques. Each technique may contain a
     * number of passes and some technique-level metadata.
     * 
     * @note    All passes in a valid technique are guaranteed to be valid.
     * 
     * @warning While a Shader may contain a number of Techniques, not all
     *          are guaranteed to be valid. Techniques are typically validated
     *          when the Shader is created and loaded. 
     */
    class VOODOO_API Technique
        : public IObject
    {
    public:
        static TechniqueRef Create(ShaderRef parent, CGtechnique cgTech);

        virtual ~Technique();

        /**
         * Retrieve a fully qualified technique name (including shader name)
         * from the technique.
         *
         * @return This technique's name
         */
        virtual String GetName();

        /**
         * Retrieve the core this technique was compiled under.
         *
         * @return The core
         */
        virtual Core * GetCore();

        /**
         * Retrieve the technique's final target. This is the surface the
         * technique expects the results of the final pass to end up in. The
         * technique may render to scratch textures, but the final result should
         * go into this texture (not, for example, the lastshader texture).
         *
         * @return A reference to the target texture
         */
        virtual TextureRef GetTarget();

        /**
         * Retrieve the number of passes in this technique.
         *
         * @return The number of passes
         */
        virtual size_t GetPassCount();

        /**
         * Retrieve a specific pass from this technique.
         *
         * @param index The pass number to retrieve
         * @return A reference to the given pass
         * @throws Exception if index is greater than the number of passes the
         *        technique has (call Technique::GetPassCount() first to find the
         *        number of passes)
         */
        virtual PassRef GetPass
        (
            _In_ size_t index
        );

        virtual ShaderPtr GetShader();

        /**
         * Retrieve the underlying Cg technique.
         * 
         * @return A pointer to the Cg technique.
         */
        virtual CGtechnique GetCgTechnique();

    private:
        Technique
        (
            _In_ TechniqueRef & self, 
            _In_ ShaderRef parent, 
            _In_ CGtechnique cgTech
        );

        void Link(TechniquePtr self);

        String m_Name;
        Core * m_Core;

        TextureRef m_Target;
        PassVector m_Passes;

        ShaderPtr m_Shader;
        CGtechnique m_CgTechnique;
    };
}