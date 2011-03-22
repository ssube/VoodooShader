#include "Core.hpp"

#include "Adapter.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "HookManager.hpp"
#include "Logger.hpp"
#include "MaterialManager.hpp"
#include "Module.hpp"
#include "Parser.hpp"

namespace VoodooShader
{
    #pragma comment(linker, "/EXPORT:CreateCore=?CreateCore@VoodooShader@@YAPAVCore@1@PBD@Z")
    #pragma comment(linker, "/EXPORT:DestroyCore=?DestroyCore@VoodooShader@@YAXPAVCore@1@@Z")

    Core * CreateCore(const char * path)
    {
        return new Core(path);
    }

    void DestroyCore(Core * core)
    {
        delete core;
    }

    Core::Core(const char * path)
        : mAdapter(NULL), mBasePath(path)
    {
        mModManager = new ModuleManager(this);

        mModManager->LoadModule("Voodoo_Xml.dll", true);
        this->mParser = (IParser*)mModManager->CreateClass("Parser");
        this->mConfig = this->mParser->LoadDocument("VoodooConfig.xmlconfig");

        INode* coreNode = this->mConfig->GetRoot()->GetSingleChild("Core");
        if ( coreNode )
        {
            NodeMap moduleNodes = coreNode->GetChildren("Module");
            NodeMap::iterator moduleIter = moduleNodes.begin();
            while ( moduleIter != moduleNodes.end() )
            {
                INode * moduleNode = moduleIter->second;
                String filename = moduleNode->GetAttribute("file");

                mModManager->LoadModule(filename, true);

                ++moduleIter;
            }
        }

        // Load logger
        String logClass = coreNode->GetSingleChild("Logger")->GetValue();
        this->mLogger = (ILogger*)mModManager->CreateClass(logClass);

        if ( mLogger == NULL )
        {
            Throw(VOODOO_CORE_NAME, "Unable to create Logger.", this);
        }

        mLogName = mConfig->GetRoot()->GetSingleChild("Logger")->GetSingleChild("File")->GetValue();
        this->mLogger->Open(mLogName.c_str(), false);

        this->mLogger->Log(LL_Info, VOODOO_CORE_NAME, "%s", VOODOO_GLOBAL_COPYRIGHT_FULL);
        this->mLogger->Log(LL_Info, VOODOO_CORE_NAME, "Preparing core components...");

        // Init hook manager
        String hookClass = coreNode->GetSingleChild("Hooker")->GetValue();
        this->mHooker = (IHookManager*)mModManager->CreateClass(hookClass);

        if ( mHooker == NULL )
        {
            Throw(VOODOO_CORE_NAME, "Unable to create Hooker.", this);
        }

        // Init Cg
        this->mCGContext = cgCreateContext();

        if ( !cgIsContext(this->mCGContext) )
        {
            Throw(VOODOO_CORE_NAME, "Could not create Cg context.", this);
        }

        cgSetErrorHandler(&(Core::CGErrorHandler), this);

        this->mManagerFS  = new FullscreenManager(this);
        this->mManagerMat = new MaterialManager(this);

        this->mLogger->Log(LL_Info, VOODOO_CORE_NAME, "Core started successfully.");

        // Log extended build information
        this->LogModule(this->GetVersion());
        Version vsver = VOODOO_META_VERSION_STRUCT(VC);
        this->LogModule(vsver);
        Version cgver = VOODOO_META_VERSION_STRUCT(CG);
        this->LogModule(cgver);

        // Load the adapter
        mAdapterName = coreNode->GetSingleChild("Adapter")->GetValue();
        this->mAdapter = (IAdapter*)mModManager->CreateClass(mAdapterName);
    }

    Core::~Core()
    {
        if ( this->mAdapter )
        {
            this->mAdapter->DestroyObject();
        }

        if ( this->mManagerFS )
        {
            delete this->mManagerFS;
        }

        if ( this->mManagerMat )
        {
            delete this->mManagerMat;
        }

        if ( cgIsContext(this->mCGContext) )
        {
            cgDestroyContext(this->mCGContext);
        }

        if ( this->mHooker )
        {
            this->mHooker->DestroyObject();
        }

        if ( this->mLogger )
        {
            this->mLogger->DestroyObject();
        }
    }

    String Core::GetBasePath()
    {
        return mBasePath;
    }

    CGcontext Core::GetCGContext()
    {
        return this->mCGContext;
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

    ModuleManager * Core::GetModuleManager()
    {
        return mModManager;
    }

    IDocument * Core::GetConfig()
    {
        return mConfig;
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
        this->mLogger->LogModule(version);
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
        
    void Core::CGErrorHandler(CGcontext context, CGerror error, void * core)
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
