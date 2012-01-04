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

#include "VSAdapter.hpp"

#include "ICore.hpp"
#include "ILogger.hpp"

#include "Format.hpp"

namespace VoodooShader
{
    VSAdapter::VSAdapter(ICore * pCore) :
        m_Refs(0), m_Core(pCore)
    { 
    }

    VSAdapter::~VSAdapter()
    { 
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
    }

    uint32_t VOODOO_METHODTYPE VSAdapter::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSAdapter::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return m_Refs;
        }
    }

    bool VOODOO_METHODTYPE VSAdapter::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = CLSID_VSAdapter;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            }
            else if (clsid == IID_IAdapter)
            {
                *ppOut = static_cast<const IAdapter*>(this);
            }
            else if (clsid == CLSID_VSAdapter)
            {
                *ppOut = static_cast<const VSAdapter*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VOODOO_METHODTYPE VSAdapter::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return VSTR("VSAdapter()");
    }

    ICore * VOODOO_METHODTYPE VSAdapter::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    bool VOODOO_METHODTYPE VSAdapter::LoadPass(IPass * pPass)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pPass);

        return true;
    }

    bool VOODOO_METHODTYPE VSAdapter::UnloadPass(IPass * pPass)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pPass);

        return true;
    }

    bool VOODOO_METHODTYPE VSAdapter::SetPass(IPass * pPass)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pPass);

        return true;
    }

    IPass * VOODOO_METHODTYPE VSAdapter::GetPass() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return nullptr;
    }

    bool VOODOO_METHODTYPE VSAdapter::ResetPass(IPass * pPass)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pPass);

        return true;
    }

    bool VOODOO_METHODTYPE VSAdapter::SetTarget(const uint32_t index, ITexture * pTarget)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(index);
        UNREFERENCED_PARAMETER(pTarget);

        return true;
    }

    ITexture * VOODOO_METHODTYPE VSAdapter::GetTarget(const uint32_t index) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(index);

        return nullptr;
    }

    ITexture * VOODOO_METHODTYPE VSAdapter::CreateTexture(const String & name, const TextureDesc pDesc)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(pDesc);

        return nullptr;
    }

    bool VOODOO_METHODTYPE VSAdapter::LoadTexture(IImage * const pFile, const TextureRegion pRegion, ITexture * const pTexture)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pFile);
        UNREFERENCED_PARAMETER(pRegion);
        UNREFERENCED_PARAMETER(pTexture);

        return false;
    }

    bool VOODOO_METHODTYPE VSAdapter::DrawGeometry(const uint32_t offset, const uint32_t count, void * const pData, const VertexFlags flags)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(offset);
        UNREFERENCED_PARAMETER(count);
        UNREFERENCED_PARAMETER(pData);
        UNREFERENCED_PARAMETER(flags);

        return true;
    }

    bool VOODOO_METHODTYPE VSAdapter::ApplyParameter(IParameter * const pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pParam);

        return true;
    }

    bool VOODOO_METHODTYPE VSAdapter::SetProperty(const wchar_t * name, Variant * const value)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(value);

        return true;
    }

    bool VOODOO_METHODTYPE VSAdapter::GetProperty(const wchar_t * name, Variant * const value) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(value);

        return false;
    }

    bool VOODOO_METHODTYPE VSAdapter::ConnectTexture(IParameter * const pParam, ITexture * const pTexture)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pParam);
        UNREFERENCED_PARAMETER(pTexture);

        return true;
    }

    bool VOODOO_METHODTYPE VSAdapter::HandleError(CGcontext const pContext, const uint32_t error)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        UNREFERENCED_PARAMETER(pContext);
        UNREFERENCED_PARAMETER(error);

        return true;
    }
}
