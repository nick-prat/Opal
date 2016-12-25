#include "texture.hpp"

#include <iostream>

#include <Utilities/utilities.hpp>

Texture::Texture()
    : IResource("texture"), m_glTexture(-1), m_loaded(false), m_filename("invalid") {

}

Texture::~Texture() {
    unload();
}

void Texture::setFileName(const std::string filename) {
    m_filename = filename;
}

std::string Texture::getFileName() const {
    return m_filename;
}

void Texture::setTexture(const GLuint glTexture) {
    m_glTexture = glTexture;
    m_loaded = true;
}

void Texture::bind() const {
    if(m_loaded && glIsTexture(m_glTexture)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_glTexture);
    } else {
        std::cout << "(" << m_glTexture << ") Trying to bind texture that doesn't exist " << m_filename << std::endl;
        Utilities::PrintGLErrors();
    }
}

void Texture::unload() {
    if(m_loaded) {
        glDeleteTextures(1, &m_glTexture);
        m_glTexture = 0;
    }
}

bool Texture::isLoaded() const {
    return m_loaded;
}
