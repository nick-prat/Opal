#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>

#include <GL/gl3w.h>

#include <Resources/resource.hpp>

class Texture : public IResource
{
public:
    Texture();
    ~Texture();

    void setFileName(const std::string filename);
    std::string getFileName() const;

    void setTexture(const GLuint glTexture);

    void bind() const;

    bool isLoaded() const;
    void unload();

private:
    GLuint m_glTexture;
    bool m_loaded;
    std::string m_filename;
};

#endif // _TEXTURE_H
