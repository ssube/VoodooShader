#include "Core.hpp"

#include "Converter.hpp"
#include "Parser.hpp"
#include "Sampler.hpp"
#include "Scalar.hpp"

namespace VoodooShader
{
    Core::Core()
        : m_Refrs(0), m_GlobalRoot(), m_LocalRoot(), m_RunRoot(), m_Target(),
          m_Config(NULL), m_Context(NULL), m_Adapter(NULL), m_Logger(NULL),
          m_HookSystem(NULL), m_FileSystem(NULL), m_Parser(NULL)
    { }

    Core::~Core()
    {
        if ( this->m_Adapter )
        {
            this->m_Adapter->Release();
        }

        if ( this->m_FileSystem )
        {
            this->m_FileSystem->Release();
        }

        if ( this->m_HookSystem )
        {
            this->m_HookSystem->Release();
        }

        if ( this->m_Logger )
        {
            this->m_Logger->Release();
        }
    }

    HRESULT Core::QueryInterface(REFIID iid, void ** pp) throw()
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

    ULONG Core::AddRef()
    {
        return (++m_Refrs);
    }

    ULONG Core::Release()
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

    HRESULT Core::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        }

        *pName = NULL;
        return E_NONAME;
    }

    HRESULT Core::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        }

        *ppCore = this;
        return S_ISCORE;
    }

    HRESULT Core::Initialize(BSTR pConfig)
    {
        using namespace std;

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

        HRESULT hr = CoCreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_Config));
        if ( FAILED(hr) )
        {
            return E_MSXMLERROR;
        }

        m_Config->put_async(VARIANT_FALSE);
        m_Config->put_validateOnParse(VARIANT_FALSE);
        m_Config->put_resolveExternals(VARIANT_FALSE);
        m_Config->put_preserveWhiteSpace(VARIANT_TRUE);

        VARIANT_BOOL loadStatus;
        VARIANT configFileName;
        VariantInit(&configFileName);
        V_VT(&configFileName) = VT_BSTR;
        V_BSTR(&configFileName) = pConfig;

        hr = m_Config->load(configFileName, &loadStatus);
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
        /*hr = CoCreateInstance(CLSID_Parser, NULL, NULL, IID_VoodooParser, (LPVOID*)&m_Parser);
        if ( FAILED(hr) )
        {
            return E_BADCLSID;
        }*/
        m_Parser = Parser::Create(this);
        if ( m_Parser == NULL )
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
                m_Parser->Parse(&str, PF_None);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&m_Logger);
                if ( FAILED(hr) || m_Logger == NULL )
                {
                    return E_BADLOGCLSID;
                } else {
                    CComBSTR queryLF = L"./LogFile/text()";
                    IXMLDOMNode * pFileNode = NULL;
                    if ( SUCCEEDED(pCoreNode->selectSingleNode(queryLF, &pFileNode)) )
                    {
                        VARIANT fn;
                        pNode->get_nodeValue(&fn);
                        CComBSTR filename(fn.bstrVal);
                        m_Parser->Parse(&filename, PF_None);
                        m_Logger->Open(filename, FALSE);
                        SysFreeString(filename);
                    }
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
                m_Parser->Parse(&str, PF_None);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&m_FileSystem);
                if ( FAILED(hr) || m_FileSystem == NULL )
                {
                    return E_BADFSCLSID;
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
                m_Parser->Parse(&str, PF_None);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&m_HookSystem);
                if ( FAILED(hr) || m_FileSystem == NULL )
                {
                    return E_BADHSCLSID;
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
                m_Parser->Parse(&str, PF_None);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&m_Adapter);
                if ( FAILED(hr) || m_FileSystem == NULL )
                {
                    return E_BADFSCLSID;
                }
                SysFreeString(str);
            }
        }

        // Log extended build information
        //BSTR configLocation = _BSTR("Config loaded from \"%s\".");
        //m_Logger->Log(LL_Info, VOODOO_CORE_NAME, configLocation, pConfig);
        //m_Logger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

        Version vfver = VOODOO_META_VERSION_STRUCT(CORE);
        Version vsver = VOODOO_META_VERSION_STRUCT(VC);
        Version cgver = VOODOO_META_VERSION_STRUCT(CG);

        m_Logger->LogModule(vfver);
        m_Logger->LogModule(vsver);
        m_Logger->LogModule(cgver);

        // Core done loading
        CComBSTR done = L"Core initialization complete.";
        //m_Logger->Log(LL_Info, VOODOO_CORE_NAME, done, NULL);

        return S_OK;
    }

    HRESULT Core::get_Adapter(IVoodooAdapter ** ppAdapter)
    {
        if ( ppAdapter == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppAdapter = m_Adapter;
            return S_OK;
        }
    }

    HRESULT Core::get_HookSystem(IVoodooHookSystem ** ppHookSystem)
    {
        if ( ppHookSystem == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppHookSystem = m_HookSystem;
            return S_OK;
        }
    }

    HRESULT Core::get_Logger(IVoodooLogger ** ppLogger)
    {
        if ( ppLogger == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppLogger = m_Logger;
            return S_OK;
        }
    }

    HRESULT Core::get_FileSystem(IVoodooFileSystem ** ppFileSystem)
    {
        if ( ppFileSystem == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppFileSystem = m_FileSystem;
            return S_OK;
        }
    }

    HRESULT Core::get_Parser(IVoodooParser ** ppParser)
    {
        if ( ppParser == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppParser = m_Parser;
            return S_OK;
        }
    }

    HRESULT Core::get_Config(IUnknown ** ppConfig)
    {
        if ( ppConfig == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppConfig = m_Config;
            return S_OK;
        }
    }

    HRESULT Core::get_CgContext(void ** ppContext)
    {
        if ( ppContext == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppContext = m_Context;
            return S_OK;
        }
    }

    HRESULT Core::put_CgContext(void * pContext)
    {
        //m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Setting Cg context to %p.", context);

        if ( pContext == NULL )
        {
            //m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Clearing object cache (null context).");

            m_Parameters.RemoveAll();
            m_Textures.RemoveAll();
            m_StageTextures.RemoveAll();
        } else {
            cgSetErrorHandler(&(Core::CgErrorHandler), this);
        }

        this->m_Context = (CGcontext)pContext;

        return S_OK;
    }

    HRESULT Core::CreateShader(IVoodooFile * pFile, IVoodooShader ** ppShader)
    {
        if ( pFile == NULL || ppShader == NULL )
        {
            return E_INVALIDARG;
        }

        HRESULT hr = CoCreateInstance(CLSID_Shader, NULL, NULL, IID_VoodooShader, (void**)ppShader);
        if ( FAILED(hr) )
        {
            return hr;
        }

        // Compile and load
        return S_OK;
    }

    HRESULT Core::CreateParameter(BSTR pName, ParameterType Type, IVoodooParameter ** ppParameter)
    {
        UNREFERENCED_PARAMETER(Type);
        if ( ppParameter == NULL )
        {
            return E_INVALIDARG;
        }

        if ( this->m_Parameters.PLookup(pName) != NULL )
        {
            return E_DUPNAME;
        } else {
            ParameterCategory pc = Converter::ToParameterCategory(Type);
            if ( pc == PC_Float || pc == PC_Matrix )
            {
                Scalar * param = Scalar::Create(this, pName, Type);
                param->QueryInterface(IID_VoodooParameter, (void**)ppParameter);
            } else if ( pc == PC_Matrix ) {
                Sampler * param = Sampler::Create(this, pName, Type);
                param->QueryInterface(IID_VoodooParameter, (void**)ppParameter);
            } else {
                return E_INVALIDARG;
            }

            m_Parameters.SetAt(pName, *ppParameter);

            //m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Created parameter named %s with type %s, returning shared pointer to %p.", name.c_str(), Converter::ToString(type), parameter.get());

            return S_OK;
        }
    }

    HRESULT Core::CreateTexture(BSTR pName, void * pData, IVoodooTexture ** ppTexture)
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
            HRESULT hr = CoCreateInstance(CLSID_Texture, NULL, NULL, IID_VoodooTexture, (void**)ppTexture);
            if ( FAILED(hr) )
            {
                return hr;
            }

            m_Textures.SetAt(pName, *ppTexture);

            //m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Added texture %s with data %p, returning shared pointer to %p.", name.c_str(), data, texture.get());

            return S_OK;
        }
    }

    HRESULT Core::GetParameter(BSTR pName, IVoodooParameter ** ppParameter)
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

    HRESULT Core::GetTexture(BSTR pName, IVoodooTexture ** ppTexture)
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

    HRESULT Core::RemoveTexture(BSTR pName)
    {
        if ( this->m_Textures.RemoveKey(pName) == 0 )
        {
            return S_NOT_FOUND;
        } else {
            return S_OK;
        }
    }

    HRESULT Core::get_StageTexture(TextureType Stage, IVoodooTexture ** ppTexture)
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

    HRESULT Core::put_StageTexture(TextureType Stage, IVoodooTexture * pTexture)
    {
        this->m_StageTextures.SetAt(Stage, pTexture);
        return S_OK;
    }
        
    void Core::CgErrorHandler(CGcontext context, CGerror error, void * core)
    {
        Core * me = reinterpret_cast<Core*>(core);
        if ( me )
        {
            const char * errorString = error ? cgGetErrorString(error) : NULL;

            if ( errorString )
            {
                //me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported error: %s", errorString);
                if ( context && error != CG_INVALID_CONTEXT_HANDLE_ERROR )
                {
                    if ( me->m_Adapter )
                    {
                        me->m_Adapter->HandleError(context, error, me);
                    }

                    // Print any compiler errors or other details we can find
                    const char * listing = cgGetLastListing(context);
                    
                    while ( listing )
                    {
                        //me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg error details: %s", listing);
                        listing = cgGetLastListing(context);
                    }
                } else {
                    //me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Invalid context for error, no further data available.");
                }
            } else {
                //me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported an unknown error (%d).", error);
            }            
        }
    }
}
