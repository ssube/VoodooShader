#include "Core.hpp"

namespace VoodooShader
{
    Core::Core()
        : mAdapter(NULL), mHooker(NULL), mLogger(NULL), mCgContext(NULL), mConfig(NULL), mRefrs(0)
    { }

    HRESULT Core::QueryInterface(REFIID iid, void ** pp)
    {
        if ( pp == NULL )
        {
            return E_POINTER;
        } else if ( iid == IID_IUnknown ) {
            *pp = this;
            return S_OK;
        } else if ( iid == IID_VoodooCore ) {
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
        }
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

        try
        {
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
            mConfig = (void*)config;

            // Start setting things up
            IXMLDOMNode * pCoreNode = NULL;
            BSTR coreNodeStr = _BSTR("/VoodooConfig/Core");
            hr = config->selectSingleNode(coreNodeStr, &pCoreNode);

            if ( FAILED(hr) || pCoreNode == NULL )
            {
                return E_INVALIDCFG;
            }

            // Create query for node text, used multiple times
            BSTR queryNodeText = _BSTR("./text()");
            BSTR queryNodeName = _BSTR("./@name");

            // Set up the internal objects
            {
                HRESULT hr = CoCreateInstance(CLSID_Parser, NULL, NULL, IID_VoodooParser, (LPVOID*)&mParser);
                if ( FAILED(hr) )
                {
                    return E_BADCLSID;
                }
            }

            // Load variables, built-in first
            mParser->AddVariable(L"globalroot", mGlobalRoot, TRUE);
            mParser->AddVariable(L"localroot", mLocalRoot, TRUE);
            mParser->AddVariable(L"runroot", mRunRoot, TRUE);
            mParser->AddVariable(L"target", mTarget, TRUE);

            BSTR queryVarNodes = _BSTR("/VoodooConfig/Variables/Variable");

            IXMLDOMNodeList * pVarList = NULL;
            hr = config->selectNodes(queryVarNodes, &pVarList);
            if ( SUCCEEDED(hr) )
            {
                IXMLDOMNode * pVarNode = NULL;
                while ( hr = pVarList->nextNode(&pVarNode) && hr == S_OK && pVarNode != NULL )
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

            // Lookup classes
            {
                BSTR query = _BSTR("./Logger/text()");
                IXMLDOMNode * pNode = NULL;
                if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
                {
                    VARIANT v;
                    pNode->get_nodeValue(&v);
                    BSTR str;
                    mParser->Parse(v.bstrVal, &str);
                }

                HRESULT hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mLogger);
                if ( FAILED(hr) || mLogger == NULL )
                {
                    return E_BADLOGCLSID;
                } else {
                    BSTR queryLF = _BSTR("./LogFile/text()");
                    IXMLDOMNode * pFileNode = NULL;
                    if ( SUCCEEDED(pCoreNode->selectSingleNode(queryLF, &pFileNode)) )
                    {
                        VARIANT fn;
                        pNode->get_nodeValue(&fn);
                        BSTR filename;
                        mParser->Parse(fn.bstrVal, &filename);
                        mLogger->Open(filename, FALSE);
                    }
                }
            }

            {
                BSTR query = _BSTR("./FileSystem/text()");
                IXMLDOMNode * pNode;
                if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
                {
                    VARIANT v;
                    pNode->get_nodeValue(&v);
                    BSTR str;
                    mParser->Parse(v.bstrVal, &str);
                }

                HRESULT hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mFileSystem);
                if ( FAILED(hr) || mFileSystem == NULL )
                {
                    return E_BADFSCLSID;
                }
            }

            {
                BSTR query = _BSTR("./HookSystem/text()");
                IXMLDOMNode * pNode;
                if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
                {
                    VARIANT v;
                    pNode->get_nodeValue(&v);
                    BSTR str;
                    mParser->Parse(v.bstrVal, &str);
                }

                HRESULT hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mHookSystem);
                if ( FAILED(hr) || mFileSystem == NULL )
                {
                    return E_BADHSCLSID;
                }
            }

