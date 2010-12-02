
#include "../Framework/VoodooFramework.hpp"
#pragma comment(lib, "../Debug/VoodooFramework.lib")

#include "../Voodoo_DX9/DX9_Module.hpp"
#pragma comment(lib, "../Debug/Voodoo_DX9.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "D3D9.lib")
#pragma comment(lib, "D3dx9.lib")

static const char define[] = "#define API_DX";

typedef IDirect3DVertexBuffer9 IDirect3DVertexBuffer;
typedef IDirect3DIndexBuffer9 IDirect3DIndexBuffer;
typedef IDirect3DSurface9 IDirect3DSurface;
typedef IDirect3DTexture9 IDirect3DTexture;
typedef IDirect3D9 IDirect3D;
typedef IDirect3DDevice9 IDirect3DDevice;
typedef D3DLIGHT9 D3DLIGHT8;
typedef D3DMATERIAL9 D3DMATERIAL8;
typedef D3DVIEWPORT9 D3DVIEWPORT8;
typedef D3DCLIPSTATUS9 D3DCLIPSTATUS8;

typedef struct RGBVECTOR
{
#ifdef __cplusplus
public:
    RGBVECTOR() {}
    RGBVECTOR( DWORD rgb );
    RGBVECTOR( CONST FLOAT * );
    RGBVECTOR( CONST D3DXFLOAT16 * );
    RGBVECTOR( CONST RGBVECTOR * );
    RGBVECTOR( CONST RGBVECTOR& );
    RGBVECTOR( CONST D3DCOLORVALUE& );
    RGBVECTOR( CONST D3DXCOLOR& );
    RGBVECTOR( FLOAT r, FLOAT g, FLOAT b );

    // casting
    operator DWORD () const;

    operator FLOAT* ();
    operator CONST FLOAT* () const;

    // assignment operators
    RGBVECTOR& operator += ( CONST RGBVECTOR& );
    RGBVECTOR& operator -= ( CONST RGBVECTOR& );
    RGBVECTOR& operator += ( CONST D3DXCOLOR& );
    RGBVECTOR& operator -= ( CONST D3DXCOLOR& );
    RGBVECTOR& operator *= ( FLOAT );
    RGBVECTOR& operator /= ( FLOAT );

    // unary operators
    RGBVECTOR operator + () const;
    RGBVECTOR operator - () const;

    // binary operators
    RGBVECTOR operator + ( CONST RGBVECTOR& ) const;
    RGBVECTOR operator - ( CONST RGBVECTOR& ) const;
    RGBVECTOR operator + ( CONST D3DXCOLOR& ) const;
    RGBVECTOR operator - ( CONST D3DXCOLOR& ) const;
    RGBVECTOR operator * ( FLOAT ) const;
    RGBVECTOR operator / ( FLOAT ) const;

    friend RGBVECTOR operator * ( FLOAT, CONST RGBVECTOR& );

    BOOL operator == ( CONST RGBVECTOR& ) const;
    BOOL operator != ( CONST RGBVECTOR& ) const;
    BOOL operator == ( CONST D3DXCOLOR& ) const;
    BOOL operator != ( CONST D3DXCOLOR& ) const;

#endif //__cplusplus
    FLOAT r, g, b;
} RGBVECTOR, *LPRGBVECTOR;

#ifndef _D3D8HEADER_INL_
#define _D3D8HEADER_INL_

#ifdef __cplusplus

//--------------------------
// Color
//--------------------------

D3DXINLINE
RGBVECTOR::RGBVECTOR( DWORD dw )
{
    CONST FLOAT f = 1.0f / 255.0f;
    r = f * (FLOAT) (unsigned char) (dw >> 16);
    g = f * (FLOAT) (unsigned char) (dw >>  8);
    b = f * (FLOAT) (unsigned char) (dw >>  0);
}

