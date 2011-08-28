#pragma once

#include "DX9_Module.hpp"

class IVoodoo3DDevice9
            : public IDirect3DDevice9
{
private:
    IDirect3DDevice9* mRealDevice;
    IDirect3D9* m_d3d;

public:
    // We need d3d so that we'd use a pointer to MyDirect3D9 instead of the original IDirect3D9 implementor
    // in functions like GetDirect3D9
    IVoodoo3DDevice9(IDirect3D9* d3d, IDirect3DDevice9* device) : m_d3d(d3d), mRealDevice(device)
    {
    }

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
    {
        return mRealDevice->QueryInterface(riid, ppvObj);
    }

    STDMETHOD_(ULONG,AddRef)(THIS)
    {
        return mRealDevice->AddRef();
    }

    STDMETHOD_(ULONG,Release)(THIS)
    {
        ULONG count = mRealDevice->Release();
        if (0 == count)
            delete this;

        return count;
    }

    /*** IDirect3DDevice9 methods ***/
    STDMETHOD(TestCooperativeLevel)(THIS)
    {
        return mRealDevice->TestCooperativeLevel();
    }

    STDMETHOD_(UINT, GetAvailableTextureMem)(THIS)
    {
        return mRealDevice->GetAvailableTextureMem();
    }

    STDMETHOD(EvictManagedResources)(THIS)
    {
        return mRealDevice->EvictManagedResources();
    }

    STDMETHOD(GetDirect3D)(THIS_ IDirect3D9** ppD3D9)
    {
        // Let the device validate the incoming pointer for us
        HRESULT hr = mRealDevice->GetDirect3D(ppD3D9);
        if (SUCCEEDED(hr))
            *ppD3D9 = m_d3d;

        return hr;
    }

    STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps)
    {
        return mRealDevice->GetDeviceCaps(pCaps);
    }

    STDMETHOD(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode)
    {
        return mRealDevice->GetDisplayMode(iSwapChain, pMode);

    }

    STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters)
    {
        return mRealDevice->GetCreationParameters(pParameters);
    }

    STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
    {
        return mRealDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
    }

    STDMETHOD_(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags)
    {
        mRealDevice->SetCursorPosition(X, Y, Flags);
    }

    STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow)
    {
        return mRealDevice->ShowCursor(bShow);
    }

    STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
    {
        return mRealDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
    }

    STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
    {
        return mRealDevice->GetSwapChain(iSwapChain, pSwapChain);
    }

    STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS)
    {
        return mRealDevice->GetNumberOfSwapChains();
    }

    STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters)
    {
        return mRealDevice->Reset(pPresentationParameters);
    }

    STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
    {
        //IDirect3DBaseTexture9 * tex;
        //IDirect3DSurface9 * rts;

        //mRealDevice->GetTexture(0, &tex);
        //mRealDevice->GetRenderTarget(0, &rts);

        HRESULT hr = mRealDevice->StretchRect(backbufferSurf, NULL, surface_ThisFrame, NULL, D3DTEXF_NONE);
        if (FAILED(hr))
        {
            VoodooLogger->Log("Voodoo DX9: Failed to stretch backbuffer to scratch texture.\n");
        }

        hr = mRealDevice->SetRenderTarget(0, backbufferSurf);
        if (FAILED(hr))
        {
            VoodooLogger->Log("Voodoo DX9: Failed to set render target.\n");
        }

        VoodooShader::TechniqueRef tech = testShader->GetDefaultTechnique();
        VoodooShader::PassRef pass = tech->GetPass(0);

        VoodooDX9->BindPass(pass);

        VoodooDX9->DrawQuad(NULL);

        VoodooDX9->UnbindPass();

        //mRealDevice->SetRenderTarget(0, rts);
        //mRealDevice->SetTexture(0, tex);

        return mRealDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }

    STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
    {
        return mRealDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
    }

    STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
    {
        return mRealDevice->GetRasterStatus(iSwapChain, pRasterStatus);
    }

    STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs)
    {
        return mRealDevice->SetDialogBoxMode(bEnableDialogs);
    }

    STDMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
    {
        return mRealDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
    }

    STDMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp)
    {
        return mRealDevice->GetGammaRamp(iSwapChain, pRamp);
    }

    STDMETHOD(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    }

    STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
    }

    STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    }

    STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    }

    STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    }

    STDMETHOD(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
    }

    STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
    }

    STDMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
    {
        return mRealDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
    }

    STDMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
    {
        return mRealDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
    }

    STDMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
    {
        return mRealDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
    }

    STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface)
    {
        return mRealDevice->GetFrontBufferData(iSwapChain, pDestSurface);
    }

    STDMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
    {
        return mRealDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
    }

    STDMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
    {
        return mRealDevice->ColorFill(pSurface, pRect, color);
    }

    STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
    {
        return mRealDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
    }

    STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
    {
        return mRealDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
    }

    STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
    {
        return mRealDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
    }

    STDMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil)
    {
        return mRealDevice->SetDepthStencilSurface(pNewZStencil);
    }

    STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface)
    {
        return mRealDevice->GetDepthStencilSurface(ppZStencilSurface);
    }

    STDMETHOD(BeginScene)(THIS)
    {
        return mRealDevice->BeginScene();
    }

    STDMETHOD(EndScene)(THIS)
    {
        return mRealDevice->EndScene();
    }

    STDMETHOD(Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
    {
        return mRealDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
    }

    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
    {
        return mRealDevice->SetTransform(State, pMatrix);
    }

    STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
    {
        return mRealDevice->GetTransform(State, pMatrix);
    }

    STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
    {
        return mRealDevice->MultiplyTransform(State, pMatrix);
    }

    STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport)
    {
        return mRealDevice->SetViewport(pViewport);
    }

    STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport)
    {
        return mRealDevice->GetViewport(pViewport);
    }

    STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial)
    {
        return mRealDevice->SetMaterial(pMaterial);
    }

    STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial)
    {
        return mRealDevice->GetMaterial(pMaterial);
    }

    STDMETHOD(SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9* pLight)
    {
        return mRealDevice->SetLight(Index, pLight);
    }

    STDMETHOD(GetLight)(THIS_ DWORD Index,D3DLIGHT9* pLight)
    {
        return mRealDevice->GetLight(Index, pLight);
    }

    STDMETHOD(LightEnable)(THIS_ DWORD Index,BOOL Enable)
    {
        return mRealDevice->LightEnable(Index, Enable);
    }

    STDMETHOD(GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable)
    {
        return mRealDevice->GetLightEnable(Index, pEnable);
    }

    STDMETHOD(SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane)
    {
        return mRealDevice->SetClipPlane(Index, pPlane);
    }

    STDMETHOD(GetClipPlane)(THIS_ DWORD Index,float* pPlane)
    {
        return mRealDevice->GetClipPlane(Index, pPlane);
    }

    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value)
    {
        return mRealDevice->SetRenderState(State, Value);
    }

    STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue)
    {
        return mRealDevice->GetRenderState(State, pValue);
    }

    STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
    {
        return mRealDevice->CreateStateBlock(Type, ppSB);
    }

    STDMETHOD(BeginStateBlock)(THIS)
    {
        return mRealDevice->BeginStateBlock();
    }

    STDMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB)
    {
        return mRealDevice->EndStateBlock(ppSB);
    }

    STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus)
    {
        return mRealDevice->SetClipStatus(pClipStatus);
    }

    STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus)
    {
        return mRealDevice->GetClipStatus(pClipStatus);
    }

    STDMETHOD(GetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture)
    {
        return mRealDevice->GetTexture(Stage, ppTexture);
    }

    STDMETHOD(SetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture)
    {
        return mRealDevice->SetTexture(Stage, pTexture);
    }

    STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
    {
        return mRealDevice->GetTextureStageState(Stage, Type, pValue);
    }

    STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
    {
        return mRealDevice->SetTextureStageState(Stage, Type, Value);
    }

    STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
    {
        return mRealDevice->GetSamplerState(Sampler, Type, pValue);
    }

    STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
    {
        return mRealDevice->SetSamplerState(Sampler, Type, Value);
    }

    STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses)
    {
        return mRealDevice->ValidateDevice(pNumPasses);
    }

    STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
    {
        return mRealDevice->SetPaletteEntries(PaletteNumber, pEntries);
    }

    STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries)
    {
        return mRealDevice->GetPaletteEntries(PaletteNumber, pEntries);
    }

    STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber)
    {
        return mRealDevice->SetCurrentTexturePalette(PaletteNumber);
    }

    STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber)
    {
        return mRealDevice->GetCurrentTexturePalette(PaletteNumber);
    }

    STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect)
    {
        return mRealDevice->SetScissorRect(pRect);
    }

    STDMETHOD(GetScissorRect)(THIS_ RECT* pRect)
    {
        return mRealDevice->GetScissorRect(pRect);
    }

    STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware)
    {
        return mRealDevice->SetSoftwareVertexProcessing(bSoftware);
    }

    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS)
    {
        return mRealDevice->GetSoftwareVertexProcessing();
    }

    STDMETHOD(SetNPatchMode)(THIS_ float nSegments)
    {
        return mRealDevice->SetNPatchMode(nSegments);
    }

    STDMETHOD_(float, GetNPatchMode)(THIS)
    {
        return mRealDevice->GetNPatchMode();
    }

    STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
    {
        return mRealDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    }

    STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
    {
        return mRealDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
    }

    STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
    {
        return mRealDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
    {
        return mRealDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
    {
        return mRealDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
    }

    STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
    {
        return mRealDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
    }

    STDMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl)
    {
        return mRealDevice->SetVertexDeclaration(pDecl);
    }

    STDMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl)
    {
        return mRealDevice->GetVertexDeclaration(ppDecl);
    }

    STDMETHOD(SetFVF)(THIS_ DWORD FVF)
    {
        return mRealDevice->SetFVF(FVF);
    }

    STDMETHOD(GetFVF)(THIS_ DWORD* pFVF)
    {
        return mRealDevice->GetFVF(pFVF);
    }

    STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
    {
        return mRealDevice->CreateVertexShader(pFunction, ppShader);
    }

    STDMETHOD(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader)
    {
        return mRealDevice->SetVertexShader(pShader);
    }

    STDMETHOD(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader)
    {
        return mRealDevice->GetVertexShader(ppShader);
    }

    STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
    {
        return mRealDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount)
    {
        return mRealDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
    {
        return mRealDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
    {
        return mRealDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
    {
        return mRealDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
    {
        return mRealDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
    {
        return mRealDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
    }

    STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)
    {
        return mRealDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
    }

    STDMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Setting)
    {
        return mRealDevice->SetStreamSourceFreq(StreamNumber, Setting);
    }

    STDMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* pSetting)
    {
        return mRealDevice->GetStreamSourceFreq(StreamNumber, pSetting);
    }

    STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData)
    {
        return mRealDevice->SetIndices(pIndexData);
    }

    STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData)
    {
        return mRealDevice->GetIndices(ppIndexData);
    }

    STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
    {
        return mRealDevice->CreatePixelShader(pFunction, ppShader);
    }

    STDMETHOD(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader)
    {
        return mRealDevice->SetPixelShader(pShader);
    }

    STDMETHOD(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader)
    {
        return mRealDevice->GetPixelShader(ppShader);
    }

    STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
    {
        return mRealDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount)
    {
        return mRealDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }

    STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
    {
        return mRealDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
    {
        return mRealDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }

    STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
    {
        return mRealDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
    {
        return mRealDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }

    STDMETHOD(DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
    {
        return mRealDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
    }

    STDMETHOD(DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
    {
        return mRealDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
    }

    STDMETHOD(DeletePatch)(THIS_ UINT Handle)
    {
        return mRealDevice->DeletePatch(Handle);
    }

    STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
    {
        return mRealDevice->CreateQuery(Type, ppQuery);
    }
};
