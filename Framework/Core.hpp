
#ifndef VOODOO_CORE_HPP
#define VOODOO_CORE_HPP

#include "Meta.hpp"

#include "Reference.hpp"
#include "Exception.hpp"
#include "Logger.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Parameter.hpp"

namespace VoodooShader
{
	/**
	 * Core engine class for the Voodoo Shader Framework. Provides a flexible, uniform
	 * API for a complete shader framework. Using various @ref Adapter "adapters", it is
	 * possible to tie the Voodoo Core into almost any game or program, during or after
	 * development.
	 */
	class VOODOO_API Core
	{
	public:
		/**
		 * Create a new Voodoo Core.
		 *
		 * @param logfile The filename for this Core's log (will be used by most managers and
		 *			connected adapters).
		 * @return A reference to the new core.
		 */
		Core(std::string logfile = "Voodoo.log");
		~Core();

		CGcontext GetCGContext();
		Logger * GetLog();

		/**
		 * Attach an Adapter to this Core.
		 *
		 * @param adapter The Adapter to attach (will be used for all graphics calls).
		 * @throws Exception if an Adapter is already connected to this Core.
		 */
		void SetAdapter(Adapter * adapter);

		/**
		 * Retrieve the Adapter attached to this Core.
		 *
		 * @return A pointer to the Adapter or NULL if no Adapter is attached.
		 */
		Adapter * GetAdapter();

		ShaderRef CreateShader(std::string filename, const char ** args);
		TextureRef CreateTexture(std::string name, void * data);

		TextureRef GetTexture(std::string name);

		/**
		 * Creates a global-level or virtual parameter. This parameter exists
		 * in the Cg runtime, but is not a part of any shader or program. This
		 * is useful for creating parameters that must be shared between programs.
		 * Only parameters created with this function may be used in Parameter::Link.
		 *
		 * @param name The name for this parameter.
		 * @param type True for texture, false for float4
		 * @todo Fix this to use a real parameter type parameter.
		 * @return A new parameter.
		 * @throws Exception if a parameter with the given name already exists.
		 */
		ParameterRef CreateParameter(std::string name, ParameterType type);

	//protected:
		static void CGErrorHandler(CGcontext context, CGerror error, void * core);

	private:

		Adapter * mAdapter;
		Logger * mLogger;

		// Manager objects:
		FullscreenManager *	mManagerFS;
		MaterialManager   *	mManagerMat;

		// Cg objects:
		CGcontext mCGContext;

		CGEffectMap mCGEffects;

		ShaderMap mShaders;
		TextureMap mTextures;
		ParameterMap mParameters;
	};
}

#endif