            {
                BSTR query = _BSTR("./Adapter/text()");
                IXMLDOMNode * pNode;
                if ( SUCCEEDED(pCoreNode->selectSingleNode(query, &pNode)) )
                {
                    VARIANT v;
                    pNode->get_nodeValue(&v);
                    BSTR str;
                    mParser->Parse(v.bstrVal, &str);
                }

                HRESULT hr = InstanceFromString(str, IID_VoodooLogger, (void**)&mAdapter);
                if ( FAILED(hr) || mFileSystem == NULL )
                {
                    return E_BADFSCLSID;
                }
            }

            // Log extended build information
            //mLogger->Log(LL_Info, VOODOO_CORE_NAME, "Config loaded from \"%s\".", ConfigPath.c_str());
            //mLogger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

            Version vfver = VOODOO_META_VERSION_STRUCT(CORE);
            Version vsver = VOODOO_META_VERSION_STRUCT(VC);
            Version cgver = VOODOO_META_VERSION_STRUCT(CG);

            mLogger->LogModule(vfver);
            mLogger->LogModule(vsver);
            mLogger->LogModule(cgver);

            // Load less vital classes
            if ( mFileSystem.get() == NULL && mModManager->ClassExists(fsClass) )
            {
                mFileSystem = mModManager->SharedCreateClass<IFileSystem>(fsClass);

                if ( mFileSystem.get() == NULL )
                {
                    throw std::exception("Unable to create FileSystem object.");
                }
            } else {
                throw std::exception("FileSystem class not found or module not loaded.");
            }

            if ( mHooker.get() == NULL && mModManager->ClassExists(hookClass) )
            {
                mHooker = mModManager->SharedCreateClass<IHookManager>(hookClass);

                if ( mHooker.get() == NULL )
                {
                    throw std::exception("Unable to create HookManager object.");
                }
            } else {
                throw std::exception("HookManager class not found or module not loaded.");
            }

            if ( mAdapter.get() == NULL && mModManager->ClassExists(adpClass) )
            {
                mAdapter = mModManager->SharedCreateClass<IAdapter>(adpClass);

                if ( mAdapter.get() == NULL )
                {
                    throw std::exception("Unable to create Adapter object.");
                }
            } else {
                throw std::exception("Adapter class not found or module not loaded.");
            }

            // Core done loading
            mLogger->Log(LL_Info, VOODOO_CORE_NAME, "Core initialization complete.");

