#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>

#include <GL/gl3w.h>

class Texture {
public:
    Texture(const GLuint glTexture, const std::string& filename);
    Texture(const Texture&) = delete;
    Texture(Texture&& texture);
    ~Texture();

    std::string getFileName() const;
    void bind() const;

private:
    const GLuint m_glTexture;
    const std::string m_filename;
};

#endif // _TEXTURE_H
