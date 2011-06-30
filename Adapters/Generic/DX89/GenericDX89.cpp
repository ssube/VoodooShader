// GenericDX89.cpp : Implementation of CGenericDX89

#include "stdafx.h"
#include "GenericDX89.h"

// CGenericDX89
CGenericDX89::CGenericDX89()
{
    m_Refrs = 0;
    m_Init = FALSE;
    m_Context = NULL;
}

CGenericDX89::~CGenericDX89()
{

}

STDMETHODIMP CGenericDX89::get_Core(IVoodooCore **ppCore)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( ppCore == NULL ) {
        return VSFERR_INVALID_ARG;
    } else {
        *ppCore = this->m_Core;
        return VSF_OK;
    }
}

STDMETHODIMP CGenericDX89::LoadPass(IVoodooPass *pPass)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( pPass == NULL )
    {
        return VSFERR_INVALID_ARG;
    }

    CComPtr<IVoodooPass> pass(pPass);

    CComBSTR passName;
    if ( SUCCEEDED(pass->get_Name(&passName) ) )
    {
        LogFormat(this->m_Logger, LL_Info|LL_Framework, VSTR("Load pass called for pass %s (DX8.9 passes do not need loaded)."), (LPTSTR)passName);
    } else {
        LogFormat(this->m_Logger, LL_Warning|LL_Framework, VSTR("Load pass called for pass at %#x, unable to look up name (DX8.9 passes do not need loaded."), (LPTSTR)passName);
    }

    return VSF_OK;
}

STDMETHODIMP CGenericDX89::UnloadPass(IVoodooPass *pPass)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( pPass == NULL )
    {
        return VSFERR_INVALID_ARG;
    }

    return VSF_OK;
}

STDMETHODIMP CGenericDX89::get_Pass(IVoodooPass **ppPass)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( ppPass == NULL ) {
        return VSFERR_INVALID_ARG;
    } else {
         return m_CurrentPass.CopyTo(ppPass);
    }
}

STDMETHODIMP CGenericDX89::put_Pass(IVoodooPass *pPass)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else {
        if ( m_CurrentPass )
        {
            VARIANT passVariant;
            if ( SUCCEEDED(m_CurrentPass->get_CgPass(&passVariant) ) )
            {
                CGpass pass = (CGpass)V_BYREF(&passVariant);
                cgResetPassState(pass);
            }
        }

        m_CurrentPass = pPass;

        if ( m_CurrentPass )
        {
            VARIANT passVariant;
            if ( SUCCEEDED(m_CurrentPass->get_CgPass(&passVariant) ) )
            {
                CGpass pass = (CGpass)V_BYREF(&passVariant);
                cgSetPassState(pass);
            }
        }

        return VSF_OK;
    }
}

STDMETHODIMP CGenericDX89::get_Target(IVoodooTexture **ppTarget)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( ppTarget == NULL ) {
        return VSFERR_INVALID_ARG;
    } else {
        return m_CurrentTarget.CopyTo(ppTarget);
    }
}

STDMETHODIMP CGenericDX89::put_Target(IVoodooTexture *pTarget)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else {
        m_CurrentTarget = pTarget;

        return VSF_OK;
    }
}

STDMETHODIMP CGenericDX89::get_Light(int Index, LightStruct *pLight)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( pLight == NULL ) {
        return VSFERR_INVALID_ARG;
    } else if ( Index < 0 ) {
        return VSFERR_INVALID_ARG;
    } else if ( Index > VOODOO_DX89_MAX_LIGHTS ) {
        return VSFERR_INVALID_ARG;
    } else {
        *pLight = this->m_Lights[Index];
        return VSF_OK;
    }
}

STDMETHODIMP CGenericDX89::put_Light(int Index, LightStruct Light)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( Index < 0 ) {
        return VSFERR_INVALID_ARG;
    } else if ( Index > VOODOO_DX89_MAX_LIGHTS ) {
        return VSFERR_INVALID_ARG;
    } else {
        this->m_Lights[Index] = Light;
        return VSF_OK;
    }
}

