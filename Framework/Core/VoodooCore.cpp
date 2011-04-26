// VoodooCore.cpp : Implementation of CVoodooCore

#include "stdafx.h"
#include "VoodooCore.h"



#include "VoodooParameter.h"
#include "VoodooParser.h"
#include "VoodooTexture.h"

// CVoodooCore
CVoodooCore::CVoodooCore()
{
    m_Init = false;
    m_Refrs = 0;
}

CVoodooCore::~CVoodooCore()
{

}

STDMETHODIMP CVoodooCore::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooCore ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooCore::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooCore::Release()
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

STDMETHODIMP CVoodooCore::Initialize(const InitParams Params)
{
    if ( Params.GlobalRoot ) { m_GlobalRoot.AssignBSTR(Params.GlobalRoot); }
    if ( Params.LocalRoot  ) { m_LocalRoot.AssignBSTR (Params.LocalRoot ); }
    if ( Params.RunRoot    ) { m_RunRoot.AssignBSTR   (Params.RunRoot   ); }
    if ( Params.Target     ) { m_Target.AssignBSTR    (Params.Target    ); }
    if ( Params.Loader     ) { m_Loader.AssignBSTR    (Params.Loader    ); }
    if ( Params.Config     ) { m_ConfigFile.AssignBSTR(Params.Config    ); }

    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_Config));
    if ( FAILED(hr) )
    {
        return E_INVALIDCFG;
    }

    m_Config->put_async(VARIANT_FALSE);
    m_Config->put_validateOnParse(VARIANT_FALSE);
    m_Config->put_resolveExternals(VARIANT_FALSE);
    m_Config->put_preserveWhiteSpace(VARIANT_TRUE);

    CComBSTR configPath = m_ConfigFile;

    if ( FAILED(LoadConfig(configPath)) )
    {
        configPath = m_RunRoot;
        configPath += m_ConfigFile;
        if ( FAILED(LoadConfig(configPath)) )
        {
            configPath = m_LocalRoot;
            configPath += m_ConfigFile;

            if ( FAILED(LoadConfig(configPath)) )
            {
                configPath = m_GlobalRoot;
                configPath += m_ConfigFile;

                if ( FAILED(LoadConfig(configPath)) )
                {
                    IXMLDOMParseError * pError = NULL;
                    m_Config->get_parseError(&pError);
                    CComBSTR fullError = L"XML Error:\n";
                    CComBSTR temp;
                    pError->get_reason(&temp);
                    fullError += temp;
                    fullError += L"\n";
                    // Handle further
                    return E_INVALIDCFG;
                }
            }
        }
    }

    // Start setting things up
    IXMLDOMNode * pCoreNode = NULL;
    CComBSTR coreNodeStr = L"/VoodooConfig/Core";
    hr = m_Config->selectSingleNode(coreNodeStr, &pCoreNode);

    if ( FAILED(hr) || pCoreNode == NULL )
    {
        return E_INVALIDCFG;
    }

    // Create query for node text, used multiple times
    CComBSTR queryNodeText = L"./text()";
    CComBSTR queryNodeName = L"./@name";

    // Set up the internal objects
    m_Parser = CVoodooParser::Create(this);
    if ( m_Parser == NULL )
    {
        return E_BADTHING;
    }

    // Load variables, built-in first
    m_Parser->AddVariable(L"globalroot",    m_GlobalRoot, VT_System);
    m_Parser->AddVariable(L"localroot",     m_LocalRoot,  VT_System);
    m_Parser->AddVariable(L"runroot",       m_RunRoot,    VT_System);
    m_Parser->AddVariable(L"target",        m_Target,     VT_System);
    m_Parser->AddVariable(L"loader",        m_Loader,     VT_System);
    m_Parser->AddVariable(L"config",        m_ConfigFile, VT_System);

    // Load config vars
    CComBSTR queryVarNodes = L"/VoodooConfig/Variables/Variable";

    IXMLDOMNodeList * pVarList = NULL;
    hr = m_Config->selectNodes(queryVarNodes, &pVarList);
    if ( SUCCEEDED(hr) )
    {
        IXMLDOMNode * pVarNode = NULL;
        while ( ( hr = pVarList->nextNode(&pVarNode) ) == S_OK && pVarNode != NULL )
        {
            IXMLDOMNode * pNameNode, * pTextNode;
            pVarNode->selectSingleNode(queryNodeName, &pNameNode);
            pVarNode->selectSingleNode(queryNodeText, &pTextNode);

            VARIANT name, text;
            pNameNode->get_nodeValue(&name);
            pTextNode->get_nodeValue(&text);

            m_Parser->AddVariable(name.bstrVal, text.bstrVal);
        }
    }

    // Lookup classes
    {
        CComBSTR query = L"./Logger/text()";
        IXMLDOMNode * pNode = NULL;
        if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
        {
            VARIANT v;
            pNode->get_nodeValue(&v);
            CComBSTR str(v.bstrVal);
            m_Parser->Parse(str, PF_None, &str);

            hr = InstanceFromString(str, IID_IVoodooLogger, (void**)&m_Logger);
            if ( FAILED(hr) || m_Logger == NULL )
            {
                return E_BADCLSID;
            } else {
                CComBSTR queryLF = L"./LogFile/text()";
                IXMLDOMNode * pFileNode = NULL;
                if ( SUCCEEDED(pCoreNode->selectSingleNode(queryLF, &pFileNode)) )
                {
                    VARIANT fn;
                    pFileNode->get_nodeValue(&fn);
                    CComBSTR filename(fn.bstrVal);
                    m_Parser->Parse(filename, PF_None, &filename);
                    m_Logger->Open(filename, false);
                }
            }
        }
    }

    {
        CComBSTR query = L"./HookSystem/text()";
        IXMLDOMNode * pNode;
        if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
        {
            VARIANT v;
            pNode->get_nodeValue(&v);
            CComBSTR str(v.bstrVal);
            m_Parser->Parse(str, PF_None, &str);

            hr = InstanceFromString(str, IID_IVoodooHookSystem, (void**)&m_HookSystem);
            if ( FAILED(hr) || m_FileSystem == NULL )
            {
                return E_BADCLSID;
            }
        }
    }

    {
        CComBSTR query = L"./FileSystem/text()";
        IXMLDOMNode * pNode;
        if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
        {
            VARIANT v;
            pNode->get_nodeValue(&v);
            CComBSTR str(v.bstrVal);
            m_Parser->Parse(str, PF_None, &str);

            hr = InstanceFromString(str, IID_IVoodooFileSystem, (void**)&m_FileSystem);
            if ( FAILED(hr) || m_FileSystem == NULL )
            {
                return E_BADCLSID;
            }
        }
    }

    {
        CComBSTR query = L"./Adapter/text()";
        IXMLDOMNode * pNode;
        if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
        {
            VARIANT v;
            pNode->get_nodeValue(&v);
            CComBSTR str(v.bstrVal);
            m_Parser->Parse(str, PF_None, &str);

            hr = InstanceFromString(str, IID_IVoodooAdapter, (void**)&m_Adapter);
            if ( FAILED(hr) || m_FileSystem == NULL )
            {
                return E_BADCLSID;
            }
        }
    }

    // Log extended build information

    LogMsg(m_Logger, LL_Info|LL_Framework, VOODOO_CORE_NAME, L"Voodoo loaded, using config %s.", configPath);
    LogMsg(m_Logger, LL_Info|LL_Framework, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

    VersionStruct vfver = VOODOO_META_VERSION_STRUCT(CORE);
    VersionStruct vsver = VOODOO_META_VERSION_STRUCT(VC);
    VersionStruct cgver = VOODOO_META_VERSION_STRUCT(CG);

    m_Logger->LogModule(vfver);
    m_Logger->LogModule(vsver);
    m_Logger->LogModule(cgver);

    // Core done loading
    LogMsg(m_Logger, LL_Info|LL_Framework, VOODOO_CORE_NAME, L"Core initialization complete.");

    m_Init = true;
    return S_OK;
}

