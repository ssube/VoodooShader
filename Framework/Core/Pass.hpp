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

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Each Pass contains a single set of programs, each operating on a different
     * stage of the render pipeline. 
     * 
     * @note    A Pass may contain programs for each stage. Valid stages vary by 
     *          underlying API and version. In OpenGL and DirectX 9 and earlier,
     *          stages may be left to the fixed-function pipeline by not specifying
     *          any program to be used. Later APIs require the vertex and pixel
     *          shaders to be specified in each pass.
     * 
     * @warning Some adapters may require programs to be provided for certain
     *          stages.
     */
    class VOODOO_API Pass
    {
    public:
        Pass
        (
            _In_ TechniquePtr parent,
            _In_ CGpass cgPass
        );

        virtual ~Pass();

        /**
         * Retrieve the fully qualified pass name, including technique and
         * shader name.
         *
         * @return The pass name
         */
        virtual String GetName();

        /**
         * Retrieve the core this pass was compiled under.
         *
         * @return The core
         */
        virtual Core * GetCore();

        /**
         * Retrieve the target this pass should render to. This will be a texture
         * that has been registered with the core. The results of the shader
         * program(s), when run, should end up <em>only</em> in this texture
         * (scratch textures may be used, but the program should not write to a
         * common texture such as lastpass).
         *
         * @return A reference to the target texture.
         */
        virtual TextureRef GetTarget();

        virtual TechniquePtr GetTechnique();

        /**
         * Retrieve a specific program stage from this pass. Each pass may have
         * many programs, each program controls one stage of the render process.
         * The most commonly used stages are the vertex and fragment stages,
         * which are supported by almost all hardware. Geometry stages are also
         * available, but not as well supported. D3D11 tessellation control 
         * stages are not yet supported by the Voodoo Shader Framework.
         *
         * @param stage The stage to retrieve.
         * @return The program corresponding to the desired stage, or null if
         *        the pass has no program for that stage or an unsupported or
         *        unknown stage is requested.
         */
        virtual CGprogram GetProgram
        (
            _In_ ProgramStage stage
        );

        /**
         * Retrieve the underlying Cg technique.
         * 
         * @return A pointer to the Cg technique.
         */
        virtual CGpass GetCgPass();

    private:
        void Link();

        String m_Name;
        Core * m_Core;

        TextureRef m_Target;

        TechniquePtr m_Technique;

        CGpass m_CgPass;

        CGprogram m_GeometryProgram;
        CGprogram m_VertexProgram;
        CGprogram m_FragmentProgram;
        CGprogram m_DomainProgram;
        CGprogram m_HullProgram;
    };

    /**
     * @}
     */
}