D3DXINLINE
RGBVECTOR::RGBVECTOR( CONST FLOAT* pf )
{
#ifdef D3DX_DEBUG
    if(!pf)
        return;
#endif

    r = pf[0];
    g = pf[1];
    b = pf[2];
}

D3DXINLINE
RGBVECTOR::RGBVECTOR( CONST D3DXFLOAT16* pf )
{
#ifdef D3DX_DEBUG
    if(!pf)
        return;
#endif

    D3DXFloat16To32Array(&r, pf, 3);
}

D3DXINLINE
RGBVECTOR::RGBVECTOR( CONST RGBVECTOR* c )
{
    r = c->r;
    g = c->g;
    b = c->b;
}

D3DXINLINE
RGBVECTOR::RGBVECTOR( CONST RGBVECTOR& c )
{
    r = c.r;
    g = c.g;
    b = c.b;
}

D3DXINLINE
RGBVECTOR::RGBVECTOR( CONST D3DCOLORVALUE& c )
{
    r = c.r;
    g = c.g;
    b = c.b;
}

D3DXINLINE
RGBVECTOR::RGBVECTOR( CONST D3DXCOLOR& c )
{
    r = c.r;
    g = c.g;
    b = c.b;
}

D3DXINLINE
RGBVECTOR::RGBVECTOR( FLOAT fr, FLOAT fg, FLOAT fb )
{
    r = fr;
    g = fg;
    b = fb;
}


// casting
D3DXINLINE
RGBVECTOR::operator DWORD () const
{
    DWORD dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (DWORD) (r * 255.0f + 0.5f);
    DWORD dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (DWORD) (g * 255.0f + 0.5f);
    DWORD dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (DWORD) (b * 255.0f + 0.5f);

    return 0xff000000 | (dwR << 16) | (dwG << 8) | dwB;
}


D3DXINLINE
RGBVECTOR::operator FLOAT * ()
{
    return (FLOAT *) &r;
}

D3DXINLINE
RGBVECTOR::operator CONST FLOAT * () const
{
    return (CONST FLOAT *) &r;
}


// assignment operators
D3DXINLINE RGBVECTOR&
RGBVECTOR::operator += ( CONST RGBVECTOR& c )
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

D3DXINLINE RGBVECTOR&
RGBVECTOR::operator -= ( CONST RGBVECTOR& c )
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

D3DXINLINE RGBVECTOR&
RGBVECTOR::operator += ( CONST D3DXCOLOR& c )
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

D3DXINLINE RGBVECTOR&
RGBVECTOR::operator -= ( CONST D3DXCOLOR& c )
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

D3DXINLINE RGBVECTOR&
RGBVECTOR::operator *= ( FLOAT f )
{
    r *= f;
    g *= f;
    b *= f;
    return *this;
}

D3DXINLINE RGBVECTOR&
RGBVECTOR::operator /= ( FLOAT f )
{
    FLOAT fInv = 1.0f / f;
    r *= fInv;
    g *= fInv;
    b *= fInv;
    return *this;
}


// unary operators
D3DXINLINE RGBVECTOR
RGBVECTOR::operator + () const
{
    return *this;
}

D3DXINLINE RGBVECTOR
RGBVECTOR::operator - () const
{
    return RGBVECTOR(-r, -g, -b);
}


// binary operators
D3DXINLINE RGBVECTOR
RGBVECTOR::operator + ( CONST RGBVECTOR& c ) const
{
    return RGBVECTOR(r + c.r, g + c.g, b + c.b);
}

D3DXINLINE RGBVECTOR
RGBVECTOR::operator - ( CONST RGBVECTOR& c ) const
{
    return RGBVECTOR(r - c.r, g - c.g, b - c.b);
}

D3DXINLINE RGBVECTOR
RGBVECTOR::operator + ( CONST D3DXCOLOR& c ) const
{
    return RGBVECTOR(r + c.r, g + c.g, b + c.b);
}

D3DXINLINE RGBVECTOR
RGBVECTOR::operator - ( CONST D3DXCOLOR& c ) const
{
    return RGBVECTOR(r - c.r, g - c.g, b - c.b);
}

