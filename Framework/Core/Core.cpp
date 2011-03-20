#include "Core.hpp"

#include "Adapter.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "Logger.hpp"
#include "MaterialManager.hpp"

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
        : mAdapter(NULL)
    {
        mBasePath = path;
        mLogName = "VoodooLog";
        mAdapterName = mBasePath + "\\Voodoo_Gem.dll";

        this->LoadSupportLibs();

        this->mLogger = (*mFuncLoggerCreate)(this, mLogName.c_str(), false);

        if ( mLogger == NULL )
        {
            Throw(VOODOO_CORE_NAME, "Unable to create Logger.", this);
        }

        this->mHooker = (*mFuncHookerCreate)(this);

        if ( mHooker == NULL )
        {
            Throw(VOODOO_CORE_NAME, "Unable to create Hooker.", this);
        }

        this->LoadAdapter();

        this->mLogger->Log(LL_Info, VOODOO_CORE_NAME, "%s", VOODOO_GLOBAL_COPYRIGHT_FULL);
        this->mLogger->Log(LL_Info, VOODOO_CORE_NAME, "Preparing core components...");

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
        Version vsver = { "MSVC 10", _MSC_FULL_VER, 0, 0, _MSC_BUILD, VOODOO_DEBUG_VALUE };
        this->LogModule(vsver);

        this->LogModule(this->GetVersion());
    }

    Core::~Core()
    {
        if ( this->mAdapter )
        {
            (*mFuncAdapterDestroy)(mAdapter);
        }

        if ( this->mHooker )
        {
            (*mFuncHookerDestroy)(mHooker);
        }

        if ( this->mLogger )
        {
            (*mFuncLoggerDestroy)(mLogger);
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
    }

    void Core::LoadSupportLibs()
    {
        // Create and load Logger and HookManager
        String loggerPath = mBasePath + "\\Voodoo_Logger.dll";

        mModuleLogger = LoadLibraryExA(loggerPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

        if ( mModuleLogger == NULL )
        {
            throw std::exception("Could not load Logger module.");
        }

        mFuncLoggerCreate  = (Functions::LoggerCreate) GetProcAddress(mModuleLogger, "CreateLogger");
        mFuncLoggerDestroy = (Functions::LoggerDestroy)GetProcAddress(mModuleLogger, "DestroyLogger");

        String hookerPath = mBasePath + "\\Voodoo_Hook.dll";

        mModuleHooker = LoadLibraryExA(hookerPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

        if ( mModuleHooker == NULL )
        {
            throw std::exception("Could not load Hooker module.");
        }

        mFuncHookerCreate  = (Functions::HookerCreate) GetProcAddress(mModuleHooker, "CreateHookManager");
        mFuncHookerDestroy = (Functions::HookerDestroy)GetProcAddress(mModuleHooker, "DestroyHookManager");
    }

    void Core::LoadAdapter()
    {
        mModuleAdapter = LoadLibraryExA(mAdapterName.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

        if ( mModuleAdapter == NULL )
        {
            throw std::exception("Could not load Adapter module.");
        }

        mFuncAdapterCreate  = (Functions::AdapterCreate) GetProcAddress(mModuleAdapter, "CreateAdapter");
        mFuncAdapterDestroy = (Functions::AdapterDestroy)GetProcAddress(mModuleAdapter, "DestroyAdapter");
        
        mAdapter = (*mFuncAdapterCreate)(this);
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

    Adapter * Core::GetAdapter()
    {
        return this->mAdapter;
    }

    HookManager * Core::GetHookManager()
    {
        return mHooker;
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
