#include "sampler.hpp"

Sampler::Sampler()
{
    gl::glGenSamplers(1, &m_sampler);
}

Sampler::~Sampler()
{
    Unload();
}

void Sampler::Bind() const
{
    // TODO bind the sampler here
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
