/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

#include "Voodoo_D3D9.hpp"
// System
#pragma warning(push,3)
#include <stack>
#pragma warning(pop)

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        /**
         * @class VSBindingDX9
         * Provides Direct3D9 bindings for Voodoo Shader, using D3DX and the effect framework. This uses the standard HLSL
         * compiler with no conversion, supports all texture types, and virtual parameters with ghosting and type-checking.
         * 
         * @clsid 382CB920-83DD-4570-875F-F4E0C192A476
         */
        VOODOO_CLASS(VSBindingDX9, IBinding, ({0x38, 0x2c, 0xb9, 0x20, 0x83, 0xdd, 0x45, 0x70, 0x87, 0x5f, 0xf4, 0xe0, 0xc1, 0x92, 0xa4, 0x76}))
        {
            friend class VSEffectDX9;
            friend class VSParameterDX9;
            friend class VSPassDX9;
            friend class VSTechniqueDX9;
            friend class VSTextureDX9;

        public:
            VSBindingDX9(_In_ ICore * pCore);
            ~VSBindingDX9();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut);
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;
            /**
             * @}
             * @name IBinding Methods
             * @{
             */
            VOODOO_METHOD(Init)(_In_ CONST uint32_t count, _In_reads_(count) Variant * pParams);
            VOODOO_METHOD(Reset)();
            /**
             * @}
             * @name Effect Methods
             * @{
             */
            VOODOO_METHOD_(IEffect *, CreateEffect)(_In_ CONST String & source);
            VOODOO_METHOD_(IEffect *, CreateEffectFromFile)(_In_ CONST IFile * pFile);
            /**
             * @}
             * @name Parameter Methods
             * @{
             */
            VOODOO_METHOD_(IParameter *, CreateParameter)(_In_ CONST String & name, _In_ CONST ParameterDesc desc);
            /**
             * @}
             * @name Texture Methods
             * @{
             */
            VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ CONST String & name, _In_ CONST TextureDesc desc);
            VOODOO_METHOD_(ITexture *, CreateTextureFromFile)(_In_ CONST String & name, _In_ IFile * pFile);
            VOODOO_METHOD_(ITexture *, CreateNullTexture)();

            // Non-interface methods
            VOODOO_METHOD_(void, ResetState)();
            VOODOO_METHOD_(void, PushState)();
            VOODOO_METHOD_(void, PopState)();

        private:            
            // Internals
            LPDIRECT3DTEXTURE9 Impl_CreateTexture(_In_ TextureDesc & desc, _In_ DWORD usage, _In_ D3DFORMAT format);

            mutable uint32_t m_Refs;
            ICore * m_Core;

            bool m_ILUT;

            LPDIRECT3DDEVICE9 m_Device;
            LPDIRECT3DSTATEBLOCK9 m_InitialState;
            std::stack<LPDIRECT3DSTATEBLOCK9> m_StateStack;

            // Internal state tracking
            EffectRef m_BoundEffect;
            PassRef m_BoundPass;
            TextureRef m_BoundSourceTexture[8];
            TextureRef m_BoundTargetTexture[4];
        };
    }
}
