#include "texture.hh"

#include <iostream>

#include <Utilities/log.hh>

Texture::Texture(const GLuint glTexture, const std::string& filename)
: m_glTexture(glTexture)
, m_filename(filename) {}

Texture::Texture(Texture&& texture)
: m_glTexture(texture.m_glTexture)
, m_filename(texture.m_filename) {
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

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_glTexture);
}

Sampler& Texture::getSampler() {
    return m_sampler;
}

const Sampler& Texture::getSampler() const {
    return m_sampler;
}
