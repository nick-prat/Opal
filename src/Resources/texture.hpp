#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include <GL/gl3w.h>

#include <Render/sampler.hpp>

class Texture {
public:
    Texture(const GLuint glTexture, const std::string& filename);
    Texture(const Texture&) = delete;
    Texture(Texture&& texture);
    ~Texture();

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    std::string getFileName() const;
    void bind() const;

    Sampler& getSampler();
    const Sampler& getSampler() const;

private:
    Sampler m_sampler;
    GLuint m_glTexture;
    std::string m_filename;
};

#endif // _TEXTURE_H
