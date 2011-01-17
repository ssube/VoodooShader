
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
	Core * Core::Create(std::string logfile)
	{
		Core * core = new Core(logfile);

		return core;
	}

	void Core::Destroy(Core * core)
	{
		delete core;
	}

	Core::Core(std::string logfile)
		: mAdapter(NULL),
#ifdef _DEBUG
			mDebugMode(true)
#else
			mDebugMode(false)
#endif
	{
		this->mLogger = new Logger(logfile.c_str());

#ifdef _DEBUG
		this->mLogger->SetBufferSize(0);
#endif

		this->mLogger->Log("%s", VOODOO_CORE_COPYRIGHT);
		this->mLogger->Log("Voodoo Core: Assembly ID: %s\n", VOODOO_META_STRING_VERSION_FULL(CORE));

		this->mLogger->Log("Voodoo Core: Preparing core components...\n");

		// Init Cg
		this->mCGContext = cgCreateContext();

		if ( !cgIsContext(this->mCGContext) )
		{
			Throw("Voodoo Core: Could not create Cg context.", this);
		}

		cgSetErrorHandler(&(Core::CGErrorHandler), this);

		this->mManagerFS  = new FullscreenManager(this);
		this->mManagerMat = new MaterialManager(this);

		this->mLogger->Log("Voodoo Core: Core started successfully.\n");
	}

	Core::~Core()
	{
		this->mLogger->Log("Voodoo Core: Stopping...\n");

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
		Version version = { VOODOO_META_VERSION_CHAIN(CORE) };

		return version;
	}

	Adapter * Core::GetAdapter()
	{
		return this->mAdapter;
	}

	void Core::SetDebug(bool mode)
	{
#ifndef _DEBUG
		this->mDebugMode = mode;
		if ( mode )
		{
			this->mLogger->SetBufferSize(0);
		}
#endif
	}

	void Core::Log(const char * msg, ...)
	{
		if ( this->mLogger )
		{
			va_list arglist;
			va_start(arglist, msg);

			this->mLogger->LogList(msg, arglist);

			va_end(arglist);

#ifdef _DEBUG
			this->mLogger->Dump();
#endif
		}
	}

	void Core::Debug(const char * msg, ...)
	{
		if ( mDebugMode && this->mLogger )
		{
			va_list arglist;
			va_start(arglist, msg);

			this->mLogger->LogList(msg, arglist);

			va_end(arglist);

			this->mLogger->Dump();
		}
	}

	void Core::SetAdapter(Adapter * adapter)
	{
		if ( this->mAdapter )
		{
			Throw(
			  "Voodoo Core: Attempted to set adapter when one is already set.", 
			  this);
		}

		this->mAdapter = adapter;
	}

	ShaderRef Core::CreateShader(std::string filename, const char ** args)
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

	TextureRef Core::CreateTexture(std::string name, void * data)
	{
		TextureMap::iterator textureEntry = this->mTextures.find(name);
		if ( textureEntry != this->mTextures.end() )
		{
			Throw(
			  "Voodoo Core: Trying to create a texture with a duplicate name.", 
			  this);
		} else {
			TextureRef texture(new Texture(name, data));
			this->mTextures[name] = texture;
			return texture;
		}
	}

	ParameterRef Core::CreateParameter(std::string name, ParameterType type)
	{
		ParameterMap::iterator paramEntry = this->mParameters.find(name);
		if ( paramEntry != this->mParameters.end() )
		{
			Throw(
			  "Voodoo Core: Trying to create parameter with a duplicate name.", 
			  this);
		} else {
			ParameterRef parameter(new Parameter(NULL, name, type));
			return parameter;
		}
	}

	TextureRef Core::GetTexture(std::string name)
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
		case TT_Unknown:
		default:
			Throw("Voodoo Core: Unknown texture type (function) given.", this);
		}
	}

	ParameterRef Core::GetParameter(std::string name, ParameterType type)
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
			me->Log("Voodoo Core: CG error: %s\n", cgGetErrorString(error));

			// Print any compiler errors or other details we can find
			const char * listing = cgGetLastListing(context);
			while ( listing )
			{
				me->Log("Voodoo Core: CG listing: %s\n", listing);
				listing = cgGetLastListing(context);
			}
		}
	}
}
