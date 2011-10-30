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

#include "IPass.hpp"

namespace VoodooShader
{
    class VSPass :
        public IPass
    {
    public:
        VSPass(_In_ ITechnique* parent, _In_ CGpass cgPass);

        virtual ~VSPass(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
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
    /* @} */
}
