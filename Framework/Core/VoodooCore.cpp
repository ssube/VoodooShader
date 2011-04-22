// VoodooCore.cpp : Implementation of CVoodooCore

#include "stdafx.h"
#include "VoodooCore.h"

#include "Common.hpp"

#include "VoodooParameter.h"
#include "VoodooParser.h"
#include "VoodooTexture.h"

// CVoodooCore
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

STDMETHODIMP CVoodooCore::Initialize(VARIANT pConfig)
{
    //m_GlobalRoot = globalroot;
    //m_GlobalRoot += "\\";
    //m_RunRoot = runroot;
    //m_RunRoot += "\\";

    // Get the local root
    TCHAR localroot[MAX_PATH];
    HMODULE targetModule = GetModuleHandle(NULL);
    GetModuleFileName(targetModule, localroot, MAX_PATH);
    //char * lastslash = strrchr(localroot, '\\');
    //if ( lastslash )
    //{
    //char targetname[MAX_PATH];
    //strcpy_s(targetname, MAX_PATH, lastslash + 1);
    //_strlwr_s(targetname, strlen(targetname));
    //m_Target = targetname;
    //    m_Target = ( lastslash + 1 );

    //    lastslash[1] = '\0';
    //}
    m_LocalRoot = localroot;
    //m_LocalRoot += "\\";

    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_Config));
    if ( FAILED(hr) )
    {
        return E_INVALIDCFG;
    }

    m_Config->put_async(VARIANT_FALSE);
    m_Config->put_validateOnParse(VARIANT_FALSE);
    m_Config->put_resolveExternals(VARIANT_FALSE);
    m_Config->put_preserveWhiteSpace(VARIANT_TRUE);

    VARIANT_BOOL loadStatus;
    hr = m_Config->load(pConfig, &loadStatus);
    if ( FAILED(hr) || loadStatus == VARIANT_FALSE )
    {
        return E_INVALIDCFG;
    }

    // Start setting things up
    IXMLDOMNode * pCoreNode = NULL;
    CComBSTR coreNodeStr = L"/VoodooConfig/Core";
    hr = m_Config->selectSingleNode(coreNodeStr, &pCoreNode);

    if ( FAILED(hr) || pCoreNode == NULL )
    {
        return E_INVALIDCFG;
    }

    SysFreeString(coreNodeStr);

    // Create query for node text, used multiple times
    CComBSTR queryNodeText = L"./text()";
    CComBSTR queryNodeName = L"./@name";

    // Set up the internal objects
    CVoodooParser * parser = new CVoodooParser(this);
    hr = parser->QueryInterface(IID_IVoodooParser, &m_Parser);
    if ( FAILED(hr) || m_Parser == NULL )
    {
        return E_BADTHING;
    }

    // Load variables, built-in first
    m_Parser->AddVariable(L"globalroot", m_GlobalRoot, TRUE);
    m_Parser->AddVariable(L"localroot", m_LocalRoot, TRUE);
    m_Parser->AddVariable(L"runroot", m_RunRoot, TRUE);
    m_Parser->AddVariable(L"target", m_Target, TRUE);

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

            m_Parser->AddVariable(name.bstrVal, text.bstrVal, false);
        }
    }

    SysFreeString(queryVarNodes);

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
                    pNode->get_nodeValue(&fn);
                    CComBSTR filename(fn.bstrVal);
                    m_Parser->Parse(filename, PF_None, &filename);
                    m_Logger->Open(filename, FALSE);
                    SysFreeString(filename);
                }
            }
            SysFreeString(str);
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
            SysFreeString(str);
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
            SysFreeString(str);
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
            SysFreeString(str);
        }
    }

    // Log extended build information
    //BSTR configLocation = _BSTR("Config loaded from \"%s\".");
    //m_Logger->Log(LL_Info, VOODOO_CORE_NAME, configLocation, pConfig);
    //m_Logger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

    /*Version vfver = VOODOO_META_VERSION_STRUCT(CORE);
    Version vsver = VOODOO_META_VERSION_STRUCT(VC);
    Version cgver = VOODOO_META_VERSION_STRUCT(CG);

    m_Logger->LogModule(vfver);
    m_Logger->LogModule(vsver);
    m_Logger->LogModule(cgver);*/

    // Core done loading
    CComBSTR done = L"Core initialization complete.";
    m_Logger->Log(LL_Info, L"Core", done, NULL);

    return S_OK;
}

