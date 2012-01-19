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

#include "VSBindingDX9.hpp"
#include "VSEffectDX9.hpp"
#include "VSParameterDX9.hpp"
#include "VSTextureDX9.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        VSBindingDX9::VSBindingDX9(_In_ ICore * pCore)
            : m_Refs(0), m_Core(pCore)
        {
        }
        ~VSBindingDX9::VSBindingDX9();

        VOODOO_METHODDEF_(uint32_t, VSBindingDX9::AddRef)() CONST;
        VOODOO_METHODDEF_(uint32_t, VSBindingDX9::Release)() CONST;
        VOODOO_METHODDEF(VSBindingDX9::QueryInterface)(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut);
        VOODOO_METHODDEF_(String, VSBindingDX9::ToString)() CONST;
        VOODOO_METHODDEF_(ICore *, VSBindingDX9::GetCore)() CONST;
        VOODOO_METHODDEF(VSBindingDX9::Initialize)(uint32_t count, _In_count_(count) Variant * pParams);
        VOODOO_METHODDEF_(IEffect *, VSBindingDX9::CreateEffect)(CONST String & source);
        VOODOO_METHODDEF_(IEffect *, VSBindingDX9::CreateEffectFromFile)(CONST String & filename);
        VOODOO_METHODDEF_(IParameter *, VSBindingDX9::CreateParameter)(CONST String & name, ParameterDesc desc);
        VOODOO_METHODDEF_(ITexture *, VSBindingDX9::CreateTexture)(CONST String & name, TextureDesc desc);
        VOODOO_METHODDEF_(ITexture *, VSBindingDX9::LoadTexture)(CONST String & filename);
    }
}
