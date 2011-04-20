#include "Pass.hpp"

namespace VoodooShader
{
    Pass::Pass()
        : m_Refrs(0), m_Core(NULL), m_Shader(NULL), m_Technique(NULL), m_Target(NULL),
          m_Pass(NULL), m_VertexProgram(NULL), m_FragmentProgram(NULL), m_GeometryProgram(NULL),
          m_DomainProgram(NULL), m_HullProgram(NULL)
    { }

    Pass::~Pass()
    {
        m_Target->Release();

        //mCore->GetAdapter()->UnloadPass(this);
    }

    Pass * Pass::Create(_In_ IVoodooTechnique * pTechnique, _In_ CGpass pPass)
    {
        if ( pTechnique == NULL || pPass == NULL || !cgIsPass(pPass) ) return NULL;
        
        Pass * pass = new Pass();
        pTechnique->GetCore(&pass->m_Core);
        pTechnique->GetShader(&pass->m_Shader);
        pass->m_Technique = pTechnique;
        pass->m_Pass = pPass;

        const char * passName = cgGetPassName(pPass);
        if ( passName )
        {
            pTechnique->GetName(&pass->m_Name);
            pass->m_Name += passName;
        } else {
            CStringW mname;
            mname.Format(L"pass_%p", pPass);
            pass->m_Name = mname;
        }

        return pass;
    }

    HRESULT Pass::QueryInterface(REFIID iid, void ** pp)
    {
        if ( pp == NULL )
        {
            return E_POINTER;
        } else if ( iid == IID_IUnknown || iid == IID_VoodooObject || iid == IID_VoodooPass ) {
            *pp = this;
            return S_OK;
        } else {
            *pp = NULL;
            return E_NOINTERFACE;
        }
    }

    ULONG Pass::AddRef()
    {
        return (++m_Refrs);
    }

    ULONG Pass::Release()
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

    HRESULT Pass::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        } else {
            return m_Name.CopyTo(pName);
        }
    }

    HRESULT Pass::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppCore = m_Core;
            return S_OK;
        }
    }

    HRESULT Pass::GetTarget(IVoodooTexture ** ppTexture)
    {
        if ( ppTexture == NULL ) return E_INVALIDARG;

        *ppTexture = m_Target;
        return S_OK;
    }

    HRESULT Pass::GetCgProgram(ProgramStage stage, void ** ppProgram)
    {
        if ( ppProgram == NULL ) return E_INVALIDARG;
        
        switch ( stage )
        {
        case PS_Vertex:
            *ppProgram = m_VertexProgram;
            return S_OK;
        case PS_Fragment:
            *ppProgram = m_FragmentProgram;
            return S_OK;
        case PS_Geometry:
            *ppProgram = m_GeometryProgram;
            return S_OK;
        case PS_Domain:
            *ppProgram = m_DomainProgram;
            return S_OK;
        case PS_Hull:
            *ppProgram = m_HullProgram;
            return S_OK;
        case PS_Unknown:
        default:
            return E_INVALIDARG;
        }
    }

    HRESULT Pass::GetCgPass(void ** ppPass)
    {
        if ( ppPass == NULL ) return E_INVALIDARG;

        *ppPass = m_Pass;
        return S_OK;
    }

    HRESULT Pass::GetShader(IVoodooShader ** ppShader)
    {
        if ( ppShader == NULL ) return E_INVALIDARG;

        *ppShader = m_Shader;
        return S_OK;
    }

    HRESULT Pass::GetTechnique(IVoodooTechnique ** ppTechnique)
    {
        if ( ppTechnique ) return E_INVALIDARG;

        *ppTechnique = m_Technique;
        return S_OK;
    }

    void Pass::Link()
    {
        m_VertexProgram   = cgGetPassProgram(m_Pass, CG_VERTEX_DOMAIN  );
        m_FragmentProgram = cgGetPassProgram(m_Pass, CG_FRAGMENT_DOMAIN);
        m_GeometryProgram = cgGetPassProgram(m_Pass, CG_GEOMETRY_DOMAIN);
        m_DomainProgram   = cgGetPassProgram(m_Pass, CG_TESSELLATION_CONTROL_DOMAIN);
        m_HullProgram     = cgGetPassProgram(m_Pass, CG_TESSELLATION_EVALUATION_DOMAIN);

        m_Target = NULL;

        CGannotation targetAnnotation = cgGetNamedPassAnnotation(m_Pass, "target");
        if ( cgIsAnnotation(targetAnnotation) && cgGetAnnotationType(targetAnnotation) == CG_STRING )
        {
            const char * targetName = cgGetStringAnnotationValue(targetAnnotation);
            CComBSTR targetbstr(targetName);

            m_Core->GetTexture(targetbstr, &m_Target);

            if ( m_Target == NULL )
            {
                m_Core->get_StageTexture(TT_PassTarget, &m_Target);
            }
        } else {
            /*mCore->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.", 
                this->GetName().c_str()
            );*/

            m_Core->get_StageTexture(TT_PassTarget, &m_Target);
        }

        // Load the programs
        IVoodooAdapter * adapter = NULL;
        m_Core->get_Adapter(&adapter);

        if ( adapter == NULL )
        {
            /*mCore->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "No adapter found, pass %s must be explicitly loaded later.", 
                this->GetName().c_str()
            )*/;
        } else {
            if ( FAILED(adapter->LoadPass(this)) )
            {
                //mCore->GetLogger()->Log(LL_Error, VOODOO_CORE_NAME, "Failed to load pass %s.", this->GetName().c_str());
            } else {
                //mCore->GetLogger()->Log(LL_Info, VOODOO_CORE_NAME, "Successfully loaded pass %s.", this->GetName().c_str());
            }
        }
    }
}