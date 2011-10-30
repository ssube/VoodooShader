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

#include "IParameter.hpp"

namespace VoodooShader
{
    class VSParameter :
        public IParameter
    {
    public:
        VSParameter(_In_opt_ ICore * const pCore, _In_ String name, _In_ const ParameterType type);
        VSParameter(_In_opt_ IShader * const pShader, _In_ const CGparameter pParam);

        virtual ~VSParameter(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual String GetName() const throw();
        virtual ParameterType GetType(void) const throw();
        virtual bool IsVirtual(void) const throw();
        virtual bool AttachParameter(_In_opt_ IParameter * const pParam) throw();
        virtual const uint32_t GetComponents(void) const throw();
        virtual ITexture * GetTexture(void) const throw();
        virtual void SetTexture(_In_opt_ ITexture * const pTexture) throw();
        _Ret_count_c_(16) virtual float * const GetScalar(void) throw();
        virtual void SetScalar(const uint32_t count, _In_count_(Count) float * const pValues) throw();
        virtual IShader * const GetShader(void) const throw();

        virtual CGparameter GetCgParameter(void) const throw();

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
        String m_Name;

        IShaderRef m_Shader;

        bool m_Virtual;
        CGparameter m_Param;
        ParameterType m_Type;

        // Value
        ITextureRef m_ValueTexture;
        float m_Valuefloat[16];
    };
}
