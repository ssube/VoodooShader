
#include "Includes.hpp"
#include "Core.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "MaterialManager.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Adapter.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    Core * Core::Create(String logfile)
    {
        return new Core(logfile);
    }

    void Core::Destroy(Core * core)
    {
        delete core;
    }

    Core::Core(String logfile)
        : mAdapter(NULL)
    {
        this->mLogger = new Logger(this, logfile.c_str());

        this->mLogger->Log(LL_Info, VOODOO_CORE_NAME, "%s", VOODOO_CORE_COPYRIGHT);
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
        this->mLogger->Log(LL_Info, VOODOO_CORE_NAME, "Stopping...");

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

        if ( this->mLogger )
        {
            delete this->mLogger;
        }
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

    void Core::SetAdapter(Adapter * adapter)
    {
        if ( this->mAdapter && adapter )
        {
            Throw(VOODOO_CORE_NAME, "Attempted to set adapter when one is already set.", this);
        }

        this->mAdapter = adapter;
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
