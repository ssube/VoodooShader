// VoodooParameter.cpp : Implementation of CVoodooParameter

#include "stdafx.h"
#include "VoodooParameter.h"




// CVoodooParameter
CVoodooParameter::CVoodooParameter()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Shader = NULL;
    m_Data.Create(16);
    m_Texture = NULL;
}

CVoodooParameter::~CVoodooParameter()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Shader = NULL;
    m_Data.Destroy();
    m_Texture = NULL;
}

IVoodooParameter * CVoodooParameter::Create(IVoodooCore * pCore, BSTR pName, ParameterType Type)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooParameter> ipParameter = NULL;

    CComObject<CVoodooParameter> * pParameter = NULL;
    HRESULT hr = CComObject<CVoodooParameter>::CreateInstance(&pParameter);
    if ( SUCCEEDED(hr) )
    {
        pParameter->AddRef();

        pParameter->m_Parameter = CreateVirtualParameter(pCore, Type);
        pParameter->m_Type = Type;
        pParameter->m_Virtual = VARIANT_TRUE;
        pParameter->m_Core = pCore;
        pParameter->m_Name = pName;

        hr = pParameter->QueryInterface(IID_IVoodooParameter, (void**)&ipParameter);
        pParameter->Release();
    }

    return ipParameter.Detach();
}

IVoodooParameter * CVoodooParameter::Create(IVoodooShader * pShader, CGparameter Parameter)
{
    if ( pShader == NULL ) return NULL;

    CComPtr<CVoodooParameter> ipParameter = NULL;

    CComObject<CVoodooParameter> * pParameter = NULL;
    HRESULT hr = CComObject<CVoodooParameter>::CreateInstance(&pParameter);
    if ( SUCCEEDED(hr) )
    {
        pParameter->AddRef();

        pParameter->m_Parameter = Parameter;
        pParameter->m_Type = ToParameterType(cgGetParameterType(Parameter));
        pParameter->m_Shader = pShader;
        pParameter->m_Virtual = VARIANT_FALSE;
        pShader->get_Core(&pParameter->m_Core);
        pParameter->m_Name = CA2W(cgGetParameterName(Parameter));

        hr = pParameter->QueryInterface(IID_IVoodooTechnique, (void**)&ipParameter);
        pParameter->Release();
    }

    return ipParameter.Detach();
}

STDMETHODIMP CVoodooParameter::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooParameter ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooParameter::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooParameter::Release()
{
    --m_Refrs;
    if ( m_Refrs == 0 )
    {
        delete this;
        return 0;
    } else {
        return m_Refrs;
    }
}

