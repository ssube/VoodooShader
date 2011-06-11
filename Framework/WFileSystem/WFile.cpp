// WFile.cpp : Implementation of CWFile

#include "stdafx.h"
#include "WFile.h"

// CWFile
IVoodooFile * CWFile::Create(CStringW Filename)
{
    if ( Filename.GetLength() == 0 ) return NULL;

    CComPtr<IVoodooFile> ipFile = NULL;

    CComObject<CWFile> * pFile = NULL;
    HRESULT hr = CComObject<CWFile>::CreateInstance(&pFile);
    if ( SUCCEEDED(hr) )
    {
        pFile->AddRef();

        pFile->m_Path = Filename;

        hr = pFile->QueryInterface(IID_IVoodooFile, (void**)&ipFile);
        pFile->Release();
    }

    return ipFile.Detach();
}

CWFile::CWFile()
    : m_Refrs(0)
{

}

CWFile::~CWFile()
{
    if ( m_File.m_hFile != CFile::hFileNull )
    {
        m_File.Close();
    }
}

STDMETHODIMP CWFile::get_Path
( 
    /* [retval][out] */ LPBSTR pPath
)
{
    if ( pPath == NULL ) return VSFERR_INVALID_ARG;

    return this->m_Path.CopyTo(pPath);
}

STDMETHODIMP CWFile::Open
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

STDMETHODIMP CWFile::Close(void)
{
    this->m_File.Close();

    return VSF_OK;
}

STDMETHODIMP CWFile::Read
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
        return VSF_FAIL;
    } else {
        return VSF_OK;
    }
}

STDMETHODIMP CWFile::Write
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