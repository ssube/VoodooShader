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

#include "VSProgram.hpp"

#include "VSParameter.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSProgram
    DeclareDebugCache();

    VSProgram::VSProgram(VSPass * pPass, XmlNode * pProgNode) :
        m_Refs(0), m_Pass(pPass) 
    {
        if (!m_Pass)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Cannot create a pass with no parent technique."), nullptr);
        }

        m_Core = m_Pass->GetCore();

        pugi::xml_attribute xProfile = (*pProgNode)->attribute(VSTR("profile"));
        pugi::xml_attribute xFunction = (*pProgNode)->attribute(VSTR("function"));
        String profile = xProfile.value();
        m_Function = xFunction.value();

        m_Name = m_Pass->GetName() + VSTR(":") + m_Function;

        AddThisToDebugCache();
    }

    VSProgram::~VSProgram()
    {
        RemoveThisFromDebugCache();
    }

    uint32_t VOODOO_METHODTYPE VSPass::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSPass::Release() CONST
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

    VoodooResult VOODOO_METHODTYPE VSProgram::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const IObject ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!ppOut) return VSF_FAIL;
            
        if (refid == IID_IObject)
        {
            *ppOut = static_cast<const IObject*>(this);
        }
        else if (refid == IID_IProgram)
        {
            *ppOut = static_cast<const IProgram*>(this);
        }
        else if (refid == CLSID_VSProgram)
        {
            *ppOut = static_cast<const VSProgram*>(this);
        }
        else
        {
            *ppOut = nullptr;
            return VSF_FAIL;
        }

        (*ppOut)->AddRef();
        return VSF_OK;
    }

    String VOODOO_METHODTYPE VSProgram::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return Format(VSTR("VSProgram(%1%)")) << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSProgram::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Core;
    }

    String VOODOO_METHODTYPE VSProgram::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return m_Name;
    }

    VoodooResult VOODOO_METHODTYPE VSProgram::GetProperty(const String & name, Variant * pValue) CONST
    {
        if (!pValue) return VSF_FAIL;

        if (name.Compare("FunctionStream", false))
        {
            ZeroMemory(pValue, sizeof(Variant));
            pValue->Type = UT_PVoid;
            pValue->Components = 1;
            pValue->VPVoid = m_Buffer;

            return VSF_OK;
        }
        else 
        {
            VariantMap::const_iterator property = m_Properties.find(name);
            if (property != m_Properties.end())
            {
                CopyMemory(pValue, &property->second, sizeof(Variant));
                return VSF_OK;
            }
        }

        return VSFERR_UNKNOWNPROPERTY;
    }

    VoodooResult VOODOO_METHODTYPE VSProgram::SetProperty(const String & name, const Variant & value)
    {
        if (name.Compare("FunctionStream", false))
        {
            return VSFERR_INVALIDPARAMS;
        }
        else
        {
            m_Properties[name] = value;
            return VSF_OK;
        }
    }
     
    ProgramProfile VOODOO_METHODTYPE VSProgram::GetProfile() CONST
    {
        return m_Profile;
    }

    VoodooResult VOODOO_METHODTYPE VSProgram::SetProfile(const ProgramProfile profile)
    {
        m_Profile = profile;
        return VSF_OK;
    }

    String VOODOO_METHODTYPE VSProgram::GetFunction() CONST
    {
        return m_Function;
    }

    VoodooResult VOODOO_METHODTYPE VSProgram::SetFunction(const String & function)
    {
        m_Function = function;
    }

    IParameter * VOODOO_METHODTYPE VSProgram::GetConstant(const String & name) CONST
    {
        if (!m_ConstantTable) return nullptr;

        D3DXHANDLE handle = m_ConstantTable->GetConstantByName(NULL, (LPCSTR)name.GetData());
        if (!handle) return nullptr;

        VSParameter param = new VSParameter(this, handle, m_ConstantTable);
        return param;
    }

    VoodooResult VOODOO_METHODTYPE VSProgram::Compile(const CompileFlags flags)
    {
        if (m_Buffer) m_Buffer->Release();
        if (m_ConstantTable) m_ConstantTable->Release();

        // Convert the source to ASCII
        VSShader * pShader = m_Pass->GetTechnique()->GetShader();
        std::string source = pShader->GetSource().ToStringA();

        // Convert function to ASCII
        std::string function = m_Function.ToStringA();

        D3DXMACRO * pMacros = pShader->m_Defines.size() > 0 ? &pShader->m_Defines[0] : NULL;
        LPCSTR profile = (LPCSTR)Converter::ToString(m_Profile);
        DWORD cflags;
        if (flags != 0)
        {
            cflags = flags;
        } else {
            cflags = m_Core->GetDefaultFlags();
        }

        LPD3DXBUFFER errors = NULL;

        // Compile
        HRESULT hr = D3DXCompileShader
        (
            source.c_str(), source.length(), 
            pMacros, NULL, function.c_str(), profile, cflags, 
            &m_Buffer, &errors, &m_ConstantTable
        );

        if (FAILED(hr))
        {
            if (errors)
            {
                m_Core->GetLogger()->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format("Error compiling shader:\n%1%") << (LPCSTR)errors->GetBufferPointer());
                errors->Release();
            } else {
                m_Core->GetLogger()->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format("Unknown error while compiling program %1%") << this);
            }

            if (m_Buffer) m_Buffer->Release(); m_Buffer = nullptr;
            if (m_ConstantTable) m_ConstantTable->Release(); m_ConstantTable = nullptr;
        }

        return VSF_OK;
    }
}