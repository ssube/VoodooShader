#include "Core.hpp"

#include "IAdapter.hpp"
#include "IFilesystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "MaterialManager.hpp"
#include "Module.hpp"
#include "Parameter.hpp"
#include "Parser.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "pugixml.hpp"

namespace VoodooShader
{
    Core * CreateCore(_In_ const char * globalroot, _In_ const char * runroot)
    {
        return new Core(globalroot, runroot);
    }

    void DestroyCore(_In_ Core * core)
    {
        delete core;
    }

    Core::Core(_In_ const char * globalroot, _In_ const char * runroot)
        : mAdapter(NULL), mHooker(NULL), mLogger(NULL), mCgContext(NULL), mConfig(NULL)
    {
        using namespace pugi;

#ifdef _DEBUG_FULL
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

        mGlobalRoot = globalroot;
        mGlobalRoot += "\\";
        mRunRoot = runroot;
        mRunRoot += "\\";

        // Get the local root
        char localroot[MAX_PATH];
        HMODULE targetModule = GetModuleHandle(NULL);
        GetModuleFileName(targetModule, localroot, MAX_PATH);
        //localroot[chars] = '\0';
        char * lastslash = strrchr(localroot, '\\');
        if ( lastslash )
        {
            //char targetname[MAX_PATH];
            //strcpy_s(targetname, MAX_PATH, lastslash + 1);
            //_strlwr_s(targetname, strlen(targetname));
            //mTarget = targetname;
            mTarget = ( lastslash + 1 );

            lastslash[1] = '\0';
        }
        mLocalRoot = localroot;
        //mLocalRoot += "\\";

        // Set up the internal objects
        mModManager = ModuleManagerRef(new ModuleManager(this));
        mParser = ParserRef(new Parser(this));

        try
        {
            xml_document * config = new xml_document();

            // Try loading the config file from each major location
            String ConfigPath = mRunRoot + "VoodooConfig.xmlconfig";
            xml_parse_result result = config->load_file(ConfigPath.c_str());

            if ( !result )
            {
                ConfigPath = mLocalRoot + "VoodooConfig.xmlconfig";
                result = config->load_file(ConfigPath.c_str());

                if ( !result )
                {
                    ConfigPath = mGlobalRoot + "VoodooConfig.xmlconfig";
                    result = config->load_file(ConfigPath.c_str());

                    if ( !result )
                    {
                        throw std::exception("Unable to find or parse config file.");
                    }
                }
            }

            // Store the config file, in case modules need it
            mConfig = (void *)config;

            // Start setting things up
            xml_node coreNode = config->select_single_node("/VoodooConfig/Core").node();

            if ( !coreNode )
            {
                throw std::exception("Could not find Core node.");
            }

            // Create query for node text, used multiple times
            xpath_query nodeTextQuery("./text()");

            // Load variables, built-in first
            mParser->AddVariable("globalroot", mGlobalRoot, true);
            mParser->AddVariable("localroot", mLocalRoot, true);
            mParser->AddVariable("runroot", mRunRoot, true);
            mParser->AddVariable("target", mTarget, true);

            xpath_query varNodesQuery("/VoodooConfig/Variables/Variable");
            xpath_query varNodeNameQuery("./@name");

            xpath_node_set varNodes = varNodesQuery.evaluate_node_set(*config);
            xpath_node_set::const_iterator varNodeIter = varNodes.begin();
            while ( varNodeIter != varNodes.end() )
            {
                String name = varNodeNameQuery.evaluate_string(*varNodeIter);
                String value = nodeTextQuery.evaluate_string(*varNodeIter);
                mParser->AddVariable(name, value);

                ++varNodeIter;
            }

            // Lookup classes
            xpath_query logQuery ("/VoodooConfig/Core/Logger/text()");
            xpath_query fsQuery  ("/VoodooConfig/Core/FileSystem/text()");
            xpath_query hookQuery("/VoodooConfig/Core/HookManager/text()");
            xpath_query adpQuery ("/VoodooConfig/Core/Adapter/text()");
            xpath_query logfQuery("/VoodooConfig/Core/LogFile/text()");

            String logClass  = mParser->ParseString(logQuery.evaluate_string( *config));
            String fsClass   = mParser->ParseString(fsQuery.evaluate_string(  *config));
            String hookClass = mParser->ParseString(hookQuery.evaluate_string(*config));
            String adpClass  = mParser->ParseString(adpQuery.evaluate_string( *config));
            String logFile   = mParser->ParseString(logfQuery.evaluate_string(*config));

            // Load modules
            xpath_node_set moduleNodes = coreNode.select_nodes("Module");
            xpath_node_set::const_iterator moduleIter = moduleNodes.begin();
            while ( moduleIter != moduleNodes.end() )
            {
                String rawfilename = nodeTextQuery.evaluate_string(*moduleIter);
                String filename = mParser->ParseString(rawfilename);

                ModuleRef cmod = mModManager->LoadModule(filename);

                // Test for debug mismatch and vital classes
                if ( mLogger.get() == NULL && mModManager->ClassExists(logClass) )
                {
                    mLogger = mModManager->SharedCreateClass<ILogger>(logClass);

                    if ( mLogger.get() == NULL || !mLogger->Open(logFile.c_str(), false) )
                    {
                        throw std::exception("Unable to create Logger object or open log file.");
                    }
                }

                ++moduleIter;
            }

            // Make sure a logger was loaded
            if ( mLogger.get() == NULL )
            {
                throw std::exception("Unable to create Logger object (class not found).");
            }

            // Log extended build information
            mLogger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

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

    String Core::GetGlobalRoot()
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