STDMETHODIMP CVoodooCore::get_Parser( 
   IVoodooParser ** ppParser)
{
    if ( ppParser == NULL ) return E_INVALIDARG;

    *ppParser = m_Parser;
    return S_OK;
}

STDMETHODIMP CVoodooCore::get_HookSystem( 
   IVoodooHookSystem **ppHookSystem)
{
    if ( ppHookSystem == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppHookSystem = m_HookSystem;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_FileSystem( 
   IVoodooFileSystem **ppFileSystem)
{
    if ( ppFileSystem == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppFileSystem = m_FileSystem;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_Adapter( 
   IVoodooAdapter **ppAdapter)
{
    if ( ppAdapter == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppAdapter = m_Adapter;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_Logger( 
   IVoodooLogger **ppLogger)
{
    if ( ppLogger == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppLogger = m_Logger;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_Config( 
   IXMLDOMDocument **ppConfig)
{
    if ( ppConfig == NULL )
    {
        return E_INVALIDARG;
    } else {
        *ppConfig = m_Config;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_CgContext( 
   VARIANT * ppCgContext)
{
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
    if ( V_VT(&pCgContext) != VT_BYREF )
    {
        return E_INVALIDARG;
    } else {
        m_Context = V_BYREF(&pCgContext);
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::CreateShader( 
   IVoodooFile *pFile,
   IVoodooShader **ppShader)
{
    if ( pFile == NULL || ppShader == NULL )
    {
        return E_INVALIDARG;
    }

    HRESULT hr = E_INVALIDARG; //CoCreateInstance(CLSID_Shader, NULL, NULL, IID_VoodooShader, (void**)ppShader);
    if ( FAILED(hr) )
    {
        return hr;
    }

    // Compile and load
    return S_OK;
}

STDMETHODIMP CVoodooCore::CreateParameter( 
   BSTR pName,
   ParameterType Type,
   IVoodooParameter **ppParameter)
{
    if ( ppParameter == NULL )
    {
        return E_INVALIDARG;
    }

    if ( this->m_Parameters.PLookup(pName) != NULL )
    {
        return E_DUPNAME;
    } else {
        ParameterCategory pc = Converter::ToParameterCategory(Type);
        if ( pc == PC_Float || pc == PC_Matrix || pc == PC_Sampler )
        {
            CVoodooParameter * param = new CVoodooParameter();
            param->QueryInterface(IID_IVoodooParameter, (void**)ppParameter);
        } else {
            return E_INVALIDARG;
        }

        m_Parameters.SetAt(pName, *ppParameter);

        //m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Created parameter named %s with type %s, returning shared pointer to %p.", name.c_str(), Converter::ToString(type), parameter.get());

        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::CreateTexture( 
   BSTR pName,
   VARIANT *pData,
   IVoodooTexture **ppTexture)
{
    UNREFERENCED_PARAMETER(pData);

    if ( ppTexture == NULL )
    {
        return E_INVALIDARG;
    }

    if ( this->m_Textures.PLookup(pName) != NULL )
    {
        return E_DUPNAME;
    } else {
        CVoodooTexture * tex = CVoodooTexture::Create(this, pName, pData);
        HRESULT hr = tex->QueryInterface(__uuidof(IVoodooTexture), (void**)ppTexture);

        if ( FAILED(hr) )
        {
            delete tex;
            return hr;
        }

        m_Textures.SetAt(pName, *ppTexture);

        //m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Added texture %s with data %p, returning shared pointer to %p.", name.c_str(), data, texture.get());

        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::GetParameter( 
   BSTR pName,
   IVoodooParameter **ppParameter)
{
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
        ppParameter = &param;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::GetTexture( 
   BSTR pName,
   IVoodooTexture **ppTexture)
{
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
        ppTexture = &texture;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::RemoveTexture( 
   BSTR pName)
{
    if ( this->m_Textures.RemoveKey(pName) == 0 )
    {
        return S_NOT_FOUND;
    } else {
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::get_StageTexture( 
   TextureType Stage,
   IVoodooTexture **ppTexture)
{
    CComPtr<IVoodooTexture> texture;
    if ( this->m_StageTextures.Lookup(Stage, texture) == 0 )
    {
        *ppTexture = NULL;
        return E_NOT_FOUND;
    } else {
        ppTexture = &texture;
        return S_OK;
    }
}

STDMETHODIMP CVoodooCore::put_StageTexture( 
   TextureType Stage,
   IVoodooTexture *pTexture)
{
    this->m_StageTextures.SetAt(Stage, pTexture);

    return S_OK;
}

