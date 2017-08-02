#include <Opal/Resources/texture.hh>
#include <Opal/Util/log.hh>

#include <iostream>

using namespace Opal;

Texture::Texture(Texture &&texture)
: m_glTexture{texture.m_glTexture}
, m_filename{texture.m_filename} {
    texture.m_glTexture = 0;
    texture.m_filename = "invalid";
}

Texture::Texture(Resources::RTexture &&texture)
: RTexture{std::move(texture)} {
    glGenTextures(1, &m_glTexture);
    glBindTexture(GL_TEXTURE_2D, m_glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, texture.bytes.data());
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

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_glTexture);
}

Sampler &Texture::getSampler() {
    return m_sampler;
}

const Sampler &Texture::getSampler() const {
    return m_sampler;
}
