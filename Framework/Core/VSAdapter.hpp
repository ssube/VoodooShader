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

#include "IAdapter.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_core_null Null Implementations
     * @ingroup voodoo_core
     *
     * @{
     */

    /**
     * Voodoo Shader null adapter implementation. Requires no graphics API and does not call core methods, simply
     * returning true or nullptr for methods. Does not perform logging or parameter validation.
     *
     * @par CLSID:
     *      e6f3129a-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSAdapter, IAdapter, {0x9A, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VSAdapter(ICore * pCore);
        virtual ~VSAdapter();

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD(LoadPass)(_In_ IPass * const pPass);
        VOODOO_METHOD(UnloadPass)(_In_ IPass * const pPass);
        VOODOO_METHOD(SetPass)(_In_ IPass * const pPass);
        VOODOO_METHOD_(IPass *, GetPass)() CONST;
        VOODOO_METHOD(ResetPass)(_In_ IPass * const pPass);
        VOODOO_METHOD(SetTarget)(_In_ const uint32_t index, _In_opt_ ITexture * const pTarget);
        VOODOO_METHOD_(ITexture *, GetTarget)(_In_ const uint32_t index) CONST;
        VOODOO_METHOD_(ITexture *, CreateTexture)(_In_ const String & name, _In_ const TextureDesc desc);
        VOODOO_METHOD(LoadTexture)(_In_ IImage * const pFile, _In_ const TextureRegion region, _Inout_ ITexture * const pTexture);
        VOODOO_METHOD(DrawGeometry)(_In_ const uint32_t offset, _In_ const uint32_t count, _In_ void * const pData, _In_ const VertexFlags flags);
        VOODOO_METHOD(ApplyParameter)(_In_ IParameter * const pParam);
        VOODOO_METHOD(SetProperty)(_In_ const wchar_t * name, _In_ Variant * const value);
        VOODOO_METHOD(GetProperty)(_In_ const wchar_t * name, _In_ Variant * const value) CONST;
        VOODOO_METHOD(ConnectTexture)(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture);
        VOODOO_METHOD(HandleError)(_In_opt_ CGcontext const pContext, _In_ uint32_t error);

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
    };
    /**
     * @}
     */
}
