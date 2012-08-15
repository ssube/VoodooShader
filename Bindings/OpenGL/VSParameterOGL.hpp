/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

#include "Voodoo_OGL.hpp"

namespace VoodooShader
{
    namespace Voodoo_OGL
    {
        /**
         * @clsid f84ac3c8-073f-11e1-83d4-005056c00008
         */
        VOODOO_CLASS(VSParameterOGL, IParameter, ({0xC8, 0xC3, 0x4A, 0xF8, 0x3F, 0x07, 0xE1, 0x11, 0x83, 0xD4, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
        {
        public:
            VSParameterOGL(_In_ VSEffectOGL * pEffect, _In_ MOJOSHADER_effectParam * pHandle);
            VSParameterOGL(_In_ VSBindingOGL * pBinding, _In_ CONST String & name, _In_ ParameterDesc desc);
            ~VSParameterOGL();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut);
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;
        
            VOODOO_METHOD_(String, GetName)() CONST;
            VOODOO_METHOD(GetProperty)(_In_ CONST Uuid propid, _Out_ Variant * pValue) CONST;
            VOODOO_METHOD(SetProperty)(_In_ CONST Uuid propid, _In_ Variant * pValue);

            VOODOO_METHOD_(ParameterDesc, GetDesc)() CONST;

            VOODOO_METHOD(GetBool)(_Out_ bool * pVal) CONST;
            VOODOO_METHOD(GetFloat)(_Out_ float * pVal) CONST;
            VOODOO_METHOD(GetInt)(_Out_ int32_t * pVal) CONST;
            VOODOO_METHOD(GetString)(_Out_ String * pVal) CONST;
            VOODOO_METHOD(GetTexture)(_Out_ ITexture ** pVal) CONST;
            VOODOO_METHOD(GetVector)(_Out_ Float4 * pVal) CONST;
            VOODOO_METHOD(SetBool)(_In_ CONST bool val);
            VOODOO_METHOD(SetFloat)(_In_ CONST float val);
            VOODOO_METHOD(SetInt)(_In_ CONST int32_t val);
            VOODOO_METHOD(SetString)(_In_ CONST String & val);
            VOODOO_METHOD(SetTexture)(_In_ ITexture * pVal);
            VOODOO_METHOD(SetVector)(_In_ CONST Float4 val);
        
            VOODOO_METHOD_(bool, IsVirtual)() CONST;
            VOODOO_METHOD(AttachParameter)(_In_ IParameter * pParam);
            VOODOO_METHOD(DetachParameter)(_In_ IParameter * pParam);

            VOODOO_METHOD_(IEffect *, GetEffect)() CONST;

            VOODOO_METHOD_(void, Link)();
            VOODOO_METHOD_(ITexture *, LinkNewTexture)();

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            String m_Name;

            VSEffectOGL * m_Effect;
            VSBindingOGL * m_Binding;
            PropertyMap m_Properties;

            ParameterDesc m_Desc;
            ParameterList m_Attached;

            MOJOSHADER_effectParam * m_Handle;

            // Value cache types
            mutable bool m_VBool;
            mutable Float4 m_VFloat;
            mutable int32_t m_VInt;
            mutable String m_VString;
            TextureRef m_Texture;
        };
    }
}
