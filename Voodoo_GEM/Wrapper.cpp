
#include "Wrapper.hpp" //test
#include "../Voodoo_DX9/DX9_Converter.hpp"

VoodooShader::Core * VoodooCore = NULL;
VoodooShader::DirectX9::Adapter * VoodooDX9 = NULL;

static int clientWND_width, clientWND_height;
static HWND client_hWND;
static bool WindowedMode;

//#define NV_PERF_HUD

static bool RenderingToTexture = false;
static bool RenderedLand = false;
static bool ShadowPlaneDrawn = false;
static bool RenderedGrass = false;
//True when the main menu or game loading screens are showing
static bool LoadingScreen = true;

static float MatrixHUDidentifier;
static bool HUDRenderingStarted = false;
static bool ShadersApplied = false;
static bool BackBufferWasCleared = false;
static bool zWriteEnabled = 0;
static DWORD CurrentSrcBlend = 0;
static DWORD CurrentDestBlend = 0;

static IDirect3D8 *fakeDirect3D;
static IDirect3DDevice8 *fakeDevice;
static D3DCAPS8 d3d8Caps;

IDirect3DTexture9 *FakeWaterTexture;

static IDirect3DSurface9 *OldDepthSurface;

static int tcount = 0;


struct RenderState {
	DWORD FVF;
	DWORD AlphaBlendEnable;
	DWORD AlphaTestEnable;
	DWORD SrcBlend;
	DWORD DestBlend;
	DWORD CullMode;
	DWORD VertexBlend;
	DWORD StencilEnable;
	DWORD ZEnable;
	DWORD ZWriteEnable;
	D3DMATRIX View;
	D3DMATRIX Projection;
	D3DMATRIX World;

	RenderState() {
		//Set default values
		FVF = 0;
		AlphaBlendEnable = 0;
		AlphaTestEnable = 0;
		SrcBlend = 0;
		DestBlend = 0;
		CullMode = D3DCULL_CCW;
		VertexBlend = 0;
		ZEnable = 1;
		ZWriteEnable = 1;

	}

	void SaveState(IDirect3DDevice9 * device)
	{
		device->GetFVF(&FVF);
		device->GetRenderState( D3DRS_ALPHABLENDENABLE, &AlphaBlendEnable );
		device->GetRenderState( D3DRS_ALPHATESTENABLE, &AlphaTestEnable );
		device->GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
		device->GetRenderState( D3DRS_DESTBLEND, &DestBlend );
		device->GetRenderState( D3DRS_CULLMODE, &CullMode );
		device->GetRenderState( D3DRS_VERTEXBLEND, &VertexBlend );
		device->GetRenderState( D3DRS_STENCILENABLE, &StencilEnable );
		device->GetRenderState( D3DRS_ZENABLE, &ZEnable );
		device->GetRenderState( D3DRS_ZWRITEENABLE, &ZWriteEnable );
		device->GetTransform( D3DTS_VIEW, &View );
		device->GetTransform( D3DTS_PROJECTION, &Projection );
		device->GetTransform( D3DTS_WORLD, &World );
	}

	void RestoreState(IDirect3DDevice9 * device) {
		device->SetFVF(FVF);
		device->SetRenderState( D3DRS_ALPHABLENDENABLE, AlphaBlendEnable );
		device->SetRenderState( D3DRS_ALPHATESTENABLE, AlphaTestEnable );
		device->SetRenderState( D3DRS_SRCBLEND, SrcBlend );
		device->SetRenderState( D3DRS_DESTBLEND, DestBlend );
		device->SetRenderState( D3DRS_CULLMODE, CullMode );
		device->SetRenderState( D3DRS_VERTEXBLEND, VertexBlend );
		device->SetRenderState( D3DRS_STENCILENABLE, StencilEnable );
		device->SetRenderState( D3DRS_ZENABLE, ZEnable );
		device->SetRenderState( D3DRS_ZWRITEENABLE, ZWriteEnable );
		device->SetTransform( D3DTS_VIEW, &View );
		device->SetTransform( D3DTS_PROJECTION, &Projection );
		device->SetTransform( D3DTS_WORLD, &World );
	}
};


//=============================================================================


class FakeSurface : public IDirect3DSurface8 
{

public:
	IDirect3DSurface9 *mRealObject;


	//-----------------------------------------------------------------------------
	/*** Constructor ***/
	//-----------------------------------------------------------------------------

