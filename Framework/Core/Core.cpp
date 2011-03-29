#include "Core.hpp"

#include "Adapter.hpp"
#include "Converter.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "HookManager.hpp"
#include "Logger.hpp"
#include "MaterialManager.hpp"
#include "Module.hpp"

#include "pugixml.hpp"

namespace VoodooShader
{
    Core * CreateCore(_In_ const char * path, _In_ const char * startdir)
    {
        return new Core(path, startdir);
    }

    void DestroyCore(_In_ Core * core)
    {
        delete core;
    }

    Core::Core(_In_ const char * path, _In_ const char * startdir)
        : mAdapter(NULL), mHooker(NULL), mLogger(NULL), mCgContext(NULL)
    {
        using namespace pugi;

        mGlobalRoot = path;
        mGlobalRoot += "\\";
        mLocalRoot = startdir;
        mLocalRoot += "\\";

        mModManager = ModuleManagerRef(new ModuleManager(this));

        xml_document * config = NULL;

        try
        {
            String logpath = mLocalRoot + "VoodooConfig.xml";
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

            // Load logger
            xml_node logNode = coreNode.child("Logger");
            String logModule = logNode.attribute("module").value();
            String logClass  = logNode.attribute("class").value();
            this->mModManager->LoadModule(logModule);
            this->mLogger = (ILogger*)mModManager->CreateClass(logClass);

            if ( mLogger == NULL )
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
            this->Log
            (
                LL_Info,
                VOODOO_CORE_NAME,
                "%s",
                VOODOO_GLOBAL_COPYRIGHT_FULL
            );
            this->LogModule(this->GetVersion());
            Version vsver = VOODOO_META_VERSION_STRUCT(VC);
            this->LogModule(vsver);
            Version cgver = VOODOO_META_VERSION_STRUCT(CG);
            this->LogModule(cgver);

            // Load hook module
            xml_node hookNode = coreNode.child("Hook");
            String hookModule = hookNode.attribute("module").value();
            String hookClass  = hookNode.attribute("class").value();
            this->mModManager->LoadModule(hookModule);
            this->mHooker = (IHookManager*)mModManager->CreateClass(hookClass);

            if ( mHooker == NULL )
            {
                throw std::exception("Unable to create hook object.");
            }

            // Core done loading
            this->Log(LL_Info, VOODOO_CORE_NAME, "Core initialization complete.");

            // Load adapter module
            xml_node adapterNode = coreNode.child("Adapter");
            String adapterModule = adapterNode.attribute("module").value();
            String adapterClass  = adapterNode.attribute("class").value();
            this->mModManager->LoadModule(adapterModule);
            this->mAdapter = (IAdapter*)mModManager->CreateClass(adapterClass);

            if ( mAdapter == NULL )
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
            if ( this->mLogger )
            {
                this->Log(LL_Error, VOODOO_CORE_NAME, "Error during Core creation: %s", exc.what());
            }

            delete config;
            throw exc;
        }

    }

    Core::~Core()
    {
        this->SetCgContext(NULL);

        // Destroy adapter (not sure what it depends on, so handle first)
        if ( this->mAdapter )
        {
            delete this->mAdapter;
        }

        // Remove hooks
        if ( this->mHooker )
        {
            delete this->mHooker;
        }

        // Kill logger
        if ( this->mLogger )
        {
            delete this->mLogger;
        }

        mModManager = NULL;
    }

    String Core::GetBasePath()
    {
        return mGlobalRoot;
    }

    void Core::SetCgContext(CGcontext context)
    {
        this->Log(LL_Debug, VOODOO_CORE_NAME, "Setting Cg context to %p.", context);

        if ( context == NULL )
        {
            this->Log(LL_Debug, VOODOO_CORE_NAME, "Clearing object cache (null context).");

            mShaders.clear();

            mParameters.clear();

            mLastPass = NULL;
            mLastShader = NULL;
            mTextures.clear();
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

    IAdapter * Core::GetAdapter()
    {
        return this->mAdapter;
    }

    IHookManager * Core::GetHookManager()
    {
        return mHooker;
    }

    ModuleManagerRef Core::GetModuleManager()
    {
        return mModManager;
    }

    void Core::Log(LogLevel level, const char * module, const char * msg, ...)
    {
        if ( this->mLogger )
        {
            va_list arglist;
            va_start(arglist, msg);

            this->mLogger->LogList(level, module, msg, arglist);

            va_end(arglist);

#ifdef _DEBUG
            this->mLogger->Dump();
#endif
        }
    }
    
    void Core::LogModule
    (
        _In_ Version version
    )
    {
        if ( this->mLogger )
        {
            this->mLogger->LogModule(version);
        }
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

            this->Log(LL_Debug, VOODOO_CORE_NAME, "Added texture %s with data %p, returning shared pointer to %p.", name.c_str(), data, texture.get());

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

            this->Log(LL_Debug, VOODOO_CORE_NAME, "Created shader from %s, returning shared pointer to %p.", filename.c_str(), newShader.get());

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

            this->Log(LL_Debug, VOODOO_CORE_NAME, "Created parameter named %s with type %s, returning shared pointer to %p.", name.c_str(), Converter::ToString(type), parameter.get());

            return parameter;
        }
    }

    TextureRef Core::GetTexture(String name)
    {
        TextureMap::iterator textureEntry = this->mTextures.find(name);
        if ( textureEntry != this->mTextures.end() )
        {
            this->Log(LL_Debug, VOODOO_CORE_NAME, "Got texture %s, returning shared pointer to %p.", name.c_str(), textureEntry->second.get());

            return textureEntry->second;
        } else {
            this->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find texture %s.", name.c_str());
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
        case TT_Count:
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
        case TT_Count:
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
                me->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported error: %s", errorString);
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
                        me->Log(LL_Error, VOODOO_CG_NAME, "Cg error details: %s", listing);
                        listing = cgGetLastListing(context);
                    }
                } else {
                    me->Log(LL_Error, VOODOO_CG_NAME, "Invalid context for error, no further data available.");
                }
            } else {
                me->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported an unknown error (%d)", error);
            }            
        }
    }
}
