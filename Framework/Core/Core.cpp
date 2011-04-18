#include "Core.hpp"

namespace VoodooShader
{
    Core::Core()
        : mAdapter(NULL), mHookSystem(NULL), mFileSystem(NULL), mLogger(NULL), mCgContext(NULL), mConfig(NULL), mRefrs(0)
    { }

    Core::~Core()
    {
        // Destroy adapter (not sure what it depends on, so handle first)
        if ( this->mAdapter )
        {
            this->mAdapter->Release();
        }

        if ( this->mFileSystem )
        {
            this->mFileSystem->Release();
        }

        if ( this->mHookSystem )
        {
            this->mHookSystem->Release();
        }

        if ( this->mLogger )
        {
            this->mLogger->Release();
        }
    }

    HRESULT Core::QueryInterface(REFIID iid, void ** pp)
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
        return (++mRefrs);
    }

    ULONG Core::Release()
    {
        --mRefrs;
        if ( mRefrs == 0 )
        {
            delete this;
            return 0;
        } else {
            return mRefrs;
        }
    }

    HRESULT Core::GetName(LPBSTR pName)
    {
        *pName = NULL;
        return E_NONAME;
    }

    HRESULT Core::GetCore(IVoodooCore ** ppCore)
    {
        *ppCore = this;
        return S_ISCORE;
    }

    HRESULT Core::Initialize(BSTR pConfig)
    {
        using namespace std;

        //mGlobalRoot = globalroot;
        //mGlobalRoot += "\\";
        //mRunRoot = runroot;
        //mRunRoot += "\\";

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
        //mTarget = targetname;
        //    mTarget = ( lastslash + 1 );

        //    lastslash[1] = '\0';
        //}
        mLocalRoot = localroot;
        //mLocalRoot += "\\";

        IXMLDOMDocument * config = NULL;
        HRESULT hr = CoCreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&config));
        if ( FAILED(hr) )
        {
            return E_MSXMLERROR;
        }

        config->put_async(VARIANT_FALSE);
        config->put_validateOnParse(VARIANT_FALSE);
        config->put_resolveExternals(VARIANT_FALSE);
        config->put_preserveWhiteSpace(VARIANT_TRUE);

        VARIANT_BOOL loadStatus;
        VARIANT configFileName;
        VariantInit(&configFileName);
        V_VT(&configFileName) = VT_BSTR;
        V_BSTR(&configFileName) = pConfig;

        hr = config->load(configFileName, &loadStatus);
        if ( FAILED(hr) || loadStatus == VARIANT_FALSE )
        {
            return E_INVALIDCFG;
        }

        // Store the config file, in case modules need it
        config->QueryInterface(IID_IUnknown, (void**)&mConfig);

        // Start setting things up
        IXMLDOMNode * pCoreNode = NULL;
        CComBSTR coreNodeStr = L"/VoodooConfig/Core";
        hr = config->selectSingleNode(coreNodeStr, &pCoreNode);

        if ( FAILED(hr) || pCoreNode == NULL )
        {
            return E_INVALIDCFG;
        }

        SysFreeString(coreNodeStr);

        // Create query for node text, used multiple times
        CComBSTR queryNodeText = L"./text()";
        CComBSTR queryNodeName = L"./@name";

        // Set up the internal objects
        hr = CoCreateInstance(CLSID_Parser, NULL, NULL, IID_VoodooParser, (LPVOID*)&mParser);
        if ( FAILED(hr) )
        {
            return E_BADCLSID;
        }

        // Load variables, built-in first
        mParser->AddVariable(L"globalroot", mGlobalRoot, TRUE);
        mParser->AddVariable(L"localroot", mLocalRoot, TRUE);
        mParser->AddVariable(L"runroot", mRunRoot, TRUE);
        mParser->AddVariable(L"target", mTarget, TRUE);

        CComBSTR queryVarNodes = L"/VoodooConfig/Variables/Variable";

        IXMLDOMNodeList * pVarList = NULL;
        hr = config->selectNodes(queryVarNodes, &pVarList);
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

                mParser->AddVariable(name.bstrVal, text.bstrVal, false);
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
                BSTR str;
                mParser->Parse(v.bstrVal, &str);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mLogger);
                if ( FAILED(hr) || mLogger == NULL )
                {
                    return E_BADLOGCLSID;
                } else {
                    CComBSTR queryLF = L"./LogFile/text()";
                    IXMLDOMNode * pFileNode = NULL;
                    if ( SUCCEEDED(pCoreNode->selectSingleNode(queryLF, &pFileNode)) )
                    {
                        VARIANT fn;
                        pNode->get_nodeValue(&fn);
                        BSTR filename;
                        mParser->Parse(fn.bstrVal, &filename);
                        mLogger->Open(filename, FALSE);
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
                BSTR str;
                mParser->Parse(v.bstrVal, &str);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mFileSystem);
                if ( FAILED(hr) || mFileSystem == NULL )
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
                BSTR str;
                mParser->Parse(v.bstrVal, &str);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mHookSystem);
                if ( FAILED(hr) || mFileSystem == NULL )
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
                BSTR str;
                mParser->Parse(v.bstrVal, &str);

                hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mAdapter);
                if ( FAILED(hr) || mFileSystem == NULL )
                {
                    return E_BADFSCLSID;
                }
                SysFreeString(str);
            }
        }

        // Log extended build information
        //BSTR configLocation = _BSTR("Config loaded from \"%s\".");
        //mLogger->Log(LL_Info, VOODOO_CORE_NAME, configLocation, pConfig);
        //mLogger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

        Version vfver = VOODOO_META_VERSION_STRUCT(CORE);
        Version vsver = VOODOO_META_VERSION_STRUCT(VC);
        Version cgver = VOODOO_META_VERSION_STRUCT(CG);

        mLogger->LogModule(vfver);
        mLogger->LogModule(vsver);
        mLogger->LogModule(cgver);

        // Core done loading
        CComBSTR done = L"Core initialization complete.";
        //mLogger->Log(LL_Info, VOODOO_CORE_NAME, done, NULL);

        return S_OK;
    }

    HRESULT Core::get_Adapter(IVoodooAdapter ** ppAdapter)
    {
        if ( ppAdapter == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppAdapter = mAdapter;
            return S_OK;
        }
    }

    HRESULT Core::get_HookSystem(IVoodooHookSystem ** ppHookSystem)
    {
        if ( ppHookSystem == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppHookSystem = mHookSystem;
            return S_OK;
        }
    }

    HRESULT Core::get_Logger(IVoodooLogger ** ppLogger)
    {
        if ( ppLogger == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppLogger = mLogger;
            return S_OK;
        }
    }

    HRESULT Core::get_FileSystem(IVoodooFileSystem ** ppFileSystem)
    {
        if ( ppFileSystem == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppFileSystem = mFileSystem;
            return S_OK;
        }
    }

    HRESULT Core::get_Parser(IVoodooParser ** ppParser)
    {
        if ( ppParser == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppParser = mParser;
            return S_OK;
        }
    }

    HRESULT Core::get_Config(IUnknown ** ppConfig)
    {
        if ( ppConfig == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppConfig = mConfig;
            return S_OK;
        }
    }

    HRESULT Core::get_CgContext(void ** ppContext)
    {
        if ( ppContext == NULL )
        {
            return E_INVALIDARG;
        } else {
            *ppContext = mCgContext;
            return S_OK;
        }
    }

    HRESULT Core::put_CgContext(void * pContext)
    {
        //mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Setting Cg context to %p.", context);

        if ( pContext == NULL )
        {
            //mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Clearing object cache (null context).");

            mParameters.clear();
            mTextures.clear();
            mStageTextures.clear();
        } else {
            cgSetErrorHandler(&(Core::CgErrorHandler), this);
        }

        this->mCgContext = (CGcontext)pContext;

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

        std::map<CComBSTR, IVoodooParameter*>::iterator paramEntry = this->mParameters.find(pName);
        if ( paramEntry != this->mParameters.end() )
        {
            return E_DUPNAME;
        } else {
            HRESULT hr = CoCreateInstance(CLSID_Parameter, NULL, NULL, IID_VoodooParameter, (void**)ppParameter);
            if ( FAILED(hr) )
            {
                return hr;
            }

            mParameters[pName] = *ppParameter;

            //mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Created parameter named %s with type %s, returning shared pointer to %p.", name.c_str(), Converter::ToString(type), parameter.get());

            return S_OK;
        }
    }

    HRESULT Core::CreateTexture(BSTR pName, IVoodooTarget * pData, IVoodooTexture ** ppTexture)
    {
        UNREFERENCED_PARAMETER(pData);

        std::map<CComBSTR, IVoodooTexture*>::iterator textureEntry = this->mTextures.find(pName);
        if ( textureEntry != this->mTextures.end() )
        {
            return E_DUPNAME;
        } else {
            HRESULT hr = CoCreateInstance(CLSID_Texture, NULL, NULL, IID_VoodooTexture, (void**)ppTexture);
            if ( FAILED(hr) )
            {
                return hr;
            }

            mTextures[pName] = *ppTexture;

            //mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Added texture %s with data %p, returning shared pointer to %p.", name.c_str(), data, texture.get());

            return S_OK;
        }
    }

    HRESULT Core::GetParameter(BSTR pName, IVoodooParameter ** ppParameter)
    {
        std::map<CComBSTR, IVoodooParameter*>::iterator paramEntry = this->mParameters.find(pName);
        if ( paramEntry != this->mParameters.end() )
        {
            *ppParameter = paramEntry->second;
            return S_OK;
        } else {
            *ppParameter = NULL;
            return E_NOT_FOUND;
        }
    }

    HRESULT Core::GetTexture(BSTR pName, IVoodooTexture ** ppTexture)
    {
        std::map<CComBSTR, IVoodooTexture*>::iterator paramEntry = this->mTextures.find(pName);
        if ( paramEntry != this->mTextures.end() )
        {
            *ppTexture = paramEntry->second;
            return S_OK;
        } else {
            *ppTexture = NULL;
            return E_NOT_FOUND;
        }
    }

    HRESULT Core::RemoveTexture(BSTR pName)
    {
        std::map<CComBSTR, IVoodooTexture*>::iterator paramEntry = this->mTextures.find(pName);
        if ( paramEntry != this->mTextures.end() )
        {
            mTextures.erase(paramEntry);
            return S_OK;
        } else {
            return S_NOT_FOUND;
        }
    }

    HRESULT Core::get_StageTexture(TextureType Stage, IVoodooTexture ** ppTexture)
    {
        std::map<TextureType, IVoodooTexture*>::iterator paramEntry = this->mStageTextures.find(Stage);
        if ( paramEntry != this->mStageTextures.end() )
        {
            *ppTexture = paramEntry->second;
            return S_OK;
        } else {
            *ppTexture = NULL;
            return E_NOT_FOUND;
        }
    }

    HRESULT Core::put_StageTexture(TextureType Stage, IVoodooTexture * pTexture)
    {
        std::map<TextureType, IVoodooTexture*>::iterator paramEntry = this->mStageTextures.find(Stage);
        if ( paramEntry != this->mStageTextures.end() )
        {
            paramEntry->second->Release();
        }

        pTexture->AddRef();
        mStageTextures[Stage] = pTexture;

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
                    if ( me->mAdapter )
                    {
                        me->mAdapter->HandleError(context, error, me);
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
