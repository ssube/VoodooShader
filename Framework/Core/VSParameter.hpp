/**
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

#include "IParameter.hpp"

namespace VoodooShader
{
    /* VSParameter: e6f312a1-05af-11e1-9e05-005056c00008 */
    DEFINE_UUID_IMPL(VSParameter, 0xA1, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);

    class VSParameter :
        public IParameter
    {
    public:
        VSParameter(_In_ ICore * const pCore, _In_ const String & name, _In_ const ParameterType type);
        VSParameter(_In_ IShader * const pShader, _In_ CGparameter const pParam);
        virtual ~VSParameter(void);

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual String GetName() const throw();
        virtual ParameterType GetType(void) const throw();
        virtual bool IsVirtual(void) const throw();
        virtual bool AttachParameter(_In_ IParameter * const pParam) throw();
        virtual bool DetachParameter() throw();
        virtual const uint32_t GetComponents(void) const throw();
        virtual ITexture * GetTexture(void) const throw();
        virtual void SetTexture(_In_opt_ ITexture * const pTexture) throw();
        _Ret_count_c_(16) virtual float * const GetScalar(void) throw();
        virtual void SetScalar(const uint32_t count, _In_count_(count) float * const pValues) throw();
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
