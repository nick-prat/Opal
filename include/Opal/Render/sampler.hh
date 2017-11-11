#ifndef _SAMPLER_H
#define _SAMPLER_H

#include <Opal/Core/gl.hh>

namespace Opal {

    // TODO Implement parameter settings
    class Sampler {
    public:
        Sampler();
        Sampler(const Sampler&) = delete;
        Sampler(Sampler&& sampler);
        ~Sampler();

        Sampler& operator=(const Sampler&) = delete;
        Sampler& operator=(Sampler&& sampler);

        void bind(const unsigned int textureUnit) const;
        void setParameter(GLenum pname, GLint param);

    private:
        GLuint m_sampler;
        int m_magnification, m_minification;
    };

}

#endif // _SAMPLER_H
