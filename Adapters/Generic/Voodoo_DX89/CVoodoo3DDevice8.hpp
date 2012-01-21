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

// Voodoo DX89
#include "DX9_Adapter.hpp"
#include "DX8_ThinHeader.hpp"
// System
#include <set>

namespace VoodooShader
{
    namespace VoodooDX8
    {
        /**
         * The core wrapper class for D3D8 devices. This class is responsible for processing, translating and handling most 
         * API calls. Due to this, it should be as optimized as possible (considering the circumstances) and must contain 
         * only tested code.
         *
         * @note The various CVoodoo3D8 interfaces provide an application-opaque wrapper that actually implements a
         *      Direct3D 8.9 layer (8 to 9 translation). For use with D3D9 applications, the CVoodoo3D9 interface set must
         *      be used.
         */
        class CVoodoo3DDevice8 :
            public IDirect3DDevice8
        {
        public:
            /**
             * The default, public constructor for CVoodoo3D objects.
             */
            CVoodoo3DDevice8(IDirect3DDevice9 *realDevice);
            ~CVoodoo3DDevice8();

            /*** IUnknown methods ***/
            STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
            STDMETHOD_(ULONG,AddRef)(THIS);
            STDMETHOD_(ULONG,Release)(THIS);

            /*** IDirect3DDevice8 methods ***/
            STDMETHOD(TestCooperativeLevel)(THIS);
            STDMETHOD_(UINT, GetAvailableTextureMem)(THIS);
            STDMETHOD(ResourceManagerDiscardBytes)(DWORD Bytes);
            STDMETHOD(GetDirect3D)(IDirect3D8** ppD3D8);
            STDMETHOD(GetDeviceCaps)(D3DCAPS8* pCaps);
            STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE* pMode);
            STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS *pParameters);
            STDMETHOD(SetCursorProperties)(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface8* pCursorBitmap);
            STDMETHOD_(void, SetCursorPosition)(UINT XScreenSpace,UINT YScreenSpace,DWORD Flags);
            STDMETHOD_(BOOL, ShowCursor)(BOOL bShow);
            STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS8* pPresentationParameters,IDirect3DSwapChain8** pSwapChain);
            STDMETHOD(Reset)(D3DPRESENT_PARAMETERS8* pPresentationParameters);
            STDMETHOD(Present)(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
            STDMETHOD(GetBackBuffer)(UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer);
            STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS* pRasterStatus);
            STDMETHOD_(void, SetGammaRamp)(DWORD Flags,CONST D3DGAMMARAMP* pRamp);
            STDMETHOD_(void, GetGammaRamp)(D3DGAMMARAMP* pRamp);
            /**
             * Creates a texture in the CVoodoo3D8 runtime. The new texture must be registered with the core and, for shader
             * purposes, we should attempt to create it as a render-target.
             */
            STDMETHOD(CreateTexture)(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8** ppTexture);
            STDMETHOD(CreateVolumeTexture)(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture8** ppVolumeTexture);
            STDMETHOD(CreateCubeTexture)(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture8** ppCubeTexture);
            STDMETHOD(CreateVertexBuffer)(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer8** ppVertexBuffer);
            STDMETHOD(CreateIndexBuffer)(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer8** ppIndexBuffer);
            /**
             * This function creates a stand-alone render target surface in the original D3D8 and 9 setups. For Voodoo's
             * purposes, this should create a render target texture if at all possible, then retrieve the surface from that
             * texture.
             *
             * @todo Find a way to force CreateRenderTarget to make RT textures and find a way to name them.
             */
            STDMETHOD(CreateRenderTarget)(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,BOOL Lockable,IDirect3DSurface8** ppSurface);
            STDMETHOD(CreateDepthStencilSurface)(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,IDirect3DSurface8** ppSurface);
            /**
             * Creates a D3D8 image surface, a sort of placeholder surface. Because D3D9 has no equivalent, this is 
             * redirected to CreateOffscreenPlainSurface at some point.
             * 
             * @warning This uses a different call than the D3D8 original, and may not perform exactly the same. 
             * @todo Test the pool these should be created in. May be default, managed or sysmem, not sure yet.
             */
            STDMETHOD(CreateImageSurface)(UINT Width,UINT Height,D3DFORMAT Format,IDirect3DSurface8** ppSurface);
            /**
             * Copies rectangular areas of data from one surface to another. There is no such function in D3D9, it instead 
             * requires each texture to be stretched separately. It is, however, possible to translate the call as D3D8 
             * applies some very specific restrictions, including a lack of stretching and the same format and surface 
             * properties.
             * 
             * @warning For testing purposes, this only copies the first given rectangle. This is easy enough to fix, but I 
             *      want to do more testing first.
             * @todo Test and then make this perform identical to the D3D8 version.
             */
            STDMETHOD(CopyRects)(IDirect3DSurface8* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,IDirect3DSurface8* pDestinationSurface,CONST POINT* pDestPointsArray);
            STDMETHOD(UpdateTexture)(IDirect3DBaseTexture8* pSourceTexture,IDirect3DBaseTexture8* pDestinationTexture);
            /**
             * Retrieves the front-buffer surface from the D3D8 device. D3D9 has no equivalent.
             */
            STDMETHOD(GetFrontBuffer)(IDirect3DSurface8* pDestSurface);
            /**
             * Sets a new render-target surface and depth/stencil buffer. D3D9 does not combine these calls, so this 
             * automatically splits the calls and returns the combined results.
             */
            STDMETHOD(SetRenderTarget)(IDirect3DSurface8* pRenderTarget,IDirect3DSurface8* pNewZStencil);
            STDMETHOD(GetRenderTarget)(IDirect3DSurface8** ppRenderTarget);
            STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface8** ppZStencilSurface);
            STDMETHOD(BeginScene)(THIS);
            STDMETHOD(EndScene)(THIS);
            STDMETHOD(Clear)(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
            STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
            STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix);
            STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE,CONST D3DMATRIX*);
            STDMETHOD(SetViewport)(CONST D3DVIEWPORT8* pViewport);
            STDMETHOD(GetViewport)(D3DVIEWPORT8* pViewport);
            STDMETHOD(SetMaterial)(CONST D3DMATERIAL8* pMaterial);
            STDMETHOD(GetMaterial)(D3DMATERIAL8* pMaterial);
            STDMETHOD(SetLight)(DWORD Index,CONST D3DLIGHT8*);
            STDMETHOD(GetLight)(DWORD Index,D3DLIGHT8*);
            STDMETHOD(LightEnable)(DWORD Index,BOOL Enable);
            STDMETHOD(GetLightEnable)(DWORD Index,BOOL* pEnable);
            STDMETHOD(SetClipPlane)(DWORD Index,CONST float* pPlane);
            STDMETHOD(GetClipPlane)(DWORD Index,float* pPlane);
            STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State,DWORD Value);
            STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State,DWORD* pValue);
            STDMETHOD(BeginStateBlock)(THIS);
            /**
             * Due to differences between the D3D8 and D3D9 APIs, this function is currently a nop.
             */
            STDMETHOD(EndStateBlock)(DWORD* pToken);
            STDMETHOD(ApplyStateBlock)(DWORD Token);
            STDMETHOD(CaptureStateBlock)(DWORD Token);
            /* This function does not exist in the D3D9 API. */
            STDMETHOD(DeleteStateBlock)(DWORD Token);
            /**
             * Due to differences between the D3D8 and D3D9 APIs, this function is currently a nop.
             */
            STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type,DWORD* pToken);
            STDMETHOD(SetClipStatus)(CONST D3DCLIPSTATUS8* pClipStatus);
            STDMETHOD(GetClipStatus)(D3DCLIPSTATUS8* pClipStatus);
            STDMETHOD(GetTexture)(DWORD Stage,IDirect3DBaseTexture8** ppTexture);
            STDMETHOD(SetTexture)(DWORD Stage,IDirect3DBaseTexture8* pTexture);
            STDMETHOD(GetTextureStageState)(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue);
            STDMETHOD(SetTextureStageState)(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
            STDMETHOD(ValidateDevice)(DWORD* pNumPasses);
            /* No D3D9 equivalent. */
            STDMETHOD(GetInfo)(DWORD DevInfoID,void* pDevInfoStruct,DWORD DevInfoStructSize);
            STDMETHOD(SetPaletteEntries)(UINT PaletteNumber,CONST PALETTEENTRY* pEntries);
            STDMETHOD(GetPaletteEntries)(UINT PaletteNumber,PALETTEENTRY* pEntries);
            STDMETHOD(SetCurrentTexturePalette)(UINT PaletteNumber);
            STDMETHOD(GetCurrentTexturePalette)(UINT *PaletteNumber);
            STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
            // ! @todo Check the value of the start index in the D3D9 call.
            STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount);
            STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
            STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
            STDMETHOD(ProcessVertices)(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer8* pDestBuffer,DWORD Flags);
            STDMETHOD(CreateVertexShader)(CONST DWORD* pDeclaration,CONST DWORD* pFunction,DWORD* pHandle,DWORD Usage);
            STDMETHOD(SetVertexShader)(DWORD Handle);
            STDMETHOD(GetVertexShader)(DWORD* pHandle);
            /* D3D9 has no equivalent function. */
            STDMETHOD(DeleteVertexShader)(DWORD Handle);
            STDMETHOD(SetVertexShaderConstant)(DWORD Register,CONST void* pConstantData,DWORD ConstantCount);
            STDMETHOD(GetVertexShaderConstant)(DWORD Register,void* pConstantData,DWORD ConstantCount);
            STDMETHOD(GetVertexShaderDeclaration)(DWORD Handle,void* pData,DWORD* pSizeOfData);
            STDMETHOD(GetVertexShaderFunction)(DWORD Handle,void* pData,DWORD* pSizeOfData);
            STDMETHOD(SetStreamSource)(UINT StreamNumber,IDirect3DVertexBuffer8* pStreamData,UINT Stride);
            STDMETHOD(GetStreamSource)(UINT StreamNumber,IDirect3DVertexBuffer8** ppStreamData,UINT* pStride);
            STDMETHOD(SetIndices)(IDirect3DIndexBuffer8* pIndexData,UINT BaseVertexIndex);
            STDMETHOD(GetIndices)(IDirect3DIndexBuffer8** ppIndexData,UINT* pBaseVertexIndex);
            STDMETHOD(CreatePixelShader)(CONST DWORD* pFunction,DWORD* pHandle);
            STDMETHOD(SetPixelShader)(DWORD Handle);
            STDMETHOD(GetPixelShader)(DWORD* pHandle);
            STDMETHOD(DeletePixelShader)(DWORD Handle);
            STDMETHOD(SetPixelShaderConstant)(DWORD Register,CONST void* pConstantData,DWORD ConstantCount);
            STDMETHOD(GetPixelShaderConstant)(DWORD Register,void* pConstantData,DWORD ConstantCount);
            STDMETHOD(GetPixelShaderFunction)(DWORD Handle,void* pData,DWORD* pSizeOfData);
            STDMETHOD(DrawRectPatch)(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
            STDMETHOD(DrawTriPatch)(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
            STDMETHOD(DeletePatch)(UINT Handle);

        private:
            /**
             * A pointer to the true underlying IDirect3DDevice9 object wrapped by this
             * CVoodoo3D8 interface.
             */
            IDirect3DDevice9 * m_RealDevice;

            IDirect3D9 * m_RealObject;

            std::set<DWORD> m_VertexShaders;
            std::set<DWORD> m_PixelShaders;

            DWORD m_CurrentVertexShader;
            DWORD m_CurrentPixelShader;

            UINT m_LastBaseIndex;
        };
    }
}