STDMETHODIMP CVoodooParameter::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooParameter::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        (*ppCore)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::get_Type(DWORD *pType)
{
    if ( pType == NULL )
    {
        return E_INVALIDARG;
    } else {
        *pType = m_Type;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::get_Virtual(VARIANT_BOOL *pVirtual)
{
    if ( pVirtual == NULL )
    {
        return E_INVALIDARG;
    } else {
        *pVirtual = m_Virtual;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::AttachParameter(IVoodooParameter *pParameter)
{
    if ( pParameter == NULL )
    {
        return E_INVALIDARG;
    }

    // Check types
    ParameterType othertype;
    pParameter->get_Type((DWORD*)&othertype);

    if ( othertype != m_Type )
    {
        return E_INVALIDARG;
    }

    CGparameter otherparam;
    VARIANT othervar;
    pParameter->get_CgParameter(&othervar);
    otherparam = (CGparameter)V_BYREF(&othervar);

    cgConnectParameter(m_Parameter, otherparam);

    return S_OK;
}

STDMETHODIMP CVoodooParameter::get_Components(int * Components)
{
    switch ( m_Type )
    {
    case PT_Float1x1:
        *Components = 1;
        break;
    case PT_Float1x2:
        *Components = 2;
        break;
    case PT_Float1x3:
        *Components = 3;
        break;
    case PT_Float1x4:
        *Components = 4;
        break;
    case PT_Float2x1:
        *Components = 2;
        break;
    case PT_Float2x2:
        *Components = 4;
        break;
    case PT_Float2x3:
        *Components = 6;
        break;
    case PT_Float2x4:
        *Components = 8;
        break;
    case PT_Float3x1:
        *Components = 3;
        break;
    case PT_Float3x2:
        *Components = 6;
        break;
    case PT_Float3x3:
        *Components = 9;
        break;
    case PT_Float3x4:
        *Components = 12;
        break;
    case PT_Float4x1:
        *Components = 4;
        break;
    case PT_Float4x2:
        *Components = 8;
        break;
    case PT_Float4x3:
        *Components = 12;
        break;
    case PT_Float4x4:
        *Components = 16;
        break;
    case PT_Sampler1D:
        *Components = 1;
        break;
    case PT_Sampler2D:
        *Components = 2;
        break;
    case PT_Sampler3D:
        *Components = 3;
        break;
    case PT_Struct:
        return E_NOTIMPL;
    default:
        *Components = -1;
        break;
    }

    return S_OK;
} 

STDMETHODIMP CVoodooParameter::get_SamplerValue( 
    /* [retval][out] */ IVoodooTexture **ppTexture)
{
    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppTexture = m_Texture;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::put_SamplerValue( 
    /* [in] */ IVoodooTexture *pTexture)
{
    pTexture = m_Texture;
    return S_OK;
}

STDMETHODIMP CVoodooParameter::get_ScalarValue( 
    /* [retval][out] */ SAFEARRAY ** ppData)
{
    return m_Data.CopyTo(ppData);
}

STDMETHODIMP CVoodooParameter::put_ScalarValue( 
    /* [in] */ SAFEARRAY * pData)
{
    return m_Data.CopyFrom(pData);
}

STDMETHODIMP CVoodooParameter::get_Shader
( 
    IVoodooShader ** ppShader
)
{
    if ( ppShader == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppShader = m_Shader;
        (*ppShader)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::get_CgParameter( 
    /* [retval][out] */ VARIANT *ppCgParameter)
{
    if ( ppCgParameter == NULL )
    {
        return E_INVALIDARG;
    } else {
        V_VT(ppCgParameter) = VT_BYREF;
        V_BYREF(ppCgParameter) = m_Parameter;
        return S_OK;
    }
}

STDMETHODIMP CVoodooParameter::LinkParameter()
{
    if ( !cgIsParameter(m_Parameter) )
    {
        return VSFERR_LINKER_ERROR;
    }

    // Check if it has a global link annotation
    CGannotation globalAnnotation = cgGetNamedParameterAnnotation(m_Parameter, "global");

    if ( cgIsAnnotation(globalAnnotation) && cgGetAnnotationType(globalAnnotation) == CG_STRING )
    {
        const char * globalName = cgGetStringAnnotationValue(globalAnnotation);
        if ( globalName != NULL && strlen(globalName) > 0 )
        {
            CComBSTR globalNameBSTR(globalName);
            IVoodooParameter * globalParam = NULL;
            m_Core->GetParameter(globalNameBSTR, &globalParam);

            if ( globalParam != NULL )
            {
                globalParam->AttachParameter(this);

                return S_OK;
            } else {
                /*m_Core->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME, 
                    "Unable to find global param %s for parameter %s.",
                    globalName, param->GetName().c_str()
                );*/
            }
        } else {
            /*m_Core->GetLogger()->Log
                (
                LL_Warning,
                VOODOO_CORE_NAME, 
                "Unable to read global annotation for parameter %s.",
                param->GetName().c_str()
                );*/
        }

        return VSFERR_LINKER_ERROR;
    }

    // If it's not linked to a global, it doesn't need linked unless it is
    // a sampler.
    if (  ToParameterCategory(m_Type) == PC_Sampler )
    {
        this->LinkSampler();
    }

    return VSF_OK;
}

STDMETHODIMP CVoodooParameter::LinkSampler()
{
    // Link to a texture
    CGannotation textureAnnotation = cgGetNamedParameterAnnotation(m_Parameter, "texture");

    if ( !cgIsAnnotation(textureAnnotation) || cgGetAnnotationType(textureAnnotation) != CG_STRING )
    {
        /*m_Core->GetLogger()->Log
        (
            LL_Warning,
            VOODOO_CORE_NAME,
            "Could not retrieve texture annotation for parameter %s.",
            param->GetName().c_str()
        );*/

        return VSFERR_LINKER_ERROR;
    }

    const char * textureName = cgGetStringAnnotationValue(textureAnnotation);
    if ( textureName == NULL || strlen(textureName) == 0 )
    {
        /*m_Core->GetLogger()->Log
            (
            LL_Warning,
            VOODOO_CORE_NAME,
            "Could not retrieve texture name for parameter %s.",
            param->GetName().c_str()
            );*/

        return VSFERR_LINKER_ERROR;
    }

    // Try to get the texture first, otherwise pass to adapter
    CComBSTR textureNameBSTR(textureName);
    IVoodooTexture * pTexture = NULL;
    m_Core->GetTexture(textureNameBSTR, &pTexture);

    if ( pTexture != NULL )
    {
        IVoodooAdapter * pAdapter = NULL;
        m_Core->get_Adapter(&pAdapter);

        if ( pAdapter != NULL )
        {
            return pAdapter->ConnectTexture(this, pTexture);
        } else {
            return VSFERR_NO_ADAPTER;
        }
    } else {
        /*m_Core->GetLogger()->Log
            (
            LL_Warning,
            VOODOO_CORE_NAME,
            "Could not find texture %s for parameter %s, attempting to load.",
            textureName, param->GetName().c_str()
            );*/

        return this->CreateSamplerTexture();
    }
}

STDMETHODIMP CVoodooParameter::CreateSamplerTexture()
{
    if ( !cgIsParameter(m_Parameter) )
    {
        /*m_Core->GetLogger()->Log
            (
            LL_Error,
            VOODOO_CORE_NAME,
            "Could not create parameter texture for unknown parameter."
            );*/

        return VSFERR_LINKER_ERROR;
    }

    CGannotation atexName = cgGetNamedParameterAnnotation(m_Parameter, "texture");
    if ( !cgIsAnnotation(atexName) || cgGetAnnotationType(atexName) != CG_STRING )
    {
        /*m_Core->GetLogger()->Log
            (
            LL_Error,
            VOODOO_CORE_NAME,
            "Invalid or missing texture name for parameter %s.",
            param->GetName().c_str()
            );*/

        return VSFERR_LINKER_ERROR;
    }

    CComBSTR texName;
    if ( cgGetAnnotationType(atexName) != CG_STRING )
    {
        /*m_Core->GetLogger()->Log
            (
            LL_Error,
            VOODOO_CORE_NAME,
            "Invalid texture name annotation type in %s.",
            param->GetName().c_str()
            );*/

        return VSFERR_LINKER_ERROR;
    }
        
    const char * texnameptr = cgGetStringAnnotationValue(atexName);
    if ( texnameptr == NULL || strlen(texnameptr) == 0 )
    {
        return VSFERR_LINKER_ERROR;
    }

    texName = texnameptr;

    IVoodooParser * pParser = NULL;
    m_Core->get_Parser(&pParser);

    texName = pParser->Parse(texName, PF_None, &texName);

    // Check for a valid texture file
    IVoodooFileSystem * pFS = NULL;
    m_Core->get_FileSystem(&pFS);
    IVoodooImage * pImage = NULL;
    pFS->FindImage(texName, &pImage);
    if ( pImage != NULL )
    {
        IVoodooAdapter * pAdapter = NULL;
        m_Core->get_Adapter(&pAdapter);
        IVoodooTexture * pTexture = NULL;
        TextureRegion region = { 0, 0, 0, VARIANT_FALSE, VARIANT_FALSE, TF_Unknown, 0, 0, 0 };
        pAdapter->LoadTexture(pImage, region, &pTexture);
        m_Texture = pTexture;
        return S_OK;
    }

    // No file, make blank texture
    CGannotation atexSize   = cgGetNamedParameterAnnotation(m_Parameter, "size");
    CGannotation atexFormat = cgGetNamedParameterAnnotation(m_Parameter, "format");
    CGannotation atexColor  = cgGetNamedParameterAnnotation(m_Parameter, "color");

    CGtype texSizeType = cgGetAnnotationType(atexSize);
    TextureDesc texDesc;

    if ( texSizeType ==  CG_INT1 )
    {
        /*m_Core->GetLogger()->Log
            (
            LL_Debug,
            VOODOO_CORE_NAME,
            "1-dimensional texture size found."
            );*/

        int outCount;
        const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
        texDesc.Width = texDims[0];
        texDesc.Height = texDesc.Depth = 1;
    } else if ( texSizeType == CG_INT2 ) {
        /*m_Core->GetLogger()->Log
            (
            LL_Debug,
            VOODOO_CORE_NAME,
            "2-dimensional texture size found."
            );*/

        int outCount;
        const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
        texDesc.Width = texDims[0];
        texDesc.Height = texDims[1];
        texDesc.Depth = 1;
    } else if ( texSizeType == CG_INT3 ) {
        /*m_Core->GetLogger()->Log
            (
            LL_Debug,
            VOODOO_CORE_NAME,
            "3-dimensional texture size found."
            );*/

        int outCount;
        const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
        texDesc.Width = texDims[0];
        texDesc.Height = texDims[1];
        texDesc.Depth = texDims[2];
    } else if ( texSizeType == CG_INT4 ) {
        /*m_Core->GetLogger()->Log
            (
            LL_Error,
            VOODOO_CORE_NAME,
            "4-dimensional texture size found. Creating quantum texture... (invalid type on %s).",
            param->GetName().c_str()
            ); */       
    } else {
        /*m_Core->GetLogger()->Log
            (
            LL_Error,
            VOODOO_CORE_NAME,
            "Invalid texture size annotation type for parameter %s.",
            param->GetName().c_str()
            );*/
    }

    texDesc.Mipmaps = VARIANT_FALSE;
    texDesc.RenderTarget = VARIANT_TRUE;

    if ( cgGetAnnotationType(atexFormat) != CG_STRING )
    {
        /*m_Core->GetLogger()->Log
            (
            LL_Error,
            VOODOO_CORE_NAME,
            "Invalid texture format annotation type in %s.",
            param->GetName().c_str()
            );*/

        return VSFERR_LINKER_ERROR;
    } else {
        const char * formatString = cgGetStringAnnotationValue(atexFormat);
        CComBSTR formatBSTR(formatString);
        texDesc.Format = ToTextureFormat(formatBSTR);
    }

    IVoodooAdapter * pAdapter = NULL;
    m_Core->get_Adapter(&pAdapter);
    IVoodooTexture * pTexture = NULL;
    pAdapter->CreateTexture(texName, texDesc, &m_Texture);
    if ( m_Texture == NULL )
    {
        /*m_Core->GetLogger()->Log
            (
            LL_Error,
            VOODOO_CORE_NAME,
            "Could not create parameter texture for unknown parameter."
            );*/

        return VSFERR_LINKER_ERROR;
    } else {
        return VSF_OK;
    }
}