            // Return
        } catch ( std::exception & exc ) {
            if ( this->mLogger.get() )
            {
                mLogger->Log(LL_Error, VOODOO_CORE_NAME, "Error during Core creation: %s", exc.what());
            }

            throw exc;
        }
    }

    Core::~Core()
    {
        this->SetCgContext(NULL);

        // Destroy adapter (not sure what it depends on, so handle first)
        this->mAdapter = NULL;

        this->mHooker = NULL;
        this->mLogger = NULL;

        mModManager = NULL;

#ifdef _DEBUG_FULL
        _CrtDumpMemoryLeaks();
#endif
    }

    STDMETHODIMPL(String Core::GetGlobalRoot()
    {
        return mGlobalRoot;
    }

    String Core::GetLocalRoot()
    {
        return mLocalRoot;
    }

    String Core::GetRunRoot()
    {
        return mRunRoot;
    }

    void Core::SetCgContext(CGcontext context)
    {
        mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Setting Cg context to %p.", context);

        if ( context == NULL )
        {
            mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Clearing object cache (null context).");

            mShaders.clear();

            mParameters.clear();

            mLastPass = NULL;
            mLastShader = NULL;
            mTextures.clear();
        } else {
            cgSetErrorHandler(&(Core::CgErrorHandler), this);
        }

        this->mCgContext = context;
    }

    CGcontext Core::GetCgContext()
    {
        return this->mCgContext;
    }

    Version Core::GetVersion()
    {
        Version version = VOODOO_META_VERSION_STRUCT(CORE);

        return version;
    }

    IAdapterRef Core::GetAdapter()
    {
        return this->mAdapter;
    }

    IHookManagerRef Core::GetHookManager()
    {
        return mHooker;
    }

    ILoggerRef Core::GetLogger()
    {
        return mLogger;
    }

    IFileSystemRef Core::GetFileSystem()
    {
        return mFileSystem;
    }

    ModuleManagerRef Core::GetModuleManager()
    {
        return mModManager;
    }

    ParserRef Core::GetParser()
    {
        return mParser;
    }

    void * Core::GetConfig()
    {
        return mConfig;
    }

    TextureRef Core::AddTexture(String name, void * data)
    {
        TextureMap::iterator textureEntry = this->mTextures.find(name);
        if ( textureEntry != this->mTextures.end() )
        {
            Throw(VOODOO_CORE_NAME, "Trying to add a texture with a duplicate name.", this);
        } else {
            TextureRef texture(new Texture(name, data));
            this->mTextures[name] = texture;

            mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Added texture %s with data %p, returning shared pointer to %p.", name.c_str(), data, texture.get());

            return texture;
        }
    }

    ShaderRef Core::CreateShader(String filename, const char ** args)
    {
        ShaderMap::iterator shader = this->mShaders.find(filename);

        if ( shader != this->mShaders.end() )
        {
            return shader->second;
        } else {
            // Load the shader from file
            ShaderRef newShader(new Shader(this, filename, args));
            this->mShaders[filename] = newShader;

            mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Created shader from %s, returning shared pointer to %p.", filename.c_str(), newShader.get());

            return newShader;
        }
    }

    ParameterRef Core::CreateParameter(String name, ParameterType type)
    {
        ParameterMap::iterator paramEntry = this->mParameters.find(name);
        if ( paramEntry != this->mParameters.end() )
        {
            Throw(VOODOO_CORE_NAME, "Trying to create parameter with a duplicate name.", this);
        } else {
            ParameterRef parameter(new Parameter(this, name, type));
            mParameters[name] = parameter;

            mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Created parameter named %s with type %s, returning shared pointer to %p.", name.c_str(), Converter::ToString(type), parameter.get());

            return parameter;
        }
    }

    TextureRef Core::GetTexture(_In_ String name)
    {
        TextureMap::iterator textureEntry = this->mTextures.find(name);
        if ( textureEntry != this->mTextures.end() )
        {
            mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Got texture %s, returning shared pointer to %p.", name.c_str(), textureEntry->second.get());

            return textureEntry->second;
        } else {
            mLogger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find texture %s.", name.c_str());
            return TextureRef();
        }
    }

    TextureRef Core::GetTexture(_In_ TextureType function)
    {
        switch ( function )
        {
        case TT_PassTarget:
            return mLastPass;
        case TT_ShaderTarget:
            return mLastShader;
        case TT_Generic:
        case TT_Unknown:
        default:
            return TextureRef();
        }
    }

    void Core::SetTexture(TextureType function, TextureRef texture)
    {
        switch ( function )
        {
        case TT_PassTarget:
            mLastPass = texture;
            break;
        case TT_ShaderTarget:
            mLastShader = texture;
            break;
        case TT_Generic:
            Throw(VOODOO_CORE_NAME, "Invalid texture type (generic) given.", this);
        case TT_Unknown:
        default:
            Throw(VOODOO_CORE_NAME, "Unknown texture type given.", this);
        }
    }

    ParameterRef Core::GetParameter(String name, ParameterType type)
    {
        ParameterMap::iterator paramEntry = this->mParameters.find(name);
        if ( paramEntry != this->mParameters.end() )
        {
            if ( type == PT_Unknown || paramEntry->second->GetType() == type )
            {
                return paramEntry->second;
            } else {
                return ParameterRef();
            }    
        } else {    
            return ParameterRef();        
        }
    }

    void Core::RemoveTexture( _In_ String texture )
    {
        TextureMap::iterator location = mTextures.find(texture);
        if ( location != mTextures.end() )
        {
            mTextures.erase(location);
        }
    }
        
    void Core::CgErrorHandler(CGcontext context, CGerror error, void * core)
    {
        Core * me = reinterpret_cast<Core*>(core);
        if ( me )
        {
            const char * errorString = error ? cgGetErrorString(error) : NULL;

            if ( errorString )
            {
                me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported error: %s", errorString);
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
                        me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg error details: %s", listing);
                        listing = cgGetLastListing(context);
                    }
                } else {
                    me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Invalid context for error, no further data available.");
                }
            } else {
                me->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported an unknown error (%d).", error);
            }            
        }
    }
}
