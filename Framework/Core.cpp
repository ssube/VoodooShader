
#include "Core.hpp"
#include "Exception.hpp"
#include "FullscreenManager.hpp"
#include "MaterialManager.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Adapter.hpp"

#pragma comment(lib, "version.lib")

namespace VoodooShader
{
	Core::Core(std::string logfile)
		: mAdapter(NULL)
	{
		this->mLogger = new Logger(logfile.c_str());

#ifdef _DEBUG
		this->mLogger->SetBufferSize(0);
#endif

		this->mLogger->Log(VOODOO_CORE_COPYRIGHT, false);
		this->mLogger->Log(VOODOO_CORE_VERSION_STRING, false);

		/*char versionBuffer[8192];
		BOOL hr = GetFileVersionInfoA("VoodooFramework.dll", NULL, 8192, versionBuffer);
		if ( hr != 0 )
		{
			VS_FIXEDFILEINFO * version = NULL;
			UINT versionSize;
			hr = VerQueryValueA(versionBuffer, "\\", (LPVOID*)&version, &versionSize);
			if ( hr != 0 )
			{
				UINT versionMajor = ( version->dwFileVersionMS & 0xFFFF0000 ) / 0x10000;
				UINT versionMinor = ( version->dwFileVersionMS & 0x0000FFFF );
				UINT versionBuild = ( version->dwFileVersionLS & 0xFFFF0000 ) / 0x10000;
				UINT versionRev   = ( version->dwFileVersionLS & 0x0000FFFF );

				this->mLogger->Format("Voodoo Core: Starting version %d.%d.%d (rev %d) core...")
					.With(versionMajor).With(versionMinor).With(versionBuild).With(versionRev).Done();
			} else {
				this->mLogger->Log("Voodoo Core: Failed to retrieve version info.");
			}
		} else {
			this->mLogger->Log("Voodoo Core: Failed to retrieve version info.");
		}*/

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

	ParameterRef Core::GetParameter(std::string name, ParameterType type)
	{
		ParameterMap::iterator paramEntry = this->mParameters.find(name);
		if ( paramEntry != this->mParameters.end() )
		{
			if ( paramEntry->second->Type() == type )
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
