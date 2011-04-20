#include "Technique.hpp"

namespace VoodooShader
{
    Technique::Technique()
        : m_Refrs(0), m_Core(NULL), m_Shader(NULL), m_Target(NULL), m_Technique(NULL)
    { }

    Technique::~Technique()
    {
        if ( m_Target != NULL )
        {
            m_Target->Release();
            m_Target = NULL;
        }
        m_Passes.RemoveAll();
    }

    Technique * Technique::Create(Shader * pShader, CGtechnique pTechnique)
    {
        if ( pShader == NULL || pTechnique == NULL ) return NULL;

        Technique * tech = new Technique();
        tech->m_Shader = pShader;
        tech->m_Technique = pTechnique;
        m_Shader->GetCore(&tech->m_Core);

        const char * techName = cgGetTechniqueName(pTechnique);
        if ( techName )
        {
            tech->m_Name = techName;
        } else {
            CStringW name;
            name.Format(L"tech_%p", pTechnique);
            tech->m_Name = name;
        }

        return tech;
    }

    HRESULT Technique::QueryInterface(REFIID iid, void ** pp)
    {
        if ( pp == NULL )
        {
            return E_POINTER;
        } else if ( iid == IID_IUnknown || iid == IID_VoodooObject || iid == IID_VoodooTechnique ) {
            *pp = this;
            return S_OK;
        } else {
            *pp = NULL;
            return E_NOINTERFACE;
        }
    }

    ULONG Technique::AddRef()
    {
        return (++m_Refrs);
    }

    ULONG Technique::Release()
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

    HRESULT Technique::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        } else {
            return m_Name.CopyTo(pName);
        }
    }

    HRESULT Technique::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppCore = m_Core;
            return S_OK;
        }
    }

    HRESULT Technique::GetTarget(IVoodooTexture ** ppTexture)
    {
        if ( ppTexture == NULL ) return E_INVALIDARG;

        *ppTexture = m_Target;
        return S_OK;
    }

    UINT Technique::get_PassCount()
    {
        return (UINT)m_Passes.GetSize();
    }

    HRESULT Technique::GetPass(UINT Number, IVoodooPass ** ppPass)
    {
        if ( ppPass == NULL ) return E_INVALIDARG;
        if ( Number > m_Passes.GetSize() ) return E_INVALIDARG;

        *ppPass = m_Passes.GetAt(Number);
        return S_OK;
    }

    HRESULT Technique::GetCgTechnique(void ** ppTechnique)
    {
        if ( ppTechnique == NULL ) return E_INVALIDARG;

        *ppTechnique = m_Technique;
        return S_OK;
    }

    HRESULT Technique::Link()
    {
        this->m_Target = NULL;

        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(m_Technique, "target");

        if ( cgIsAnnotation(targetAnnotation) && cgGetAnnotationType(targetAnnotation) == CG_STRING )
        {
            const char * targetName = cgGetStringAnnotationValue(targetAnnotation);
            CComBSTR targetbstr(targetName);

            m_Core->GetTexture(targetbstr, &m_Target);

            if ( m_Target == NULL )
            {
                /*mCore->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Pass %s cannot find target %s.",
                    this->GetName().c_str(), targetName
                );*/

                m_Core->GetTexture(TT_ShaderTarget, m_Target);
            }
        } else {
            /*mCore->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.",
                this->GetName().c_str()
            );*/

            m_Core->GetTexture(TT_ShaderTarget, m_Target);
        }


        this->m_Passes.RemoveAll();

        CGpass cPass = cgGetFirstPass(m_Technique);

        while ( cgIsPass(cPass) )
        {
            // Insert the pass into the vector
            IVoodooPass * passi = NULL;

            Pass * pass = new Pass(this, cPass);
            pass->Link();

            pass->QueryInterface(IID_VoodooPass, &passi);
            m_Passes.Add(passi);

            cPass = cgGetNextPass(cPass);
        }
    }
}
