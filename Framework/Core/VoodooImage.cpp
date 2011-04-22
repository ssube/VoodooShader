// VoodooImage.cpp : Implementation of CVoodooImage

#include "stdafx.h"
#include "VoodooImage.h"

#include "Common.hpp"


// CVoodooImage
CVoodooImage::CVoodooImage()
{
    m_Refrs = 0;
    m_Core = NULL;
}

CVoodooImage::~CVoodooImage()
{
    m_Refrs = 0;
    m_Core = NULL;
}

IVoodooImage * CVoodooImage::Create(IVoodooCore * pCore, BSTR pPath)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooImage> ipImage = NULL;

    CComObject<CVoodooImage> * pImage = NULL;
    HRESULT hr = CComObject<CVoodooImage>::CreateInstance(&pImage);
    if ( SUCCEEDED(hr) )
    {
        pImage->AddRef();
        pImage->m_Core = pCore;
        pImage->m_Name = pPath;
        pImage->m_Path = pPath;
        hr = pImage->QueryInterface(IID_IVoodooTechnique, (void**)&ipImage);
        pImage->Release();
    }

    return ipImage.Detach();
}

STDMETHODIMP CVoodooImage::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooFile || iid == IID_IVoodooImage ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooImage::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooImage::Release()
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

STDMETHODIMP CVoodooImage::get_Name(LPBSTR pName)
{
    if ( pName == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Name.CopyTo(pName);
    }
}

STDMETHODIMP CVoodooImage::get_Core(IVoodooCore **ppCore)
{
    if ( ppCore == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppCore = m_Core;
        return S_OK;
    }
}

STDMETHODIMP CVoodooImage::get_Desc(TextureDesc *pDesc)
{
    if ( pDesc == NULL )
    {
        return E_INVALIDARG;
    } else {
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooImage::get_Path(LPBSTR pPath)
{
    if ( pPath == NULL )
    {
        return E_INVALIDARG;
    } else {
        return m_Path.CopyTo(pPath);
    }
}

STDMETHODIMP CVoodooImage::Open(FileOpenMode Mode)
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooImage::Close()
{
    return E_NULLIMPL;
}

STDMETHODIMP CVoodooImage::Read(int *Length, byte Buffer[ ])
{
    if ( Length == NULL || Buffer == NULL )
    {
        return E_INVALIDARG;
    } else {
        *Length = 0;
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooImage::Write(int Length, byte Buffer[ ])
{
    if ( Buffer == NULL )
    {
        return E_INVALIDARG;
    } else {
        return E_NULLIMPL;
    }
}

STDMETHODIMP CVoodooImage::get_Data(TextureRegion Region, int *pSize, byte ppBuffer[ ])
{
    if ( pSize == NULL )
    {
        return E_INVALIDARG;
    } else {
        return E_NULLIMPL;
    }
}