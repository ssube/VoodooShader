
#ifndef VOODOO_MATMANAGER_HPP
#define VOODOO_MATMANAGER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	/**
	 * Manager class to simplify loading and using material (texture-bound) shaders. This
	 * object provides control over the various linkage and lookups needed by the material
	 * system.
	 */
	class VOODOO_API MaterialManager
	{
	public:
		MaterialManager(Core * parent);

		/**
		 * Adds a material (a connected Texture/Shader pair) to the internal material map.
		 *
		 * @param texture The texture to associate with the material (also serves as the basetexture
		 *			for the shader, when the material is bound.
		 * @param shader The shader to associate with the materal.
		 * @param global If set, this material will become the "global" material (any geometry with
		 *			no other material will use the global material for rendering).
		 * @note If the given Texture is already present in the material map, it will be rebound to
		 *			the new Shader.
		 */
		void Add(TextureRef texture, ShaderRef shader, bool global = false);

		/**
		 * Removes a material from the internal material map.
		 *
		 * @param texture The texture half of the material to be removed.
		 * @throws Exception if no material exists with the given Texture.
		 */
		void Remove(TextureRef texture);

		/**
		 * Binds a material for use in drawing.
		 *
		 * @param texture The Texture to look up the shader for.
		 * @note If no shader is associated with this Texture <em>and</em> no default shader is
		 *			registered with this MaterialManager, an Exception will be thrown. Be prepared
		 *			to catch this.
		 * @throws Exception if texture is not in the material map <em>and</em> no default shader is
		 *			registered with this MaterialManager.
		 * @throws Exception if no graphics adapter is registered with the parent Core.
		 */
		void Bind(TextureRef texture);

		/**
		 * Unbinds the currently bound material (future draw calls will not use any material shader).
		 *
		 * @throws Exception if no graphics adapter is registered with the parent Core.
		 * @note This function should rarely, if ever, throw. The only time it can throw is if the
		 *			adapter is lost between binding and unbinding a material, or an exception thrown
		 *			by MaterialManager::BindMaterial() is ignored.
		 */
		void Unbind();

		/**
		 * Retrieve a shader from a bound material.
		 *
		 * @param texture The Texture half of the material, used as the index.
		 * @note Returns NULL if the texture is not found in the material map.
		 */
		ShaderRef Find(TextureRef texture);

	private:
		Core * mParent;
		MaterialMap mMaterials;
		ShaderRef mDefault;
		ShaderRef mBound;
	};
}

#endif /*VOODOO_MATMANAGER_HPP*/