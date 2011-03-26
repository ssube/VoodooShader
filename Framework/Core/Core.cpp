#include "Core.hpp"

#include "Adapter.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "HookManager.hpp"
#include "Logger.hpp"
#include "MaterialManager.hpp"
#include "Module.hpp"

#include "pugixml.hpp"

namespace VoodooShader
{
    #pragma comment(linker, "/EXPORT:CreateCore=?CreateCore@VoodooShader@@YAPAVCore@1@PBD@Z")
    #pragma comment(linker, "/EXPORT:DestroyCore=?DestroyCore@VoodooShader@@YAXPAVCore@1@@Z")

    Core * CreateCore(_In_ const char * path)
    {
        return new Core(path);
    }

    void DestroyCore(_In_ Core * core)
    {
        delete core;
    }

    Core::Core(_In_ const char * path)
        : mAdapter(NULL), mHooker(NULL), mLogger(NULL), mCgContext(NULL)
    {
        using namespace pugi;

        mBasePath = path;
        mBasePath += "\\";

        // Init Cg
        this->mCgContext = cgCreateContext();

        if ( !cgIsContext(this->mCgContext) )
        {
            throw std::exception("Unable to create Cg context.");
        }

        cgSetErrorHandler(&(Core::CgErrorHandler), this);

        mModManager = ModuleManagerRef(new ModuleManager(this));

        xml_document * config = NULL;

        try
        {
            config = new pugi::xml_document();
            xml_parse_result result = config->load_file("VoodooConfig.xmlconfig");

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
            String logFileName = logFileQuery.evaluate_string(config->root());
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
        } catch ( xpath_exception & exc ) {
            delete config;
            throw std::exception(exc);
        } catch ( std::exception & exc ) {
            delete config;
            throw exc;
        }

    }

    Core::~Core()
    {
        if ( this->mAdapter )
        {
            this->mAdapter->DestroyObject();
        }

        if ( this->mHooker )
        {
            this->mHooker->DestroyObject();
        }

        if ( this->mLogger )
        {
            this->mLogger->DestroyObject();
        }

        this->mModManager = NULL;

        if ( cgIsContext(this->mCgContext) )
        {
            // Failsafe destruction of all effects
            CGeffect effect = cgGetFirstEffect(mCgContext);
            while ( cgIsEffect(effect) )
            {
                CGeffect next = cgGetNextEffect(effect);
                cgDestroyEffect(effect);
                effect = next;
            }

            cgDestroyContext(this->mCgContext);
        }
    }

    String Core::GetBasePath()
    {
        return mBasePath;
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
            return newShader;
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
            return texture;
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

            return parameter;
        }
    }

    TextureRef Core::GetTexture(String name)
    {
        TextureMap::iterator textureEntry = this->mTextures.find(name);
        if ( textureEntry != this->mTextures.end() )
        {
            return textureEntry->second;
        } else {
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
