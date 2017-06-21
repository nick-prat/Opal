#include "sampler.hh"

#include <Core/gl.hh>

Sampler::Sampler() {
    glGenSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Sampler::Sampler(Sampler&& sampler)
: m_sampler(sampler.m_sampler)
, m_magnification(sampler.m_magnification)
, m_minification(sampler.m_minification) {
    sampler.m_sampler = 0;
    sampler.m_magnification = 0;
    sampler.m_minification = 0;
}

Sampler::~Sampler() {
    if(glIsSampler(m_sampler)) {
        glDeleteSamplers(1, &m_sampler);
    }
}

Sampler& Sampler::operator=(Sampler&& sampler) {
    m_sampler = sampler.m_sampler;
    m_magnification = sampler.m_magnification;
    m_minification = sampler.m_minification;

    sampler.m_sampler = 0;
    sampler.m_magnification = 0;
    sampler.m_minification = 0;

    return *this;
}

void Sampler::setParameter(GLenum pname, GLint param) {
    glSamplerParameteri(m_sampler, pname, param);
}

void Sampler::bind() const {
    glBindSampler(0, m_sampler);
}
