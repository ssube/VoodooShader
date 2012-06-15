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

#include "VSParameterDX9.hpp"
// Voodoo D3D9
#include "VSBindingDX9.hpp"
#include "VSEffectDX9.hpp"
#include "D3D9_Version.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        #define VOODOO_DEBUG_TYPE VSParameterDX9
        DeclareDebugCache();

        VSParameterDX9::VSParameterDX9(_In_ VSEffectDX9 * pEffect, _In_ D3DXHANDLE pParamHandle) :
            m_Refs(0), m_Effect(pEffect), m_DXHandle(pParamHandle)
        {
            if (!m_Effect)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create effect parameter with no effect."), nullptr);
            }

            m_Core = m_Effect->m_Core;
            m_Binding = m_Effect->m_Binding;

            if (!m_Effect)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create parameter with no hardware effect."), m_Core);
            }
            else if (!m_DXHandle)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create parameter with no hardware handle."), m_Core);
            }

            D3DXPARAMETER_DESC desc;
            ZeroMemory(&desc, sizeof(D3DXPARAMETER_DESC));
            if (FAILED(m_Effect->m_DXEffect->GetParameterDesc(m_DXHandle, &desc)))
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to retrieve parameter description."), m_Core);
            }

            m_Name = desc.Name;
            m_Desc.Type = (ParameterType)desc.Type;
            m_Desc.Columns = desc.Columns;
            m_Desc.Rows = desc.Rows;
            m_Desc.Elements = desc.Elements;

            AddThisToDebugCache();
        }

        VSParameterDX9::VSParameterDX9(_In_ VSBindingDX9 * pBinding, _In_ CONST String & name, _In_ ParameterDesc desc) :
            m_Refs(0), m_Binding(pBinding), m_Effect(nullptr), m_Name(name), m_Desc(desc), m_DXHandle(nullptr)
        {
            if (!m_Binding)
            {
                Throw(VOODOO_D3D9_NAME, VSTR("Unable to create virtual parameter with no binding."), nullptr);
            }

            m_Core = m_Binding->m_Core;

            AddThisToDebugCache();
        }

        VSParameterDX9::~VSParameterDX9()
        {
            RemoveThisFromDebugCache();
        }

        uint32_t VOODOO_METHODTYPE VSParameterDX9::AddRef() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VOODOO_METHODTYPE VSParameterDX9::Release() CONST
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

        VoodooResult VOODOO_METHODTYPE VSParameterDX9::QueryInterface(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
            if (!ppOut)
            {
                return VSFERR_INVALIDPARAMS;
            }
            else
            {
                if (refid == IID_IObject)
                {
                    *ppOut = static_cast<IObject*>(this);
                }
                else if (refid == IID_IParameter)
                {
                    *ppOut = static_cast<IParameter*>(this);
                }
                else if (refid == CLSID_VSParameterDX9)
                {
                    *ppOut = static_cast<VSParameterDX9*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return VSFERR_INVALIDUUID;
                }

                (*ppOut)->AddRef();
                return VSF_OK;
            }
        }

        String VOODOO_METHODTYPE VSParameterDX9::ToString() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return StringFormat("VSParameter(%1%)") << m_Name;
        }

        ICore * VOODOO_METHODTYPE VSParameterDX9::GetCore() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Core;
        }

        String VOODOO_METHODTYPE VSParameterDX9::GetName() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Name;
        }

        VoodooResult VOODOO_METHODTYPE VSParameterDX9::GetProperty(_In_ CONST Uuid propid, _Out_ Variant * pValue) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pValue) VSFERR_INVALIDPARAMS;

            if (propid == PropIds::D3DX9Handle)
            {
                pValue->Type = VSUT_PVoid;
                pValue->VPVoid = (PVOID)m_DXHandle;
                return VSF_OK;
            }
            else
            {
                PropertyMap::const_iterator property = m_Properties.find(propid);
                if (property != m_Properties.end())
                {
                    CopyMemory(pValue, &property->second, sizeof(Variant));
                    return VSF_OK;
                }
            }

			*pValue = CreateVariant();
            return VSFERR_INVALIDCALL;
        }

        VoodooResult VOODOO_METHODTYPE VSParameterDX9::SetProperty(_In_ CONST Uuid propid, _In_ Variant * pValue)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (propid == PropIds::D3DX9Handle)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Properties[propid] = (*pValue);
            return VSF_OK;
        }

        VOODOO_METHODDEF_(ParameterDesc, VSParameterDX9::GetDesc)() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Desc;
        }

        VOODOO_METHODDEF(VSParameterDX9::GetBool)(_Out_ bool * pVal) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pVal) return VSFERR_INVALIDPARAMS;
            if (m_Desc.Type != VSPT_Bool) 
			{
				*pVal = false;
				return VSFERR_INVALIDCALL;
			}

            if (m_Effect && m_DXHandle)
            {
                BOOL rv = 0;
                if (SUCCEEDED(m_Effect->m_DXEffect->GetBool(m_DXHandle, &rv)))
                {
                    m_VBool = (rv != 0);
                }
                else
                {
					*pVal = false;
                    return VSFERR_APIERROR;
                }
            }

            (*pVal) = m_VBool;

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::GetFloat)(_Out_ float * pVal) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pVal) return VSFERR_INVALIDPARAMS;
            if (m_Desc.Type != VSPT_Float) 
			{
				*pVal = 0.0;
				return VSFERR_INVALIDCALL;
			}

            if (m_Effect && m_DXHandle)
            {
                float tVal;
                if (SUCCEEDED(m_Effect->m_DXEffect->GetFloat(m_DXHandle, &tVal)))
                {
                    m_VFloat.X = tVal;
                }
                else
                {
					*pVal = 0.0f;
                    return VSFERR_APIERROR;
                }
            }
        
            (*pVal) = m_VFloat.X;

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::GetInt)(_Out_ int32_t * pVal) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pVal) return VSFERR_INVALIDPARAMS;
            if (m_Desc.Type != VSPT_Int) 
			{
				*pVal = 0;
				return VSFERR_INVALIDCALL;
			}

            if (m_Effect && m_DXHandle && FAILED(m_Effect->m_DXEffect->GetInt(m_DXHandle, &m_VInt)))
            {
				*pVal = 0;
                return VSFERR_APIERROR;
            }

            (*pVal) = m_VInt;

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::GetString)(_Out_ String * pVal) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pVal) return VSFERR_INVALIDPARAMS;
            if (m_Desc.Type != VSPT_String) 
			{
				*pVal = String();
				return VSFERR_INVALIDCALL;
			}

            if (m_Effect && m_DXHandle)
            {
                LPCSTR rv = nullptr;
                if (SUCCEEDED(m_Effect->m_DXEffect->GetString(m_DXHandle, &rv)) && rv)
                {
                    m_VString = String(rv);
                }
                else
                {
					*pVal = String();
                    return VSFERR_APIERROR;
                }
            }

            (*pVal) = m_VString;

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::GetTexture)(_Out_ ITexture ** ppVal) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!ppVal) return VSFERR_INVALIDPARAMS;
            if (m_Desc.Type < VSPT_Texture || m_Desc.Type > VSPT_TextureCube)
			{
				*ppVal = nullptr;
				return VSFERR_INVALIDCALL;
			}

            (*ppVal) = m_Texture.get();

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::GetVector)(_Out_ Float4 * pVal) CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pVal) return VSFERR_INVALIDPARAMS;
            if (m_Desc.Type != VSPT_Float && m_Desc.Type != VSPT_Bool && m_Desc.Type != VSPT_Int)
			{
				ZeroMemory(pVal, sizeof(Float4));
				return VSFERR_INVALIDCALL;
			}

            if (m_Effect && m_DXHandle)
            {
                D3DXVECTOR4 rv;
                if (SUCCEEDED(m_Effect->m_DXEffect->GetVector(m_DXHandle, &rv)))
                {
                    m_VFloat.X = rv.x;
                    m_VFloat.Y = rv.y;
                    m_VFloat.Z = rv.z;
                    m_VFloat.W = rv.w;
                }
                else
                {
					ZeroMemory(pVal, sizeof(Float4));
                    return VSFERR_APIERROR;
                }
            }

            (*pVal) = m_VFloat;

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::SetBool)(_In_ CONST bool val)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (m_Desc.Type != VSPT_Bool) return VSFERR_INVALIDCALL;

            m_VBool = val;

            ParameterList::iterator child = m_Attached.begin();
            while (child != m_Attached.end())
            {
                (*child)->SetBool(val);
            }

            if (m_Effect && m_DXHandle && FAILED(m_Effect->m_DXEffect->SetBool(m_DXHandle, val)))
            {
                return VSFERR_APIERROR;
            }

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::SetFloat)(_In_ CONST float val)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (m_Desc.Type != VSPT_Float) return VSFERR_INVALIDCALL;

            m_VFloat.X = val;

            ParameterList::iterator child = m_Attached.begin();
            while (child != m_Attached.end())
            {
                (*child)->SetFloat(val);
            }

            if (m_Effect && m_DXHandle && FAILED(m_Effect->m_DXEffect->SetFloat(m_DXHandle, val)))
            {
                return VSFERR_APIERROR;
            }

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::SetInt)(_In_ CONST int32_t val)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (m_Desc.Type != VSPT_Int) return VSFERR_INVALIDCALL;

            m_VInt = val;

            ParameterList::iterator child = m_Attached.begin();
            while (child != m_Attached.end())
            {
                (*child)->SetInt(val);
            }

            if (m_Effect && m_DXHandle && FAILED(m_Effect->m_DXEffect->SetInt(m_DXHandle, val)))
            {
                return VSFERR_APIERROR;
            }

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::SetString)(_In_ CONST String & val)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (m_Desc.Type != VSPT_String) return VSFERR_INVALIDCALL;

            m_VString = val;

            ParameterList::iterator child = m_Attached.begin();
            while (child != m_Attached.end())
            {
                (*child)->SetString(val);
            }

            if (m_Effect && m_DXHandle)
            {
                std::string vstr = val.ToStringA();
                if (FAILED(m_Effect->m_DXEffect->SetString(m_DXHandle, vstr.c_str())))
                {
                    return VSFERR_APIERROR;
                }
            }

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::SetTexture)(_In_ ITexture * pVal)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (m_Desc.Type < VSPT_Texture || m_Desc.Type > VSPT_TextureCube) return VSFERR_INVALIDCALL;

            m_Texture = pVal;

            ParameterList::iterator child = m_Attached.begin();
            while (child != m_Attached.end())
            {
                (*child)->SetTexture(pVal);
            }

            if (m_Effect && m_DXHandle)
            {
                Variant propVar = CreateVariant();
                if (FAILED(pVal->GetProperty(PropIds::D3D9Texture, &propVar)))
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_D3D9_NAME, StringFormat("Unable to get hardware texture from texture %1%.") << pVal);
                    return VSFERR_INVALIDPARAMS;
                }

                LPDIRECT3DTEXTURE9 pDXTexture = reinterpret_cast<LPDIRECT3DTEXTURE9>(propVar.VPVoid);

                HRESULT hr = m_Effect->m_DXEffect->SetTexture(m_DXHandle, pDXTexture);

                if (FAILED(hr))
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugError, VOODOO_D3D9_NAME, StringFormat("Unable to bind texture %1% to parameter %2%.") << pVal << this);
                    return VSFERR_INVALIDCALL;
                }

                m_Core->GetLogger()->LogMessage(VSLog_PlugDebug, VOODOO_D3D9_NAME, StringFormat("Bound texture %1% to parameter %2%.") << pVal << this);
            }

            return VSF_OK;
        }

        VOODOO_METHODDEF(VSParameterDX9::SetVector)(_In_ CONST Float4 val)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (m_Desc.Type != VSPT_Float) return VSFERR_INVALIDCALL;

            m_VFloat = val;

            ParameterList::iterator child = m_Attached.begin();
            while (child != m_Attached.end())
            {
                (*child)->SetVector(val);
            }

            if (m_Effect && m_DXHandle)
            {
                D3DXVECTOR4 sv;
                sv.x = val.X;
                sv.y = val.Y;
                sv.z = val.Z;
                sv.w = val.W;

                if (FAILED(m_Effect->m_DXEffect->SetVector(m_DXHandle, &sv)))
                {
                    return VSFERR_APIERROR;
                }
            }

            return VSF_OK;
        }

        bool VOODOO_METHODTYPE VSParameterDX9::IsVirtual() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return (m_Effect && m_DXHandle);
        }

        VoodooResult VOODOO_METHODTYPE VSParameterDX9::AttachParameter(_In_ IParameter * pParam)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            if (!pParam)
            {
                return VSFERR_INVALIDPARAMS;
            }

            m_Attached.push_back(pParam);

            return VSF_OK;
        }

        VoodooResult VOODOO_METHODTYPE VSParameterDX9::DetachParameter(_In_ IParameter * pParam)
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            std::remove_if(m_Attached.begin(), m_Attached.end(), 
				[&pParam](ParameterRef param){
				return param.get() == pParam;
			});

            return VSF_OK;
        }

        IEffect * VOODOO_METHODTYPE VSParameterDX9::GetEffect() CONST
        {
            VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

            return m_Effect;
        }

        void VOODOO_METHODTYPE VSParameterDX9::Link()
        {
            // Handle textures 
            if (m_Desc.Type >= VSPT_Texture && m_Desc.Type <= VSPT_TextureCube)
            {
                D3DXHANDLE texAnnot = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texture");
                if (texAnnot)
                {
                    LPCSTR texName = nullptr;
                    if (SUCCEEDED(m_Effect->m_DXEffect->GetString(texAnnot, &texName)) && texName)
                    {
                        m_Texture = m_Core->GetTexture(texName);

                        if (!m_Texture)
                        {
                            m_Texture = this->LinkNewTexture();
                        }

                        if (m_Texture)
                        {
                            this->SetTexture(m_Texture.get());
                        }
                    }
                    else
                    {
                        m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                            StringFormat("Texture parameter %1% has no source texture annotation.") << m_Name);
                    }
                }
            }

            // Handle linkage
            D3DXHANDLE sourceAnnot = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_source");
            if (sourceAnnot)
            {
                LPCSTR sourceName = NULL;
                if (SUCCEEDED(m_Effect->m_DXEffect->GetString(sourceAnnot, &sourceName)) && sourceName)
                {
                    IParameter * sourceParam = m_Core->GetParameter(sourceName, m_Desc);
                    if (sourceParam)
                    {
                        sourceParam->AttachParameter(this);
                    }
                }
                else
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to get source annotation for parameter %1%.") << m_Name);
                }
            }
        }

        ITexture * VOODOO_METHODTYPE VSParameterDX9::LinkNewTexture()
        {
            if (m_Desc.Type < VSPT_Texture || m_Desc.Type > VSPT_TextureCube)
            {
                return nullptr;
            }

            D3DXHANDLE nameA = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texture");
            D3DXHANDLE dimA  = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texdim");
            D3DXHANDLE mipA  = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texmip");
            D3DXHANDLE rttA  = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texrtt");
            D3DXHANDLE fmtA  = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texfmt");
            D3DXHANDLE srcA  = m_Effect->m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texsrc");

            if (srcA && nameA)
            {
                LPCSTR nameStr = NULL;
                if (FAILED(m_Effect->m_DXEffect->GetString(nameA, &nameStr)))
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to get texture name from parameter %1%.") << m_Name);
                    return nullptr;
                }

                m_Core->GetLogger()->LogMessage(VSLog_PlugInfo, VOODOO_D3D9_NAME, 
                    StringFormat("Loading texture %1% for parameter %2%.") << nameStr << m_Name);

                LPCSTR srcStr = nullptr;
                if (FAILED(m_Effect->m_DXEffect->GetString(srcA, &srcStr)) || !srcStr)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to get texture source from parameter %1%.") << m_Name);
                    return nullptr;
                }

                IFile * pFile = m_Core->GetFileSystem()->GetFile(srcStr);
                if (!pFile)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to get find texture file %1%.") << srcStr);
                    return nullptr;
                }

                ITexture * pSrcTex = m_Core->LoadTexture(nameStr, pFile);
                if (!pSrcTex)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to open texture file %1%.") << srcStr);
                    return nullptr;
                }

                return pSrcTex;
            } 
            else if (nameA || dimA || fmtA)
            {
                LPCSTR nameStr = NULL;
                if (FAILED(m_Effect->m_DXEffect->GetString(nameA, &nameStr)) || !nameStr)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to get texture name from parameter %1%.") << m_Name);
                    return nullptr;
                }

                DECLARE_AND_ZERO(D3DXVECTOR4, dimVec);
                if (FAILED(m_Effect->m_DXEffect->GetVector(dimA, &dimVec)))
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to get texture dimensions from parameter %1%.") << m_Name);
                    return nullptr;
                }

                UInt3 dim = {(uint32_t)dimVec.x, (uint32_t)dimVec.y, (uint32_t)dimVec.z};

                LPCSTR fmtStr = NULL;
                if (FAILED(m_Effect->m_DXEffect->GetString(fmtA, &fmtStr)) || !fmtStr)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to get texture format from parameter %1%.") << m_Name);
                    return nullptr;
                }

                TextureFormat fmt = Converter::ToTextureFormat(String(fmtStr).GetData());
                if (fmt == VSFmt_Unknown)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unknown texture format on parameter %1%.") << m_Name);
                    return nullptr;
                }            

                BOOL mipB = FALSE;
                m_Effect->m_DXEffect->GetBool(mipA, &mipB);
                uint32_t mip = (mipB == TRUE) ? 0 : 1;

                BOOL rttB = TRUE;
                m_Effect->m_DXEffect->GetBool(rttA, &rttB);
                TextureFlags rtt = (rttB == TRUE) ? VSTexFlag_Target : VSTexFlag_None;

                TextureDesc desc = {dim, mip, rtt, fmt};

                m_Core->GetLogger()->LogMessage(VSLog_PlugInfo, VOODOO_D3D9_NAME, 
                    StringFormat("Creating texture %1% as %2% for parameter %3%.") << nameStr << desc << m_Name);

                ITexture * pTex = m_Core->CreateTexture(nameStr, desc);
                if (!pTex)
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Unable to create texture for parameter %1%.") << m_Name);
                    return nullptr;
                }
                else
                {
                    m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                        StringFormat("Successfully created texture for parameter %1%.") << m_Name);
                    return pTex;
                }
            } else {
                m_Core->GetLogger()->LogMessage(VSLog_PlugWarning, VOODOO_D3D9_NAME, 
                    StringFormat("Texture parameter %1% has texture create flag set but is missing required parameters.") << 
                    m_Name);
                return nullptr;
            }
        }
    }
}
