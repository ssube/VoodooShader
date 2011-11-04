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
        return ++m_Refs;
    }

    uint32_t VSAdapter::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VSAdapter::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
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
        return L"VSAdapter";
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

    void VSAdapter::SetPass(IPass * pPass)
    {
        UNREFERENCED_PARAMETER(pPass);
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

    ITexture * VSAdapter::CreateTexture(const String & name, const TextureDesc * const pDesc)
    {
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(pDesc);

        return nullptr;
    }

    ITexture * VSAdapter::LoadTexture(IFile * const pFile, const TextureRegion * const pRegion)
    {
        UNREFERENCED_PARAMETER(pFile);
        UNREFERENCED_PARAMETER(pRegion);

        return nullptr;
    }

    void VSAdapter::DrawGeometry(const uint32_t count, const VertexStruct * const pVertexData)
    {
        UNREFERENCED_PARAMETER(count);
        UNREFERENCED_PARAMETER(pVertexData);
    }

    void VSAdapter::DrawShader(IShader * const pShader)
    {
        UNREFERENCED_PARAMETER(pShader);
    }

    void VSAdapter::ApplyParameter(IParameter * const pParam)
    {
        UNREFERENCED_PARAMETER(pParam);
    }

    void VSAdapter::SetProperty(const String & property, const String & value)
    {
        UNREFERENCED_PARAMETER(property);
        UNREFERENCED_PARAMETER(value);
    }

    String VSAdapter::GetProperty(const String & property) const
    {
        UNREFERENCED_PARAMETER(property);

        return String();
    }

    bool VSAdapter::ConnectTexture(IParameter * const pParam, ITexture * const pTexture)
    {
        UNREFERENCED_PARAMETER(pParam);
        UNREFERENCED_PARAMETER(pTexture);

        return true;
    }

    void VSAdapter::HandleError(CGcontext const pContext, const uint32_t error)
    {
        UNREFERENCED_PARAMETER(pContext);
        UNREFERENCED_PARAMETER(error);
    }
}