D3DXINLINE RGBVECTOR
RGBVECTOR::operator * ( FLOAT f ) const
{
    return RGBVECTOR(r * f, g * f, b * f);
}

D3DXINLINE RGBVECTOR
RGBVECTOR::operator / ( FLOAT f ) const
{
    FLOAT fInv = 1.0f / f;
    return RGBVECTOR(r * fInv, g * fInv, b * fInv);
}


D3DXINLINE RGBVECTOR
operator * (FLOAT f, CONST RGBVECTOR& c )
{
    return RGBVECTOR(f * c.r, f * c.g, f * c.b);
}


D3DXINLINE BOOL
RGBVECTOR::operator == ( CONST RGBVECTOR& c ) const
{
    return r == c.r && g == c.g && b == c.b;
}

D3DXINLINE BOOL
RGBVECTOR::operator != ( CONST RGBVECTOR& c ) const
{
    return r != c.r || g != c.g || b != c.b;
}

D3DXINLINE BOOL
RGBVECTOR::operator == ( CONST D3DXCOLOR& c ) const
{
    return r == c.r && g == c.g && b == c.b && c.a == 1;
}

D3DXINLINE BOOL
RGBVECTOR::operator != ( CONST D3DXCOLOR& c ) const
{
    return r != c.r || g != c.g || b != c.b || c.a != 1;
}


#endif /* __cplusplus */

#endif /* _D3D8HEADER_INL_ */

typedef struct _D3DCAPS8 {
    /* Device Info */
    D3DDEVTYPE  DeviceType;
    UINT    AdapterOrdinal;

    /* Caps from DX7 Draw */
    DWORD   Caps;
    DWORD   Caps2;
    DWORD   Caps3;
    DWORD   PresentationIntervals;

    /* Cursor Caps */
    DWORD   CursorCaps;

    /* 3D Device Caps */
    DWORD   DevCaps;

    DWORD   PrimitiveMiscCaps;
    DWORD   RasterCaps;
    DWORD   ZCmpCaps;
    DWORD   SrcBlendCaps;
    DWORD   DestBlendCaps;
    DWORD   AlphaCmpCaps;
    DWORD   ShadeCaps;
    DWORD   TextureCaps;
    DWORD   TextureFilterCaps;          // D3DPTFILTERCAPS for IDirect3DTexture8's
    DWORD   CubeTextureFilterCaps;      // D3DPTFILTERCAPS for IDirect3DCubeTexture8's
    DWORD   VolumeTextureFilterCaps;    // D3DPTFILTERCAPS for IDirect3DVolumeTexture8's
    DWORD   TextureAddressCaps;         // D3DPTADDRESSCAPS for IDirect3DTexture8's
    DWORD   VolumeTextureAddressCaps;   // D3DPTADDRESSCAPS for IDirect3DVolumeTexture8's

    DWORD   LineCaps;                   // D3DLINECAPS

    DWORD   MaxTextureWidth, MaxTextureHeight;
    DWORD   MaxVolumeExtent;

    DWORD   MaxTextureRepeat;
    DWORD   MaxTextureAspectRatio;
    DWORD   MaxAnisotropy;
    float   MaxVertexW;

    float   GuardBandLeft;
    float   GuardBandTop;
    float   GuardBandRight;
    float   GuardBandBottom;

    float   ExtentsAdjust;
    DWORD   StencilCaps;

    DWORD   FVFCaps;
    DWORD   TextureOpCaps;
    DWORD   MaxTextureBlendStages;
    DWORD   MaxSimultaneousTextures;

    DWORD   VertexProcessingCaps;
    DWORD   MaxActiveLights;
    DWORD   MaxUserClipPlanes;
    DWORD   MaxVertexBlendMatrices;
    DWORD   MaxVertexBlendMatrixIndex;

    float   MaxPointSize;

    DWORD   MaxPrimitiveCount;          // max number of primitives per DrawPrimitive call
    DWORD   MaxVertexIndex;
    DWORD   MaxStreams;
    DWORD   MaxStreamStride;            // max stride for SetStreamSource

    DWORD   VertexShaderVersion;
    DWORD   MaxVertexShaderConst;       // number of vertex shader constant registers

    DWORD   PixelShaderVersion;
    float   MaxPixelShaderValue;        // max value of pixel shader arithmetic component

} D3DCAPS8;

