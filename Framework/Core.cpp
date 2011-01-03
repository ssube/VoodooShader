
#include "Core.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "MaterialManager.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Adapter.hpp"
#include "../Version.hpp"

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
		: mAdapter(NULL)
	{
		this->mLogger = new Logger(logfile.c_str());

		/**
		 * @todo Buffer size for the core logger is 0 for debugging purposes,
		 *		make it bigger before release.
		 */
//#ifdef _DEBUG
		this->mLogger->SetBufferSize(0);
//#endif

		this->mLogger->Log(VOODOO_CORE_COPYRIGHT, false);
		this->mLogger->Log(
			"Voodoo Core: Assembly ID: "
			VOODOO_META_STRING_VERSION_FULL(CORE)
			"\n");

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

	Adapter * Core::GetAdapter()
	{
		return this->mAdapter;
	}

	Logger * Core::GetLog()
	{
		return this->mLogger;
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
			me->mLogger->Format("Voodoo Core: CG error: %s\n")
				.With(cgGetErrorString(error)).Done();

			// Print any compiler errors or other details we can find
			const char * listing = cgGetLastListing(context);
			while ( listing )
			{
				me->mLogger->Format("Voodoo Core: CG listing: %s\n")
					.With(listing).Done();
				listing = cgGetLastListing(context);
			}
		}
	}
}