STDMETHODIMP CVoodooCore::get_Parser( 
   IVoodooParser ** ppParser)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppParser == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppParser = m_Parser;
        (*ppParser)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_HookSystem
( 
   IVoodooHookSystem ** ppHookSystem
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppHookSystem == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppHookSystem = m_HookSystem;
        (*ppHookSystem)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_FileSystem
( 
   IVoodooFileSystem ** ppFileSystem
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppFileSystem == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppFileSystem = m_FileSystem;
        (*ppFileSystem)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_Adapter
( 
   IVoodooAdapter ** ppAdapter
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppAdapter == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppAdapter = m_Adapter;
        (*ppAdapter)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_Logger
( 
   IVoodooLogger **ppLogger
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppLogger == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppLogger = m_Logger;
        (*ppLogger)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_Config
( 
   IXMLDOMDocument ** ppConfig
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppConfig == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppConfig = m_Config;
        (*ppConfig)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_CgContext( 
   VARIANT * ppCgContext)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppCgContext == NULL )
    {
        return E_INVALIDARG;
    } else {
        V_VT(ppCgContext) = VT_BYREF;
        V_BYREF(ppCgContext) = m_Context;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::put_CgContext( 
   VARIANT pCgContext)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( V_VT(&pCgContext) != VT_BYREF )
    {
        return E_INVALIDARG;
    } else {
        m_Context = (CGcontext)V_BYREF(&pCgContext);
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::CreateShader( 
   IVoodooFile *pFile,
   SAFEARRAY * pArgs,
   IVoodooShader **ppShader)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( pFile == NULL || ppShader == NULL )
    {
        return E_INVALIDARG;
    }

    HRESULT hr = E_NOTIMPL; //CoCreateInstance(CLSID_Shader, NULL, NULL, IID_VoodooShader, (void**)ppShader);
    if ( FAILED(hr) )
    {
        return hr;
    }

    // Compile and load
    return S_OK;
}

STDMETHODIMP CVoodooCore::CreateParameter( 
   BSTR pName,
   DWORD Type,
   IVoodooParameter **ppParameter)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppParameter == NULL )
    {
        return E_INVALIDARG;
    }

    if ( this->m_Parameters.PLookup(pName) != NULL )
    {
        return E_DUPNAME;
    } else {
        ParameterType type = (ParameterType)Type;
        ParameterCategory pc = ToParameterCategory(type);
        if ( pc == PC_Float || pc == PC_Sampler )
        {
            *ppParameter = CVoodooParameter::Create(this, pName, type);
        } else {
            return E_INVALIDARG;
        }

        m_Parameters.SetAt(pName, *ppParameter);

        LogMsg
        (
            m_Logger, LL_Debug, VOODOO_CORE_NAME, 
            L"Created parameter named %s with type %X, returning shared pointer to %p.", 
            pName, Type, *ppParameter
        );

        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::CreateTexture( 
   BSTR pName,
   TextureDesc Desc,
   VARIANT Data,
   IVoodooTexture **ppTexture)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    }

    if ( this->m_Textures.PLookup(pName) != NULL )
    {
        return E_DUPNAME;
    } else {
        *ppTexture = CVoodooTexture::Create(this, pName, Desc, Data);

        if ( *ppTexture == NULL )
        {
            return E_BADTHING;
        }

        m_Textures.SetAt(pName, *ppTexture);

        LogMsg
        (
            m_Logger, LL_Debug, VOODOO_CORE_NAME, 
            L"Added texture %s with data %p, returning shared pointer to %p.", 
            pName, &Data, *ppTexture
        );

        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::GetParameter
( 
   BSTR pName,
   IVoodooParameter ** ppParameter
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppParameter == NULL )
    {
        return E_INVALIDARG;
    }

    CComPtr<IVoodooParameter> param;
    if ( this->m_Parameters.Lookup(pName, param) == 0 )
    {
        *ppParameter = NULL;
        return E_NOT_FOUND;
    } else {
        *ppParameter = param;
        (*ppParameter)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::GetTexture
( 
   BSTR pName,
   IVoodooTexture ** ppTexture
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    }

    CComPtr<IVoodooTexture> texture;
    if ( this->m_Textures.Lookup(pName, texture) == 0 )
    {
        *ppTexture = NULL;
        return E_NOT_FOUND;
    } else {
        *ppTexture = texture;
        (*ppTexture)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::RemoveTexture
( 
   BSTR pName
)
{
    if ( !m_Init ) return E_NOTINIT;
    if ( this->m_Textures.RemoveKey(pName) == 0 )
    {
        return S_NOT_FOUND;
    } else {
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::GetStageTexture
( 
   DWORD Stage,
   IVoodooTexture ** ppTexture
)
{
    if ( !m_Init ) return E_NOTINIT;
    CComPtr<IVoodooTexture> texture;
    if ( this->m_StageTextures.Lookup((TextureStage)Stage, texture) == 0 )
    {
        *ppTexture = NULL;
        return E_NOT_FOUND;
    } else {
        *ppTexture = texture;
        (*ppTexture)->AddRef();
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::SetStageTexture
( 
   DWORD Stage,
   IVoodooTexture * pTexture
)
{
    if ( !m_Init ) return E_NOTINIT;
    this->m_StageTextures.SetAt((TextureStage)Stage, pTexture);

    return S_OK;
}

STDMETHODIMP CVoodooCore::LoadConfig(BSTR pFile)
{
    VARIANT_BOOL loadStatus;
    VARIANT vConfig;
    VariantInit(&vConfig);
    V_VT(&vConfig) = VT_BSTR;
    V_BSTR(&vConfig) = pFile;

    HRESULT hr = m_Config->load(vConfig, &loadStatus);
    
    if ( hr == S_FALSE && loadStatus == VARIANT_FALSE ) {
        return E_INVALIDCFG;
    } else {
        return hr;
    }
}