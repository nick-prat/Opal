#include "sampler.hpp"

#include <GL/gl3w.h>

Sampler::Sampler()
{
    glGenSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Sampler::~Sampler() {
    glDeleteSamplers(1, &m_sampler);
}

void Sampler::bind() const {
    glBindSampler(0, m_sampler);
}

void Sampler::setParams(int magnification, int minification) {
    m_magnification = magnification;
    m_minification = minification;
}
