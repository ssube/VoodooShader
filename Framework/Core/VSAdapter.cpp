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

#include "VSAdapter.hpp"

namespace VoodooShader
{
    VSAdapter::VSAdapter(ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { }

    VSAdapter::~VSAdapter(void)
    { }

    uint32_t VSAdapter::AddRef() const
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VSAdapter::Release() const
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VSAdapter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSAdapter;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_IAdapter) {
                *ppOut = static_cast<const IAdapter*>(this);
            } else if (clsid == CLSID_VSAdapter) {
                *ppOut = static_cast<const VSAdapter*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VSAdapter::ToString(void) const
    {
        return L"VSAdapter()";
    }

    ICore * VSAdapter::GetCore(void) const
    {
        return m_Core;
    }

    bool VSAdapter::LoadPass(IPass * pPass)
    {
        UNREFERENCED_PARAMETER(pPass);

        return true;
    }

    bool VSAdapter::UnloadPass(IPass * pPass)
    {
        UNREFERENCED_PARAMETER(pPass);

        return true;
    }

    bool VSAdapter::SetPass(IPass * pPass)
    {
        UNREFERENCED_PARAMETER(pPass);

        return true;
    }

    IPass * VSAdapter::GetPass(void) const
    {
        return nullptr;
    }

    bool VSAdapter::SetTarget(const uint32_t index, ITexture * pTarget)
    {
        UNREFERENCED_PARAMETER(index);
        UNREFERENCED_PARAMETER(pTarget);

        return true;
    }

    ITexture * VSAdapter::GetTarget(const uint32_t index) const
    {
        UNREFERENCED_PARAMETER(index);

        return nullptr;
    }

    bool VSAdapter::CreateTexture(const String & name, const TextureDesc * const pDesc, ITexture * const pTexture)
    {
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(pDesc);
        UNREFERENCED_PARAMETER(pTexture);

        return false;
    }

    bool VSAdapter::LoadTexture(IFile * const pFile, const TextureRegion * const pRegion, ITexture * const pTexture)
    {
        UNREFERENCED_PARAMETER(pFile);
        UNREFERENCED_PARAMETER(pRegion);
        UNREFERENCED_PARAMETER(pTexture);

        return false;
    }

    bool VSAdapter::DrawGeometry(const uint32_t count, const VertexStruct * const pVertexData, const VertexFlags flags)
    {
        UNREFERENCED_PARAMETER(count);
        UNREFERENCED_PARAMETER(pVertexData);
        UNREFERENCED_PARAMETER(flags);

        return true;
    }

    bool VSAdapter::ApplyParameter(IParameter * const pParam)
    {
        UNREFERENCED_PARAMETER(pParam);

        return true;
    }

    bool VSAdapter::SetProperty(const wchar_t * name, Variant & value)
    {
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(value);

        return true;
    }

    Variant VSAdapter::GetProperty(const wchar_t * name) const
    {
        UNREFERENCED_PARAMETER(name);

        Variant ret = {UT_None, nullptr};

        return ret;
    }

    bool VSAdapter::ConnectTexture(IParameter * const pParam, ITexture * const pTexture)
    {
        UNREFERENCED_PARAMETER(pParam);
        UNREFERENCED_PARAMETER(pTexture);

        return true;
    }

    bool VSAdapter::HandleError(CGcontext const pContext, const uint32_t error)
    {
        UNREFERENCED_PARAMETER(pContext);
        UNREFERENCED_PARAMETER(error);

        return true;
    }
}
