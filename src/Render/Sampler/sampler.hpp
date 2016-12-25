#ifndef _SAMPLER_H
#define _SAMPLER_H

#include <GL/gl3w.h>

class Sampler {
public:
    Sampler();
    ~Sampler();

    void bind() const;
    void setParams(int magnification, int minification);

private:
    GLuint m_sampler;
    int m_magnification, m_minification;
};

#endif // _SAMPLER_H
