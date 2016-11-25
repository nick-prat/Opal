#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>

#include <GL/gl3w.h>

class Texture
{
public:
    Texture();
    ~Texture();

    void SetFileName(const std::string filename);
    std::string GetFileName() const;

    void SetTexture(const GLuint glTexture);

    void Bind() const;

    bool IsLoaded() const;
    void Unload();

private:
    GLuint m_glTexture;
    bool m_loaded;
    std::string m_filename;
};

#endif // _TEXTURE_H
