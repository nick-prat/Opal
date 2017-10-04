#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <Opal/Core/gl.hh>
#include <Opal/Render/sampler.hh>
#include <Opal/Resources/resources.hh>

#include <string>

namespace Opal {

    class Texture {
    public:
        Texture(const Texture&) = delete;
        Texture(Texture&& texture);
        Texture(const Resources::RTexture& texture);
        ~Texture();

        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;

        std::string getFileName() const;
        void bind(const int loc) const;
        void unbind() const;
        bool isBound() const;
        int getTextureUnit() const;

        Sampler& getSampler();
        const Sampler& getSampler() const;

    private:
        Sampler m_sampler;
        GLuint m_glTexture;
        unsigned int m_width, m_height;
        GLint m_textureType;
        std::string m_filename;
        mutable int m_textureUnit;
    };

}

#endif // _TEXTURE_H