STDMETHODIMP CGenericDX89::SetProperty(BSTR pName, VARIANT *pData)
{
    CComBSTR property(pName);

    if ( property == VSTR("D3D8Object") )
    {
        if ( V_VT(pData) != VT_UNKNOWN )
        {
            return VSFERR_INVALID_ARG;
        } else if ( m_FakeObject != NULL ) {
            return VSF_FAIL;
        } else {
            IUnknown * pUnk = V_UNKNOWN(pData);
            IDirect3D9 * pD3D9 = NULL;
            if ( pUnk && SUCCEEDED(pUnk->QueryInterface(IID_IDirect3D9, (void**)&pD3D9)) )
            {
                m_RealObject = pD3D9;
                m_FakeObject = new IVoodoo3D8(m_RealObject);
                return m_FakeObject->QueryInterface(IID_IUnknown, V_UNKNOWN(pData));
            } else {
                // Fail messily.
                return VSF_FAIL;
            }
        }
    }
}

STDMETHODIMP CGenericDX89::DrawGeometry(int Vertexes,  VertexStruct pVertexData[])
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( pVertexData == NULL ) {
        return VSFERR_INVALID_ARG;
    } else if ( Vertexes < 0 ) {
        return VSFERR_INVALID_ARG;
    }

    IDirect3DVertexBuffer9 * sourceBuffer;
    UINT sourceOffset, sourceStride;
    DWORD zEnabled, aEnabled, cullMode;
    IDirect3DVertexDeclaration9 * pOldVertDecl;

    //this->m_RealDevice->GetStreamSource(0, &sourceBuffer, &sourceOffset, &sourceStride);
    this->m_RealDevice->GetVertexDeclaration(&pOldVertDecl);
    this->m_RealDevice->GetRenderState(D3DRS_ZENABLE, &zEnabled);
    this->m_RealDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &aEnabled);
    this->m_RealDevice->GetRenderState(D3DRS_CULLMODE, &cullMode);

    //this->m_RealDevice->SetStreamSource(0, FSQuadVerts, 0, sizeof(FSVert));
    this->m_RealDevice->SetVertexDeclaration(this->m_VertDecl);
    this->m_RealDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    this->m_RealDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    this->m_RealDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    HRESULT hr = this->m_RealDevice->BeginScene();
    if ( SUCCEEDED(hr) )
    {
        hr = this->m_RealDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertexData, sizeof(VertexStruct));
        this->m_RealDevice->EndScene();
    }

    //this->mDevice->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
    this->m_RealDevice->SetVertexDeclaration(pOldVertDecl);
    this->m_RealDevice->SetRenderState(D3DRS_ZENABLE, zEnabled);
    this->m_RealDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, aEnabled);
    this->m_RealDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
}

