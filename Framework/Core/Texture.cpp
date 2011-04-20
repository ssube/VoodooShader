#include "Texture.hpp"

namespace VoodooShader
{
    Texture::Texture()
        : m_Refrs(0), m_Data(NULL)
    { }

    static Texture::Texture * Create(_In_ BSTR pName, _In_opt_ void * pData)
    {
        if ( pData == NULL ) return NULL;

        Texture * texture = new Texture();
        texture->m_Name.Copy(pName);
        texture->m_Data = pData;

        return texture;
    }

    // IUnknown
    HRESULT Texture::QueryInterface(REFIID riid, void ** ppvObj)
    {
        if ( ppvObj == NULL )
        {
            return E_INVALIDARG;
        } else if ( riid == IID_IUnknown || riid == IID_VoodooObject || riid == IID_VoodooTexture ) {
            *ppvObj = this;
            return S_OK;
        } else {
            *ppvObj = NULL;
            return E_NOINTERFACE;
        }
    }

    ULONG Texture::AddRef()
    {
        return (++m_Refrs);
    }

    ULONG Texture::Release()
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

    HRESULT CoTexturere::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        }

        return m_Name.CopyTo(pName);
    }

    HRESULT Texture::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        }

        *ppCore = m_Core;
        return S_OK;
    }

    // IVoodooTexture
    HRESULT Texture::GetData(void ** ppData)
    {
        if ( ppData == NULL ) return E_INVALIDARG;

        *ppData = m_Data;
        return S_OK;
    }
}
