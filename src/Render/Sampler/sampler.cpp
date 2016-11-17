#include "sampler.hpp"

#include <glapi.hpp>

using namespace gl;

Sampler::Sampler()
{
    glGenSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Sampler::~Sampler()
{
    Unload();
}

void Sampler::Bind() const
{
    glBindSampler(0, m_sampler);
}

void Sampler::SetParams(int magnification, int minification)
{
    m_magnification = magnification;
    m_minification = minification;
}

void Sampler::Unload()
{
    glDeleteSamplers(1, &m_sampler);
}