STDMETHODIMP CGenericDX89::ApplyParameter(IVoodooParameter *pParameter)
{
    if ( m_Init != TRUE )
    {
        return VSFERR_NOT_INIT;
    } else if ( pParameter == NULL ) {
        return VSFERR_INVALID_ARG;
    }

    ParameterType Type;
    pParameter->get_Type((EnumType*)&Type);

    VARIANT CgParameterVar;
    CGparameter pCgParameter;
    pParameter->get_CgParameter(&CgParameterVar);
    pCgParameter = (CGparameter)V_BYREF(&CgParameterVar);

    switch ( ToParameterCategory(Type) )
    {
    case PC_Float:
        {
            CComSafeArray<float> ScalarSafeArray;
            float ScalarArray[16];
            ZeroMemory(ScalarArray, sizeof(float) * 16);

            pParameter->get_ScalarValue(&ScalarSafeArray);
            ULONG size = min(ScalarSafeArray.GetCount(), (ULONG)16);

            for ( ULONG i = 0; i < size; ++i )
            {
                ScalarArray[i] = ScalarSafeArray.GetAt(i);
            }

            cgD3D9SetUniform(pCgParameter, ScalarArray);
        }
        return VSF_OK;
    case PC_Sampler:
        {
            IVoodooTexture * pTexture = NULL;
            if ( SUCCEEDED(pParameter->get_SamplerValue(&pTexture)) )
            {
                VARIANT BaseTexture;
                pTexture->get_Data(&BaseTexture);
                IDirect3DBaseTexture9 * pBaseTexture = (IDirect3DBaseTexture9*)V_UNKNOWN(&BaseTexture);
                cgD3D9SetTextureParameter(pCgParameter, pBaseTexture);
            }
        }
        return VSF_OK;
    case PC_Struct:
        return VSFERR_NOT_IMPL;
    case PC_Unknown:
    default:
        {
            CComBSTR ParamName;
            pParameter->get_Name(&ParamName);
            LogFormat
            (
                m_Core, LL_Info | LL_Framework,
                VOODOO_DX89_NAME,
                VSTR("Unable to bind parameter %s of unknown type."),
                (LPTSTR)ParamName
            );
        }
    }
}

STDMETHODIMP CGenericDX89::DrawShader(IVoodooShader *pShader)
{
    return VSFERR_NOT_IMPL;
}

STDMETHODIMP CGenericDX89::CreateTexture(BSTR pName, TextureDesc Description, IVoodooTexture **ppTexture)
{
    return VSFERR_NOT_IMPL;
    /*IDirect3DTexture9 * tex = NULL;
    D3DFORMAT fmt = DX89_Converter::ToD3DFormat(format);

    HRESULT hr = mDevice->CreateTexture(width, height, depth, 
        D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex, NULL);
    if ( SUCCEEDED(hr) )
    {
        TextureRef texRef = mCore->CreateTexture(name, reinterpret_cast<void*>(tex));
        return texRef;
    } else {
        const char * error = cgD3D9TranslateHRESULT(hr);
        mCore->Log
            (
            LL_Error,
            VOODOO_DX89_NAME,
            "Error creating texture %s: %s",
            name, error
            );
        return TextureRef();
    }*/
}

STDMETHODIMP CGenericDX89::LoadTexture(IVoodooImage *pImage, TextureRegion Region, IVoodooTexture **ppTexture)
{
    return VSFERR_NOT_IMPL;
}

STDMETHODIMP CGenericDX89::ConnectTexture(IVoodooParameter *pParameter, IVoodooTexture *pTexture)
{
    return VSFERR_NOT_IMPL;
    /*if ( Converter::ToParameterCategory(param->GetType()) == PC_Sampler )
    {
        param->Set(texture);

        IDirect3DTexture9 * texObj = (IDirect3DTexture9 *)texture->GetTexture();
        CGparameter texParam = param->GetParameter();
        cgD3D9SetTextureParameter(texParam, texObj);
        mCore->Log
            (
            LL_Info,
            VOODOO_DX89_NAME,
            "Bound texture %s to parameter %s.",
            texture->Name().c_str(), param->Name().c_str()
            );
        return true;
    } else {
        Throw(VOODOO_DX89_NAME, "Invalid binding attempt, parameter is not a sampler.\n", this->mCore);
        return false;
    }*/
}

