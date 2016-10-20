#include "texture.hpp"

Texture::Texture()
    : m_loaded(false)
{}

Texture::~Texture()
{
    Unload();
}

void Texture::SetFileName(const std::string filename)
{
    m_filename = filename;
}

std::string Texture::GetFileName() const
{
    return m_filename;
}

void Texture::SetTexture(const GLuint glTexture)
{
    m_glTexture = glTexture;
    m_loaded = true;
}

void Texture::Unload()
{
    if(m_loaded)
    {
        glDeleteTextures(1, &m_glTexture);
        m_glTexture = 0;
    }
}

bool Texture::IsLoaded() const
{
    return m_loaded;
}
