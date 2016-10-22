#include "sampler.hpp"

#include <Core/glapi.hpp>

Sampler::Sampler()
{
    gl::glGenSamplers(1, &m_sampler);
    gl::glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl::glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

Sampler::~Sampler()
{
    Unload();
}

void Sampler::Bind() const
{
    gl::glBindSampler(0, m_sampler);
}

void Sampler::SetParams(int magnification, int minification)
{
    m_magnification = magnification;
    m_minification = minification;
}

void Sampler::Unload()
{
    gl::glDeleteSamplers(1, &m_sampler);
}