STDMETHODIMP CGenericDX89::HandleError(VARIANT pContext, DWORD Error, IVoodooCore *pCore)
{
    return VSFERR_NOT_IMPL;
}
/*

STDMETHODIMP CGenericDX89::Initialize(IVoodooCore *pCore)
{
    if ( pCore == NULL ) return VSFERR_INVALID_ARG;

    m_Core = pCore;
    m_Core->get_Logger(&m_Logger);

    m_Logger->Log(LL_Info, VOODOO_DX89_NAME, VSTR("Starting adapter..."));
    m_Logger->LogModule(this->GetVersion());

    // Init Cg
    m_Context = cgCreateContext();
    if ( !cgIsContext(m_Context) )
    {
        return VSFERR_INVALID_CG;
    }

    HRESULT hr = cgD3D9SetDevice(device);
    if ( !SUCCEEDED(hr) )
    {
        Throw(VOODOO_DX89_NAME, "Could not set Cg device.", core);
    } else {
        core->Log
            (
            LL_Debug,
            VOODOO_DX89_NAME,
            "Set Cg device to %p.",
            device
            );
    }

    //#ifdef _DEBUG
    cgD3D9EnableDebugTracing(CG_TRUE);
    //#endif

    cgD3D9SetManageTextureParameters(core->GetCGContext(), CG_TRUE);

    cgD3D9RegisterStates(core->GetCGContext());

    HRESULT errors = cgD3D9GetLastError();
    if ( !SUCCEEDED(errors) )
    {
        core->Log
            (
            LL_Errors,
            VOODOO_DX89_NAME,
            "Errors setting Cg states: %s",
            cgD3D9TranslateHRESULT(errors)
            );
    } else {
        core->Log
            (
            LL_Debug,
            VOODOO_DX89_NAME,
            "Cg states set successfully."
            );
    }

    // Setup profiles
    CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
    CGprofile bestVert = cgD3D9GetLatestVertexProfile();

    core->Log
        (
        LL_Info,
        VOODOO_DX89_NAME,
        "Detected the following profiles: %s (vertex); %s (fragment)",
        cgGetProfileString(bestVert), cgGetProfileString(bestFrag)
        );

    // Get params
    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);

    float fx = ( (float)viewport.Width  / 2 ) + 0.5f    ;/// 2;
    float fy = ( (float)viewport.Height / 2 ) + 0.5f    ;/// 2;

    mCore->Log
        (
        LL_Info,
        VOODOO_DX89_NAME,
        "Prepping for %d by %d target.",
        fx, fy
        );

    hr = this->mDevice->CreateVertexBuffer(6 * sizeof(FSVert), 0, D3DFVF_CUSTOMVERTEX,
        D3DPOOL_DEFAULT, &FSQuadVerts, NULL);

    if ( FAILED(hr) )
    {
        mCore->Log
            (
            LL_Error,
            VOODOO_DX89_NAME,
            "Failed to create vertex buffer for fullscreen quad."
            );
    }

    FSVert g_Vertices[4];
    memset(g_Vertices, 0, sizeof(FSVert) * 4);

    g_Vertices[0].x = -0.5f; g_Vertices[0].y = -0.5f; g_Vertices[0].z = 0.5f;
    g_Vertices[1].x =    fx; g_Vertices[1].y = -0.5f; g_Vertices[1].z = 0.5f;
    g_Vertices[2].x = -0.5f; g_Vertices[2].y =    fy; g_Vertices[2].z = 0.5f;
    g_Vertices[3].x =    fx; g_Vertices[3].y =    fy; g_Vertices[3].z = 0.5f;

    g_Vertices[0].rhw = g_Vertices[1].rhw = g_Vertices[2].rhw = g_Vertices[3].rhw = 1.0f;

    g_Vertices[0].tu = 0.0f; g_Vertices[0].tv = 0.0f;
    g_Vertices[1].tu = 1.0f; g_Vertices[1].tv = 0.0f;
    g_Vertices[2].tu = 0.0f; g_Vertices[2].tv = 1.0f;
    g_Vertices[3].tu = 1.0f; g_Vertices[3].tv = 1.0f;

    void * pVertices;
    FSQuadVerts->Lock(0, sizeof(FSVert) * 4, &pVertices, 0);

    memcpy(pVertices, g_Vertices, sizeof(FSVert) * 4);

    FSQuadVerts->Unlock();
}
*/

