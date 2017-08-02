#include <Opal/Render/sampler.hh>
#include <Opal/Core/gl.hh>

Opal::Sampler::Sampler() {
    glGenSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Opal::Sampler::Sampler(Sampler &&sampler)
: m_sampler(sampler.m_sampler)
, m_magnification(sampler.m_magnification)
, m_minification(sampler.m_minification) {
    sampler.m_sampler = 0;
    sampler.m_magnification = 0;
    sampler.m_minification = 0;
}

Opal::Sampler::~Sampler() {
    if(glIsSampler(m_sampler)) {
        glDeleteSamplers(1, &m_sampler);
    }
}

Opal::Sampler &Opal::Sampler::operator=(Sampler &&sampler) {
    m_sampler = sampler.m_sampler;
    m_magnification = sampler.m_magnification;
    m_minification = sampler.m_minification;

    sampler.m_sampler = 0;
    sampler.m_magnification = 0;
    sampler.m_minification = 0;

    return *this;
}

void Opal::Sampler::setParameter(GLenum pname, GLint param) {
    glSamplerParameteri(m_sampler, pname, param);
}

void Opal::Sampler::bind() const {
    glBindSampler(0, m_sampler);
}
