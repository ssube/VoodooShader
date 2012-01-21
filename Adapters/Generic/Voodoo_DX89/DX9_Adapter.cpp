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

// Voodoo DX89
#include "DX9_Adapter.hpp"
#include "DX9_Version.hpp"
// CVoodoo3D
#include "CVoodoo3D8.hpp"
#include "CVoodoo3DDevice8.hpp"
#include "CVoodoo3D9.hpp"
// Voodoo Core
#include "Format.hpp"
#include "Support.inl"

VoodooShader::IEffect * testEffect = nullptr;

IDirect3DVertexBuffer9 * gpFSQuadVerts = nullptr;
IDirect3DSurface9 *backbufferSurf = nullptr;

IDirect3DSurface9 *surface_Frame0 = nullptr;
VoodooShader::ITexture* texture_Frame0 = nullptr;
IDirect3DSurface9 *surface_Pass0 = nullptr;
VoodooShader::ITexture* texture_Pass0 = nullptr;
IDirect3DSurface9 * surface_Scratch = nullptr;
VoodooShader::ITexture* texture_Scratch = nullptr;

namespace VoodooShader
{
    namespace VoodooDX9
    {
        VoodooResult DX9Adapter::SetTarget(const uint32_t index, _In_opt_ ITexture * pTarget)
        {
            ILoggerRef logger = m_Core->GetLogger();

            if (index > 3) {
                logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Invalid render target index %1%.") << index);
                return false;
            }

            m_RenderTarget[index] = pTarget;

            HRESULT result;

            if (!pTarget)
            {
                if (index == 0)
                {
                    result = m_RealDevice->SetRenderTarget(index, m_BackBuffer);

                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound backbuffer to render target %1%.") << index);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind backbuffer to render target %1%: %2%") << index << result);
                        return false;
                    }
                }
                else
                {
                    result = m_RealDevice->SetRenderTarget(index, NULL);

                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound null texture to render target %1%.") << index);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind null texture to render target %1%: %2%") << index << result);
                        return false;
                    }
                }
            }
            else
            {
                Variant texVar = CreateVariant();
                if (FAILED(pTarget->GetProperty(PropIds::D3D9Texture, &texVar)))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Unable to retrieve hardware texture property from texture %1%.") << pTarget);
                    return VSFERR_INVALIDPARAMS;
                }

                IDirect3DTexture9 * pD3D9Tex = reinterpret_cast<IDirect3DTexture9*>(texVar.VPVoid);

                if (!pD3D9Tex)
                {
                    result = m_RealDevice->SetRenderTarget(index, nullptr);
                    if (SUCCEEDED(result))
                    {
                        logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound null texture data to render target %1%: %2%") << index << result);
                        return true;
                    }
                    else
                    {
                        logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind null texture data to render target %1%: %2%") << index << result);
                        return false;
                    }
                }

                pD3D9Tex->AddRef();

                IDirect3DSurface9 * pD3D9Surf = nullptr;
                result = pD3D9Tex->GetSurfaceLevel(0, &pD3D9Surf);

                if (!SUCCEEDED(result))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Failed to get surface of texture '%1%', data %2%: %3%") << pTarget << pD3D9Tex << result);
                    pD3D9Surf->Release();
                    pD3D9Tex->Release();
                    return false;
                }

                result = m_RealDevice->SetRenderTarget(index, pD3D9Surf);

                pD3D9Surf->Release();
                pD3D9Tex->Release();

                if (!SUCCEEDED(result))
                {
                    logger->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, Format("Device failed to bind texture '%1%' to render target %2%: %3%") << pTarget << index << result);
                    return false;
                }
                else
                {
                    logger->LogMessage(VSLog_ModDebug, VOODOO_DX89_NAME, Format("Bound texture '%1%' to render target %2%: %3%") << pTarget << index << result);
                    return true;
                }
            }
        }

        VoodooResult VOODOO_METHODTYPE DX9Adapter::DrawGeometry
        (
            _In_ const uint32_t offset,
            _In_ const uint32_t count,
            _In_ void * const pData,
            _In_ const VertexFlags flags
        )
        {
            if (!pData)
            {
                m_Core->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("No geometry given to draw."));
                return false;
            }

            HRESULT hr;

            // Set the necessary states
            hr = m_RealDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
            hr = m_RealDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
            hr = m_RealDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
            hr = m_RealDevice->SetRenderState(D3DRS_CULLMODE, D3DCUVSLog_NONE);

            if (flags & VF_Transformed)
            {
                m_RealDevice->SetVertexDeclaration(m_VertDeclT);
            }
            else
            {
                m_RealDevice->SetVertexDeclaration(m_VertDecl);
            }

            if (flags & VF_Buffer)
            {
                IDirect3DVertexBuffer9 * pVertexBuffer = reinterpret_cast<IDirect3DVertexBuffer9*>(pData);

                hr = m_RealDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VertexDesc));

                hr = m_RealDevice->BeginScene();

                if (SUCCEEDED(hr))
                {
                    hr = m_RealDevice->DrawPrimitive(D3DPT_TRIANGLELIST, offset, count);
                    hr = m_RealDevice->EndScene();
                }
                else
                {
                    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to draw geometry."));
                    return false;
                }
            }
            else
            {
                hr = m_RealDevice->BeginScene();

                if (SUCCEEDED(hr))
                {
                    hr = m_RealDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count, pData, sizeof(VertexDesc));
                    hr = m_RealDevice->EndScene();
                }
                else
                {
                    gpVoodooCore->GetLogger()->LogMessage(VSLog_ModError, VOODOO_DX89_NAME, VSTR("Failed to draw geometry."));
                    return false;
                }
            }

            return true;
        }

}