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

#include "DX9_Module.hpp"

#include "DX9_Adapter.hpp"
#include "DX9_Version.hpp"

VoodooShader::ICore * gpVoodooCore = nullptr;

VoodooShader::VoodooDX9::IVoodoo3D9 * VoodooObject = nullptr;
VoodooShader::VoodooDX9::IVoodoo3DDevice9 * VoodooDevice = nullptr;

IDirect3DSurface9 *backbufferSurf = nullptr;

IDirect3DSurface9 *surface_ThisFrame = nullptr;
VoodooShader::ITexture* texture_ThisFrame;

VoodooShader::IShader* testShader;
/*

namespace VoodooShader
{
 namespace DirectX9
{

  hr = this->mDevice->CreateVertexBuffer
   (
    6 * sizeof(FSVert),
    0,
    D3DFVF_CUSTOMVERTEX,
    D3DPOOL_DEFAULT,
    &FSQuadVerts,
    nullptr
   );

  if (FAILED(hr))
  {
   mCore->Log("Voodoo DX9: Failed to create vertex buffer.\n");
  }

  FSVert g_Vertices[4];

  memset(g_Vertices, 0, sizeof(FSVert) * 4);

  g_Vertices[0].x = -0.5f;
  g_Vertices[0].y = -0.5f;
  g_Vertices[0].z = 0.5f;
  g_Vertices[1].x = fx;
  g_Vertices[1].y = -0.5f;
  g_Vertices[1].z = 0.5f;
  g_Vertices[2].x = -0.5f;
  g_Vertices[2].y = fy;
  g_Vertices[2].z = 0.5f;
  g_Vertices[3].x = fx;
  g_Vertices[3].y = fy;
  g_Vertices[3].z = 0.5f;

  g_Vertices[0].rhw = g_Vertices[1].rhw = g_Vertices[2].rhw = g_Vertices[3].rhw = 1.0f;

  g_Vertices[0].tu = 0.0f;
  g_Vertices[0].tv = 0.0f;
  g_Vertices[1].tu = 1.0f;
  g_Vertices[1].tv = 0.0f;
  g_Vertices[2].tu = 0.0f;
  g_Vertices[2].tv = 1.0f;
  g_Vertices[3].tu = 1.0f;
  g_Vertices[3].tv = 1.0f;

  void *pVertices;

  FSQuadVerts->Lock(0, sizeof(FSVert) * 4, &pVertices, 0);

  memcpy(pVertices, g_Vertices, sizeof(FSVert) * 4);

  FSQuadVerts->Unlock();
 }*/

const VoodooShader::Version * VOODOO_CALL API_VersionFunc()
{
    static const VoodooShader::Version dx9version = VOODOO_META_VERSION_STRUCT(DX9);
    return &dx9version;
}

const uint32_t VOODOO_CALL API_CountFunc()
{
    return 1;
}

const wchar_t * VOODOO_CALL API_InfoFunc(_In_ const uint32_t index, _Out_ VoodooShader::Uuid * refid)
{
    if (!refid)
    {
        return nullptr;
    }

    if (index == 0)
    {
        *refid = VoodooShader::VoodooDX9::CLSID_DX9Adapter;
        return L"DX9Adapter";
    } else {
        return nullptr;
    }
}

VoodooShader::IObject * VOODOO_CALL API_CreateFunc(_In_ const uint32_t index, _In_ VoodooShader::ICore * pCore)
{
    if (index == 0)
    {
        return new VoodooShader::VoodooDX9::DX9Adapter(pCore);
    } else {
        return nullptr;
    }
}
