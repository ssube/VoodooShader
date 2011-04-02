#include "Core.hpp"

#include "Adapter.hpp"
#include "Converter.hpp"
#include "Exception.hpp"
#include "Filesystem.hpp"
#include "FullscreenManager.hpp"
#include "HookManager.hpp"
#include "Logger.hpp"
#include "MaterialManager.hpp"
#include "Module.hpp"
#include "Parser.hpp"

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
        : mAdapter(NULL), mHooker(NULL), mLogger(NULL), mCgContext(NULL)
    {
        using namespace pugi;

        mGlobalRoot = globalroot;
        mGlobalRoot += "\\";
        mRunRoot = runroot;
        mRunRoot += "\\";

        // Get the local root
        char localroot[MAX_PATH];
        HMODULE targetModule = GetModuleHandle(NULL);
        GetModuleFileName(targetModule, localroot, MAX_PATH);
        char * lastslash = strrchr(localroot, '\\');
        if ( lastslash )
        {
            lastslash[0] = '\0';
        }
        mLocalRoot = localroot;
        mLocalRoot += "\\";

        // Set up the internal objects
        mModManager = ModuleManagerRef(new ModuleManager(this));
        mParser = ParserRef(new Parser(this));

        xml_document * config = NULL;

        try
        {
            String logpath = mRunRoot + "VoodooConfig.xmlconfig";
            config = new pugi::xml_document();
            xml_parse_result result = config->load_file(logpath.c_str());

            if ( !result )
            {
                throw std::exception("Error parsing config file.");
            }

            xml_node coreNode = config->select_single_node("/VoodooConfig/Core").node();

            if ( !coreNode )
            {
                throw std::exception("Could not find Core node.");
            }

            // Load variables
            xpath_query varNodesQuery("/VoodooConfig/Variables/Variable");
            xpath_node_set varNodes = varNodesQuery.evaluate_node_set(*config);
            xpath_node_set::const_iterator varNodeIter = varNodes.begin();
            while ( varNodeIter != varNodes.end() )
            {
                xml_node varNode = (*varNodeIter).node();

                String name = varNode.attribute("name").value();
                String value = varNode.first_child().value();
                mParser->AddVariable(name, value);

                ++varNodeIter;
            }

            // Load logger
            xml_node logNode = coreNode.child("Logger");
            String logModule = logNode.attribute("module").value();
            String logClass  = logNode.attribute("class").value();
            this->mModManager->LoadModule(logModule);
            this->mLogger = mModManager->SharedCreateClass<ILogger>(logClass);

            if ( mLogger.get() == NULL )
            {
                throw std::exception("Unable to create logger object.");
            }

            xpath_query logFileQuery("/VoodooConfig/Logger/File/text()");
            String logFileName = logFileQuery.evaluate_string(*config);
            if ( !this->mLogger->Open(logFileName.c_str(), false) )
            {
                throw std::exception("Unable to open log file.");
            }

            // Log extended build information
            mLogger->Log
            (
                LL_Info,
                VOODOO_CORE_NAME,
                VOODOO_GLOBAL_COPYRIGHT_FULL
            );
            mLogger->LogModule(this->GetVersion());
            Version vsver = VOODOO_META_VERSION_STRUCT(VC);
            mLogger->LogModule(vsver);
            Version cgver = VOODOO_META_VERSION_STRUCT(CG);
            mLogger->LogModule(cgver);

            // Load hook module
            xml_node hookNode = coreNode.child("Hook");
            String hookModule = hookNode.attribute("module").value();
            String hookClass  = hookNode.attribute("class").value();
            this->mModManager->LoadModule(hookModule);
            this->mHooker = mModManager->SharedCreateClass<IHookManager>(hookClass);

            if ( mHooker.get() == NULL )
            {
                throw std::exception("Unable to create hook object.");
            }

            // Load filesystem module
            xml_node fsNode = coreNode.child("Filesystem");
            String fsModule = fsNode.attribute("module").value();
            String fsClass  = fsNode.attribute("class").value();
            this->mModManager->LoadModule(fsModule);
            this->mFileSystem = mModManager->SharedCreateClass<IFileSystem>(fsClass);

            if ( mFileSystem.get() == NULL )
            {
                throw std::exception("Unable to create filesystem object.");
            }

            xpath_query fspathsQuery("/VoodooConfig/Filesystem/Paths/Path");
            xpath_node_set fspaths = fspathsQuery.evaluate_node_set(*config);
            xpath_node_set::const_iterator fspathIter = fspaths.begin();
            while ( fspathIter != fspaths.end() )
            {
                xml_node fspathNode = (*fspathIter).node();
                String path = fspathNode.first_child().value();

                mFileSystem->AddDirectory(path);

                ++fspathIter;
            }

            // Core done loading
            mLogger->Log(LL_Info, VOODOO_CORE_NAME, "Core initialization complete.");

            // Load adapter module
            xml_node adapterNode = coreNode.child("Adapter");
            String adapterModule = adapterNode.attribute("module").value();
            String adapterClass  = adapterNode.attribute("class").value();
            this->mModManager->LoadModule(adapterModule);
            this->mAdapter = mModManager->SharedCreateClass<IAdapter>(adapterClass);

            if ( mAdapter.get() == NULL )
            {
                throw std::exception("Unable to create adapter object.");
            }

            // Load other modules
            xpath_node_set moduleNodes = coreNode.select_nodes("Module");
            xpath_node_set::const_iterator moduleIter = moduleNodes.begin();
            while ( moduleIter != moduleNodes.end() )
            {
                xml_node moduleNode = (*moduleIter).node();
                String filename = moduleNode.attribute("file").value();
                
                mModManager->LoadModule(filename);

                ++moduleIter;
            }

            delete config;
        } catch ( std::exception & exc ) {
            if ( this->mLogger.get() )
            {
                mLogger->Log(LL_Error, VOODOO_CORE_NAME, "Error during Core creation: %s", exc.what());
            }

            delete config;
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

    TextureRef Core::GetTexture(String name)
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

    TextureRef Core::GetTexture(TextureType function)
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
