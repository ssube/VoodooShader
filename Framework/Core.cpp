
#include "Core.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "MaterialManager.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Adapter.hpp"

namespace VoodooShader
{
	Core::Core(std::string logfile)
		: mAdapter(NULL)
	{
		this->mLogger = new Logger(logfile.c_str());
#ifdef _DEBUG
		this->mLogger->SetBufferSize(0);
#endif
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
			Throw("Voodoo Core: Attempted to set adapter when one is already set.", this);
		}

		this->mAdapter = adapter;
	}

	ShaderRef Core::CreateShader(std::string filename, const char ** args)
	{
		CGEffectMap::iterator CGEffect = this->mCGEffects.find(filename);
		if ( CGEffect != this->mCGEffects.end() )
		{
			return ShaderRef(new Shader(this, CGEffect->second));
		} else {
			// Load the shader from file
			return ShaderRef(new Shader(this, filename, args));
		}
	}

	TextureRef Core::CreateTexture(std::string name, void * data)
	{
		TextureMap::iterator textureEntry = this->mTextures.find(name);
		if ( textureEntry != this->mTextures.end() )
		{
			Throw("Voodoo Core: Trying to create a texture with a duplicate name.", this);
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
			Throw("Voodoo Core: Trying to create parameter with a duplicate name.", this);
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
			return TextureRef();
		} else {
			return textureEntry->second;
		}
	}
		
	void Core::CGErrorHandler(CGcontext context, CGerror error, void * core)
	{
		Core * me = reinterpret_cast<Core*>(core);
		if ( me )
		{
			me->mLogger->Format("Voodoo Core: CG error: %s\n").With(cgGetErrorString(error)).Done();

			// Print any compiler errors or other details we can find
			const char * listing = cgGetLastListing(me->mCGContext);
			if ( listing )
			{
				me->mLogger->Format("Voodoo Core: CG listing: %s\n").With(listing).Done();
			}
		}
	}
}
