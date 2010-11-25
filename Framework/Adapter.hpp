
#ifndef VOODOO_ADAPTER_HPP
#define VOODOO_ADAPTER_HPP

#include "Meta.hpp"
#include "Texture.hpp"
#include "Core.hpp"

namespace VoodooShader
{
	/**
	 * Graphics adapter class, responsible for interfacing the Voodoo Core with
	 * a given graphics program. This class should be implemented for a specific
	 * graphics application; some generic adapters are provided, but in most cases
	 * application-specific behavior will make these only partially helpful. This
	 * class does include a generic error-handler function, which adapters may
	 * choose not to override if they do not need specific error-handling.
	 */
	class VOODOO_API Adapter
	{
	public:
		void Bind(PassRef shader);
		void Unbind();

		void DrawFSQuad();

		void Apply(ParameterRef param);

		bool ConnectTexture(ParameterRef param, TextureRef texture);

		TextureRef CreateTexture(size_t width, size_t height, size_t depth, bool mipmaps,
			TextureFormat format);

		void HandleError(CGcontext context, CGerror error, void * core);
	};
}

#endif