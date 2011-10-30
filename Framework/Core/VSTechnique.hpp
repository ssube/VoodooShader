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

#include "ITechnique.hpp"

namespace VoodooShader
{
     /**
      * @addtogroup VoodooCore @{
      */

    class VSTechnique :
        public ITechnique
    {
    public:
        VSTechnique(_In_ IShader * pShader, _In_ CGtechnique pCgTech);

        virtual ~VSTechnique(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual String GetName() const throw();
        virtual ITexture * GetTarget(void) const throw();
        virtual const uint32_t GetPassCount(void) const throw();
        virtual IPass * GetPass(_In_ const uint32_t index) const throw();
        virtual IShader * GetShader(void) const throw();

        virtual CGtechnique GetCgTechnique(void) const throw();

    private:
        void Link();

        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        ITextureRef m_Target;
        PassVector m_Passes;

        IShader * m_Shader;
        CGtechnique m_CgTechnique;
    };
}
