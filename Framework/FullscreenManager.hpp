
#ifndef VOODOO_FSMANAGER_HPP
#define VOODOO_FSMANAGER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	/**
	 * Manager class to simplify loading and unloading fullscreen (post-processing)
	 * shader effects.
	 * This object provides complete control and support for a multi-shader, multi-pass
	 * render chain. It provides functions for inserting and removing @ref Shader shaders.
	 */
	class VOODOO_API FullscreenManager
	{
	public:
		/**
		 * Default constructor. Requires a VoodooShader::Core parent object.
		 * This must only be called by the Core::Create function, as having multiple
		 * fullscreen managers is not supported and may lead to resource leaks.
		 *
		 * @param parent The Core to use as the parent.
		 */
		FullscreenManager(Core * parent);

		/**
		 * Insert a shader into the render queue.
		 *
		 * @param shader The shader object to be inserted.
		 * @param position The position at which to insert the shader.
		 *		-1 indicates the end of the queue (akin to a push), 0 the beginning.
		 * @return The index of the shader in the queue.
		 * @throws Exception if position is less than -1 or greater than the end of the queue.
		 */
		size_t Add(Shader * shader, int position = -1);

		/**
		 * Removes a shader from the render queue.
		 *
		 * @param position The position of the shader to be removed.
		 *		-1 indicates the end of the queue (aking to pop), 0 the beginning.
		 * @throws Exception if position is less than -1 or greater than the end of the queue.
		 */
		void Remove(size_t position);

		/**
		 * Binds shaders from the post-processing queue and draws them to screen.
		 * 
		 * @param start The first shader to be drawn (0-based index).
		 * @param count The number of shaders to draw (-1 for all from start to the end
		 *		of the queue).
		 * @throws Exception if start is past the end of the queue.
		 * @throws Exception if count is 0.
		 * @throws Exception if the parent Core has no Adapter set.
		 */
		void Render(unsigned int start = 0, int count = -1);

	private:
		//! A constant reference to the parent VoodooShader::Core
		Core * mParent;

		//! A vector containing the list of shaders to render, in order.
		ShaderVector mShaders;
	};
}

#endif