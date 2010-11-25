
#include "Texture.hpp"

namespace VoodooShader
{
	Texture::Texture(std::string name, void * texture)
		: mTextureObject(texture), mName(name)
	{ };
}