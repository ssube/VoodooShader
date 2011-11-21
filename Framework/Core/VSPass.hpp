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

#include "IPass.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_uuids 
     */
    DEFINE_CLSID(VSPass) = {0xA3, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};
    
    /**
     * @par CLSID
     *      e6f312a3-05af-11e1-9e05-005056c00008
     */
    class VSPass :
        public IPass
    {
    public:
        VSPass(_In_ ITechnique * pTechnique, _In_ CGpass pCgPass);
        virtual ~VSPass(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual bool QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual String GetName() const throw();
        virtual ITexture * GetTarget(void) const throw();
        virtual ITechnique * GetTechnique(void) const throw();
        virtual CGprogram GetProgram(_In_ const ProgramStage stage) const throw();
        virtual CGpass GetCgPass(void) const throw();

    private:
        void Link(void);

        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        ITechnique * m_Technique;
        ITextureRef m_Target;

        CGpass m_CgPass;

        CGprogram m_GeometryProgram;
        CGprogram m_VertexProgram;
        CGprogram m_FragmentProgram;
        CGprogram m_DomainProgram;
        CGprogram m_HullProgram;
    };
}
