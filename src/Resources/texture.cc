#include <Opal/Resources/texture.hh>
#include <Opal/Util/log.hh>

#include <iostream>

using namespace Opal;

Texture::Texture(const Resources::RTexture& texture)
: m_width{texture.width}
, m_height{texture.height}
, m_filename{texture.filename} {
    glGenTextures(1, &m_glTexture);
    glBindTexture(GL_TEXTURE_2D, m_glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, texture.bytes.data());
}

Texture::Texture(Texture&& texture)
: m_glTexture{texture.m_glTexture}
, m_width{texture.m_width}
, m_height{texture.m_height}
, m_filename{texture.m_filename} {
    texture.m_glTexture = 0;
    texture.m_filename = "invalid";
}

Texture::~Texture() {
    if(glIsTexture(m_glTexture)) {
        glDeleteTextures(1, &m_glTexture);
        m_glTexture = 0;
    }
}

std::string Texture::getFileName() const {
    return m_filename;
}

void Texture::bind(const unsigned int loc) const {
    glActiveTexture(GL_TEXTURE0 + loc);
    glBindTexture(GL_TEXTURE_2D, m_glTexture);
}

Sampler& Texture::getSampler() {
    return m_sampler;
}

const Sampler& Texture::getSampler() const {
    return m_sampler;
}
