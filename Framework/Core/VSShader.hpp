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

#include "IShader.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore 
     * @{
     */

    class VSShader :
        public IShader
    {
    public:
        VSShader(_In_ ICore * pParent, _In_ const String & Path, _In_opt_ const char ** ppArgs = nullptr);

        virtual ~VSShader(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual const uint32_t GetTechniqueCount(void) const throw();
        virtual ITechnique * GetTechnique(_In_ const uint32_t index) const throw();
        virtual ITechnique * GetDefaultTechnique(void) const throw();
        virtual bool SetDefaultTechnique(_In_ ITechnique * pTechnique) throw();
        virtual const uint32_t GetParameterCount(void) const throw();
        virtual IParameter * GetParameter(_In_ const uint32_t index) const throw();
        virtual CGeffect GetCgEffect(void) const throw();

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

        mutable uint32_t m_Refs;
        String m_Name;
        ICore * m_Core;

        ITechniqueRef m_DefaultTechnique;
        TechniqueVector m_Techniques;
        ParameterVector m_Parameters;

        CGeffect m_CgEffect;
    };
    /* @} */
}
