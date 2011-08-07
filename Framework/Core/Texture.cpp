#include "Texture.hpp"

namespace VoodooShader
{
    Texture::Texture(String name, void * texture)
        : m_Name(name), m_Data(texture)
    { };

    String Texture::GetName()
    {
        return m_Name;
    }

    Core * Texture::GetCore()
    {
        return m_Core;
    }

    void * Texture::GetData()
    {
        return m_Data;
    }

    TextureDesc Texture::GetDesc()
    {
        return m_Desc;
    };
}
