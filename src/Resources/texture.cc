#include <Opal/Resources/texture.hh>
#include <Opal/Util/log.hh>
#include <FreeImage.h>

#include <iostream>

using  Opal::Texture;

Texture::Texture(const Resources::RTexture& texture)
: m_width{texture.width}
, m_height{texture.height}
, m_textureType{GL_TEXTURE_2D}
, m_filename{texture.filename} {
    glGenTextures(1, &m_glTexture);
    glBindTexture(m_textureType, m_glTexture);
    glTexImage2D(m_textureType, 0, GL_RGBA8, texture.width, texture.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, texture.bytes.data());
}

Texture::Texture(Texture&& texture)
: m_glTexture{texture.m_glTexture}
, m_width{texture.m_width}
, m_height{texture.m_height}
, m_textureType{texture.m_textureType}
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

void Texture::bind(const int loc) const {
    m_textureUnit = loc;
    glActiveTexture(GL_TEXTURE0 + loc);
    glBindTexture(m_textureType, m_glTexture);
}

void Texture::unbind() const {
    GLint currentTextureUnit = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureUnit);
    if(currentTextureUnit == m_textureUnit) {
        m_textureUnit = 0;
        glBindTexture(m_textureType, 0);
    }
}

bool Texture::isBound() const {
    GLint currentTextureUnit = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureUnit);
    return currentTextureUnit == m_textureUnit;
}

int Texture::getTextureUnit() const {
    return m_textureUnit;
}

std::vector<int> Texture::getBytes() const {
    std::vector<int> bytes;
    bytes.resize(m_width * m_height);
    glGetTextureImage(m_glTexture, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_width * m_height * 4, bytes.data());
    return bytes;
}

std::pair<unsigned int, unsigned int> Texture::getDimensions() const {
    return {m_width, m_height};
}

Opal::Sampler& Texture::getSampler() {
    return m_sampler;
}

const Opal::Sampler& Texture::getSampler() const {
    return m_sampler;
}