//Some defines to get everything to compile nicely
#define IDirect3DIndexBuffer8 void
#define IDirect3DVertexBuffer8 void
#define IDirect3DSwapChain8 void
#define IDirect3DBaseTexture8 IDirect3DTexture8
#define IDirect3DVolumeTexture8 void
#define IDirect3DCubeTexture8 void

#define D3DSURFACE_DESC9 D3DSURFACE_DESC
#define D3DPRESENT_PARAMETERS9 D3DPRESENT_PARAMETERS

interface IDirect3D8;
interface IDirect3DDevice8;
interface IDirect3DTexture8;
interface IDirect3DSurface8;

//DX8 enums
#define D3DTSS_ADDRESSU       13
#define D3DTSS_ADDRESSV       14
#define D3DTSS_BORDERCOLOR    15
#define D3DTSS_MAGFILTER      16
#define D3DTSS_MINFILTER      17
#define D3DTSS_MIPFILTER      18
#define D3DTSS_MIPMAPLODBIAS  19
#define D3DTSS_MAXMIPLEVEL    20
#define D3DTSS_MAXANISOTROPY  21
#define D3DTSS_ADDRESSW       25

//Object crearion functions
typedef IDirect3D8* (*D3DProc8)(UINT);
typedef IDirect3D9* (*D3DProc9)(UINT);

//DX8 structs
typedef struct _D3DADAPTER_IDENTIFIER8
{
    char            Driver[MAX_DEVICE_IDENTIFIER_STRING];
    char            Description[MAX_DEVICE_IDENTIFIER_STRING];

    LARGE_INTEGER   DriverVersion;

    DWORD           VendorId;
    DWORD           DeviceId;
    DWORD           SubSysId;
    DWORD           Revision;

    GUID            DeviceIdentifier;

    DWORD           WHQLLevel;

} D3DADAPTER_IDENTIFIER8;
typedef struct _D3DPRESENT_PARAMETERS8_
{
    UINT                BackBufferWidth;
    UINT                BackBufferHeight;
    D3DFORMAT           BackBufferFormat;
    UINT                BackBufferCount;

    D3DMULTISAMPLE_TYPE MultiSampleType;

    D3DSWAPEFFECT       SwapEffect;
    HWND                hDeviceWindow;
    BOOL                Windowed;
    BOOL                EnableAutoDepthStencil;
    D3DFORMAT           AutoDepthStencilFormat;
    DWORD               Flags;

    /* Following elements must be zero for Windowed mode */
    UINT                FullScreen_RefreshRateInHz;
    UINT                FullScreen_PresentationInterval;

} D3DPRESENT_PARAMETERS8;
typedef struct _D3DSURFACE_DESC8
{
    D3DFORMAT           Format;
    D3DRESOURCETYPE     Type;
    DWORD               Usage;
    D3DPOOL             Pool;
    UINT                Size;

    D3DMULTISAMPLE_TYPE MultiSampleType;
    UINT                Width;
    UINT                Height;
} D3DSURFACE_DESC8;

