#include "Includes.hpp"
#include "Texture.hpp"

namespace VoodooShader
{
	Texture::Texture(String name, void * texture)
		: mTextureObject(texture), mName(name)
	{ };

	void * Texture::GetTexture()
	{
		return mTextureObject;
	}

	String Texture::Name()
	{
		return mName;
	};
}
