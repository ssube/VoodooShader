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

#include "Includes.hpp"

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore 
     * @{
     */

    /**
     * Complete shader class, managing techniques, passes and metadata. 
     * 
     * Shaders contain significant linking and processing data, as well as the Voodoo-specific linking stage. IShader file
     * loading and compilation are handled internally, the shader can be constructed from a file location or existing Cg 
     * effect object. 
     *
     * @note Shaders correspond to DirectX effects or a group of OpenGL programs. 
     * @note A shader may contain multiple @ref Technique techniques. IShader techniques must be separately validated, 
     *     only techniques valid on the current hardware will be usable in the shader. 
     *     
     * @warning For any technique in a shader to be valid, the entire shader must compile without errors. This 
     *     <em>does not</em> mean that all techniques will be valid.
     */
    class IShader :
        public IObject
    {
    public:
        /**
         * Create a shader from a file, passing options to the compiler. 
         * 
         * @param pParent The ICore this shader is created under. 
         * @param Path The file to use for source.
         * @param ppArgs Optionals arguments to be passed to the compiler (last item in the array must be nullptr).
         */
        IShader(_In_ ICore * pParent, _In_ String Path, _In_opt_ const char ** ppArgs = nullptr);

        /**
         * Destroys the shader, unlinking and cleaning up the effect and all techniques. 
         */
        virtual ~IShader(void);

        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 AddRef(void) throw();

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 Release(void) throw();

        /**
         * Get the name (usually filename of source file) for this shader.
         * 
         * @return The name.
         */
        virtual String ToString(void) throw();

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) throw();

        virtual Int32 GetTechniqueCount(void) throw();

        /**
         * Retrieve a technique from the shader by name. Most cases should use the default technique, but some specific 
         * applications may want a particular technique.
         * 
         * @param Index The index of the technique to get.
         * @return The technique, if it is found.
         */
        virtual ITechnique * GetTechnique(_In_ Int32 Index) throw();

        /**
         * Retrieves the default technique from this shader. All drawing should be done with the default technique: it is
         * guaranteed to be valid for the current shader profiles and API. The default technique is the first technique
         * to validate.

         * @return A reference to the default technique. 
         * 
         * @note To influence the priority of techniques, simply place the most specific or least compatible first (often
         *     high-quality ones) and count down with the most compatible and simplest technique last. The first valid 
         *     technique found becomes the default.
         */
        virtual ITechnique * GetDefaultTechnique(void) throw();

        /**
         * Set a technique from this shader to be used as the default technique. Some adapter functions simply retrieve the 
         * default technique to draw with. 
         * 
         * @param Technique The technique within this shader to use.
         * @return Success of the setting.
         * 
         * @note This validates that the technique belongs to this shader.
         */
        virtual Bool SetDefaultTechnique(_In_ ITechnique * Technique) throw();

        /**
         * Retrieve the number of effect-level parameters in this shader. These hold a single value for all passes. 
         * 
         * @return The parameter count.
         */
        Int32 GetParameterCount(void) throw();

        /**
         * Retrieve a specific parameter from the shader. These may be modified at runtime and will automatically update Cg 
         * and the GPU with their value (in most cases).
         * 
         * @param Index The index of the parameter to retrieve.
         * @return The parameter, if valid.
         */
        IParameter * GetParameter(_In_ Int32 Index) throw();

        /**
         * Retrieve the underlying Cg technique. 
         * 
         * @return A pointer to the Cg technique. 
         */
        CGeffect GetCgEffect(void) throw();

    private:
        /**
         * Initialize delayed linking (or relinking) for this shader. This function
         * rebuilds the technique and pass structure, but <em>does not </em> reload or
         * recompile the effect (Cg effects are handled by the core).
         */
        void Link();

        void SetupTechniques();

        /**
         * Link a particular effect-level parameter against various core elements (exact
         * behavior depends on param type). 
         * 
         * @param param The parameter to link.
         */
        void LinkParameter(_In_ IParameter * pParam);

        /**
         * Links a particular effect-level sampler against a core texture. This generally
         * should be called by IShader::LinkParameter(). 
         * 
         * @param param The sampler to link
         */
        void LinkSampler(_In_ IParameter * pParam);

        /**
         * Find texture information from a parameter and create a texture based on that
         * data. Calls IShader::LinkSampler() after texture creation if appropriate. @param
         * param The parameter to use.
         */
        void CreateParameterTexture(_In_ IParameter * pParam);

        Int32 m_Refs;
        String m_Name;
        ICoreRef m_Core;

        ITechniqueRef m_DefaultTechnique;
        TechniqueVector m_Techniques;
        ParameterVector m_Parameters;

        CGeffect m_CgEffect;
    };
    /* @} */
}
