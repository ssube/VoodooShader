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

#include "IShader.hpp"

namespace VoodooShader
{
    /* VSShader: e6f312a4-05af-11e1-9e05-005056c00008 */
    DEFINE_CLSID(VSShader) = {0xA4, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

    class VSShader :
        public IShader
    {
    public:
        VSShader(_Pre_notnull_ ICore * const pCore, _In_ const String & Path, _In_opt_ const char ** ppArgs = nullptr);
        virtual ~VSShader(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual String GetName() const throw();
        virtual const uint32_t GetTechniqueCount(void) const throw();
        virtual ITechnique * GetTechnique(_In_ const uint32_t index) const throw();
        virtual ITechnique * GetDefaultTechnique(void) const throw();
        virtual bool SetDefaultTechnique(_In_ ITechnique * const pTechnique) throw();
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
         * @param pParam The parameter to link.
         */
        void LinkParameter(_In_ IParameter * pParam);

        /**
         * Links a particular effect-level sampler against a core texture. This generally
         * should be called by IShader::LinkParameter().
         *
         * @param pParam The sampler to link
         */
        void LinkSampler(_In_ IParameter * pParam);

        /**
         * Find texture information from a parameter and create a texture based on that
         * data. Calls IShader::LinkSampler() after texture creation if appropriate.
         *
         * @param pParam The parameter to use.
         */
        void CreateParameterTexture(_In_ IParameter * pParam);

        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        ITechniqueRef m_DefaultTechnique;
        TechniqueVector m_Techniques;
        ParameterVector m_Parameters;

        CGeffect m_CgEffect;
    };
}
