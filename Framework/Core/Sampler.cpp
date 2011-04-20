#include "Sampler.hpp"

#include "Converter.hpp"

namespace VoodooShader
{
    Sampler::Sampler()
        : m_Refrs(0), m_Core(NULL), m_Shader(NULL), m_Virtual(FALSE), m_Parameter(NULL), 
          m_Texture(NULL), m_Type(PT_Unknown)
    { }

    Sampler * Sampler::Create(_In_ IVoodooCore * pCore, _In_ BSTR pName, _In_ ParameterType Type)
    {
        if ( pCore == NULL ) return NULL;

        Sampler * sampler = new Sampler();
        sampler->m_Core = pCore;
        sampler->m_Virtual = TRUE;
        sampler->m_Name = pName;
        sampler->m_Type = Type;
        sampler->m_Parameter = CreateVirtualParameter(pCore, Type);
        
        return sampler;
    }

    Sampler * Sampler::Create(_In_ IVoodooShader * pShader, CGparameter pParameter)
    {
        if ( pShader == NULL ) return NULL;

        Sampler * sampler = new Sampler();
        pShader->GetCore(&sampler->m_Core);
        sampler->m_Shader = pShader;
        sampler->m_Virtual = FALSE;
        sampler->m_Parameter = pParameter;

        CGtype cgtype = cgGetParameterType(pParameter);
        sampler->m_Type = Converter::ToParameterType(cgtype);

        const char * name = cgGetParameterName(pParameter);
        if ( name == NULL )
        {
            CStringW mname;
            mname.Format(L"parameter_%p", pParameter);
            sampler->m_Name = mname;
        } else {
            sampler->m_Name = name;
        }

        return sampler;
    }

    HRESULT Sampler::QueryInterface(REFIID iid, void ** pp)
    {
        if ( pp == NULL )
        {
            return E_POINTER;
        } else if ( iid == IID_IUnknown || iid == IID_VoodooObject || iid == IID_VoodooCore ) {
            *pp = this;
            return S_OK;
        } else {
            *pp = NULL;
            return E_NOINTERFACE;
        }
    }

    ULONG Sampler::AddRef()
    {
        return (++m_Refrs);
    }

    ULONG Sampler::Release()
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

    HRESULT Sampler::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        }

        return m_Name.CopyTo(pName);
    }

    HRESULT Sampler::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        }

        *ppCore = m_Core;
        return S_OK;
    }

    BOOL Sampler::get_IsVirtual()
    {
        return m_Virtual;
    }

    HRESULT Sampler::GetShader(IVoodooShader ** ppShader)
    {
        if ( m_Virtual == TRUE )
        {
            return E_VIRTUAL;
        } else if ( ppShader == NULL ) {
            return E_INVALIDARG;
        }

        *ppShader = m_Shader;
        return S_OK;
    }

    HRESULT Sampler::GetType(ParameterType * pType)
    {
        if ( pType == NULL ) return E_INVALIDARG;

        *pType = m_Type;
        return S_OK;
    }

    HRESULT Sampler::AttachParameter(IVoodooSampler * pParameter)
    {
        if ( m_Virtual == FALSE ) return E_NOTVIRTUAL;
        if ( pParameter == NULL ) return E_INVALIDARG;

        CGparameter other = NULL;
        pParameter->GetCgParameter((void**)&other);

        cgConnectParameter(m_Parameter, other);

        return S_OK;
    }

    HRESULT Sampler::GetCgParameter(void ** ppCgParameter)
    {
        if ( ppCgParameter == NULL ) return E_INVALIDARG;

        *ppCgParameter = m_Parameter;
        return S_OK;
    }

    HRESULT Sampler::put_Texture(IVoodooTexture * pTexture)
    {
        //! @todo Add texture dimension checking
        m_Texture = pTexture;
        return S_OK;
    }

    HRESULT Sampler::get_Texture(IVoodooTexture ** ppTexture)
    {
        if ( ppTexture == NULL ) return E_INVALIDARG;

        *ppTexture = m_Texture;
        return S_OK;
    }

    UINT Sampler::get_Dimensions()
    {
        if ( m_Type == PT_Sampler1D ) return 1;
        if ( m_Type == PT_Sampler2D ) return 2;
        if ( m_Type == PT_Sampler3D ) return 3;
        return 0;
    }
}