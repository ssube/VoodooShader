#include "Scalar.hpp"

#include "Converter.hpp"

namespace VoodooShader
{
    Scalar::Scalar()
        : m_Refrs(0), m_Shader(NULL), m_Core(NULL), m_Virtual(FALSE), m_Parameter(NULL), m_Type(PT_Unknown)
    { }

    Scalar * Scalar::Create(_In_ IVoodooCore * pCore, _In_ BSTR pName, _In_ ParameterType Type)
    {
        if ( pCore == NULL ) return NULL;

        Scalar * scalar = new Scalar();
        scalar->m_Core = pCore;
        scalar->m_Shader = NULL;
        scalar->m_Name = pName;
        scalar->m_Type = Type;
        scalar->m_Value.Create(16);
        scalar->m_Parameter = CreateVirtualParameter(pCore, Type);
    }

    Scalar * Scalar::Create(_In_ IVoodooShader * pShader, CGparameter pParameter)
    {
        if ( pShader == NULL ) return NULL;

        Scalar * scalar = new Scalar();
        pShader->GetCore(&scalar->m_Core);
        scalar->m_Parameter = pParameter;
        scalar->m_Value.Create(16);

        const char * name = cgGetParameterName(pParameter);
        if ( name == NULL )
        {
            CStringW mname;
            mname.Format(L"parameter_%p", pParameter);
            scalar->m_Name = mname;
        } else {
            scalar->m_Name = name;
        }

        CGtype cgtype = cgGetParameterType(pParameter);
        scalar->m_Type = Converter::ToParameterType(cgtype);

        return scalar;
    }

    HRESULT Scalar::QueryInterface(REFIID iid, void ** pp)
    {
        if ( pp == NULL )
        {
            return E_POINTER;
        } else if ( iid == IID_IUnknown || iid == IID_VoodooObject || iid == IID_VoodooParameter || iid == IID_VoodooScalar ) {
            *pp = this;
            return S_OK;
        } else {
            *pp = NULL;
            return E_NOINTERFACE;
        }
    }

    ULONG Scalar::AddRef()
    {
        return (++m_Refrs);
    }

    ULONG Scalar::Release()
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

    HRESULT Scalar::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        }

        return m_Name.CopyTo(pName);
    }

    HRESULT Scalar::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        }

        *ppCore = m_Core;
        return S_OK;
    }

    BOOL Scalar::get_IsVirtual()
    {
        return m_Virtual;
    }

    HRESULT Scalar::GetShader(IVoodooShader ** ppShader)
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

    HRESULT Scalar::GetType(ParameterType * pType)
    {
        if ( pType == NULL ) return E_INVALIDARG;

        *pType = m_Type;
        return S_OK;
    }

    HRESULT Scalar::AttachParameter(IVoodooScalar * pParameter)
    {
        if ( m_Virtual == FALSE ) return E_NOTVIRTUAL;
        if ( pParameter == NULL ) return E_INVALIDARG;

        CGparameter other = NULL;
        pParameter->GetCgParameter((void**)&other);

        cgConnectParameter(m_Parameter, other);

        return S_OK;
    }

    HRESULT Scalar::GetCgParameter(void ** ppCgParameter)
    {
        if ( ppCgParameter == NULL ) return E_INVALIDARG;

        *ppCgParameter = m_Parameter;
        return S_OK;
    }

    HRESULT Scalar::put_Value(SAFEARRAY * pData)
    {
        if ( pData == NULL ) return E_INVALIDARG;

        m_Value.CopyFrom(pData);
        return S_OK;
    }

    HRESULT Scalar::get_Value(SAFEARRAY ** ppData)
    {
        if ( ppData == NULL ) return E_INVALIDARG;

        m_Value.CopyTo(ppData);
        return S_OK;
    }

    UINT Scalar::get_Components()
    {
        if ( m_Type == PT_Float1 ) return 1;
        if ( m_Type == PT_Float2 ) return 2;
        if ( m_Type == PT_Float3 ) return 3;
        if ( m_Type == PT_Float4 ) return 4;
        if ( m_Type == PT_Matrix ) return 16;
        return 0;
    }
}