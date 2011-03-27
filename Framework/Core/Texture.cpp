#include "Texture.hpp"

namespace VoodooShader
{
    Texture::Texture(String name, void * texture)
        : mData(texture), mName(name)
    { };

    void * Texture::GetData()
    {
        return mData;
    }

    String Texture::GetName()
    {
        return mName;
    };
}
