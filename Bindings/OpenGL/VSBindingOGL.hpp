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
// System
#pragma warning(push,3)
#include <stack>
#pragma warning(pop)

namespace VoodooShader
{
    namespace Voodoo_OGL
    {
        /**
         * @class VSBindingOGL
         * Provides Direct3D9 bindings for Voodoo Shader, using D3DX and the effect framework. This uses the standard HLSL
         * compiler with no conversion, supports all texture types, and virtual parameters with ghosting and type-checking.
         * 
         * @clsid f84ac3c6-073f-11e1-83d4-005056c00008
         */
        VOODOO_CLASS(VSBindingOGL, IBinding, ({0xC6, 0xC3, 0x4A, 0xF8, 0x3F, 0x07, 0xE1, 0x11, 0x83, 0xD4, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
        {
            friend class VSEffectOGL;
            friend class VSParameterOGL;
            friend class VSPassOGL;
            friend class VSTechniqueOGL;
            friend class VSTextureOGL;

        public:
            VSBindingOGL(_In_ ICore * pCore);
            ~VSBindingOGL();

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
            GLint Impl_CreateTexture(_In_ TextureDesc & desc, _In_ DWORD usage, _In_ GLint format);

            mutable uint32_t m_Refs;
            ICore * m_Core;

            bool m_ILUT;

            HDC m_Device;
            HGLRC m_Context;

            // Internal state tracking
            EffectRef m_BoundEffect;
            PassRef m_BoundPass;
            TextureRef m_BoundSourceTexture[4];
            TextureRef m_BoundTargetTexture[4];
        };
    }
}
