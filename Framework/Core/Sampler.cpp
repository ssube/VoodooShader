#include "Sampler.hpp"

namespace VoodooShader
{
    Sampler::Sampler( _In_ IVoodooCore * pCore, _In_ BSTR pName, _In_ ParameterType Type )
        : m_Core(pCore), m_Shader(NULL), m_Name(pName), m_Type(Type), m_Refrs(0)
    {

    }

    Sampler::Sampler( _In_ IVoodooShader * pShader, CGparameter pParameter)
        : m_Shader(pShader), m_Parameter(pParameter), m_Refrs(0)
    {
        m_Shader->GetCore(&m_Core);
        const char * name = cgGetParameterName(m_Parameter);
        if ( name == NULL )
        {
            CStringW mname;
            mname.Format("parameter_%p", m_Parameter);
            m_Name = mname;
        } else {
            m_Name = name;
        }
    }

    HRESULT Sampler::QueryInterface(REFIID iid, void ** pp) throw()
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

    UINT Sampler::get_IsVirtual()
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

    HRESULT Sampler::GetParameterType(ParameterType * pType)
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
        pParameter->GetCgParameter(&other);

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