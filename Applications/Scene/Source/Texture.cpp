#include "Texture.hpp"

#include <SOIL.h>

Texture::Texture(const std::string& texturePath)
{
    m_data = SOIL_load_image(texturePath.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGB);
}

Texture::Texture(Texture&& texture)
{
    this->m_data = texture.m_data;
    this->m_height = texture.m_height;
    this->m_width = texture.m_width;

    texture.m_data = nullptr;
    texture.m_height = 0;
    texture.m_width = 0;
}

Texture::~Texture()
{
    SOIL_free_image_data(m_data);
    m_height = 0;
    m_width = 0;
}
