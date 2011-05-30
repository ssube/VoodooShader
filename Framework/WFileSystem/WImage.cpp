// WImage.cpp : Implementation of CWImage

#include "stdafx.h"
#include "WImage.h"

// CWImage
IVoodooFile * CWImage::Create(CStringW Filename)
{
    if ( Filename.GetLength() == 0 ) return NULL;

    ILuint ilImage = ilGenImage();
    ilBindImage(ilImage);

    ILboolean isImage = ilLoadImage(Filename);

    ilBindImage(0);

    if ( loaded == IL_FALSE )
    {
        ilDeleteImage(ilImage);
        return NULL;
    }

    CComPtr<IVoodooFile> ipFile = NULL;

    CComObject<CWImage> * pFile = NULL;
    HRESULT hr = CComObject<CWImage>::CreateInstance(&pFile);
    if ( SUCCEEDED(hr) )
    {
        pFile->AddRef();

        pFile->m_Path = Filename;
        pFile->m_Image = ilImage;

        hr = pFile->QueryInterface(IID_IVoodooFile, (void**)&ipFile);
        pFile->Release();
    }

    return ipFile.Detach();
}

CWImage::CWImage()
    : m_Refrs(0), m_Image(0)
{

}

CWImage::~CWImage()
{
    if ( m_File.m_hFile != CFile::hFileNull )
    {
        m_File.Close();
    }
}

STDMETHODIMP CWImage::get_Path
    ( 
    /* [retval][out] */ LPBSTR pPath
    )
{
    if ( pPath == NULL ) return VSFERR_INVALID_ARG;

    return this->m_Path.CopyTo(pPath);
}

STDMETHODIMP CWImage::Open
    ( 
    /* [in] */ DWORD Mode
    )
{
    UINT flags = 0;
    if ( Mode == FM_Read )
    {
        flags = CFile::modeRead;
    } else if ( Mode == FM_Write ) {
        flags = CFile::modeWrite;
    } else if ( Mode == FM_ReadWrite ) {
        flags = CFile::modeReadWrite;
    } else {
        return VSFERR_INVALID_ARG;
    }

    BOOL success = this->m_File.Open(this->m_Path, flags);
    if ( success == TRUE )
    {
        return VSF_OK;
    } else {
        return VSFERR_FILE_ERROR;
    }
}

STDMETHODIMP CWImage::Close(void)
{
    this->m_File.Close();
}

STDMETHODIMP CWImage::Read
    ( 
    /* [out][in] */ int *Length,
    /* [size_is][out] */ byte Buffer[  ]
)
{
    if ( Length == NULL || Buffer == NULL ) return VSFERR_INVALID_ARG;

    UINT tryLen = (*Length > 0) ? *Length : 0;
    UINT gotLen = this->m_File.Read(Buffer, tryLen);

    if ( gotLen != tryLen )
    {
        *Length = gotLen;
        return VSFERR_READ_LEN;
    } else {
        return VSF_OK;
    }
}

STDMETHODIMP CWImage::Write
    ( 
    /* [in] */ int Length,
    /* [size_is][in] */ byte Buffer[  ]
)
{
    if ( Length < 0 || Buffer == NULL ) return VSFERR_INVALID_ARG;

    UINT tryLen = (UINT)Length;
    this->m_File.Write(Buffer, tryLen);

    return VSF_OK;
}

STDMETHODIMP CWImage::get_Desc
( 
    /* [retval][out] */ TextureDesc *pDesc
)
{
    if ( pDesc == NULL ) return VSFERR_INVALID_ARG;
    if ( m_Image == 0 ) return VSFERR_NO_IMAGE;

    ZeroMemory(pDesc, sizeof(*pDesc));

    ilBindImage(m_Image);

    pDesc->Width = ilGetInteger(IL_IMAGE_WIDTH);
    pDesc->Height = ilGetInteger(IL_IMAGE_HEIGHT);
    pDesc->Depth = ilGetInteger(IL_IMAGE_DEPTH);
    pDesc->Mipmaps = VARIANT_FALSE;
    pDesc->Format = TF_Unknown;

    ILint texFmt  = ilGetInteger(IL_IMAGE_FORMAT);
    ILint texType = ilGetInteger(IL_IMAGE_TYPE);

    if ( texFmt == IL_RGB )
    {
        if ( texType == IL_BYTE || texType == IL_UNSIGNED_BYTE )
        {
            pDesc->Format = TF_RGB8;
        }
    } else if ( texFmt == IL_RGBA ) {
        if ( texType == IL_BYTE || texType == IL_UNSIGNED_BYTE )
        {
            pDesc->Format = TF_RGBA8;
        } else if ( texType == IL_FLOAT ) {
            pDesc->Format = TF_RGBA16F;
        } else if ( texType == IL_DOUBLE ) {
            pDesc->Format = TF_RGBA32F;
        }
    }

    ilBindImage(0);

    return VSF_OK;
}

STDMETHODIMP CWImage::get_Data
( 
    /* [in] */ TextureRegion Region,
    /* [out][in] */ int *pSize,
    /* [size_is][retval][out] */ byte ppBuffer[  ]
)
{
    ILint ilFmt = 0;
    ILint ilType = 0;

    int pixelSize = 0;

    // Convert TextureFormat to DevIL format
    switch ( Region.Format )
    {
    case TF_RGB8:
        pixelSize = 3;
        ilFmt = IL_RGB;
        ilType = IL_UNSIGNED_BYTE;
        break;
    case TF_RGBA8:
        pixelSize = 4;
        ilFmt = IL_RGBA;
        ilType = IL_UNSIGNED_BYTE;
        break;
    case TF_RGBA16F:
        pixelSize = 8;
        ilFmt = IL_RGBA;
        ilType = IL_FLOAT;
        break;
    case TF_RGBA32F:
        pixelSize = 16;
        ilFmt = IL_RGBA;
        ilType = IL_DOUBLE;
        break;
    case TF_Unknown:
    default:
        return VSFERR_INVALID_ARG;
    }

    // Verify buffer size
    int reqSize = pixelSize * Region.Width * Region.Height * Region.Depth;
    if ( reqSize > *pSize )
    {
        return VSFERR_BUFFER_ERROR;
    }

    ilBindImage(m_Image);

    ilCopyPixels
    (
        Region.OffX, Region.OffY, Region.OffZ, 
        Region.Width, Region.Height, Region.Depth, 
        ilFmt, ilType, ppBuffer
    );

    ilBindImage(0);
}