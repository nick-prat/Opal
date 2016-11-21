#ifndef _SAMPLER_H
#define _SAMPLER_H

#include <GL/gl3w.h>

class Sampler
{
public:
    Sampler();
    ~Sampler();

    void Bind() const;
    void SetParams(int magnification, int minification);
    void Unload();

private:
    GLuint m_sampler;
    int m_magnification, m_minification;
};

#endif // _SAMPLER_H