	FakeSurface(IDirect3DSurface9 *Real)
	{
		mRealObject = Real;
		void *This = this;
		Real->SetPrivateData (guid, (void *)&This, 4, 0);
	}


	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObj)
	{
		return mRealObject->QueryInterface (riid, ppvObj);
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef()
	{
		return mRealObject->AddRef ();
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release()
	{
		ULONG Refs = mRealObject->Release ();
		if (!Refs) {
			delete this;
			return 0;
		}
		return Refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DSurface8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDevice(IDirect3DDevice8 **ppDevice)
	{
		*ppDevice = fakeDevice;
		fakeDevice->AddRef ();
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPrivateData(REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags)
	{
		return mRealObject->SetPrivateData (refguid, pData, SizeOfData, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPrivateData(REFGUID refguid, void *pData, DWORD *pSizeOfData)
	{
		return mRealObject->GetPrivateData (refguid, pData, pSizeOfData);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall FreePrivateData(REFGUID refguid)
	{
		return mRealObject->FreePrivateData (refguid);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetContainer(REFIID riid, void **ppContainer)
	{
		return mRealObject->GetContainer (riid, ppContainer);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDesc(D3DSURFACE_DESC8 *pDesc)
	{
		D3DSURFACE_DESC a2;
		HRESULT hr = mRealObject->GetDesc (&a2);
		if (hr == D3D_OK) {
			pDesc->Format = a2.Format;
			pDesc->Height = a2.Height;
			pDesc->MultiSampleType = a2.MultiSampleType;
			pDesc->Pool = a2.Pool;
			pDesc->Size = 0; //TODO: Fix;
			pDesc->Type = a2.Type;
			pDesc->Usage = a2.Usage;
			pDesc->Width = a2.Width;
		}
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall LockRect(D3DLOCKED_RECT *pLockedRect, CONST RECT *pRect, DWORD Flags)
	{
		return mRealObject->LockRect (pLockedRect, pRect, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall UnlockRect()
	{
		return mRealObject->UnlockRect ();
	}

	//-----------------------------------------------------------------------------
};


//=============================================================================
unsigned int index = 0;

class FakeTexture : public IDirect3DTexture8 {

private:
	int ltcount;


public:
	IDirect3DTexture9 *mRealObject;
	VoodooShader::Texture * mTexture;


	//-----------------------------------------------------------------------------
	/*** Constructor ***/
	//-----------------------------------------------------------------------------

	FakeTexture (IDirect3DTexture9 *Real) 
	{
		ltcount = tcount++;
		if (ltcount == 76) {
			int iii = 0;	// What is this for? /krzymark/
		}
		mRealObject = Real;
		void *This = this;
		Real->SetPrivateData (guid, (void *)&This, 4, 0);
		/*std::string textureName = "texture";
		char indexString[8];
		_itoa(index++, indexString, 10);
		textureName += indexString;		

		VoodooCore->CreateTexture(textureName, (void*)this);*/
	}

	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface (REFIID riid, void **ppvObj) {
		return mRealObject->QueryInterface (riid, ppvObj);
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef () {
		return mRealObject->AddRef ();
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release () {
		/*if (ltcount == 76) {
			int iii = 0;	// What is this for? /krzymark/
		}*/
		ULONG Refs = mRealObject->Release ();
		if (!Refs) {
			delete this;
			return 0;
		}
		return Refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DBaseTexture8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDevice (IDirect3DDevice8 **ppDevice) {
		*ppDevice = fakeDevice;
		fakeDevice->AddRef ();
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPrivateData (REFGUID refguid, CONST void *pData, DWORD SizeOfData, DWORD Flags) {
		return mRealObject->SetPrivateData (refguid, pData, SizeOfData, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPrivateData (REFGUID refguid, void *pData, DWORD *pSizeOfData) {
		return mRealObject->GetPrivateData (refguid, pData, pSizeOfData);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall FreePrivateData (REFGUID refguid) {
		return mRealObject->FreePrivateData (refguid);
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall SetPriority (DWORD PriorityNew) {
		return mRealObject->SetPriority (PriorityNew);
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetPriority () {
		return mRealObject->GetPriority ();
	}

	//-----------------------------------------------------------------------------

	void _stdcall PreLoad () {
		return mRealObject->PreLoad ();
	}

	//-----------------------------------------------------------------------------

	D3DRESOURCETYPE _stdcall GetType () {
		return mRealObject->GetType ();
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall SetLOD (DWORD LODNew) {
		return mRealObject->SetLOD (LODNew);
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetLOD () {
		return mRealObject->GetLOD ();
	}

	//-----------------------------------------------------------------------------

	DWORD _stdcall GetLevelCount () {
		return mRealObject->GetLevelCount ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetLevelDesc (UINT Level, D3DSURFACE_DESC8 *pDesc) {
		D3DSURFACE_DESC b2;
		HRESULT hr = mRealObject->GetLevelDesc (Level, &b2);
		if (hr == D3D_OK) {
			pDesc->Format = b2.Format;
			pDesc->Height = b2.Height;
			pDesc->MultiSampleType = b2.MultiSampleType;
			pDesc->Pool = b2.Pool;
			pDesc->Size = 0; //TODO: Fix;
			pDesc->Type = b2.Type;
			pDesc->Usage = b2.Usage;
			pDesc->Width = b2.Width;
		}
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetSurfaceLevel (UINT Level, IDirect3DSurface8 **ppSurfaceLevel) {
		IDirect3DSurface9 *b2;
		*ppSurfaceLevel = NULL;
		DWORD unused = 4;
		HRESULT hr = mRealObject->GetSurfaceLevel (Level, &b2);
		if (hr != D3D_OK || b2 == NULL) return hr;
		hr = b2->GetPrivateData (guid, (void *)ppSurfaceLevel, &unused);
		if (hr != D3D_OK) *ppSurfaceLevel = new FakeSurface (b2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall LockRect (UINT Level, D3DLOCKED_RECT *pLockedRect, CONST RECT *pRect, DWORD Flags) {
		return mRealObject->LockRect (Level, pLockedRect, pRect, Flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall UnlockRect (UINT Level) {
		return mRealObject->UnlockRect (Level);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall AddDirtyRect (CONST RECT *pDirtyRect) {
		return mRealObject->AddDirtyRect (pDirtyRect);
	}

	//-----------------------------------------------------------------------------
};


//=============================================================================


class FakeDevice : public IDirect3DDevice8 {

public:

	//-----------------------------------------------------------------------------
	/*** Fields ***/
	//-----------------------------------------------------------------------------

	IDirect3D8 * Object;
	IDirect3DDevice9 * RealDevice;
	DWORD Refs;
	UINT BaseVertexIndex;

	bool RecordRenders;
	D3DXMATRIX CurrentWorldTrans;
	const D3DMATERIAL8 *CurrentMaterial;
	IDirect3DTexture9 *CurrentTexture;
	IDirect3DVertexBuffer9 *CurrentVBuffer;
	IDirect3DIndexBuffer9 *CurrentIndices;
	DWORD CurrentVertexBlendState;
	D3DXMATRIX CurrentBlendTrans1;
	D3DXMATRIX CurrentBlendTrans2;
	D3DXMATRIX CurrentBlendTrans3;
	DWORD CurrentCullModeState;

	DWORD CurrentFVF;
	UINT CurrentStreamSpan;
	DWORD CurrentPShader;
	DWORD CurrentVShader;
	IDirect3DVertexShader9 *NewVShaders [4];
	IDirect3DPixelShader9 *NewPShaders [9];
	DWORD pShaderCount;
	DWORD vShaderCount;
	IDirect3DVertexDeclaration9 *decl [2];

	RenderState mRenderState;
	RenderState mRenderStateClean;

	std::map<DWORD, IDirect3DVertexShader9*> mVShaders;

	//-----------------------------------------------------------------------------
	/*** Misc methods ***/
	//-----------------------------------------------------------------------------

	FakeDevice (IDirect3D9 * object, IDirect3DDevice9 * device) 
	{
		VoodooCore->GetLog()->Format("Voodoo GEM: Creating FakeDevice from %d and %d.\n").With(object).With(device).Done();

		this->Object = reinterpret_cast<IDirect3D8*>(object);
		this->RealDevice = device;
		// Save the defaults
		this->mRenderStateClean.SaveState(device);
		Refs = 1;
		//TempBool = false;
		BaseVertexIndex = 0;
		CurrentPShader = 0;
		CurrentVShader = 0;
		pShaderCount = 0;
		vShaderCount = 1;
		RecordRenders = false;
		//ZeroStruct(NewVShaders);
		//ZeroStruct(NewPShaders);
		D3DVERTEXELEMENT9 elements [6] = {
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION},
			{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD},
			D3DDECL_END (),
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION},
			{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR},
			D3DDECL_END ()
		};
		RealDevice->CreateVertexDeclaration (elements, &decl [0]);
		RealDevice->CreateVertexDeclaration (&elements [3], &decl [1]);
	}


	//-----------------------------------------------------------------------------
	/*** IUnknown methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall QueryInterface (REFIID a, LPVOID *b) {
		return RealDevice->QueryInterface (a, b);
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall AddRef (void) {
		return ++Refs;
	}

	//-----------------------------------------------------------------------------

	ULONG _stdcall Release (void) {
		if (--Refs == 0) {
			// Shut down
			
			return 0;
		}
		return Refs;
	}


	//-----------------------------------------------------------------------------
	/*** IDirect3DDevice8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall TestCooperativeLevel(void)
	{
		return RealDevice->TestCooperativeLevel();
	}

	//-----------------------------------------------------------------------------

	/**
	 * Retrieves the amount of free texture memory, as reported by the video card drivers.
	 *
	 * @return The free texture memory, rounded to the nearest MB.
	 * @note This value is an estimate and should not be read as exact data. It is sufficient
	 *		for large-scale resource management only.
	 */
	UINT _stdcall GetAvailableTextureMem(void)
	{
		return RealDevice->GetAvailableTextureMem ();
	}

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall ResourceManagerDiscardBytes(DWORD a) 
	{
		return D3D_OK;
		//return RealDevice->EvictManagedResources();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDirect3D (IDirect3D8 **a) {
		*a = Object;
		Object->AddRef ();
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDeviceCaps (D3DCAPS8 *a) {
		*a = d3d8Caps;
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDisplayMode(D3DDISPLAYMODE * mode)
	{
		return RealDevice->GetDisplayMode(0, mode);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS * params)
	{
		return RealDevice->GetCreationParameters(params);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetCursorProperties(UINT a, UINT b, IDirect3DSurface8 *c)
	{
		//return RealDevice->SetCursorProperties (a, b, ((FakeSurface *)c)->mRealObject);
		return 1;
	}

	//-----------------------------------------------------------------------------

	void _stdcall SetCursorPosition(int X, int Y, DWORD c)
	{
		RealDevice->SetCursorPosition(X, Y, c);
	}

	//-----------------------------------------------------------------------------

	BOOL _stdcall ShowCursor (BOOL a)
	{
		return RealDevice->ShowCursor(a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS8 * a, IDirect3DSwapChain8 ** b)
	{
		return UnusedFunction();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall Reset(D3DPRESENT_PARAMETERS8 * presentparams)
	{
		D3DPRESENT_PARAMETERS9 pp;
		pp.Flags = presentparams->Flags;
		pp.Windowed = presentparams->Windowed;
		pp.BackBufferCount = presentparams->BackBufferCount;
		pp.BackBufferFormat = presentparams->BackBufferFormat;
		pp.BackBufferHeight = presentparams->BackBufferHeight;
		pp.BackBufferWidth = presentparams->BackBufferWidth;
		pp.hDeviceWindow = presentparams->hDeviceWindow;
		pp.EnableAutoDepthStencil = presentparams->EnableAutoDepthStencil;
		pp.SwapEffect = presentparams->SwapEffect;
		pp.MultiSampleQuality = 0;
		pp.MultiSampleType = presentparams->MultiSampleType;

		if (pp.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER) pp.Flags ^= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		pp.AutoDepthStencilFormat = D3DFMT_D24S8;
		pp.FullScreen_RefreshRateInHz = 0;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		return RealDevice->Reset(&pp);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall Present(const RECT * a, const RECT * b, HWND c, const RGNDATA * d)
	{
		
		// Shader draw and flip
		//VoodooDX9->DrawFSQuad();
		// Draw a custom quad to the screen
		struct TLVertex
		{
			float x, y, z, rhw;
			DWORD color;
		};

		TLVertex vertices[] =
		{
			{ -0.5f, -0.5f, 1.0f, 1.0f, 0xffff0000 },
			{ 99.5f, -0.5f, 1.0f, 1.0f, 0xff00ff00 },
			{ -0.5f, 99.5f, 1.0f, 1.0f, 0xff0000ff },
			{ 99.5f, 99.5f, 1.0f, 1.0f, 0xffff0000 }
		};

		this->mRenderState.SaveState(this->RealDevice);
		HRESULT overlay = RealDevice->BeginScene();
		if ( SUCCEEDED(overlay) )
		{
			RealDevice->SetVertexShader(0);
			RealDevice->SetPixelShader(0);
			RealDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
			RealDevice->SetRenderState(D3DRS_ZENABLE, false);
			RealDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//RealDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			//RealDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			//RealDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			//RealDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
			RealDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(TLVertex));
			RealDevice->SetRenderState(D3DRS_ZENABLE, true);
			RealDevice->EndScene();
		} else {
			VoodooCore->GetLog()->Format("Voodoo GEM: Error starting overlay scene: %d\n").With(overlay).Done();
		}

		this->mRenderState.RestoreState(this->RealDevice);

		//if (DepthTexture) RealDevice->SetDepthStencilSurface (DepthBuffer);
		HRESULT hr = this->RealDevice->Present(a, b, c, d);
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetBackBuffer(UINT a, D3DBACKBUFFER_TYPE b, IDirect3DSurface8 ** c) 
	{
		IDirect3DSurface9 * c2 = NULL;
		*c = NULL;
		DWORD unused = 4;

		HRESULT hr = RealDevice->GetBackBuffer(0, a, b, &c2);
		if (hr != D3D_OK || c2 == NULL) 
		{
			return hr;
		}

		hr = c2->GetPrivateData (guid, (void *)c, &unused);
		if (hr != D3D_OK) *c = new FakeSurface (c2);

		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetRasterStatus(D3DRASTER_STATUS * a)
	{
		return RealDevice->GetRasterStatus(0, a);
	}

	//-----------------------------------------------------------------------------

	void _stdcall SetGammaRamp(DWORD a, const D3DGAMMARAMP *b)
	{
		RealDevice->SetGammaRamp(0, a, b);
	}

	//-----------------------------------------------------------------------------

	void _stdcall GetGammaRamp(D3DGAMMARAMP *a)
	{
		RealDevice->GetGammaRamp(0, a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateTexture(UINT a, UINT b, UINT c, DWORD d, D3DFORMAT e, D3DPOOL f, IDirect3DTexture8 ** g)
	{
		// Need to prepare hooks for manually-read data

		IDirect3DTexture9 *g2 = NULL;
		HRESULT hr = RealDevice->CreateTexture (a, b, c, d, e, f, &g2, NULL);
		if (hr != D3D_OK || g2 == NULL) return hr;
		*g = new FakeTexture (g2);
		//*g = g2;
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	/**
	 * @todo Implement, required for some other games.
	 * @warning Not implemented.
	 */
	HRESULT _stdcall CreateVolumeTexture(UINT a, UINT b, UINT c, UINT d, DWORD e, D3DFORMAT f, D3DPOOL g, IDirect3DVolumeTexture8 ** h)
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @todo Implement, required for some other games.
	 * @warning Not implemented.
	 */
	HRESULT _stdcall CreateCubeTexture(UINT a, UINT b, DWORD c, D3DFORMAT d, D3DPOOL e, IDirect3DCubeTexture8 ** f)
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateVertexBuffer (UINT a, DWORD b, DWORD c, D3DPOOL d, IDirect3DVertexBuffer8 **e) 
	{
		return RealDevice->CreateVertexBuffer (a, b, c, d, (IDirect3DVertexBuffer9 **)e, NULL);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateIndexBuffer (UINT a, DWORD b, D3DFORMAT c, D3DPOOL d, IDirect3DIndexBuffer8 **e) 
	{
		return RealDevice->CreateIndexBuffer (a, b, c, d, (IDirect3DIndexBuffer9 **)e, NULL);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateRenderTarget (UINT a, UINT b, D3DFORMAT c, D3DMULTISAMPLE_TYPE d, BOOL e, IDirect3DSurface8 **f) 
	{
		IDirect3DSurface9 *f2 = NULL;
		HRESULT hr = RealDevice->CreateRenderTarget (a, b, c, d, 0, e, &f2, NULL);
		if (hr != D3D_OK || f2 == NULL) return hr;
		*f = new FakeSurface (f2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateDepthStencilSurface (UINT a, UINT b, D3DFORMAT c, D3DMULTISAMPLE_TYPE d, IDirect3DSurface8 **e) 
	{
		//Not sure if Discard should be true or false
		IDirect3DSurface9 *e2 = NULL;
		HRESULT hr = RealDevice->CreateDepthStencilSurface (a, b, c, d, 0, false, &e2, NULL);
		if (hr != D3D_OK) return hr;
		*e = new FakeSurface (e2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateImageSurface (UINT a, UINT b, D3DFORMAT c, IDirect3DSurface8 **d) 
	{
		IDirect3DSurface9 *d2 = NULL;
		//Use default to get stretchrect to work, scratch otherwise
		HRESULT hr = RealDevice->CreateOffscreenPlainSurface (a, b, c, D3DPOOL_SYSTEMMEM, &d2, NULL);
		//HRESULT hr = RealDevice->CreateOffscreenPlainSurface (a, b, c, D3DPOOL_DEFAULT, &d2, NULL);
		if (hr != D3D_OK || d2 == NULL) return hr;
		*d = new FakeSurface (d2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CopyRects (IDirect3DSurface8 *a, const RECT *b, UINT c, IDirect3DSurface8 *d, const POINT *e) 
	{
		IDirect3DSurface9 *a2 = ((FakeSurface *)a)->mRealObject;
		IDirect3DSurface9 *d2 = ((FakeSurface *)d)->mRealObject;
		if (b == NULL && e == NULL) {
			D3DSURFACE_DESC9 source;
			D3DSURFACE_DESC9 dest;
			if (a2->GetDesc (&source) != D3D_OK || d2->GetDesc (&dest) != D3D_OK) {
				return UnusedFunction ();
			} else if (source.Usage == 1 && dest.Usage == 0) {
				return RealDevice->GetRenderTargetData (a2, d2);
			} else if (source.Usage == 0 && dest.Usage == 1) {
				return RealDevice->UpdateSurface (a2, NULL, d2, NULL);
			} else {
				return RealDevice->StretchRect (a2, NULL, d2, NULL, D3DTEXF_NONE);
			}
		} else {
			//Really cant be bothered to get this working unless absolutely neccessery
			return UnusedFunction ();
			/* HRESULT hr;
			RECT r;
			r.top = e->y;
			r.left = e->x;
			for (UINT i = 0; i < c; ++i) {
			r.bottom = r.top + b [i].bottom - b [i].top;
			r.right = r.left + b [i].right - b [i].left;
			hr = RealDevice->StretchRect (a2, &b [i], d2, &r, D3DTEXF_LINEAR);
			}
			return hr; */
		}
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall UpdateTexture (IDirect3DBaseTexture8 *a, IDirect3DBaseTexture8 *b) 
	{
		IDirect3DTexture9 *a2 = ((FakeTexture *)a)->mRealObject;
		IDirect3DTexture9 *b2 = ((FakeTexture *)b)->mRealObject;
		return RealDevice->UpdateTexture (a2, b2);
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall GetFrontBuffer (IDirect3DSurface8 *a) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetRenderTarget (IDirect3DSurface8 *a, IDirect3DSurface8 *b) 
	{
		//wrapper_log_file << "Setting Render target to " << (unsigned int)a << " at time " << GetTickCount () << endl;
		IDirect3DSurface9 *a2 = NULL;
		IDirect3DSurface9 *b2 = NULL;
		if (a != NULL) a2 = ((FakeSurface *)a)->mRealObject;
		if (b != NULL) b2 = ((FakeSurface *)b)->mRealObject;
		HRESULT hr = D3D_OK;
		if (a2 != NULL) hr |= RealDevice->SetRenderTarget (0, a2);
		hr |= RealDevice->SetDepthStencilSurface (b2);
		return hr;
	}

	//-----------------------------------------------------------------------------

	// Voodoo DX8.9 ---------------------------------------------------
	/**
	 * Retrieves the current render target from this device.
	 *
	 * @param surface A pointer-to-surface-pointer that will store a reference to the render-target
	 *		surface.
	 * @result The result code of the underlying get function.
	 */
	/*HRESULT _stdcall GetRenderTarget(IDirect3DSurface8 ** surface)
	{
		IDirect3DSurface9 * realSurface = NULL;
		*surface = NULL;
		DWORD unused = 4;

		HRESULT hr = RealDevice->GetRenderTarget(0, &realSurface);

		if (hr != D3D_OK || realSurface == NULL)
		{
			return hr;
		}

		hr = realSurface->GetPrivateData(guid, (void *)surface, &unused);

		if (hr != D3D_OK) *surface = new FakeSurface(realSurface);

		return D3D_OK;
	}*/

	// MGE ------------------------------------------------------------
	HRESULT _stdcall GetRenderTarget (IDirect3DSurface8 **a) {
		IDirect3DSurface9 *a2 = NULL;
		*a = NULL;
		DWORD unused = 4;
		HRESULT hr = RealDevice->GetRenderTarget (0, &a2);
		if (hr != D3D_OK || a2 == NULL) return hr;
		hr = a2->GetPrivateData (guid, (void *)a, &unused);
		if (hr != D3D_OK) *a = new FakeSurface (a2);
		return D3D_OK;
	}

	// There are some coding style differences.

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDepthStencilSurface (IDirect3DSurface8 **a) {
		IDirect3DSurface9 *a2 = NULL;
		*a = NULL;
		DWORD unused = 4;
		HRESULT hr = RealDevice->GetDepthStencilSurface (&a2);
		if (hr != D3D_OK || a2 == NULL) return hr;
		hr = a2->GetPrivateData (guid, (void *)a, &unused);
		if (hr != D3D_OK) *a = new FakeSurface (a2);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall BeginScene () {
		//wraper_log_file << "Begin Scene. Count = " << (int)SceneCount << " AlphaScene = " << (int)AlphaScene << std::endl;
		////Start stencil tracking over
		//StencilEnabled = false;
		return RealDevice->BeginScene ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall EndScene(void)
	{
		return RealDevice->EndScene();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall Clear(DWORD a, const D3DRECT *b, DWORD c, D3DCOLOR d, float e, DWORD f)
	{
		d = 0xFFF00F88;
		return RealDevice->Clear(a, b, c, d, e, f);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetTransform(D3DTRANSFORMSTATETYPE a, const D3DMATRIX * b)
	{
		return RealDevice->SetTransform(a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetTransform(D3DTRANSFORMSTATETYPE a, D3DMATRIX *b)
	{
		return RealDevice->GetTransform(a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE a, const D3DMATRIX *b)
	{
		return RealDevice->MultiplyTransform(a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetViewport(const D3DVIEWPORT8 *a)
	{
		return RealDevice->SetViewport(a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetViewport(D3DVIEWPORT8 *a)
	{
		return RealDevice->GetViewport(a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetMaterial(const D3DMATERIAL8 *a)
	{
		CurrentMaterial = a;
		return RealDevice->SetMaterial(a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetMaterial(D3DMATERIAL8 *a)
	{
		return RealDevice->GetMaterial (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetLight(DWORD a, const D3DLIGHT8 *b)
	{
		return RealDevice->SetLight (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetLight(DWORD a, D3DLIGHT8 *b)
	{
		return RealDevice->GetLight (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall LightEnable(DWORD a, BOOL b)
	{
		/*if (a == 6) {
			// Light #6 (in exteriors) is the sun
			//SunEnabled = b;
		}*/
		return RealDevice->LightEnable (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetLightEnable(DWORD a, BOOL *b)
	{
		return RealDevice->GetLightEnable(a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetClipPlane(DWORD a, const float *b)
	{
		return RealDevice->SetClipPlane(a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetClipPlane(DWORD a, float *b)
	{
		return RealDevice->GetClipPlane(a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetRenderState(D3DRENDERSTATETYPE type, DWORD flags)
	{
		return RealDevice->SetRenderState(type, flags);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetRenderState (D3DRENDERSTATETYPE a, DWORD *b) 
	{
		return RealDevice->GetRenderState (a, b);
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall BeginStateBlock (void) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall EndStateBlock (DWORD *a) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall ApplyStateBlock (DWORD a) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall CaptureStateBlock(DWORD a) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall DeleteStateBlock(DWORD a) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall CreateStateBlock(D3DSTATEBLOCKTYPE a, DWORD *b) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall SetClipStatus(const D3DCLIPSTATUS8 *a) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall GetClipStatus(D3DCLIPSTATUS8 *a) 
	{
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	/**
	 * @warning Not implemented.
	 */
	HRESULT _stdcall GetTexture(DWORD a, IDirect3DBaseTexture8 ** b) 
	{
		//return RealDevice->GetTexture(a, b);
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetTexture (DWORD a, IDirect3DBaseTexture8 *b) 
	{
		// Look up Texture * from our internal map and call Core::Bind
		if ( b )
		{
			FakeTexture * b2 = reinterpret_cast<FakeTexture*>(b);
			return RealDevice->SetTexture(a, b2->mRealObject);
		} else {
			return RealDevice->SetTexture(a, NULL);
		}
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetTextureStageState(DWORD a, D3DTEXTURESTAGESTATETYPE b, DWORD * c)
	{
		//Some bugger decided to split this into two functions : (
		D3DSAMPLERSTATETYPE sampler;
		switch (b)
		{
		case D3DTSS_ADDRESSU:
			sampler = D3DSAMP_ADDRESSU;
			break;
		case D3DTSS_ADDRESSV:
			sampler = D3DSAMP_ADDRESSV;
			break;
		case D3DTSS_ADDRESSW:
			sampler = D3DSAMP_ADDRESSW;
			break;
		case D3DTSS_BORDERCOLOR:
			sampler = D3DSAMP_BORDERCOLOR;
			break;
		case D3DTSS_MAGFILTER:
			sampler = D3DSAMP_MAGFILTER;
			break;
		case D3DTSS_MINFILTER:
			sampler = D3DSAMP_MINFILTER;
			break;
		case D3DTSS_MIPFILTER:
			sampler = D3DSAMP_MIPFILTER;
			break;
		case D3DTSS_MIPMAPLODBIAS:
			sampler = D3DSAMP_MIPMAPLODBIAS;
			break;
		case D3DTSS_MAXMIPLEVEL:
			sampler = D3DSAMP_MAXMIPLEVEL;
			break;
		case D3DTSS_MAXANISOTROPY:
			sampler = D3DSAMP_MAXANISOTROPY;
			break;
		default:
			return RealDevice->GetTextureStageState(a, b, c);
		}

		return RealDevice->GetSamplerState(a, sampler, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetTextureStageState (DWORD a, D3DTEXTURESTAGESTATETYPE b, DWORD c) {
		//if (b == 11) return D3DERR_INVALIDCALL;
		//If it's something MGE controls then override it
		/*if (a == 0) {
			if ((b == D3DTSS_MINFILTER /*|| b == D3DTSS_MAGFILTER* /) && c == 2) {
				D3DSAMP_MINFILTER;
				return RealDevice->SetSamplerState (0, (D3DSAMPLERSTATETYPE) (b - 11), ScaleFilter);
			}
			if (b == D3DTSS_MIPFILTER && c == 2) {
				return RealDevice->SetSamplerState (0, D3DSAMP_MIPFILTER, MipFilter);
			}
			if (b == D3DTSS_MIPMAPLODBIAS && LODBias) {
				return RealDevice->SetSamplerState (0, D3DSAMP_MIPMAPLODBIAS, *(DWORD *)&LODBias);
			}
		}*/
		//Some bugger decided to split this into two functions : (
		D3DSAMPLERSTATETYPE sampler;
		switch (b) {
		case D3DTSS_ADDRESSU:
			sampler = D3DSAMP_ADDRESSU;
			break;
		case D3DTSS_ADDRESSV:
			sampler = D3DSAMP_ADDRESSV;
			break;
		case D3DTSS_ADDRESSW:
			sampler = D3DSAMP_ADDRESSW;
			break;
		case D3DTSS_BORDERCOLOR:
			sampler = D3DSAMP_BORDERCOLOR;
			break;
		case D3DTSS_MAGFILTER:
			sampler = D3DSAMP_MAGFILTER;
			break;
		case D3DTSS_MINFILTER:
			sampler = D3DSAMP_MINFILTER;
			break;
		case D3DTSS_MIPFILTER:
			sampler = D3DSAMP_MIPFILTER;
			break;
		case D3DTSS_MIPMAPLODBIAS:
			sampler = D3DSAMP_MIPMAPLODBIAS;
			break;
		case D3DTSS_MAXMIPLEVEL:
			sampler = D3DSAMP_MAXMIPLEVEL;
			break;
		case D3DTSS_MAXANISOTROPY:
			sampler = D3DSAMP_MAXANISOTROPY;
			break;
		default:
			return RealDevice->SetTextureStageState (a, b, c);
		}
		return RealDevice->SetSamplerState (a, sampler, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall ValidateDevice (DWORD *a) {
		return RealDevice->ValidateDevice (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetInfo (DWORD a, void *b, DWORD c) {
		//Doesn't seem to have a DX9 equivalent
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPaletteEntries (UINT a, const PALETTEENTRY *b) {
		return RealDevice->SetPaletteEntries (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPaletteEntries (UINT a, PALETTEENTRY *b) {
		return RealDevice->GetPaletteEntries (a, b);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetCurrentTexturePalette (UINT a) {
		return RealDevice->SetCurrentTexturePalette (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetCurrentTexturePalette (UINT *a) {
		return RealDevice->GetCurrentTexturePalette (a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawPrimitive (D3DPRIMITIVETYPE a, UINT b, UINT c) {
		/*++NumDrawCalls;

		if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawPrimitive (a, b, c);
			}
			return hr;
		}

		//Record the render if recording is turned on
		if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;
			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_PRIM;
			rdc.primType = a;
			rdc.startIndex = b;
			rdc.primitiveCount = c;

			DepthDrawRecorder.RecordDrawCall(rdc);
		}*/
		return RealDevice->DrawPrimitive (a, b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawIndexedPrimitive (D3DPRIMITIVETYPE a, UINT b, UINT c, UINT d, UINT e) {
		//++NumDrawCalls;

		////Detect drawing of Shadow plane
		//if (StencilEnabled && AlphaBlendEnabled && a == D3DPT_TRIANGLELIST && b == 0 && c == 4 && d == 0 && e == 2) {
		//	ShadowPlaneDrawn = true;
		//	wrapper_log_file << "Shadow Plane Drawn during scene " << int (SceneCount) << " after " << int (ShadowsDrawn) << " shadows drawn." << endl;
		//}

		//Record the render if recording is turned on
		/*if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;
			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_INDEXED;
			rdc.primType = a;
			rdc.baseVertexIndex = BaseVertexIndex;
			rdc.minIndex = b;
			rdc.numVertices = c;
			rdc.startIndex = d;
			rdc.primitiveCount = e;

			DepthDrawRecorder.RecordDrawCall(rdc);

			//D3DXMatrixIdentity (&CurrentBlendTrans1);
			//D3DXMatrixIdentity (&CurrentBlendTrans2);
			//D3DXMatrixIdentity (&CurrentBlendTrans3);
		}*/

		/*if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawIndexedPrimitive (a, BaseVertexIndex, b, c, d, e);
			}
			return hr;
		}*/

		return RealDevice->DrawIndexedPrimitive (a, BaseVertexIndex, b, c, d, e);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawPrimitiveUP (D3DPRIMITIVETYPE a, UINT b, const void *c, UINT d) {
		//++NumDrawCalls;
		/*if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawPrimitiveUP (a, b, c, d);
			}
			return hr;
		}
		//Record the render if recording is turned on
		if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;
			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_PRIM_UP;
			rdc.primType = a;
			rdc.primitiveCount = b;
			rdc.vertexStreamZeroData = c;
			rdc.vertexStreamZeroSride = d;

			DepthDrawRecorder.RecordDrawCall(rdc);
		}*/
		return RealDevice->DrawPrimitiveUP (a, b, c, d);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawIndexedPrimitiveUP (D3DPRIMITIVETYPE a, UINT b, UINT c, UINT d, const void *e, D3DFORMAT f, const void *g, UINT h) {
		/*++NumDrawCalls;
		if (HookedShaderActive > 1) {
			HRESULT hr = 0;
			for (DWORD i = 0; i < HookedShaderActive; ++i) {
				ShaderStruct.MWSEeffects [ActiveHookedShader].effect->BeginPass (i);
				hr |= RealDevice->DrawIndexedPrimitiveUP (a, b, c, d, e, f, g, h);
			}
			return hr;
		}

		//Record the render if recording is turned on
		if (!(CurrentDestBlend == D3DBLEND_ONE && AlphaBlendEnabled) && zWriteEnabled && !HUDRenderingStarted
			&& RecordRenders && !StencilEnabled && !RenderingToTexture
		) {
			DrawRecorder::RecordedDrawCall rdc;

			rdc.material = CurrentMaterial;
			rdc.texture = CurrentTexture;
			rdc.vBuffer = CurrentVBuffer;
			rdc.streamSpan = CurrentStreamSpan;
			rdc.indices = CurrentIndices;
			rdc.fvf = CurrentFVF;
			rdc.vertexBlendState = CurrentVertexBlendState;
			rdc.worldTrans [0] = CurrentWorldTrans;
			rdc.worldTrans [1] = CurrentBlendTrans1;
			rdc.worldTrans [2] = CurrentBlendTrans2;
			rdc.worldTrans [3] = CurrentBlendTrans3;
			rdc.cullModeState = CurrentCullModeState;
			rdc.alphaBlendEnabled = AlphaBlendEnabled;
			rdc.alphaTestEnabled = AlphaTestEnabled;
			rdc.alphaBlendFunc = CurrentSrcBlend;

			rdc.renderType = DrawRecorder::DRAW_INDEXED_UP;
			rdc.primType = a;
			rdc.minIndex = b;
			rdc.numVertices = c;
			rdc.primitiveCount = d;
			rdc.indexData = e;
			rdc.indexDataFormat = f;
			rdc.vertexStreamZeroData = g;
			rdc.vertexStreamZeroSride = h;

			DepthDrawRecorder.RecordDrawCall(rdc);
		}*/
		return RealDevice->DrawIndexedPrimitiveUP (a, b, c, d, e, f, g, h);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall ProcessVertices(UINT a, UINT b, UINT c, IDirect3DVertexBuffer8 *d, DWORD e)
	{
		return RealDevice->ProcessVertices (a, b, c, (IDirect3DVertexBuffer9 *)d, NULL, e);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateVertexShader(
		const DWORD * pDeclaration, const DWORD * pFunction, DWORD * pHandle, DWORD Usage
		)
	{
		// The Usage parameter is for legacy purposes only and is disregarded in actual behavior,
		// as all vertex processing is done in hardware.
		
		IDirect3DVertexShader9 * shader = NULL;
		HRESULT hr = RealDevice->CreateVertexShader(pFunction, &shader);
		if ( SUCCEEDED(hr) )
		{
			(*pHandle) = (DWORD)shader;
			mVShaders[*pHandle] = shader;

			VoodooCore->GetLog()->Format("Voodoo GEM: FakeDevice::CreateVertexShader called, shader %d created successfully.\n")
				.With(*pHandle).Done();
			return hr;
		} else {

		}

		VoodooCore->GetLog()->Log("Voodoo GEM: FakeDevice::CreateVertexShader called. Shader translation is not yet enabled.\n");
		return D3D_OK;

		/**c = (vShaderCount += 2);
		ID3DXBuffer *ShaderData = NULL;
		int i = 0;
		switch (*c) {
		case 9: ++i;
		case 7: ++i;
		case 5: ++i;
		case 3: ++i;
			char shaderfile [MAX_PATH];
			sprintf_s (shaderfile, sizeof (shaderfile), "Data Files\\shaders\\water\\vs%i.txt", i);
			D3DXAssembleShaderFromFileA (shaderfile, NULL, NULL, 0, &ShaderData, NULL);
			break;
		default:
			//ERRORMSG ("Morrowind attempted to create a vertex shader which MGE did not expect");
			/* ID3DXBuffer *output;
			HANDLE h;
			DWORD unused;
			D3DXDisassembleShader (b, false, NULL, &output);
			h = CreateFileA ("vout.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			WriteFile (h, output->GetBufferPointer (), output->GetBufferSize () - 1, &unused, NULL);
			CloseHandle (h);
			D3DXAssembleShaderFromFileA ("vout.txt", NULL, NULL, 0, &ShaderData, &Errors); break;
			break; * /
		}*/
		//HRESULT hr = RealDevice->CreateVertexShader ((DWORD *)ShaderData->GetBufferPointer (), &NewVShaders [(*c - 3) / 2]);
		//HRESULT hr = RealDevice->CreateVertexShader(
		//if (ShaderData) ShaderData->Release ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetVertexShader(DWORD a)
	{		
		VoodooCore->GetLog()->
			Format("Voodoo GEM: FakeDevice::SetVertexShader called on shader %u. Shader translation is not yet enabled.\n")
			.With(a).Done();

		//this->CurrentVShader = shaderHandle;

		//return D3D_OK;

		std::map<DWORD, IDirect3DVertexShader9*>::iterator shaderIter = mVShaders.find(a);
		if ( shaderIter != mVShaders.end() )
		{
			return RealDevice->SetVertexShader(shaderIter->second);
		} else {
			return RealDevice->SetFVF(a);
		}

		/*
		switch (a) 
		{
		case 0:
			RealDevice->SetVertexShader (NULL);
		case 3:
		case 5:
		case 9:
			RealDevice->SetVertexDeclaration (decl [0]);
			break;
		case 7:
			RealDevice->SetVertexDeclaration (decl [1]);
			break;
		default:
			//Save FVF setting
			mRenderState.FVF = a;

			CurrentVShader = 0;
			RealDevice->SetVertexShader (NULL);
			CurrentFVF = a;
			return RealDevice->SetFVF (a);
		}
		return RealDevice->SetVertexShader (NewVShaders [(a - 3) / 2]);
		*/
		
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShader(DWORD * a) 
	{
		VoodooCore->GetLog()->
			Format("Voodoo GEM: FakeDevice::GetVertexShader called on shader %u. Shader translation is not yet enabled.\n")
			.With(*a).Done();

		//*shaderHandle = this->CurrentVShader;

		//return D3D_OK;
		
		if (CurrentVShader != 0) {
			*a = CurrentVShader;
			return D3D_OK;
		} else {
			return RealDevice->GetFVF (a);
		}
		
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DeleteVertexShader(DWORD shaderHandle)
	{
		VoodooCore->GetLog()->
			Format("Voodoo GEM: FakeDevice::DeleteVertexShader called on shader %d. Shader translation is not yet enabled.\n")
			.With(shaderHandle).Done();

		if ( shaderHandle == this->CurrentVShader )
		{
			VoodooCore->GetLog()->Log("Voodoo GEM: Attempted to delete currently bound shader. This may be an error.\n");
		}

		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetVertexShaderConstant (DWORD a, const void *b, DWORD c) {
		return RealDevice->SetVertexShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShaderConstant (DWORD a, void *b, DWORD c) {
		return RealDevice->GetVertexShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShaderDeclaration (DWORD a, void *b, DWORD *c) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetVertexShaderFunction (DWORD a, void *b, DWORD *c) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetStreamSource (UINT a, IDirect3DVertexBuffer8 *b, UINT c) {
		CurrentVBuffer = (IDirect3DVertexBuffer9 *)b;
		CurrentStreamSpan = c;
		return RealDevice->SetStreamSource (a, (IDirect3DVertexBuffer9 *)b, 0, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetStreamSource (UINT a, IDirect3DVertexBuffer8 **b, UINT *c) {
		UINT unused;
		return RealDevice->GetStreamSource (a, (IDirect3DVertexBuffer9 **)b, &unused, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetIndices (IDirect3DIndexBuffer8 *a, UINT b) {
		BaseVertexIndex = b; //BaseVertexIndex has been moved to ProcessVerticies ()
		CurrentIndices = (IDirect3DIndexBuffer9 *)a;
		return RealDevice->SetIndices ((IDirect3DIndexBuffer9 *)a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetIndices (IDirect3DIndexBuffer8 **a, UINT *b) {
		*b = BaseVertexIndex;
		return RealDevice->GetIndices ((IDirect3DIndexBuffer9 **)a);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreatePixelShader (const DWORD *a, DWORD *b) {
		/*PixelShading = true;
		*b = ++pShaderCount;
		ID3DXBuffer *ShaderData = NULL;
		char shaderfile [MAX_PATH];
		sprintf_s (shaderfile, sizeof (shaderfile), "Data Files\\shaders\\water\\ps%i.txt", *b);
		if (*b <= 9 && *b > 0) D3DXAssembleShaderFromFileA (shaderfile, NULL, NULL, 0, &ShaderData, NULL);
		else ERRORMSG ("Morrowind tried to create an unexpected pixel shader");
		HRESULT hr = RealDevice->CreatePixelShader ((DWORD *)ShaderData->GetBufferPointer (), &NewPShaders [*b - 1]);
		if (hr != D3D_OK) {
			DebugBreak ();
		}
		if (ShaderData != NULL) ShaderData->Release ();
		return hr;*/
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPixelShader (DWORD a) {
		//CurrentPShader = a;
		//if (a == 0) return RealDevice->SetPixelShader (NULL);
		//else 
		//return RealDevice->SetPixelShader (a);
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPixelShader (DWORD *a) {
		*a = NULL;
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DeletePixelShader (DWORD a) {
		/*if (NewPShaders [a - 1] != NULL) {
			while (NewPShaders [a - 1]->Release ());
			NewPShaders [a - 1] = NULL;
		}*/
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall SetPixelShaderConstant (DWORD a, const void *b, DWORD c) {
		return RealDevice->SetPixelShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPixelShaderConstant (DWORD a, void *b, DWORD c) {
		return RealDevice->GetPixelShaderConstantF (a, (float *)b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetPixelShaderFunction (DWORD a, void * b, DWORD * c) {
		return UnusedFunction ();
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawRectPatch (UINT a, const float *b, const D3DRECTPATCH_INFO *c) {
		return RealDevice->DrawRectPatch (a, b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DrawTriPatch (UINT a, const float *b, const D3DTRIPATCH_INFO *c) {
		return RealDevice->DrawTriPatch (a, b, c);
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall DeletePatch (UINT a) {
		return RealDevice->DeletePatch (a);
	}

	//-----------------------------------------------------------------------------

};


/**
 * Fake DX8 object class for use by the game. This class transparently filters DX8
 * commands into the appropriate DX9 commands. The COM setup of DirectX makes it
 * simpler than it could be, but still tedious. In debug mode, this will log <em>a
 * lot</em> of things, to provide the most information possible.
 */
class FakeObject 
	: public IDirect3D8 
{
private:
	/** 
	 * Private reference to the real DX object. All methods called from this object
	 * are passed down to the real object, and some are filtered.
	 */
	IDirect3D9 * mRealObject;

public:
	/**
	 * Reference count for this object. If a reference is destroyed, this object will
	 * not completely destruct until Refs is 0.
	 */
	DWORD Refs;

	/**
	 * Default ctor, simply wraps a real object and initializes the reference count.
	 *
	 * @param realObject The object to be wrapped.
	 */
	FakeObject(IDirect3D9 * realObject)
	{
		mRealObject = realObject;
		Refs = 1;
#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: Created FakeObject at %d.\n").With(this).Done();
#endif
	}

	/**
	 * @addtogroup FakeObject_IUnknown "Fake Object: IUnknown Interface"
	 * @{
	 */
	HRESULT _stdcall QueryInterface(REFIID guid, LPVOID * dest)
	{
#ifdef _DEBUG
		HRESULT hr = mRealObject->QueryInterface(guid, dest);
		// Log the GUID and return pointer for the query.
		// Unnecessary: yes. Potentially interesting: yes.
		VoodooCore->GetLog()->Format
			("Voodoo GEM: FakeObject::QueryInterface({%d,%d,%d,{%d,%d,%d,%d,%d,%d,%d,%d}}, %d) == %d\n")
			.With(guid.Data1).With(guid.Data2).With(guid.Data3)
			.With(guid.Data4[0]).With(guid.Data4[1]).With(guid.Data4[2]).With(guid.Data4[3])
			.With(guid.Data4[4]).With(guid.Data4[5]).With(guid.Data4[6]).With(guid.Data4[7])
			.With(dest).With(hr).Done();

		return hr;
#else
		return mRealObject->QueryInterface(guid, dest);
#endif
	}

	ULONG _stdcall AddRef(void)
	{
#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::AddRef() == %d\n").With(++Refs).Done();
#else
		++Refs;
#endif

		return Refs;
	}

	ULONG _stdcall Release(void) 
	{
#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::Release() == %d\n").With(--Refs).Done();
#else
		--Refs;
#endif

		if (Refs == 0) 
		{
			VoodooCore->GetLog()->Log("Voodoo GEM: Refs == 0, releasing underlying object.\n");

			mRealObject->Release();
			delete this;
			return 0;
		}

		return Refs;
	}
	/**
	 * @}
	 */


	//-----------------------------------------------------------------------------
	/*** IDirect3D8 methods ***/
	//-----------------------------------------------------------------------------

	HRESULT _stdcall RegisterSoftwareDevice (void * device)
	{
#ifdef _DEBUG
		HRESULT hr = mRealObject->RegisterSoftwareDevice(device);

		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::RegisterSoftwareDevice(%d) == %d\n")
			.With(device).With(hr).Done();

		return hr;
#else
		return mRealObject->RegisterSoftwareDevice(device);
#endif
	}

	//-----------------------------------------------------------------------------

	UINT _stdcall GetAdapterCount(void)
	{
#ifdef _DEBUG
		UINT count = mRealObject->GetAdapterCount();

		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::GetAdapterCount() == %d\n")
			.With(count).Done();

		return count;
#else
		return mRealObject->GetAdapterCount();
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetAdapterIdentifier(UINT adapter, DWORD flags, D3DADAPTER_IDENTIFIER8 * dest)
	{
		// Get the results, but force the WHQL flags to 0 (no validation)
		D3DADAPTER_IDENTIFIER9 buffer;
		HRESULT hr = mRealObject->GetAdapterIdentifier(adapter, 0, &buffer);
#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::GetAdapterIdentifier(%d, %d, %d) == %d\n")
			.With(adapter).With(flags).With(dest).With(hr).Done();
#endif

		if (hr == D3D_OK) {
			//Strings, so use copymemory instead of assignment
			::CopyMemory (dest, &buffer, 1024);
			dest->DeviceId			= buffer.DeviceId;
			dest->DeviceIdentifier	= buffer.DeviceIdentifier;
			dest->DriverVersion		= buffer.DriverVersion;
			dest->Revision			= buffer.Revision;
			dest->SubSysId			= buffer.SubSysId;
			dest->VendorId			= buffer.VendorId;
			dest->WHQLLevel			= buffer.WHQLLevel;
		}

		return hr;
	}

	//-----------------------------------------------------------------------------

	UINT _stdcall GetAdapterModeCount(UINT adapter)
	{
		// By default, use the X8R8G8B8 format as the test format (it's standard screen format).
#ifdef _DEBUG
		UINT result = mRealObject->GetAdapterModeCount(adapter, D3DFMT_X8R8G8B8);
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::GetAdapterModeCount(%d, D3DFMT_X8R8G8B8) == %d\n")
			.With(adapter).With(result).Done();
		return result;
#else
		return mRealObject->GetAdapterModeCount(adapter, D3DFMT_X8R8G8B8);
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall EnumAdapterModes(UINT adapter, UINT mode, D3DDISPLAYMODE * displayMode)
	{
#ifdef _DEBUG
		UINT result = mRealObject->EnumAdapterModes(adapter, D3DFMT_X8R8G8B8, mode, displayMode);
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::EnumAdapterModes(%d, D3DFMT_X8R8G8B8, %d, %d) == %d\n")
			.With(adapter).With(mode).With(displayMode).With(result).Done();
		return result;
#else
		return mRealObject->EnumAdapterModes(adapter, D3DFMT_X8R8G8B8, mode, displayMode);
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetAdapterDisplayMode(UINT adapter, D3DDISPLAYMODE * displayMode)
	{
		// Hide the fact that windows is using a 16bit colour depth (?)
		HRESULT hr = mRealObject->GetAdapterDisplayMode(adapter, displayMode);
		displayMode->Format = D3DFMT_X8R8G8B8;
#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::GetAdapterDisplayMode(%d, %d) == %d\n")
			.With(adapter).With(displayMode).With(hr).Done();
#endif
		return hr;
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDeviceType(UINT adapter, D3DDEVTYPE type, D3DFORMAT format,
		D3DFORMAT bbformat, BOOL windowed) 
	{
#ifdef _DEBUG
		HRESULT hr = mRealObject->CheckDeviceType(adapter, type, format, bbformat, windowed);
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::CheckDeviceType(%d, %d, %d, %d, %d) == %d\n")
			.With(adapter).With(type).With(format).With(bbformat).With(windowed).With(hr).Done();
		return hr;
#else
		return mRealObject->CheckDeviceType(adapter, type, format, bbformat, windowed);
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDeviceFormat(UINT adapter, D3DDEVTYPE type, D3DFORMAT format,
		DWORD usage, D3DRESOURCETYPE rtype, D3DFORMAT checkformat)
	{
#ifdef _DEBUG
		HRESULT hr = mRealObject->CheckDeviceFormat(adapter, type, format, usage, rtype, checkformat);
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::CheckDeviceFormat(%d, %d, %d, %d, %d, %d) == %d\n")
			.With(adapter).With(type).With(format).With(usage).With(rtype).With(checkformat).With(hr).Done();
		return hr;
#else
		return mRealObject->CheckDeviceFormat(adapter, type, format, usage, rtype, checkformat);
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDeviceMultiSampleType(UINT adapter, D3DDEVTYPE type, D3DFORMAT format,
		BOOL windowed, D3DMULTISAMPLE_TYPE multisampletype)
	{
		// Retrieve the multisample type info, ignoring quality levels.
#ifdef _DEBUG
		HRESULT hr = mRealObject->CheckDeviceMultiSampleType(adapter, type, format, windowed, multisampletype, NULL);
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::CheckDeviceMultiSampleType(%d, %d, %d, %d, %d) == %d\n")
			.With(adapter).With(type).With(format).With(windowed).With(multisampletype).With(hr).Done();
		return hr;
#else
		return mRealObject->CheckDeviceMultiSampleType(adapter, type, format, windowed, multisampletype, NULL);
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CheckDepthStencilMatch(UINT adapter, D3DDEVTYPE type, D3DFORMAT format,
		D3DFORMAT rtformat, D3DFORMAT dsformat)
	{
#ifdef _DEBUG
		HRESULT hr = mRealObject->CheckDepthStencilMatch(adapter, type, format, rtformat, dsformat);
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::CheckDepthStencilMatch(%d, %d, %d, %d, %d) == %d\n")
			.With(adapter).With(type).With(format).With(rtformat).With(dsformat).With(hr).Done();
		return hr;
#else
		return mRealObject->CheckDepthStencilMatch(adapter, type, format, rtformat, dsformat);
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall GetDeviceCaps(UINT adapter, D3DDEVTYPE type, D3DCAPS8 * caps) 
	{
		// Return stored (and modified) capabilities structure.
		*caps = d3d8Caps;

#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::GetDeviceCaps(%d, %d, %d) == D3D_OK\n")
			.With(adapter).With(type).With(caps).Done();
#endif
		return D3D_OK;
	}

	//-----------------------------------------------------------------------------

	HMONITOR _stdcall GetAdapterMonitor(UINT adapter)
	{
#ifdef _DEBUG
		HMONITOR hm = mRealObject->GetAdapterMonitor(adapter);
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::GetAdapterMonitor(%d) == %d\n")
			.With(adapter).With(hm).Done();
		return hm;
#else
		return mRealObject->GetAdapterMonitor(adapter);
#endif
	}

	//-----------------------------------------------------------------------------

	HRESULT _stdcall CreateDevice(UINT adapter, D3DDEVTYPE type, HWND window,
		DWORD flags, D3DPRESENT_PARAMETERS8 * presentparams, IDirect3DDevice8 ** device) 
	{
		// Need to record resolution and such here, as well as create temporary surfaces.
#ifdef _DEBUG
		VoodooCore->GetLog()->Log("Voodoo GEM: FakeObject::CreateDevice called.\n");
#endif

		D3DPRESENT_PARAMETERS9 pp;
		pp.Flags = presentparams->Flags;
		pp.Windowed = presentparams->Windowed;
		pp.BackBufferCount = presentparams->BackBufferCount;
		pp.BackBufferFormat = presentparams->BackBufferFormat;
		pp.BackBufferHeight = presentparams->BackBufferHeight;
		pp.BackBufferWidth = presentparams->BackBufferWidth;
		pp.hDeviceWindow = presentparams->hDeviceWindow;
		pp.EnableAutoDepthStencil = presentparams->EnableAutoDepthStencil;
		pp.SwapEffect = presentparams->SwapEffect;
		pp.MultiSampleQuality = 0;
		pp.MultiSampleType = presentparams->MultiSampleType;

		if (pp.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER) pp.Flags ^= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		pp.AutoDepthStencilFormat = D3DFMT_D24S8;
		pp.FullScreen_RefreshRateInHz = 0;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

#ifdef _DEBUG
		const char * textureType = 
			VoodooShader::Converter::ToString(
			VoodooShader::DirectX9::DX9_Converter::FromD3DFMT(pp.BackBufferFormat)
			);
		VoodooCore->GetLog()->Format("Voodoo GEM: Backbuffer parameters: %d by %d (%d buffers), TF: %s.\n")
			.With(pp.BackBufferWidth).With(pp.BackBufferHeight).With(pp.BackBufferCount)
			.With(textureType).Done();
#endif

		IDirect3DDevice9 * RealDevice;

		HRESULT hr = mRealObject->CreateDevice(adapter, type, window, flags, &pp, &RealDevice);

#ifdef _DEBUG
		VoodooCore->GetLog()->Format("Voodoo GEM: FakeObject::CreateDevice(%d, %d, %d, %d, %d, %d) == %d\n")
			.With(adapter).With(type).With(window).With(flags).With(&pp).With(RealDevice).With(hr).Done();
#endif

		if (hr != D3D_OK || RealDevice == NULL) return hr;

#ifdef _DEBUG
		VoodooCore->GetLog()->Log("Voodoo GEM: Real device created.\n");
#endif

			/*D3DFORMAT fmt;
			switch (ZBufFormat) {
			case 1:
				fmt = (D3DFORMAT)MAKEFOURCC ('I', 'N', 'T', 'Z');
				break;
			case 2:
				fmt = (D3DFORMAT)MAKEFOURCC ('R', 'A', 'W', 'Z');
				break;
			case 3:
				fmt = (D3DFORMAT)MAKEFOURCC ('D', 'F', '1', '6');
				break;
			case 4:
				fmt = (D3DFORMAT)MAKEFOURCC ('D', 'F', '2', '4');
				break;
			}*/
		//hr = RealDevice->CreateTexture (e->BackBufferWidth, e->BackBufferHeight, 1, 
		//				D3DUSAGE_DEPTHSTENCIL, fmt, D3DPOOL_DEFAULT, &DepthTexture, 0);


		//RealDevice->SetRenderState (D3DRS_ZENABLE, D3DZB_USEW);
		//RealDevice->SetRenderState (D3DRS_CLIPPING, (D3DTEXTURESTAGESTATETYPE)0);
		
		//Create a font object for scribbling over the screen
		//if (D3DXCreateFontA (RealDevice, 12, 0, 400, 1, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		//	DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "arial", &Font)

#ifdef _DEBUG
		VoodooCore->GetLog()->Log("Voodoo GEM: Creating fake device...\n");
#endif
		FakeDevice * fdevice = new FakeDevice(this->mRealObject, RealDevice);
		*device = fdevice;

		VoodooCore->GetLog()->Log("Voodoo GEM: Starting VoodooDX9 module...\n");
		VoodooDX9 = new VoodooShader::DirectX9::Adapter(VoodooCore, RealDevice);

		try
		{
			VoodooShader::ShaderRef a = VoodooCore->CreateShader("J:\\Morrowind_clean\\test.cgfx", NULL);
		} catch ( VoodooShader::Exception & exc ) {
			VoodooCore->GetLog()->Log("Voodoo GEM: Error compiling shader.\n");
		}

		return D3D_OK;
	}

	//-----------------------------------------------------------------------------
};


//=============================================================================

__declspec(dllexport) void * __stdcall Direct3DCreate8 (UINT version)
{
	// Voodoo DX8.9 Init function
	VoodooCore = new VoodooShader::Core("Voodoo_GEM.log");

#ifdef _DEBUG
	VoodooCore->GetLog()->SetBufferSize(0);
#endif

	VoodooCore->GetLog()->Format("Voodoo GEM: Direct3DCreate8 called, SDK version: %d.\n").With(version).Done();

	// First, load D3D8.dll and check capabilities of the DX8 device/drivers
	char Path[MAX_PATH];
	GetSystemDirectoryA(Path, MAX_PATH);
	strcat_s (Path, MAX_PATH, "\\d3d8.dll");

	HMODULE D3D8Library = LoadLibraryA(Path);
	if (D3D8Library == NULL) 
	{
		VoodooCore->GetLog()->Log("Voodoo GEM: Unable to load D3D8.dll.\n");
		return NULL;
	}

	typedef IDirect3D8 * (__stdcall*D3DFunc8)(UINT);
	D3DFunc8 D3D8Create = (D3DFunc8)GetProcAddress (D3D8Library, "Direct3DCreate8");
	if (D3D8Create == NULL)
	{
		VoodooCore->GetLog()->Log("Voodoo GEM: Unable to find function Direct3DCreate8 in D3D8.dll.\n");
		return NULL;
	}

	IDirect3D8 * TempObject = (D3D8Create)(version);
	HRESULT hr = TempObject->GetDeviceCaps (0, D3DDEVTYPE_HAL, &d3d8Caps);
	if (hr != D3D_OK)
	{ 
		VoodooCore->GetLog()->Log("Voodoo GEM: Unable to retrieve DX8 capabilities.\n"); 
	}
	TempObject->Release();
	FreeLibrary(D3D8Library);

	VoodooCore->GetLog()->Log("Voodoo GEM: DX8 capabilities retrieved successfully.\n"); 

	// Forcibly disable shaders in the caps
	d3d8Caps.PixelShaderVersion = 0;
	d3d8Caps.VertexShaderVersion = 0;

	// Call DX9 to create a real device with the latest version
	IDirect3D9 * object = Direct3DCreate9(D3D_SDK_VERSION);
	// Turn it into a FakeObject and return it.
	return new FakeObject(object);
}

// Visual Studio-specific linker directive, forces the function to be exported with and
// without decoration. The actual symbol is undecorated, but I'd rather allow exception
// handling than use extern "C".
#pragma comment(linker, "/export:Direct3DCreate8=?Direct3DCreate8@@YGPAXI@Z")

//=============================================================================