//DX8 interface declerations
DECLARE_INTERFACE_(IDirect3DSurface8, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)() PURE;
    STDMETHOD_(ULONG,Release)() PURE;

    /*** IDirect3DSurface8 methods ***/
    STDMETHOD(GetDevice)(IDirect3DDevice8** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(REFGUID refguid) PURE;
    STDMETHOD(GetContainer)(REFIID riid,void** ppContainer) PURE;
    STDMETHOD(GetDesc)(D3DSURFACE_DESC8 *pDesc) PURE;
    STDMETHOD(LockRect)(D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
    STDMETHOD(UnlockRect)() PURE;
};

DECLARE_INTERFACE_(IDirect3DTexture8, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)() PURE;
    STDMETHOD_(ULONG,Release)() PURE;

    /*** IDirect3DBaseTexture8 methods ***/
    STDMETHOD(GetDevice)(IDirect3DDevice8** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(DWORD PriorityNew) PURE;
    STDMETHOD_(DWORD, GetPriority)() PURE;
    STDMETHOD_(void, PreLoad)() PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)() PURE;
    STDMETHOD_(DWORD, SetLOD)(DWORD LODNew) PURE;
    STDMETHOD_(DWORD, GetLOD)() PURE;
    STDMETHOD_(DWORD, GetLevelCount)() PURE;
    STDMETHOD(GetLevelDesc)(UINT Level,D3DSURFACE_DESC8 *pDesc) PURE;
    STDMETHOD(GetSurfaceLevel)(UINT Level,IDirect3DSurface8** ppSurfaceLevel) PURE;
    STDMETHOD(LockRect)(UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(UINT Level) PURE;
    STDMETHOD(AddDirtyRect)(CONST RECT* pDirtyRect) PURE;
};

DECLARE_INTERFACE_(IDirect3DDevice8, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)() PURE;
    STDMETHOD_(ULONG,Release)() PURE;

    /*** IDirect3DDevice8 methods ***/
    STDMETHOD(TestCooperativeLevel)() PURE;
    STDMETHOD_(UINT, GetAvailableTextureMem)() PURE;
    STDMETHOD(ResourceManagerDiscardBytes)(DWORD Bytes) PURE;
    STDMETHOD(GetDirect3D)(IDirect3D8** ppD3D8) PURE;
    STDMETHOD(GetDeviceCaps)(D3DCAPS8* pCaps) PURE;
    STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS *pParameters) PURE;
    STDMETHOD(SetCursorProperties)(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface8* pCursorBitmap) PURE;
    STDMETHOD_(void, SetCursorPosition)(int X,int Y,DWORD Flags) PURE;
    STDMETHOD_(BOOL, ShowCursor)(BOOL bShow) PURE;
    STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS8* pPresentationParameters,IDirect3DSwapChain8** pSwapChain) PURE;
    STDMETHOD(Reset)(D3DPRESENT_PARAMETERS8* pPresentationParameters) PURE;
    STDMETHOD(Present)(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) PURE;
    STDMETHOD(GetBackBuffer)(UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer) PURE;
    STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS* pRasterStatus) PURE;
    STDMETHOD_(void, SetGammaRamp)(DWORD Flags,CONST D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD_(void, GetGammaRamp)(D3DGAMMARAMP* pRamp) PURE;
    STDMETHOD(CreateTexture)(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8** ppTexture) PURE;
    STDMETHOD(CreateVolumeTexture)(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture8** ppVolumeTexture) PURE;
    STDMETHOD(CreateCubeTexture)(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture8** ppCubeTexture) PURE;
    STDMETHOD(CreateVertexBuffer)(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer8** ppVertexBuffer) PURE;
    STDMETHOD(CreateIndexBuffer)(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer8** ppIndexBuffer) PURE;
    STDMETHOD(CreateRenderTarget)(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,BOOL Lockable,IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CreateDepthStencilSurface)(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CreateImageSurface)(UINT Width,UINT Height,D3DFORMAT Format,IDirect3DSurface8** ppSurface) PURE;
    STDMETHOD(CopyRects)(IDirect3DSurface8* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,IDirect3DSurface8* pDestinationSurface,CONST POINT* pDestPointsArray) PURE;
    STDMETHOD(UpdateTexture)(IDirect3DBaseTexture8* pSourceTexture,IDirect3DBaseTexture8* pDestinationTexture) PURE;
    STDMETHOD(GetFrontBuffer)(IDirect3DSurface8* pDestSurface) PURE;
    STDMETHOD(SetRenderTarget)(IDirect3DSurface8* pRenderTarget,IDirect3DSurface8* pNewZStencil) PURE;
    STDMETHOD(GetRenderTarget)(IDirect3DSurface8** ppRenderTarget) PURE;
    STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface8** ppZStencilSurface) PURE;
    STDMETHOD(BeginScene)() PURE;
    STDMETHOD(EndScene)() PURE;
    STDMETHOD(Clear)(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) PURE;
    STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix) PURE;
    STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix) PURE;
    STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE,CONST D3DMATRIX*) PURE;
    STDMETHOD(SetViewport)(CONST D3DVIEWPORT9* pViewport) PURE;
    STDMETHOD(GetViewport)(D3DVIEWPORT9* pViewport) PURE;
    STDMETHOD(SetMaterial)(CONST D3DMATERIAL9* pMaterial) PURE;
    STDMETHOD(GetMaterial)(D3DMATERIAL9* pMaterial) PURE;
    STDMETHOD(SetLight)(DWORD Index,CONST D3DLIGHT9*) PURE;
    STDMETHOD(GetLight)(DWORD Index,D3DLIGHT9*) PURE;
    STDMETHOD(LightEnable)(DWORD Index,BOOL Enable) PURE;
    STDMETHOD(GetLightEnable)(DWORD Index,BOOL* pEnable) PURE;
    STDMETHOD(SetClipPlane)(DWORD Index,CONST float* pPlane) PURE;
    STDMETHOD(GetClipPlane)(DWORD Index,float* pPlane) PURE;
    STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State,DWORD Value) PURE;
    STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State,DWORD* pValue) PURE;
    STDMETHOD(BeginStateBlock)() PURE;
    STDMETHOD(EndStateBlock)(DWORD* pToken) PURE;
    STDMETHOD(ApplyStateBlock)(DWORD Token) PURE;
    STDMETHOD(CaptureStateBlock)(DWORD Token) PURE;
    STDMETHOD(DeleteStateBlock)(DWORD Token) PURE;
    STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type,DWORD* pToken) PURE;
    STDMETHOD(SetClipStatus)(CONST D3DCLIPSTATUS9* pClipStatus) PURE;
    STDMETHOD(GetClipStatus)(D3DCLIPSTATUS9* pClipStatus) PURE;
    STDMETHOD(GetTexture)(DWORD Stage,IDirect3DBaseTexture8** ppTexture) PURE;
    STDMETHOD(SetTexture)(DWORD Stage,IDirect3DBaseTexture8* pTexture) PURE;
    STDMETHOD(GetTextureStageState)(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue) PURE;
    STDMETHOD(SetTextureStageState)(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) PURE;
    STDMETHOD(ValidateDevice)(DWORD* pNumPasses) PURE;
    STDMETHOD(GetInfo)(DWORD DevInfoID,void* pDevInfoStruct,DWORD DevInfoStructSize) PURE;
    STDMETHOD(SetPaletteEntries)(UINT PaletteNumber,CONST PALETTEENTRY* pEntries) PURE;
    STDMETHOD(GetPaletteEntries)(UINT PaletteNumber,PALETTEENTRY* pEntries) PURE;
    STDMETHOD(SetCurrentTexturePalette)(UINT PaletteNumber) PURE;
    STDMETHOD(GetCurrentTexturePalette)(UINT *PaletteNumber) PURE;
    STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) PURE;
    STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount) PURE;
    STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) PURE;
    STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) PURE;
    STDMETHOD(ProcessVertices)(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer8* pDestBuffer,DWORD Flags) PURE;
    STDMETHOD(CreateVertexShader)(CONST DWORD* pDeclaration,CONST DWORD* pFunction,DWORD* pHandle,DWORD Usage) PURE;
    STDMETHOD(SetVertexShader)(DWORD Handle) PURE;
    STDMETHOD(GetVertexShader)(DWORD* pHandle) PURE;
    STDMETHOD(DeleteVertexShader)(DWORD Handle) PURE;
    STDMETHOD(SetVertexShaderConstant)(DWORD Register,CONST void* pConstantData,DWORD ConstantCount) PURE;
    STDMETHOD(GetVertexShaderConstant)(DWORD Register,void* pConstantData,DWORD ConstantCount) PURE;
    STDMETHOD(GetVertexShaderDeclaration)(DWORD Handle,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(GetVertexShaderFunction)(DWORD Handle,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(SetStreamSource)(UINT StreamNumber,IDirect3DVertexBuffer8* pStreamData,UINT Stride) PURE;
    STDMETHOD(GetStreamSource)(UINT StreamNumber,IDirect3DVertexBuffer8** ppStreamData,UINT* pStride) PURE;
    STDMETHOD(SetIndices)(IDirect3DIndexBuffer8* pIndexData,UINT BaseVertexIndex) PURE;
    STDMETHOD(GetIndices)(IDirect3DIndexBuffer8** ppIndexData,UINT* pBaseVertexIndex) PURE;
    STDMETHOD(CreatePixelShader)(CONST DWORD* pFunction,DWORD* pHandle) PURE;
    STDMETHOD(SetPixelShader)(DWORD Handle) PURE;
    STDMETHOD(GetPixelShader)(DWORD* pHandle) PURE;
    STDMETHOD(DeletePixelShader)(DWORD Handle) PURE;
    STDMETHOD(SetPixelShaderConstant)(DWORD Register,CONST void* pConstantData,DWORD ConstantCount) PURE;
    STDMETHOD(GetPixelShaderConstant)(DWORD Register,void* pConstantData,DWORD ConstantCount) PURE;
    STDMETHOD(GetPixelShaderFunction)(DWORD Handle,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(DrawRectPatch)(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) PURE;
    STDMETHOD(DrawTriPatch)(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) PURE;
    STDMETHOD(DeletePatch)(UINT Handle) PURE;
};

DECLARE_INTERFACE_(IDirect3D8, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)() PURE;
    STDMETHOD_(ULONG,Release)() PURE;

    /*** IDirect3D8 methods ***/
    STDMETHOD(RegisterSoftwareDevice)(void* pInitializeFunction) PURE;
    STDMETHOD_(UINT, GetAdapterCount)() PURE;
    STDMETHOD(GetAdapterIdentifier)(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER8* pIdentifier) PURE;
    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter) PURE;
    STDMETHOD(EnumAdapterModes)(UINT Adapter,UINT Mode,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(GetAdapterDisplayMode)(UINT Adapter,D3DDISPLAYMODE* pMode) PURE;
    STDMETHOD(CheckDeviceType)(UINT Adapter,D3DDEVTYPE CheckType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL Windowed) PURE;
    STDMETHOD(CheckDeviceFormat)(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) PURE;
    STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType) PURE;
    STDMETHOD(CheckDepthStencilMatch)(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) PURE;
    STDMETHOD(GetDeviceCaps)(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS8* pCaps) PURE;
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter) PURE;
    STDMETHOD(CreateDevice)(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS8* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface) PURE;
};

static const GUID guid = 
{ 0x7c204fb1, 0x9cac, 0x4b8d, { 0xba, 0xed, 0x7b, 0xf4, 0x8b, 0xf6, 0x3b, 0xb2 } }; // <- Private data storage GUID,
// may be replaced with a new one later
// {8C51D624-D739-467f-918D-D912EA27C693}
//{ 0x8c51d624, 0xd739, 0x467f, { 0x91, 0x8d, 0xd9, 0x12, 0xea, 0x27, 0xc6, 0x93 } }; // <- Voodoo_DX8's GUID


class FakeSurface;
class FakeTexture;
class FakeDevice;
class FakeObject;

#define UnusedFunction() D3DERR_INVALIDCALL
